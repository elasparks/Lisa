#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wshadow"
#ifndef LISA_TRACK_H
#define LISA_TRACK_H

#include <cstdint>
#include <string>
#include <vector>
#include <entity/Artist.h>
#include <entity/Album.h>

class Track {
private:
    std::shared_ptr<uint64_t> id;
    std::shared_ptr<std::string> name;
    std::shared_ptr<std::vector<Artist>> artists;
    std::shared_ptr<uint64_t> duration;
    std::shared_ptr<uint64_t> playCount;
    std::shared_ptr<uint64_t> trackNumber;
    std::shared_ptr<Album> album;
    std::shared_ptr<uint64_t> albumId;
public:
    [[nodiscard]] const std::shared_ptr<uint64_t> &getId() const {
        return this->id;
    }

    void setId(const uint64_t &id) {
        this->id = std::make_shared<uint64_t>(id);
    }

    [[nodiscard]] const std::shared_ptr<std::string> &getName() const {
        return this->name;
    }

    void setName(const std::string &name) {
        this->name = std::make_shared<std::string>(name);
    }

    [[nodiscard]] const std::shared_ptr<std::vector<Artist>> &getArtists() const {
        return this->artists;
    }

    void setArtists(const std::vector<Artist> &artists) {
        this->artists = std::make_shared<std::vector<Artist>>(artists);
    }

    [[nodiscard]] const std::shared_ptr<uint64_t> &getDuration() const {
        return this->duration;
    }

    void setDuration(const uint64_t &duration) {
        this->duration = std::make_shared<uint64_t>(duration);
    }

    [[nodiscard]] const std::shared_ptr<uint64_t> &getPlayCount() const {
        return this->playCount;
    }

    void setPlayCount(const uint64_t &playCount) {
        this->playCount = std::make_shared<uint64_t>(playCount);
    }

    [[nodiscard]] const std::shared_ptr<uint64_t> &getTrackNumber() const {
        return this->trackNumber;
    }

    void setTrackNumber(const uint64_t &trackNumber) {
        this->trackNumber = std::make_shared<uint64_t>(trackNumber);
    }

    [[nodiscard]] const std::shared_ptr<Album> &getAlbum() const {
        return this->album;
    }

    void setAlbum(const Album &album) {
        this->album = std::make_shared<Album>(album);
    }

    [[nodiscard]] const std::shared_ptr<uint64_t> &getAlbumId() const {
        return this->albumId;
    }

    void setAlbumId(const uint64_t &albumId) {
        this->albumId = std::make_shared<uint64_t>(albumId);
    }

    static Track fromJson(const nlohmann::json &json) {
        Track track;
        if (json.contains("id"))
            track.setId(json["id"]);
        if (json.contains("name"))
            track.setName(json["name"]);
        if (json.contains("artists")){
            std::vector<Artist> artists;
            if (json["artists"].contains("totalCount"))
                artists.reserve(json["artists"]["totalCount"]);
            if (json["artists"].contains("items")){
                for (const auto &artist : json["artists"]["items"])
                    artists.push_back(Artist::fromJson(artist));
                track.setArtists(artists);
            }
        }
        if (json.contains("trackNumber"))
            track.setTrackNumber(json["trackNumber"]);
        if (json.contains("album"))
            track.setAlbum(Album::fromJson(json["album"]));
        return track;
    }

    [[nodiscard]] nlohmann::json toJson() const {
        nlohmann::json track;
        if (this->id)
            track["id"] = *this->id;
        if (this->name)
            track["name"] = *this->name;
        if (this->artists){
            nlohmann::json artists = nlohmann::json::array();
            for (auto &&artist : *this->artists)
                artists.push_back(artist.toJson());
            track["artists"]["totalCount"] = this->artists->size();
            track["artists"]["items"] = artists;
        }
        if (this->duration)
            track["duration"]["totalMilliseconds"] = *this->duration;
        if (this->playCount)
            track["playCount"] = *this->playCount;
        if (this->trackNumber)
            track["trackNumber"] = *this->trackNumber;
        if (this->album)
            track["album"] = this->album->toJson();
        return track;
    }
};


#endif

#pragma clang diagnostic pop