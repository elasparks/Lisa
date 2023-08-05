add_library(entities STATIC entity/Album.h entity/Artist.h entity/Playlist.h entity/Track.h entity/AlbumContent.h entity/AlbumType.h entity/PlaylistContent.h)

set_target_properties(entities PROPERTIES LINKER_LANGUAGE CXX)

target_include_directories(entities PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(entities nlohmann_json::nlohmann_json)