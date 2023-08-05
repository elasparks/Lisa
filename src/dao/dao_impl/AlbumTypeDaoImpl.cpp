#include "AlbumTypeDaoImpl.h"

uint64_t AlbumTypeDaoImpl::create(const AlbumType &albumType) {
    auto fieldValuesMap = collectFieldsAndValues(albumType);
    for (const auto &[field, exception]: this->throwableFields)
        if (fieldValuesMap.count(field) == 0)
            throw std::runtime_error(exception);

    std::string query = "INSERT INTO " + this->TABLE_NAME;
    std::vector<std::string> fields, values;
    for (auto const &[field, value]: fieldValuesMap) {
        fields.push_back(field);
        values.push_back(value);
    }
    query.append(" (" + pqxx::separated_list(",", fields.begin(), fields.end()) + ")");
    query.append(" VALUES (" + pqxx::separated_list(",", values.begin(), values.end()) + ") RETURNING id;");

    auto connection = PGConnectionPool::getInstance().getConnection();
    try {
        pqxx::work transaction(*connection.getConnection());
        pqxx::result result(transaction.exec(query));
        transaction.commit();
        return result[0][0].as<uint64_t>();
    } catch (const std::exception &exception) {
        throw std::runtime_error("Database error!");
    }
}

void AlbumTypeDaoImpl::update(const uint64_t &id, const AlbumType &changes) {
    auto fieldValuesMap = collectFieldsAndValues(changes);
    if (fieldValuesMap.empty())
        throw std::runtime_error("Nothing to do!");

    std::string query = "UPDATE " + this->TABLE_NAME + " SET ";
    std::vector<std::string> values;
    for (auto const &[field, value]: fieldValuesMap) {
        values.push_back(field + "=" + value);
    }
    query.append(pqxx::separated_list(",", values.begin(), values.end()));
    query.append(" WHERE id=" + std::to_string(id) + ";");

    auto connection = PGConnectionPool::getInstance().getConnection();
    try {
        pqxx::work transaction(*connection.getConnection());
        transaction.exec(query);
        transaction.commit();
    } catch (const std::exception &exception) {
        throw std::runtime_error("Database error!");
    }
}

void AlbumTypeDaoImpl::remove(const uint64_t &id) {
    std::string query = "DELETE FROM " + this->TABLE_NAME + " WHERE id=" + std::to_string(id) + ";";

    auto connection = PGConnectionPool::getInstance().getConnection();
    try {
        pqxx::work transaction(*connection.getConnection());
        transaction.exec(query);
        transaction.commit();
    } catch (const std::exception &exception) {
        throw std::runtime_error("Database error!");
    }
}

std::vector<AlbumType> AlbumTypeDaoImpl::findById(const std::vector<uint64_t> &ids) {
    std::vector<std::string> idsString;
    idsString.reserve(ids.size());
    std::transform(ids.begin(), ids.end(), std::back_inserter(idsString),
                   [](const uint64_t &id) { return std::to_string(id); });

    return findById(idsString);
}

std::vector<AlbumType> AlbumTypeDaoImpl::findById(const std::vector<std::string> &ids) {
    std::string query = "SELECT * FROM " + this->TABLE_NAME + " WHERE id IN ("
                        + pqxx::separated_list(",", ids.begin(), ids.end()) + ");";

    std::vector<AlbumType> albumTypes;
    auto connection = PGConnectionPool::getInstance().getConnection();
    try {
        pqxx::nontransaction nonTransaction(*connection.getConnection());
        pqxx::result result(nonTransaction.exec(query));
        albumTypes = handleResult(result);
    } catch (const std::exception &runtimeError) {
        throw std::runtime_error("Database error!");
    }

    if (ids.size() != albumTypes.size())
        throw std::runtime_error("One or more entries were not found in the database!");
    return albumTypes;
}

std::vector<AlbumType>
AlbumTypeDaoImpl::findByName(const std::string &name, const uint64_t &offset, const uint64_t &limit) {
    std::string query = "SELECT * FROM " + this->TABLE_NAME + " WHERE LOWER(name) LIKE LOWER('%" + name + "%')"
                        + " LIMIT " + std::to_string(limit)
                        + " OFFSET " + std::to_string(offset) + ";";

    auto connection = PGConnectionPool::getInstance().getConnection();
    try {
        pqxx::nontransaction nonTransaction(*connection.getConnection());
        pqxx::result result(nonTransaction.exec(query));
        return handleResult(result);
    } catch (const std::exception &runtimeError) {
        throw std::runtime_error("Database error!");
    }
}

std::vector<AlbumType> AlbumTypeDaoImpl::handleResult(const pqxx::result &result) {
    std::vector<AlbumType> albumTypes;
    albumTypes.reserve(result.size());
    for (auto it = result.begin(); it != result.end(); ++it)
        albumTypes.emplace_back(it[0].as<uint64_t>(), it[1].as<std::string>());
    return albumTypes;
}

std::map<std::string, std::string> AlbumTypeDaoImpl::collectFieldsAndValues(const AlbumType &albumType) {
    std::map<std::string, std::string> result;

    if (albumType.getName() != nullptr) {
        if (!albumType.getName()->empty())
            result["name"] = "'" + *albumType.getName() + "'";
        else
            throw std::runtime_error("Name can't be empty!");
    }

    return result;
}

