#ifndef LISA_ALBUMTYPEDAOIMPL_H
#define LISA_ALBUMTYPEDAOIMPL_H

#include "dao/AlbumTypeDao.h"
#include "pgconnection/PGConnectionPool.h"

class AlbumTypeDaoImpl : public AlbumTypeDao {
private:
    const std::string TABLE_NAME = "albumtypes";

    const std::map<std::string, std::string> throwableFields{{"name", "Expected name!"}};

    static std::vector<AlbumType> handleResult(const pqxx::result &result);

    static std::map<std::string, std::string> collectFieldsAndValues(const AlbumType &albumType);

public:
    uint64_t create(const AlbumType &albumType) override;

    void update(const uint64_t &id, const AlbumType &changes) override;

    void remove(const uint64_t &id) override;

    std::vector<AlbumType> findById(const std::vector<uint64_t> &ids) override;

    std::vector<AlbumType> findById(const std::vector<std::string> &ids) override;

    std::vector<AlbumType> findByName(const std::string &name, const uint64_t &offset, const uint64_t &limit) override;
};


#endif
