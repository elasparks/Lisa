#include "AlbumDaoImpl.h"

uint64_t AlbumDaoImpl::create(const Album &album) {
    auto fieldValuesMap = collectFieldsAndValues(album);
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

void AlbumDaoImpl::update(const uint64_t &id, const Album &changes) {
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

void AlbumDaoImpl::remove(const uint64_t &id) {
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

std::vector<Album> AlbumDaoImpl::findById(const std::vector<uint64_t> &ids) {
    std::vector<std::string> idsString;
    idsString.reserve(ids.size());
    std::transform(ids.begin(), ids.end(), std::back_inserter(idsString),
                   [](const uint64_t &id) { return std::to_string(id); });

    return findById(idsString);
}

std::vector<Album> AlbumDaoImpl::findById(const std::vector<std::string> &ids) {
    std::string query = "SELECT * FROM " + this->TABLE_NAME + " WHERE id IN ("
                        + pqxx::separated_list(",", ids.begin(), ids.end()) + ");";

    std::vector<Album> albums;
    auto connection = PGConnectionPool::getInstance().getConnection();
    try {
        pqxx::nontransaction nonTransaction(*connection.getConnection());
        pqxx::result result(nonTransaction.exec(query));
        albums = handleResult(result);
    } catch (const std::exception &runtimeError) {
        throw std::runtime_error("Database error!");
    }

    if (ids.size() != albums.size())
        throw std::runtime_error("One or more entries were not found in the database!");
    return albums;
}

std::vector<Album> AlbumDaoImpl::findByName(const std::string &name, const uint64_t &offset, const uint64_t &limit) {
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

std::vector<Album> AlbumDaoImpl::findByArtist(const uint64_t &id, const uint64_t &offset, const uint64_t &limit) {
    std::string query = "SELECT * FROM " + this->TABLE_NAME + " WHERE " + std::to_string(id) + "=ANY(artistsId)"
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

std::vector<Album> AlbumDaoImpl::handleResult(const pqxx::result &result) {
    std::vector<Album> albums;
    albums.reserve(result.size());
    ArtistDaoImpl artistDao;
    AlbumTypeDaoImpl albumTypeDao;
    for (auto it = result.begin(); it != result.end(); ++it) {
        auto array = it[2].as_array();
        std::vector<std::string> artistsId;
        std::pair<pqxx::array_parser::juncture, std::string> element;
        while ((element = array.get_next()).first != pqxx::array_parser::juncture::done)
            if (element.first == pqxx::array_parser::juncture::string_value)
                artistsId.push_back(element.second);

        Album album;
        album.setId(it[0].as<uint64_t>());
        album.setName(it[1].as<std::string>());
        album.setArtists(artistDao.findById(artistsId));
        if (!it[3].is_null())
            album.setCoverArt(it[3].as<std::string>());
        album.setType(albumTypeDao.findById(std::vector<uint64_t>{it[4].as<uint64_t>()})[0]);
        if (!it[5].is_null())
            album.setDate(it[5].as<std::string>());

        albums.push_back(album);
    }

    return albums;
}

std::map<std::string, std::string> AlbumDaoImpl::collectFieldsAndValues(const Album &album) {
    std::map<std::string, std::string> result;

    if (album.getName() != nullptr) {
        if (!album.getName()->empty())
            result["name"] = "'" + *album.getName() + "'";
        else
            throw std::runtime_error("Name can't be empty!");
    }

    if (album.getArtists() != nullptr && !album.getArtists()->empty()) {
        std::vector<uint64_t> artistIds;
        artistIds.reserve(album.getArtists()->size());
        std::transform(album.getArtists()->begin(), album.getArtists()->end(), std::back_inserter(artistIds),
                       [](const Artist &artist) {
                           if (artist.getId())
                               return *artist.getId();
                           throw std::runtime_error("Artist id was not specified!");
                       });
        result["artistsId"] = "ARRAY [" + pqxx::separated_list(",", artistIds.begin(), artistIds.end()) + "]";
    }

    if (album.getType() != nullptr) {
        if (album.getType()->getId() != nullptr)
            result["typeId"] = std::to_string(*album.getType()->getId());
        else
            throw std::runtime_error("Id of album type was not specified!");
    }

    if (album.getDate() != nullptr) {
        const std::regex pattern(R"(^(\d\d\d\d)-(01|02|03|04|05|06|07|08|09|10|11|12)-([0-3]\d)$)");
        if (std::regex_match(*album.getDate(), pattern)) {
            result["createdAt"] = "'" + *album.getDate() + "'";
        } else
            throw std::runtime_error("Incorrect date!");
    }
    return result;
}

