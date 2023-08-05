#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wshadow"
#ifndef LISA_ARTIST_H
#define LISA_ARTIST_H

#include <cstdint>
#include <string>
#include <utility>
#include <nlohmann/json.hpp>

class Artist {
private:
    std::shared_ptr<uint64_t> id;
    std::shared_ptr<std::string> name;
    std::shared_ptr<std::string> about;
    std::shared_ptr<std::string> coverArt;
public:

    Artist() = default;

    explicit Artist(const uint64_t &id) : id(std::make_shared<uint64_t>(id)) {}

    Artist(const uint64_t &id, const std::string &name,
           const std::string &about, const std::string &coverArt) : id(std::make_shared<uint64_t>(id)),
                                                                    name(std::make_shared<std::string>(name)),
                                                                    about(std::make_shared<std::string>(about)),
                                                                    coverArt(std::make_shared<std::string>(coverArt)) {}

    [[nodiscard]] std::shared_ptr<const uint64_t> getId() const {
        return id;
    }

    void setId(uint64_t id) {
        this->id = std::make_shared<uint64_t>(id);
    }

    [[nodiscard]] std::shared_ptr<const std::string> getName() const {
        return this->name;
    }

    void setName(const std::string &name) {
        this->name = std::make_shared<std::string>(name);
    }

    [[nodiscard]] std::shared_ptr<const std::string> getAbout() const {
        return about;
    }

    void setAbout(const std::string &about) {
        this->about = std::make_shared<std::string>(about);
    }

    [[nodiscard]] std::shared_ptr<const std::string> getCoverArt() const {
        return coverArt;
    }

    void setCoverArt(const std::string &coverArt) {
        this->coverArt = std::make_shared<std::string>(coverArt);
    }

    static Artist fromJson(const nlohmann::json &json){
        Artist artist;
        if (json.contains("id"))
            artist.setId(json["id"]);
        if (json.contains("profile")){
            if (json["profile"].contains("name"))
                artist.setName(json["profile"]["name"]);
            if (json["profile"].contains("about"))
                artist.setAbout(json["profile"]["about"]);
        }
        return artist;
    }

    [[nodiscard]] nlohmann::json toJson() const {
        nlohmann::json artist;
        if (this->id)
            artist["id"] = *this->id;
        if (this->name)
            artist["profile"]["name"] = *this->name;
        if (this->about)
            artist["profile"]["about"] = *this->about;
        if (this->coverArt)
            artist["coverArt"]["url"] = *this->coverArt;
        return artist;
    }
};


#endif

#pragma clang diagnostic pop