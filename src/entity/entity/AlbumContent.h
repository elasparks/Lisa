#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wshadow"
#ifndef LISA_ALBUMCONTENT_H
#define LISA_ALBUMCONTENT_H

#include <nlohmann/json.hpp>
#include <entity/Track.h>

class AlbumContent{
private:
    uint64_t totalCount;
    uint64_t offset;
    uint64_t limit;
    std::vector<Track> tracks;
public:
    [[nodiscard]] uint64_t getTotalCount() const {
        return this->totalCount;
    }

    void setTotalCount(uint64_t totalCount) {
        this->totalCount = totalCount;
    }

    [[nodiscard]] uint64_t getOffset() const {
        return this->offset;
    }

    void setOffset(uint64_t offset) {
        this->offset = offset;
    }

    [[nodiscard]] uint64_t getLimit() const {
        return this->limit;
    }

    void setLimit(uint64_t limit) {
        this->limit = limit;
    }

    [[nodiscard]] const std::vector<Track> &getTracks() const {
        return this->tracks;
    }

    void setTracks(const std::vector<Track> &tracks) {
        this->tracks = std::vector(tracks);
    }

    nlohmann::json toJson(){
        nlohmann::json tracks = nlohmann::json::array();
        for (auto &&track : this->tracks)
            tracks.push_back(track.toJson());
        nlohmann::json content{
                {"content", {
                        {"totalCount", this->totalCount},
                        {"paddingInfo", {
                                {"offset", this->offset},
                                {"limit", this->limit},
                                {"items", tracks}
                        }},
                }}
        };
        return content;
    }
};


#endif

#pragma clang diagnostic pop