#ifndef LISA_ALBUMCONTROLLER_H
#define LISA_ALBUMCONTROLLER_H

#include <libcrow/crow.h>
#include <dao/AlbumDao.h>
#include <dao_impl/AlbumDaoImpl.h>

class AlbumController {
private:
    std::shared_ptr<AlbumDao> albumDao = std::make_shared<AlbumDaoImpl>();

    void createAlbumRoute(crow::Crow<crow::UTF8> &app) {
        CROW_ROUTE(app, "/albums").methods(crow::HTTPMethod::POST)(
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
                        Album album = Album::fromJson(content);
                        uint64_t id = albumDao->create(album);
                        res.code = 201;
                        res.add_header("Location", "/albums/" + std::to_string(id));
                    } catch (const std::runtime_error &error) {
                        res.code = 400;
                        res.write(error.what());
                    }
                    res.end();
                });
    }

    void updateAlbumRoute(crow::Crow<crow::UTF8> &app) {
        CROW_ROUTE(app, "/albums/<string>").methods(crow::HTTPMethod::PUT)(
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
                        Album album = Album::fromJson(content);
                        albumDao->update(id, album);
                        res.code = 204;
                    } catch (const std::runtime_error &error) {
                        res.code = 400;
                        res.write(error.what());
                    }
                    res.end();
                });
    }

    void deleteAlbumRoute(crow::Crow<crow::UTF8> &app) {
        CROW_ROUTE(app, "/albums/<string>").methods(crow::HTTPMethod::DELETE)(
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
                        albumDao->findById(std::vector<uint64_t>{id});
                        albumDao->remove(id);
                        res.code = 204;
                    } catch (...) {
                        res.code = 404;
                        res.write("404 Not Found");
                    }
                    res.end();
                });
    }

    void findAlbumByIdRoute(crow::Crow<crow::UTF8> &app) {
        CROW_ROUTE(app, "/albums/<string>").methods(crow::HTTPMethod::GET)(
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
                        auto album = albumDao->findById(std::vector<uint64_t>{id})[0];
                        res.write(album.toJson().dump());
                    } catch (...) {
                        res.code = 404;
                        res.write("404 Not Found");
                    }
                    res.end();
                });
    }

    void findAlbumByNameRoute(crow::Crow<crow::UTF8> &app) {
        CROW_ROUTE(app, "/albums").methods(crow::HTTPMethod::GET)(
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
                        auto albums = albumDao->findByName(pattern, offset, limit);
                        nlohmann::json albumsJson = nlohmann::json::array();
                        for (const auto &album : albums)
                            albumsJson.push_back(album.toJson());
                        res.write(albumsJson.dump());
                    } catch (...) {
                        res.code = 500;
                        res.write("Internal Server Error");
                    }
                    res.end();
                });
    }

    void findArtistAlbumsRoute(crow::Crow<crow::UTF8> &app) {
        CROW_ROUTE(app, "/artists/<string>/albums").methods(crow::HTTPMethod::GET)(
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
                    crow::query_string queryString(req.url_params);
                    uint64_t offset = 0, limit = 100;
                    std::string pattern;
                    if (!queryString.keys().empty()) {
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
                        auto albums = albumDao->findByArtist(id, offset, limit);
                        nlohmann::json albumsJson = nlohmann::json::array();
                        for (const auto &album : albums)
                            albumsJson.push_back(album.toJson());
                        res.write(albumsJson.dump());
                    } catch (...) {
                        res.code = 500;
                        res.write("Internal Server Error");
                    }
                    res.end();
                });
    }

public:
    explicit AlbumController(crow::Crow<crow::UTF8> &app) {
        createAlbumRoute(app);
        updateAlbumRoute(app);
        deleteAlbumRoute(app);
        findAlbumByIdRoute(app);
        findAlbumByNameRoute(app);
        findArtistAlbumsRoute(app);
    }
};


#endif
