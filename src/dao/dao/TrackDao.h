#ifndef LISA_TRACKDAO_H
#define LISA_TRACKDAO_H

#include "entity/Track.h"

class TrackDao {
public:
    virtual uint64_t create(const Track &track) = 0;

    virtual void update(const uint64_t &id, const Track &changes) = 0;

    virtual void remove(const uint64_t &id) = 0;

    virtual std::vector<Track> findById(const std::vector<uint64_t> &ids) = 0;

    virtual std::vector<Track> findById(const std::vector<std::string> &ids) = 0;

    virtual std::vector<Track> findByName(const std::string &name, const uint64_t &offset, const uint64_t &limit) = 0;
};


#endif
