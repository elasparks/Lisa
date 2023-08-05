#ifndef LISA_PLAYLISTDAO_H
#define LISA_PLAYLISTDAO_H

#include <vector>
#include <entity/Playlist.h>

class PlaylistDao {
public:
    virtual uint64_t create(const Playlist &playlist) = 0;

    virtual void update(const uint64_t &id, const Playlist &changes) = 0;

    virtual void remove(const uint64_t &id) = 0;

    virtual std::vector<Playlist> findById(const std::vector<uint64_t> &ids) = 0;

    virtual std::vector<Playlist> findById(const std::vector<std::string> &ids) = 0;

    virtual std::vector<Playlist>
    findByName(const std::string &name, const uint64_t &offset, const uint64_t &limit) = 0;
};


#endif
