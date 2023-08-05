#ifndef LISA_ALBUMDAO_H
#define LISA_ALBUMDAO_H

#include <entity/Album.h>
#include <vector>

class AlbumDao {
public:
    virtual uint64_t create(const Album &album) = 0;

    virtual void update(const uint64_t &id, const Album &changes) = 0;

    virtual void remove(const uint64_t &id) = 0;

    virtual std::vector<Album> findById(const std::vector<uint64_t> &ids) = 0;

    virtual std::vector<Album> findById(const std::vector<std::string> &ids) = 0;

    virtual std::vector<Album> findByName(const std::string &name, const uint64_t &offset, const uint64_t &limit) = 0;

    virtual std::vector<Album> findByArtist(const uint64_t &id, const uint64_t &offset, const uint64_t &limit) = 0;
};


#endif
