#ifndef LISA_PLAYLISTDAOIMPL_H
#define LISA_PLAYLISTDAOIMPL_H

#include <dao/PlaylistDao.h>
#include <pgconnection/PGConnectionPool.h>

class PlaylistDaoImpl : public PlaylistDao {
private:
    const std::string TABLE_NAME = "playlists";

    const std::map<std::string, std::string> throwableFields{{"name", "Expected name!"}};

    static std::vector<Playlist> handleResult(const pqxx::result &result);

    static std::map<std::string, std::string> collectFieldsAndValues(const Playlist &playlist);

public:
    uint64_t create(const Playlist &playlist) override;

    void update(const uint64_t &id, const Playlist &changes) override;

    void remove(const uint64_t &id) override;

    std::vector<Playlist> findById(const std::vector<uint64_t> &ids) override;

    std::vector<Playlist> findById(const std::vector<std::string> &ids) override;

    std::vector<Playlist> findByName(const std::string &name, const uint64_t &offset, const uint64_t &limit) override;
};


#endif
