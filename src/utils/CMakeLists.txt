add_library(pgconnection STATIC pgconnection/PGConnectionPool.cpp pgconnection/PGConnectionPool.h)
set_target_properties(pgconnection PROPERTIES LINKER_LANGUAGE CXX)
target_include_directories(pgconnection PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(pgconnection pqxx pq)