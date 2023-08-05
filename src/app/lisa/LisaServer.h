#ifndef LISA_LISASERVER_H
#define LISA_LISASERVER_H

#include <libcrow/crow.h>
#include <lisa/configuration/Configuration.h>
#include <pgconnection/PGConnectionPool.h>
#include <controller/ArtistController.h>
#include <controller/AlbumController.h>
#include <controller/AlbumTypeController.h>
#include <controller/TrackController.h>
#include <controller/PlaylistController.h>
#include <controller/AlbumContentController.h>
#include <controller/PlaylistContentController.h>

class LisaServer {
private:
    crow::Crow<crow::UTF8> app;
    Configuration configuration;

    void initializeConnectionPull();

public:
    LisaServer();

    void start();
};


#endif
