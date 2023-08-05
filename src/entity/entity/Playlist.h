#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wshadow"
#ifndef LISA_PLAYLIST_H
#define LISA_PLAYLIST_H

#include <cstdint>
#include <string>
#include <nlohmann/json.hpp>

class Playlist {
private:
    std::shared_ptr<uint64_t> id;
    std::shared_ptr<std::string> name;
    std::shared_ptr<std::string> description;
    std::shared_ptr<std::string> coverArt;

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

    [[nodiscard]] std::shared_ptr<const std::string> getDescription() const {
        return this->description;
    }

    void setDescription(const std::string &description) {
        this->description = std::make_shared<std::string>(description);
    }

    [[nodiscard]] std::shared_ptr<const std::string> getCoverArt() const {
        return this->coverArt;
    }

    void setCoverArt(const std::string &coverArt) {
        this->coverArt = std::make_shared<std::string>(coverArt);
    }

    static Playlist fromJson(const nlohmann::json &json){
        Playlist playlist;

        if (json.contains("id"))
            playlist.setId(json["id"]);
        if (json.contains("name"))
            playlist.setName(json["name"]);
        if (json.contains("description"))
            playlist.setDescription(json["description"]);

        return playlist;
    }

    [[nodiscard]] nlohmann::json toJson() const {
        nlohmann::json playlist;
        if (this->id)
            playlist["id"] = *this->id;
        if (this->name)
            playlist["name"] = *this->name;
        if (this->description)
            playlist["description"] = *this->description;
        if (this->coverArt)
            playlist["coverArt"]["url"] = *this->coverArt;
        return playlist;
    }
};


#endif

#pragma clang diagnostic pop