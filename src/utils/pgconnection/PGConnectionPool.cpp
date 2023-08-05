#include "PGConnectionPool.h"

PGConnectionPool::Connection::Connection(const std::shared_ptr<pqxx::connection> &connection) : connection(
        connection) {}

PGConnectionPool::Connection::~Connection() {
    connection->cancel_query();
    PGConnectionPool::getInstance().releaseConnection(connection);
}

const std::shared_ptr<pqxx::connection> &PGConnectionPool::Connection::getConnection() const {
    return connection;
}

PGConnectionPool::PGConnectionPool() {
    this->createPool();
}

void PGConnectionPool::createPool() {
    std::lock_guard<std::mutex> locker(this->poolMutex);
    for (auto i = 0; i < poolSize; ++i) {
        this->pool.emplace_back(std::make_shared<pqxx::connection>(PGConnectionPool::connectionString));
    }
}

PGConnectionPool::Connection PGConnectionPool::getConnection() {
    std::unique_lock<std::mutex> locker(this->poolMutex);
    conditionPool.wait(locker, [&] { return !pool.empty(); });

    auto connection = *this->pool.rbegin();
    this->pool.pop_back();

    return Connection(connection);
}

void PGConnectionPool::releaseConnection(const std::shared_ptr<pqxx::connection> &connection) {
    std::unique_lock<std::mutex> locker(this->poolMutex);
    this->pool.push_back(connection);

    locker.unlock();
    conditionPool.notify_one();
}

PGConnectionPool &PGConnectionPool::getInstance() {
    static PGConnectionPool pool;
    return pool;
}

void PGConnectionPool::setConnectionString(const std::string &host, const std::string &port, const std::string &dbname,
                                           const std::string &user, const std::string &password) {
    PGConnectionPool::connectionString = "host=" + host +
                                         " port=" + port +
                                         " dbname=" + dbname +
                                         " user=" + user +
                                         " password=" + password;
}

void PGConnectionPool::setPoolSize(const uint32_t &size) {
    PGConnectionPool::poolSize = size;
}
