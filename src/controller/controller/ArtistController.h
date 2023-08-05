#ifndef LISA_ARTISTCONTROLLER_H
#define LISA_ARTISTCONTROLLER_H

#include <libcrow/crow.h>
#include <dao/ArtistDao.h>
#include <dao_impl/ArtistDaoImpl.h>

class ArtistController {
private:
    std::shared_ptr<ArtistDao> artistDao = std::make_shared<ArtistDaoImpl>();

    void createArtistRoute(crow::Crow<crow::UTF8> &app) {
        CROW_ROUTE(app, "/artists").methods(crow::HTTPMethod::POST)(
                [this](const crow::request &req, crow::response &res) {
                    nlohmann::json content;
                    try {
                        content = nlohmann::json::parse(req.body);
                    } catch (...) {
                        res.code = 400;
                        res.write("Expected JSON!");
                        res.end();
                        return;
                    }
                    try {
                        Artist artist = Artist::fromJson(content);
                        uint64_t id = artistDao->create(artist);
                        res.code = 201;
                        res.add_header("Location", "/artists/" + std::to_string(id));
                    } catch (const std::runtime_error& error) {
                        res.code = 400;
                        res.write(error.what());
                    }
                    res.end();
                });
    }

    void updateArtistRoute(crow::Crow<crow::UTF8> &app) {
        CROW_ROUTE(app, "/artists/<string>").methods(crow::HTTPMethod::PUT)(
                [this](const crow::request &req, crow::response &res, const std::string &idString) {
                    uint64_t id;
                    try {
                        id = std::stoull(idString);
                    } catch (...) {
                        res.code = 400;
                        res.write("400 Bad Request");
                        res.end();
                        return;
                    }
                    nlohmann::json content;
                    try {
                        content = nlohmann::json::parse(req.body);
                    } catch (...) {
                        res.code = 400;
                        res.write("Expected JSON!");
                        res.end();
                        return;
                    }
                    try {
                        Artist artist = Artist::fromJson(content);
                        artistDao->update(id, artist);
                        res.code = 204;
                    } catch (const std::runtime_error& error) {
                        res.code = 400;
                        res.write(error.what());
                    }
                    res.end();
                });
    }

    void deleteArtistRoute(crow::Crow<crow::UTF8> &app) {
        CROW_ROUTE(app, "/artists/<string>").methods(crow::HTTPMethod::DELETE)(
                [this](const crow::request &req, crow::response &res, const std::string &idString) {
                    uint64_t id;
                    try {
                        id = std::stoull(idString);
                    } catch (...) {
                        res.code = 400;
                        res.write("400 Bad Request");
                        res.end();
                        return;
                    }
                    try {
                        artistDao->findById(std::vector<uint64_t>{id});
                        artistDao->remove(id);
                        res.code = 204;
                    } catch (...) {
                        res.code = 404;
                        res.write("404 Not Found");
                    }
                    res.end();
                });
    }

    void findArtistByIdRoute(crow::Crow<crow::UTF8> &app) {
        CROW_ROUTE(app, "/artists/<string>").methods(crow::HTTPMethod::GET)(
                [this](const crow::request &req, crow::response &res, const std::string &idString) {
                    uint64_t id;
                    try {
                        id = std::stoull(idString);
                    } catch (...) {
                        res.code = 400;
                        res.write("400 Bad Request");
                        res.end();
                        return;
                    }
                    try {
                        auto artist = artistDao->findById(std::vector<uint64_t>{id})[0];
                        res.write(artist.toJson().dump());
                    } catch (...) {
                        res.code = 404;
                        res.write("404 Not Found");
                    }
                    res.end();
                });
    }

    void findArtistByNameRoute(crow::Crow<crow::UTF8> &app) {
        CROW_ROUTE(app, "/artists").methods(crow::HTTPMethod::GET)(
                [this](const crow::request &req, crow::response &res) {
                    crow::query_string queryString(req.url_params);
                    uint64_t offset = 0, limit = 100;
                    std::string pattern;
                    if (!queryString.keys().empty()) {
                        if (queryString.get("name")) {
                            pattern = queryString.get("name");
                        }
                        if (queryString.get("offset")) {
                            try {
                                offset = std::stoull(queryString.get("offset"));
                            } catch (...) {
                                res.code = 400;
                                res.write("400 Bad Request");
                                res.end();
                                return;
                            }
                        }
                        if (queryString.get("limit")) {
                            try {
                                limit = std::stoull(queryString.get("limit"));
                                if (limit > 100)
                                    limit=100;
                            } catch (...) {
                                res.code = 400;
                                res.write("400 Bad Request");
                                res.end();
                                return;
                            }
                        }
                    }

                    try {
                        auto artists = artistDao->findByName(pattern, offset, limit);
                        nlohmann::json artistsJson = nlohmann::json::array();
                        for (const auto &artist : artists)
                            artistsJson.push_back(artist.toJson());
                        res.write(artistsJson.dump());
                    } catch (...) {
                        res.code = 404;
                        res.write("404 Not Found");
                    }
                    res.end();
                });
    }

public:
    explicit ArtistController(crow::Crow<crow::UTF8> &app) {
        createArtistRoute(app);
        updateArtistRoute(app);
        deleteArtistRoute(app);
        findArtistByIdRoute(app);
        findArtistByNameRoute(app);
    }
};


#endif
