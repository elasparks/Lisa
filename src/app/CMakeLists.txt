add_library(lisa_configuration STATIC lisa/configuration/Configuration.h lisa/configuration/Configuration.cpp)
set_target_properties(lisa_configuration PROPERTIES LINKER_LANGUAGE CXX)
target_include_directories(lisa_configuration PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(lisa_configuration nlohmann_json::nlohmann_json)

add_library(lisa_server STATIC lisa/LisaServer.h lisa/LisaServer.cpp)
set_target_properties(lisa_server PROPERTIES LINKER_LANGUAGE CXX)
target_include_directories(lisa_server PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(lisa_server lisa_configuration crow pgconnection controllers nlohmann_json::nlohmann_json)