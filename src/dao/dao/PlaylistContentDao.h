#ifndef LISA_PLAYLISTCONTENTDAO_H
#define LISA_PLAYLISTCONTENTDAO_H

#include <entity/PlaylistContent.h>

class PlaylistContentDao {
public:
    virtual bool areTrackInPlaylist(const uint64_t &playlistId, const uint64_t &trackId) = 0;

    virtual uint64_t addTrackToPlaylist(const uint64_t &playlistId, const uint64_t &trackId) = 0;

    virtual void removeTrackFromPlaylist(const uint64_t &playlistTrackId) = 0;

    virtual PlaylistContent
    findContent(const uint64_t &playlistId, const uint64_t &offset, const uint64_t &limit) = 0;
};


#endif
