#ifndef LISA_UTFMIDDLEWARE_H
#define LISA_UTFMIDDLEWARE_H

#include <libcrow/crow.h>

struct UTFMiddleware {
    struct context {
    };

    void before_handle(crow::request & /*req*/, crow::response & /*res*/, context & /*ctx*/) {}

    void after_handle(crow::request & /*req*/, crow::response &res, context & /*ctx*/) {
        if (get_header_value(res.headers, "Content-Type").empty()) {
            res.set_header("Content-Type", "application/json; charset=utf-8");
        }
    }
};

#endif
