#ifndef LISA_ALBUMCONTENTDAOIMPL_H
#define LISA_ALBUMCONTENTDAOIMPL_H

#include <dao/AlbumContentDao.h>
#include <dao_impl/ArtistDaoImpl.h>
#include <pgconnection/PGConnectionPool.h>

class AlbumContentDaoImpl : public AlbumContentDao{
private:
    const std::string TABLE_NAME = "tracks";

    uint64_t getTotalCount(const uint64_t &albumId);

    std::vector<Track> getAlbumTracks(const uint64_t &albumId, const uint64_t &offset, const uint64_t &limit);

    std::vector<Track> handleResult(const pqxx::result &result);

public:
    AlbumContent findContent(const uint64_t &albumId, const uint64_t &offset, const uint64_t &limit) override;
};


#endif
