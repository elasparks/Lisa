#ifndef LISA_ALBUMCONTENTCONTROLLER_H
#define LISA_ALBUMCONTENTCONTROLLER_H

#include <libcrow/crow.h>
#include <dao/AlbumContentDao.h>
#include <dao_impl/AlbumContentDaoImpl.h>

class AlbumContentController {
private:
    std::shared_ptr<AlbumContentDao> albumContentDao = std::make_shared<AlbumContentDaoImpl>();

    void findContentRoute(crow::Crow<crow::UTF8> &app) {
        CROW_ROUTE(app, "/albums/<string>/content").methods(crow::HTTPMethod::GET)(
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
                        auto albumContent = this->albumContentDao->findContent(id, offset, limit);
                        res.write(albumContent.toJson().dump());
                    } catch (...) {
                        res.code = 500;
                        res.write("Internal Server Error");
                    }
                    res.end();
                });
    }

public:
    explicit AlbumContentController(crow::Crow<crow::UTF8> &app){
        findContentRoute(app);
    }
};


#endif
