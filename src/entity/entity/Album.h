#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wshadow"
#ifndef LISA_ALBUM_H
#define LISA_ALBUM_H

#include <cstdint>
#include <string>
#include <utility>
#include <vector>
#include <nlohmann/json.hpp>
#include <entity/Artist.h>
#include <entity/AlbumType.h>

class Album {
private:
    std::shared_ptr<uint64_t> id;
    std::shared_ptr<std::string> name;
    std::shared_ptr<std::vector<Artist>> artists;
    std::shared_ptr<std::string> coverArt;
    std::shared_ptr<AlbumType> type;
    std::shared_ptr<std::string> date;
public:
    [[nodiscard]] std::shared_ptr<const uint64_t> getId() const {
        return this->id;
    }

    void setId(const uint64_t &id) {
        this->id = std::make_shared<uint64_t>(id);
    }

    [[nodiscard]] std::shared_ptr<const std::string> getName() const {
        return this->name;
    }

    void setName(const std::string &name) {
        this->name = std::make_shared<std::string>(name);
    }

    [[nodiscard]] std::shared_ptr<const std::vector<Artist>> getArtists() const {
        return artists;
    }

    void setArtists(const std::vector<Artist> &artists) {
        this->artists = std::make_shared<std::vector<Artist>>(artists);
    }

    [[nodiscard]] std::shared_ptr<const std::string> getCoverArt() const {
        return coverArt;
    }

    void setCoverArt(const std::string &coverArt) {
        this->coverArt = std::make_shared<std::string>(coverArt);
    }

    [[nodiscard]] std::shared_ptr<const AlbumType> getType() const {
        return this->type;
    }

    void setType(const AlbumType &type) {
        this->type = std::make_shared<AlbumType>(type);
    }

    [[nodiscard]] std::shared_ptr<const std::string> getDate() const {
        return this->date;
    }

    void setDate(const std::string &date) {
        this->date = std::make_shared<std::string>(date);
    }

    static Album fromJson(const nlohmann::json& json){
        Album album;
        if (json.contains("id"))
            album.setId(json["id"]);
        if (json.contains("name"))
            album.setName(json["name"]);
        if (json.contains("artists")){
            std::vector<Artist> artists;
            if (json["artists"].contains("totalCount"))
                artists.reserve(json["artists"]["totalCount"]);
            if (json["artists"].contains("items")){
                for (const auto &artist : json["artists"]["items"])
                    artists.push_back(Artist::fromJson(artist));
                album.setArtists(artists);
            }
        }
        if (json.contains("date"))
            album.setDate(json["date"]);
        if (json.contains("type"))
            album.setType(AlbumType::fromJson(json["type"]));
        return album;
    }

    [[nodiscard]] nlohmann::json toJson() const {
        nlohmann::json album;
        if (this->id)
            album["id"] = *this->id;
        if (this->name)
            album["name"] = *this->name;
        if (this->artists) {
            nlohmann::json artists = nlohmann::json::array();
            for (auto &&artist: *this->artists)
                artists.push_back(artist.toJson());
            album["artists"]["totalCount"] = this->artists->size();
            album["artists"]["items"] = artists;
        }
        if (this->coverArt)
            album["coverArt"]["url"] = *this->coverArt;
        if (this->date)
            album["date"] = *this->date;
        if (this->type)
            album["type"] = this->type->toJson();
        return album;
    }
};


#endif

#pragma clang diagnostic pop