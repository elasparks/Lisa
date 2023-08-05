#ifndef LISA_ARTISTDAO_H
#define LISA_ARTISTDAO_H

#include <vector>
#include <entity/Artist.h>

class ArtistDao {
public:
    virtual uint64_t create(const Artist &artist) = 0;

    virtual void update(const uint64_t &id, const Artist &changes) = 0;

    virtual void remove(const uint64_t &id) = 0;

    virtual std::vector<Artist> findById(const std::vector<uint64_t> &ids) = 0;

    virtual std::vector<Artist> findById(const std::vector<std::string> &ids) = 0;

    virtual std::vector<Artist> findByName(const std::string &name, const uint64_t &offset, const uint64_t &limit) = 0;
};


#endif
