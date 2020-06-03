cmake_minimum_required(VERSION 3.12)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -lcurl -lpthread")

project(beatsaverspotifyfinder VERSION 0.0.1 DESCRIPTION "Find beatsaver songs for spotify playlists")

include(GNUInstallDirs)
find_package(PkgConfig REQUIRED)
# (beatcppver REQUIRED)
pkg_check_modules(BEATCPPVER REQUIRED beatcppver)

file(GLOB_RECURSE SOURCE_FILES "src/*.cpp")
file(GLOB_RECURSE HEADER_FILES "src/*.h")

add_executable(bssf ${SOURCE_FILES})

# include_directories(${CURL_INCLUDE_DIRS})
# link_directories(${CURL_LIBRARIES})

set_target_properties(bssf PROPERTIES VERSION ${PROJECT_VERSION})

target_include_directories(bssf PRIVATE src)
target_link_libraries(bssf beatcppver)
