#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wshadow"
#ifndef LISA_ALBUMTYPE_H
#define LISA_ALBUMTYPE_H

#include <nlohmann/json.hpp>

class AlbumType {
private:
    std::shared_ptr<uint64_t> id;
    std::shared_ptr<std::string> name;
public:
    AlbumType() = default;

    AlbumType(const uint64_t &id, const std::string &name) : id(std::make_shared<uint64_t>(id)),
                                                             name(std::make_shared<std::string>(name)) {}

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

    static AlbumType fromJson(const nlohmann::json& json){
        AlbumType albumType;
        if (json.contains("id"))
            albumType.setId(json["id"]);
        if (json.contains("name"))
            albumType.setName(json["name"]);
        return albumType;
    }

    [[nodiscard]] nlohmann::json toJson() const{
        nlohmann::json result;
        if (this->id)
            result["id"] = *this->id;
        if (this->id)
            result["name"] = *this->name;
        return result;
    }
};


#endif

#pragma clang diagnostic pop