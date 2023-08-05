#ifndef LISA_PLAYLISTCONTENTDAOIMPL_H
#define LISA_PLAYLISTCONTENTDAOIMPL_H

#include <dao/PlaylistContentDao.h>
#include <dao_impl/TrackDaoImpl.h>
#include <pgconnection/PGConnectionPool.h>

class PlaylistContentDaoImpl : public PlaylistContentDao{
private:
    const std::string TABLE_NAME = "playlisttracks";

    uint64_t getTotalCount(const uint64_t &playlistId);

    std::vector<std::pair<PlaylistContent::TrackMetadata, Track>> handleResult(const pqxx::result &result);

public:
    bool areTrackInPlaylist(const uint64_t &playlistId, const uint64_t &trackId) override;

    uint64_t addTrackToPlaylist(const uint64_t &playlistId, const uint64_t &trackId) override;

    void removeTrackFromPlaylist(const uint64_t &playlistTrackId) override;

    PlaylistContent findContent(const uint64_t &playlistId, const uint64_t &offset, const uint64_t &limit) override;
};


#endif
