add_library(dao STATIC dao/ArtistDao.h dao/PlaylistDao.h dao/AlbumDao.h dao/TrackDao.h dao/AlbumTypeDao.h dao/AlbumContentDao.h dao/PlaylistContentDao.h)
set_target_properties(dao PROPERTIES LINKER_LANGUAGE CXX)
target_include_directories(dao PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(dao entities)

add_library(dao_impl STATIC dao_impl/ArtistDaoImpl.h dao_impl/ArtistDaoImpl.cpp dao_impl/AlbumDaoImpl.cpp dao_impl/AlbumDaoImpl.h dao_impl/AlbumTypeDaoImpl.cpp dao_impl/AlbumTypeDaoImpl.h dao_impl/TrackDaoImpl.cpp dao_impl/TrackDaoImpl.h dao_impl/PlaylistDaoImpl.cpp dao_impl/PlaylistDaoImpl.h dao_impl/AlbumContentDaoImpl.cpp dao_impl/AlbumContentDaoImpl.h dao_impl/PlaylistContentDaoImpl.cpp dao_impl/PlaylistContentDaoImpl.h)
set_target_properties(dao_impl PROPERTIES LINKER_LANGUAGE CXX)
target_include_directories(dao_impl PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(dao_impl entities pgconnection)
