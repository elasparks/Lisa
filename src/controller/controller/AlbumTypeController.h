#ifndef LISA_ALBUMTYPECONTROLLER_H
#define LISA_ALBUMTYPECONTROLLER_H

#include <libcrow/crow.h>
#include <dao/AlbumTypeDao.h>
#include <dao_impl/AlbumTypeDaoImpl.h>

class AlbumTypeController {
private:
    std::shared_ptr<AlbumTypeDao> albumTypeDao = std::make_shared<AlbumTypeDaoImpl>();

    void createAlbumTypeRoute(crow::Crow<crow::UTF8> &app) {
        CROW_ROUTE(app, "/albums/types").methods(crow::HTTPMethod::POST)(
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
                        AlbumType albumType = AlbumType::fromJson(content);
                        uint64_t id = albumTypeDao->create(albumType);
                        res.code = 201;
                        res.add_header("Location", "/albums/types/" + std::to_string(id));
                    } catch (const std::runtime_error &error) {
                        res.code = 400;
                        res.write(error.what());
                    }
                    res.end();
                });
    }

    void updateAlbumTypeRoute(crow::Crow<crow::UTF8> &app) {
        CROW_ROUTE(app, "/albums/types/<string>").methods(crow::HTTPMethod::PUT)(
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
                        AlbumType albumType = AlbumType::fromJson(content);
                        albumTypeDao->update(id, albumType);
                        res.code = 204;
                        res.add_header("Location", "/artists/" + std::to_string(id));
                    } catch (const std::runtime_error &error) {
                        res.code = 400;
                        res.write(error.what());
                    }
                    res.end();
                });
    }

    void deleteAlbumTypeRoute(crow::Crow<crow::UTF8> &app) {
        CROW_ROUTE(app, "/albums/types/<string>").methods(crow::HTTPMethod::DELETE)(
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
                        albumTypeDao->findById(std::vector<uint64_t>{id});
                        albumTypeDao->remove(id);
                        res.code = 204;
                    } catch (...) {
                        res.code = 404;
                        res.write("404 Not Found");
                    }
                    res.end();
                });
    }

    void findAlbumTypeByIdRoute(crow::Crow<crow::UTF8> &app) {
        CROW_ROUTE(app, "/albums/types/<string>").methods(crow::HTTPMethod::GET)(
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
                        auto albumType = albumTypeDao->findById(std::vector<uint64_t>{id})[0];
                        res.write(albumType.toJson().dump());
                    } catch (...) {
                        res.code = 404;
                        res.write("404 Not Found");
                    }
                    res.end();
                });
    }

    void findAlbumTypeByNameRoute(crow::Crow<crow::UTF8> &app) {
        CROW_ROUTE(app, "/albums/types").methods(crow::HTTPMethod::GET)(
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
                        auto albumTypes = albumTypeDao->findByName(pattern, offset, limit);
                        nlohmann::json albumTypesJson = nlohmann::json::array();
                        for (const auto &albumType : albumTypes)
                            albumTypesJson.push_back(albumType.toJson());
                        res.write(albumTypesJson.dump());
                    } catch (...) {
                        res.code = 500;
                        res.write("Internal Server Error");
                    }
                    res.end();
                });
    }

public:
    explicit AlbumTypeController(crow::Crow<crow::UTF8> &app) {
        createAlbumTypeRoute(app);
        updateAlbumTypeRoute(app);
        deleteAlbumTypeRoute(app);
        findAlbumTypeByIdRoute(app);
        findAlbumTypeByNameRoute(app);
    }
};


#endif
