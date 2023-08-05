#ifndef LISA_ALBUMTYPEDAO_H
#define LISA_ALBUMTYPEDAO_H

#include <entity/AlbumType.h>
#include <vector>

class AlbumTypeDao {
public:
    virtual uint64_t create(const AlbumType &type) = 0;

    virtual void update(const uint64_t &id, const AlbumType &changes) = 0;

    virtual void remove(const uint64_t &id) = 0;

    virtual std::vector<AlbumType> findById(const std::vector<uint64_t> &ids) = 0;

    virtual std::vector<AlbumType> findById(const std::vector<std::string> &ids) = 0;

    virtual std::vector<AlbumType> findByName(const std::string &name, const uint64_t &offset, const uint64_t &limit) = 0;
};

#endif
