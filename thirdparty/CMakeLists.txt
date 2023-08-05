add_library(crow STATIC libcrow/crow.h)
set_target_properties(crow PROPERTIES LINKER_LANGUAGE CXX)
target_include_directories(crow INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

set(PostgreSQL_ADDITIONAL_VERSIONS "14")
find_package(PostgreSQL REQUIRED)
add_subdirectory(libpqxx build-pqxx)

add_subdirectory(libjson build-json)