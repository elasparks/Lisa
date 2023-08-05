#include "LisaServer.h"

void LisaServer::start() {
    ArtistController artistController(this->app);
    AlbumTypeController albumTypeController(this->app);
    AlbumController albumController(this->app);
    TrackController trackController(this->app);
    PlaylistController playlistController(this->app);
    PlaylistContentController playlistContentController(this->app);
    AlbumContentController albumContentController(this->app);

    this->app.port(this->configuration.getServerPort());
    this->app.multithreaded().run();
}

LisaServer::LisaServer() : configuration("configuration.json") {
    LisaServer::initializeConnectionPull();
}

void LisaServer::initializeConnectionPull() {
    PGConnectionPool::setPoolSize(this->configuration.getPoolSize());
    PGConnectionPool::setConnectionString(this->configuration.getDbHost(),
                                          std::to_string(this->configuration.getDbPort()),
                                          this->configuration.getDbName(),
                                          this->configuration.getDbUser(),
                                          this->configuration.getDbPassword());
    PGConnectionPool::getInstance();
}
