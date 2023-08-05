#ifndef LISA_ARTISTDAOIMPL_H
#define LISA_ARTISTDAOIMPL_H

#include <dao/ArtistDao.h>
#include <pgconnection/PGConnectionPool.h>

class ArtistDaoImpl : public ArtistDao {
private:
    const std::string TABLE_NAME = "artists";

    const std::map<std::string, std::string> throwableFields{{"name", "Expected name!"}};

    static std::vector<Artist> handleResult(const pqxx::result &result);

    static std::map<std::string, std::string> collectFieldsAndValues(const Artist &artist);

public:
    uint64_t create(const Artist &artist) override;

    void update(const uint64_t &id, const Artist &changes) override;

    void remove(const uint64_t &id) override;

    std::vector<Artist> findById(const std::vector<uint64_t> &ids) override;

    std::vector<Artist> findById(const std::vector<std::string> &ids) override;

    std::vector<Artist> findByName(const std::string &name, const uint64_t &offset, const uint64_t &limit) override;
};


#endif
