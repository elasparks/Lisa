#ifndef LISA_TRACKCONTROLLER_H
#define LISA_TRACKCONTROLLER_H

#include <libcrow/crow.h>
#include <dao/TrackDao.h>
#include <dao_impl/TrackDaoImpl.h>

class TrackController {
private:
    std::shared_ptr<TrackDao> trackDao = std::make_shared<TrackDaoImpl>();

    void createTrackRoute(crow::Crow<crow::UTF8> &app) {
        CROW_ROUTE(app, "/tracks").methods(crow::HTTPMethod::POST)(
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
                        Track track = Track::fromJson(content);
                        track.setDuration(0);
                        track.setPlayCount(0);
                        uint64_t id = trackDao->create(track);
                        res.code = 201;
                        res.add_header("Location", "/tracks/" + std::to_string(id));
                    } catch (const std::runtime_error &error) {
                        res.code = 400;
                        res.write(error.what());
                    }
                    res.end();
                });
    }

    void updateTrackRoute(crow::Crow<crow::UTF8> &app) {
        CROW_ROUTE(app, "/tracks/<string>").methods(crow::HTTPMethod::PUT)(
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
                        Track track = Track::fromJson(content);
                        trackDao->update(id, track);
                        res.code = 204;
                    } catch (const std::runtime_error &error) {
                        res.code = 400;
                        res.write(error.what());
                    }
                    res.end();
                });
    }

    void deleteTrackRoute(crow::Crow<crow::UTF8> &app) {
        CROW_ROUTE(app, "/tracks/<string>").methods(crow::HTTPMethod::DELETE)(
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
                        trackDao->findById(std::vector<uint64_t>{id});
                        trackDao->remove(id);
                        res.code = 204;
                    } catch (...) {
                        res.code = 404;
                        res.write("404 Not Found");
                    }
                    res.end();
                });
    }

    void findTrackByIdRoute(crow::Crow<crow::UTF8> &app) {
        CROW_ROUTE(app, "/tracks/<string>").methods(crow::HTTPMethod::GET)(
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
                        auto track = trackDao->findById(std::vector<uint64_t>{id})[0];
                        res.write(track.toJson().dump());
                    } catch (...) {
                        res.code = 404;
                        res.write("404 Not Found");
                    }
                    res.end();
                });
    }

    void findTrackByNameRoute(crow::Crow<crow::UTF8> &app) {
        CROW_ROUTE(app, "/tracks").methods(crow::HTTPMethod::GET)(
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
                        auto tracks = trackDao->findByName(pattern, offset, limit);
                        nlohmann::json tracksJson = nlohmann::json::array();
                        for (const auto &artist : tracks)
                            tracksJson.push_back(artist.toJson());
                        res.write(tracksJson.dump());
                    } catch (...) {
                        res.code = 500;
                        res.write("Internal Server Error");
                    }
                    res.end();
                });
    }

    void uploadAudioRoute(crow::Crow<crow::UTF8> &app) {
        CROW_ROUTE(app, "/audio/<string>").methods(crow::HTTPMethod::PUT)(
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
                    res.code = convertAudio(id, req.body);
                    if (res.code != 204) {
                        res.end();
                        return;
                    }
                    try {
                        Track track;
                        track.setDuration(getOpusfileDuration("./tracks/"+std::to_string(id) + ".opus"));
                        this->trackDao->update(id, track);
                    } catch (...) {
                        res.code = 500;
                    }
                    res.end();
                });
    }

    void getAudioRoute(crow::Crow<crow::UTF8> &app){
        CROW_ROUTE(app, "/audio/<string>").methods(crow::HTTPMethod::GET)(
                [this](const crow::request &req, crow::response &res, const std::string &idString){
                    uint64_t id;
                    try {
                        id = std::stoull(idString);
                    } catch (...) {
                        res.code = 400;
                        res.write("400 Bad Request");
                        res.end();
                        return;
                    }
                    std::string trackPath = "./tracks/"+std::to_string(id) + ".opus";
                    try {
                        res.add_header("content-type", "audio/opus");
                        auto bytes = readAllBytes(trackPath);
                        res.write(std::string(bytes.begin(), bytes.end()));
                        res.code = 200;
                    } catch(...) {
                        res.code = 404;
                    }
                    res.end();
                });
    }

    static int convertAudio(const uint64_t &id, const std::string &data){
        std::filesystem::create_directories("./tmp");
        std::filesystem::create_directories("./tracks");
        std::string tmpPath = "./tmp/"+std::to_string(id);
        std::string trackPath = "./tracks/"+std::to_string(id) + ".opus";
        std::ofstream tmp(tmpPath);
        if (!tmp.is_open() || tmp.bad())
            return 500;
        tmp << data;
        tmp.close();
        std::remove(trackPath.c_str());
        std::string command = "ffmpeg -i " + tmpPath + " -c:a libopus " + trackPath + " > /dev/null 2>&1";
        int result = system(command.c_str());
        std::remove(tmpPath.c_str());
        if (result != 0)
            return 400;
        return 204;
    }

    static uint64_t getOpusfileDuration(const std::string &path){
        std::string command = "ffprobe -i " + path + " -show_entries format=duration -v quiet -of csv=\"p=0\"";
        std::array<char, 128> buffer{};
        std::string result;

        std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(command.c_str(), "r"), pclose);
        if (!pipe) {
            throw std::runtime_error("Can't open pipe!");
        }

        while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
            result += buffer.data();
        }

        double milliseconds = std::stod(result)*1000;
        return (uint64_t)std::round(milliseconds);
    }

    static std::vector<char> readAllBytes(const std::string& filename)
    {
        std::ifstream file(filename, std::ios::binary | std::ios::ate);
        if (!file.is_open() || file.bad() || file.fail())
            throw std::runtime_error("Can't open file!");
        std::ifstream::pos_type pos = file.tellg();

        if (pos == 0) {
            return std::vector<char>{};
        }

        std::vector<char>  result(pos);

        file.seekg(0, std::ios::beg);
        file.read(&result[0], pos);

        return result;
    }

public:
    explicit TrackController(crow::Crow<crow::UTF8> &app) {
        createTrackRoute(app);
        updateTrackRoute(app);
        deleteTrackRoute(app);
        findTrackByIdRoute(app);
        findTrackByNameRoute(app);
        uploadAudioRoute(app);
        getAudioRoute(app);
    }
};


#endif
