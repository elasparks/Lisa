#ifndef LISA_ALBUMDAOIMPL_H
#define LISA_ALBUMDAOIMPL_H

#include <regex>
#include <dao/AlbumDao.h>
#include <dao_impl/ArtistDaoImpl.h>
#include <dao_impl/AlbumTypeDaoImpl.h>
#include <pgconnection/PGConnectionPool.h>

class AlbumDaoImpl : public AlbumDao {
private:
    const std::string TABLE_NAME = "albums";

    const std::map<std::string, std::string> throwableFields{{"name", "Expected name!"},
                                                             {"artistsId", "Expected artists!"},
                                                             {"typeId", "Expected album type!"}};

    static std::vector<Album> handleResult(const pqxx::result &result);

    static std::map<std::string, std::string> collectFieldsAndValues(const Album &album);

public:
    uint64_t create(const Album &album) override;

    void update(const uint64_t &id, const Album &changes) override;

    void remove(const uint64_t &id) override;

    std::vector<Album> findById(const std::vector<uint64_t> &ids) override;

    std::vector<Album> findById(const std::vector<std::string> &ids) override;

    std::vector<Album> findByName(const std::string &name, const uint64_t &offset, const uint64_t &limit) override;

    std::vector<Album> findByArtist(const uint64_t &id, const uint64_t &offset, const uint64_t &limit) override;
};


#endif
