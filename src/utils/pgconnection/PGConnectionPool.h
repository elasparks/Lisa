#ifndef LISA_PGCONNECTIONPOOL_H
#define LISA_PGCONNECTIONPOOL_H

#include <mutex>
#include <condition_variable>
#include "pqxx/pqxx"

class PGConnectionPool {
public:
    class Connection{
    private:
        const std::shared_ptr<pqxx::connection> connection;

    public:
        explicit Connection(const std::shared_ptr<pqxx::connection> &connection);

        virtual ~Connection();

        [[nodiscard]] const std::shared_ptr<pqxx::connection> &getConnection() const;
    };
private:
    std::mutex poolMutex;
    std::condition_variable conditionPool;
    std::vector<std::shared_ptr<pqxx::connection>> pool;
    inline static uint32_t poolSize;
    inline static std::string connectionString;

    PGConnectionPool();

    void createPool();

    void releaseConnection(const std::shared_ptr<pqxx::connection> &connection);

public:

    Connection getConnection();

    static PGConnectionPool &getInstance();

    static void setConnectionString(const std::string &host, const std::string &port, const std::string &dbname,
                                    const std::string &user, const std::string &password);

    static void setPoolSize(const uint32_t &size);

    friend class Connection;
};


#endif
