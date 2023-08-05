#include "PlaylistContentDaoImpl.h"

bool PlaylistContentDaoImpl::areTrackInPlaylist(const uint64_t &playlistId, const uint64_t &trackId) {
    std::string query = "SELECT exists(SELECT 1 FROM " + this->TABLE_NAME + " WHERE trackId=" + std::to_string(trackId)
                        + "AND playlistId=" + std::to_string(playlistId) + ";";

    auto connection = PGConnectionPool::getInstance().getConnection();
    try {
        pqxx::nontransaction transaction(*connection.getConnection());
        pqxx::result result(transaction.exec(query));
        return result[0][0].as<bool>();
    } catch (const std::exception &runtimeError) {
        throw std::runtime_error("Database error!");
    }
}

uint64_t PlaylistContentDaoImpl::addTrackToPlaylist(const uint64_t &playlistId, const uint64_t &trackId) {
    std::string query = "INSERT INTO " + this->TABLE_NAME + "(playlistId, trackId) VALUES ("
                        + std::to_string(playlistId) + "," + std::to_string(trackId) + ");";

    auto connection = PGConnectionPool::getInstance().getConnection();
    try {
        pqxx::work transaction(*connection.getConnection());
        pqxx::result result(transaction.exec(query));
        transaction.commit();
        return result[0][0].as<uint64_t>();
    } catch (const std::exception &runtimeError) {
        throw std::runtime_error("Database error!");
    }
}

void PlaylistContentDaoImpl::removeTrackFromPlaylist(const uint64_t &playlistTrackId) {
    std::string query = "DELETE FROM " + this->TABLE_NAME + " WHERE id=" + std::to_string(playlistTrackId) + ";";

    auto connection = PGConnectionPool::getInstance().getConnection();
    try {
        pqxx::work transaction(*connection.getConnection());
        transaction.exec(query);
        transaction.commit();
    } catch (const std::exception &runtimeError) {
        throw std::runtime_error("Database error!");
    }
}

PlaylistContent
PlaylistContentDaoImpl::findContent(const uint64_t &playlistId, const uint64_t &offset, const uint64_t &limit) {
    std::string query = "SELECT * FROM " + this->TABLE_NAME + " WHERE playlistId=" + std::to_string(playlistId)
                        + " LIMIT " + std::to_string(limit)
                        + " OFFSET " + std::to_string(offset) + ";";

    PlaylistContent playlistContent;
    playlistContent.setTotalCount(this->getTotalCount(playlistId));
    playlistContent.setOffset(offset);
    playlistContent.setLimit(limit);

    auto connection = PGConnectionPool::getInstance().getConnection();
    try {
        pqxx::nontransaction nonTransaction(*connection.getConnection());
        pqxx::result result(nonTransaction.exec(query));
        playlistContent.setTracks(handleResult(result));
    } catch (const std::exception &runtimeError) {
        throw std::runtime_error("Database error!");
    }

    return playlistContent;
}

uint64_t PlaylistContentDaoImpl::getTotalCount(const uint64_t &playlistId) {
    std::string query =
            "SELECT count(*) FROM " + this->TABLE_NAME + " WHERE playlistId=" + std::to_string(playlistId) + ";";

    auto connection = PGConnectionPool::getInstance().getConnection();
    try {
        pqxx::nontransaction nonTransaction(*connection.getConnection());
        pqxx::result result(nonTransaction.exec(query));
        return result[0][0].as<uint64_t>();
    } catch (const std::exception &runtimeError) {
        throw std::runtime_error("Database error!");
    }
}

std::vector<std::pair<PlaylistContent::TrackMetadata, Track>>
PlaylistContentDaoImpl::handleResult(const pqxx::result &result) {
    std::vector<std::pair<PlaylistContent::TrackMetadata, Track>> playlistTracks;
    playlistTracks.reserve(result.size());
    TrackDaoImpl trackDao;
    for (auto it = result.begin(); it != result.end(); ++it) {
        PlaylistContent::TrackMetadata trackMetadata;
        trackMetadata.setId(it[0].as<uint64_t>());
        trackMetadata.setAddedAt(it[3].as<std::string>());
        Track track = trackDao.findById({it[2].as<std::string>()})[0];
        playlistTracks.emplace_back(trackMetadata,track);
    }

    return playlistTracks;
}
