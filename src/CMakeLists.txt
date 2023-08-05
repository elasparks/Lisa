add_subdirectory(entity)
add_subdirectory(dao)
add_subdirectory(controller)
add_subdirectory(app)
add_subdirectory(utils)

add_executable(lisa main.cpp)

target_link_libraries(lisa lisa_server)