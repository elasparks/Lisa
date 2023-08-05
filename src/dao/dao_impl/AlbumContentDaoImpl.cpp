#include "AlbumContentDaoImpl.h"

AlbumContent AlbumContentDaoImpl::findContent(const uint64_t &albumId, const uint64_t &offset, const uint64_t &limit) {
    AlbumContent albumContent;
    albumContent.setTotalCount(getTotalCount(albumId));
    albumContent.setOffset(offset);
    albumContent.setLimit(limit);
    albumContent.setTracks(getAlbumTracks(albumId,offset,limit));
    return albumContent;
}

uint64_t AlbumContentDaoImpl::getTotalCount(const uint64_t &albumId) {
    std::string query = "SELECT count(*) FROM " + this->TABLE_NAME + " WHERE albumid=" + std::to_string(albumId) + ";";

    auto connection = PGConnectionPool::getInstance().getConnection();
    try {
        pqxx::nontransaction nonTransaction(*connection.getConnection());
        pqxx::result result(nonTransaction.exec(query));
        return result[0][0].as<uint64_t>();
    } catch (const std::exception &runtimeError) {
        throw std::runtime_error("Database error!");
    }
}

std::vector<Track>
AlbumContentDaoImpl::getAlbumTracks(const uint64_t &albumId, const uint64_t &offset, const uint64_t &limit) {
    std::string query = "SELECT * FROM " + this->TABLE_NAME + " WHERE albumid=" + std::to_string(albumId)
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

std::vector<Track> AlbumContentDaoImpl::handleResult(const pqxx::result &result) {
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