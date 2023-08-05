#ifndef LISA_TRACKDAOIMPL_H
#define LISA_TRACKDAOIMPL_H

#include <dao/TrackDao.h>
#include <dao_impl/ArtistDaoImpl.h>
#include <pgconnection/PGConnectionPool.h>

class TrackDaoImpl : public TrackDao {
private:
    const std::string TABLE_NAME = "tracks";

    const std::map<std::string, std::string> throwableFields{{"name",        "Expected name!"},
                                                             {"artistsId",   "Expected artists!"},
                                                             {"albumId",   "Expected album!"},
                                                             {"duration",    "Expected track duration!"},
                                                             {"playCount",   "Expected track play count!"},
                                                             {"trackNumber", "Expected track number!"}};

    static std::vector<Track> handleResult(const pqxx::result &result);

    static std::map<std::string, std::string> collectFieldsAndValues(const Track &track);

public:
    uint64_t create(const Track &track) override;

    void update(const uint64_t &id, const Track &changes) override;

    void remove(const uint64_t &id) override;

    std::vector<Track> findById(const std::vector<uint64_t> &ids) override;

    std::vector<Track> findById(const std::vector<std::string> &ids) override;

    std::vector<Track>
    findByName(const std::string &name, const uint64_t &offset, const uint64_t &limit) override;
};


#endif
