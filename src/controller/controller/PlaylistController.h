#ifndef LISA_PLAYLISTCONTROLLER_H
#define LISA_PLAYLISTCONTROLLER_H

#include <libcrow/crow.h>
#include <dao/PlaylistDao.h>
#include <dao_impl/PlaylistDaoImpl.h>

class PlaylistController {
private:
    std::shared_ptr<PlaylistDao> playlistDao = std::make_shared<PlaylistDaoImpl>();

    void createPlaylistRoute(crow::Crow<crow::UTF8> &app) {
        CROW_ROUTE(app, "/playlists").methods(crow::HTTPMethod::POST)(
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
                        Playlist playlist = Playlist::fromJson(content);
                        uint64_t id = playlistDao->create(playlist);
                        res.code = 201;
                        res.add_header("Location", "/playlists/" + std::to_string(id));
                    } catch (const std::runtime_error &error) {
                        res.code = 400;
                        res.write(error.what());
                    }
                    res.end();
                });
    }

    void updatePlaylistRoute(crow::Crow<crow::UTF8> &app) {
        CROW_ROUTE(app, "/playlists/<string>").methods(crow::HTTPMethod::PUT)(
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
                        Playlist playlist = Playlist::fromJson(content);
                        playlistDao->update(id, playlist);
                        res.code = 204;
                    } catch (const std::runtime_error &error) {
                        res.code = 400;
                        res.write(error.what());
                    }
                    res.end();
                });
    }

    void deletePlaylistRoute(crow::Crow<crow::UTF8> &app) {
        CROW_ROUTE(app, "/playlists/<string>").methods(crow::HTTPMethod::DELETE)(
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
                        playlistDao->findById(std::vector<uint64_t>{id});
                        playlistDao->remove(id);
                        res.code = 204;
                    } catch (...) {
                        res.code = 404;
                        res.write("404 Not Found");
                    }
                    res.end();
                });
    }

    void findPlaylistByIdRoute(crow::Crow<crow::UTF8> &app) {
        CROW_ROUTE(app, "/playlists/<string>").methods(crow::HTTPMethod::GET)(
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
                        auto playlist = playlistDao->findById(std::vector<uint64_t>{id})[0];
                        res.write(playlist.toJson().dump());
                    } catch (...) {
                        res.code = 404;
                        res.write("404 Not Found");
                    }
                    res.end();
                });
    }

    void findPlaylistByNameRoute(crow::Crow<crow::UTF8> &app) {
        CROW_ROUTE(app, "/playlists").methods(crow::HTTPMethod::GET)(
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
                        auto playlists = playlistDao->findByName(pattern, offset, limit);
                        nlohmann::json playlistsJson = nlohmann::json::array();
                        for (const auto &playlist : playlists)
                            playlistsJson.push_back(playlist.toJson());
                        res.write(playlistsJson.dump());
                    } catch (...) {
                        res.code = 500;
                        res.write("Internal Server Error");
                    }
                    res.end();
                });
    }

public:
    explicit PlaylistController(crow::Crow<crow::UTF8> &app) {
        createPlaylistRoute(app);
        updatePlaylistRoute(app);
        deletePlaylistRoute(app);
        findPlaylistByIdRoute(app);
        findPlaylistByNameRoute(app);
    }
};


#endif
