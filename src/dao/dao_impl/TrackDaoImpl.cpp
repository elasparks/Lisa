#include "TrackDaoImpl.h"

uint64_t TrackDaoImpl::create(const Track &track) {
    auto fieldValuesMap = collectFieldsAndValues(track);
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

void TrackDaoImpl::update(const uint64_t &id, const Track &changes) {
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

void TrackDaoImpl::remove(const uint64_t &id) {
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

std::vector<Track> TrackDaoImpl::findById(const std::vector<uint64_t> &ids) {
    std::vector<std::string> idsString;
    idsString.reserve(ids.size());
    std::transform(ids.begin(), ids.end(), std::back_inserter(idsString),
                   [](const uint64_t &id) { return std::to_string(id); });

    return findById(idsString);
}

std::vector<Track> TrackDaoImpl::findById(const std::vector<std::string> &ids) {
    std::string query = "SELECT * FROM " + this->TABLE_NAME +
                        " WHERE id IN (" + pqxx::separated_list(",", ids.begin(), ids.end()) + ");";

    std::vector<Track> tracks;
    auto connection = PGConnectionPool::getInstance().getConnection();
    try {
        pqxx::nontransaction nonTransaction(*connection.getConnection());
        pqxx::result result(nonTransaction.exec(query));
        tracks = handleResult(result);
    } catch (const std::exception &runtimeError) {
        throw std::runtime_error("Database error!");
    }

    if (ids.size() != tracks.size())
        throw std::runtime_error("One or more entries were not found in the database!");
    return tracks;
}

std::vector<Track> TrackDaoImpl::findByName(const std::string &name, const uint64_t &offset, const uint64_t &limit) {
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

std::vector<Track> TrackDaoImpl::handleResult(const pqxx::result &result) {
    std::vector<Track> tracks;
    tracks.reserve(result.size());
    ArtistDaoImpl artistDao;
    for (auto it = result.begin(); it != result.end(); ++it) {
        auto array = it[2].as_array();
        std::vector<std::string> artistsId;
        std::pair<pqxx::array_parser::juncture, std::string> element;
        while ((element = array.get_next()).first != pqxx::array_parser::juncture::done)
            if (element.first == pqxx::array_parser::juncture::string_value)
                artistsId.push_back(element.second);

        Track track;
        track.setId(it[0].as<uint64_t>());
        track.setName(it[1].as<std::string>());
        track.setArtists(artistDao.findById(artistsId));
        track.setDuration(it[3].as<uint64_t>());
        track.setPlayCount(it[4].as<uint64_t>());
        track.setAlbumId(it[5].as<uint64_t>());
        track.setTrackNumber(it[6].as<uint64_t>());

        tracks.push_back(track);
    }

    return tracks;
}

std::map<std::string, std::string> TrackDaoImpl::collectFieldsAndValues(const Track &track) {
    std::map<std::string, std::string> result;

    if (track.getName() != nullptr) {
        if (!track.getName()->empty())
            result["name"] = "'" + *track.getName() + "'";
        else
            throw std::runtime_error("Name can't be empty!");
    }

    if (track.getArtists() != nullptr && !track.getArtists()->empty()) {
        std::vector<uint64_t> artistIds;
        artistIds.reserve(track.getArtists()->size());
        std::transform(track.getArtists()->begin(), track.getArtists()->end(), std::back_inserter(artistIds),
                       [](const Artist &artist) {
                           if (artist.getId())
                               return *artist.getId();
                           throw std::runtime_error("Artist id was not specified!");
                       });
        result["artistsId"] = "ARRAY [" + pqxx::separated_list(",", artistIds.begin(), artistIds.end()) + "]";
    }
    if (track.getDuration() != nullptr){
        if (*track.getDuration()>=0)
            result["duration"] = std::to_string(*track.getDuration());
        else
            throw std::runtime_error("Duration can't be negative!");
    }

    if (track.getPlayCount() != nullptr){
        if (*track.getPlayCount()>=0)
            result["playCount"] = std::to_string(*track.getPlayCount());
        else
            throw std::runtime_error("Play count can't be negative!");
    }

    if (track.getTrackNumber() != nullptr){
        if (*track.getTrackNumber()>=0)
            result["trackNumber"] = std::to_string(*track.getTrackNumber());
        else
            throw std::runtime_error("Track number can't be negative!");
    }

    if (track.getAlbum() != nullptr){
        if (*track.getAlbum()->getId()>=0)
            result["albumId"] = std::to_string(*track.getAlbum()->getId());
        else
            throw std::runtime_error("Album id was not specified!");
    }

    return result;
}
