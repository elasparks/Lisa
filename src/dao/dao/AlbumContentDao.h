#ifndef LISA_ALBUMCONTENTDAO_H
#define LISA_ALBUMCONTENTDAO_H

#include <entity/AlbumContent.h>

class AlbumContentDao {
public:
    virtual AlbumContent findContent(const uint64_t &albumId, const uint64_t &offset, const uint64_t &limit) = 0;
};


#endif
