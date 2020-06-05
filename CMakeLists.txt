cmake_minimum_required(VERSION 3.12)

set(CMAKE_CXX_STANDARD 14)
if(UNIX)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -lcurl -lpthread")
endif(UNIX)

project(beatsaverspotifyfinder VERSION 0.0.1 DESCRIPTION "Find beatsaver songs for spotify playlists")

file(GLOB_RECURSE SOURCE_FILES "src/*.cpp")
file(GLOB_RECURSE HEADER_FILES "include/*.h")

find_package(nlohmann_json 3.2.0 REQUIRED)
find_package(CURL REQUIRED)

if(UNIX)
	include(GNUInstallDirs)
	find_package(PkgConfig REQUIRED)
	pkg_check_modules(BEATCPPVER REQUIRED beatcppver)
endif(UNIX)

if(WIN32)
	set(BEATCPPVER_STATIC 1)
	add_subdirectory(${CMAKE_SOURCE_DIR}/deps/beatcppver)
endif(WIN32)

add_executable(bssf ${SOURCE_FILES})

target_link_libraries(bssf PRIVATE nlohmann_json::nlohmann_json)
target_link_libraries(bssf PRIVATE CURL::libcurl)
target_include_directories(bssf PRIVATE ${CMAKE_SOURCE_DIR}/include)
target_include_directories(bssf PRIVATE ${CMAKE_SOURCE_DIR}/deps/beatcppver/include)
target_link_libraries(bssf PRIVATE beatcppver)

set_target_properties(bssf PROPERTIES VERSION ${PROJECT_VERSION})

