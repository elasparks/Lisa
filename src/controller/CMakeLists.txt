add_library(controllers STATIC controller/ArtistController.h controller/AlbumController.h controller/AlbumTypeController.h controller/TrackController.h controller/PlaylistController.h controller/AlbumContentController.h controller/PlaylistContentController.h)
set_target_properties(controllers PROPERTIES LINKER_LANGUAGE CXX)
target_include_directories(controllers PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(controllers crow dao dao_impl)