cmake_minimum_required(VERSION 3.12)

set(CMAKE_CXX_STANDARD 14)
set(BEATCPPVER_DIRECTORY "" CACHE STRING "Set the folder where beatcppver is installed, if applicable")
if(UNIX)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -lcurl -lpthread")
endif(UNIX)

project(beatsaverspotifyfinder VERSION 0.0.1 DESCRIPTION "Find beatsaver songs for spotify playlists")


file(GLOB_RECURSE SOURCE_FILES "src/*.cpp")
file(GLOB_RECURSE HEADER_FILES "include/*.h")

add_executable(bssf ${SOURCE_FILES})

find_package(nlohmann_json 3.2.0 REQUIRED)
find_package(CURL REQUIRED)

if(BEATCPPVER_DIRECTORY STREQUAL "")
if(UNIX)
	include(GNUInstallDirs)
	find_package(PkgConfig REQUIRED)
	pkg_check_modules(BEATCPPVER REQUIRED beatcppver)
endif(UNIX)

if(WIN32)
	include(FetchContent)
	FetchContent_Declare(
		beatcppver
		GIT_REPOSITORY https://gitlab.cs.fau.de/yp98ocaw/beatcppver.git
		GIT_TAG v0.0.1.1
		)
	FetchContent_GetProperties(beatcppver)
	if (NOT beatcppver_POPULATED)
		FetchContent_Populate(beatcppver)
	endif()
	add_subdirectory(${beatcppver_SOURCE_DIR} ${beatcppver_BINARY_DIR})
	include_directories("${beatcppver_SOURCE_DIR}/include")
	link_directories("${beatcppver_BINARY_DIR}/Debug")
endif(WIN32)

else()

find_library(BEATCPPVER beatcppver PATHS "${BEATCPPVER_DIRECTORY}")
#include_directories("${BEATCPPVER_DIRECTORY}/include")
#link_directories("${BEATCPPVER_DIRECTORY}/bin")
include_directories("${BEATCPPVER_DIRECTORY}/include")
#link_directories("${BEATCPPVER_LIB}")
endif()

target_link_libraries(bssf PRIVATE nlohmann_json::nlohmann_json)
target_link_libraries(bssf PRIVATE CURL::libcurl)
target_include_directories(bssf PRIVATE include)
target_link_libraries(bssf PRIVATE beatcppver)

set_target_properties(bssf PROPERTIES VERSION ${PROJECT_VERSION})

