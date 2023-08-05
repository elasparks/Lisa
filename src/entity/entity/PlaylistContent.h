#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wshadow"
#ifndef LISA_PLAYLISTCONTENT_H
#define LISA_PLAYLISTCONTENT_H

#include <entity/Track.h>

class PlaylistContent {
public:
    class TrackMetadata {
    private:
        uint64_t id;
        std::string addedAt;

    public:
        [[nodiscard]] uint64_t getId() const {
            return id;
        }

        void setId(uint64_t id) {
            this->id = id;
        }

        [[nodiscard]] const std::string &getAddedAt() const {
            return addedAt;
        }

        void setAddedAt(const std::string &addedAt) {
            this->addedAt = addedAt;
        }

        nlohmann::json toJson() {
            return {
                    {"meta", {
                            {"id", this->id},
                            {"addedAt", this->addedAt}
                    }}
            };
        }
    };

private:
    uint64_t totalCount;
    uint64_t offset;
    uint64_t limit;
    std::vector<std::pair<TrackMetadata, Track>> tracks;

public:
    [[nodiscard]] uint64_t getTotalCount() const {
        return totalCount;
    }

    void setTotalCount(uint64_t totalCount) {
        this->totalCount = totalCount;
    }

    [[nodiscard]] uint64_t getOffset() const {
        return offset;
    }

    void setOffset(uint64_t offset) {
        this->offset = offset;
    }

    [[nodiscard]] uint64_t getLimit() const {
        return limit;
    }

    void setLimit(uint64_t limit) {
        this->limit = limit;
    }

    [[nodiscard]] const std::vector<std::pair<TrackMetadata, Track>> &getTracks() const {
        return tracks;
    }

    void setTracks(const std::vector<std::pair<TrackMetadata, Track>> &tracks) {
        this->tracks = tracks;
    }

    nlohmann::json toJson() {
        nlohmann::json tracks = nlohmann::json::array();
        for (auto &&track: this->tracks) {
            auto tmp = track.first.toJson();
            tmp.push_back({"item",track.second.toJson()});
            tracks.push_back(tmp);
        }
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