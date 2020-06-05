cmake_minimum_required(VERSION 3.12)

set(CMAKE_CXX_STANDARD 14)
if(UNIX)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -lcurl -lpthread")
endif(UNIX)

project(beatsaverspotifyfinder VERSION 0.0.1.1 DESCRIPTION "Find beatsaver songs for spotify playlists")

find_package(nlohmann_json 3.2.0 REQUIRED)
find_package(CURL REQUIRED)

if(UNIX)
	include(GNUInstallDirs)
	find_package(PkgConfig REQUIRED)
	pkg_check_modules(BEATCPPVER REQUIRED beatcppver)
endif(UNIX)

if(WIN32)
	#set(BEATCPPVER_STATIC 1)
	add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/deps/beatcppver")
endif(WIN32)

set(SOURCE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/src/bssf.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/src/spotifyget.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/src/b64_encode.cpp")
set(HEADER_FILES "${CMAKE_CURRENT_SOURCE_DIR}/include/bssf.h" "${CMAKE_CURRENT_SOURCE_DIR}/include/spotifyget.h" "${CMAKE_CURRENT_SOURCE_DIR}/include/b64_encode.h" )

add_executable(bssf ${SOURCE_FILES} ${HEADER_FILES})

message("curl dir: ${CURL_INCLUDE_DIR}")

target_link_libraries(bssf nlohmann_json::nlohmann_json)
target_link_libraries(bssf CURL::libcurl)
message("source dir is: ${CMAKE_CURRENT_SOURCE_DIR}")
target_include_directories(bssf PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include")
target_include_directories(bssf PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/deps/beatcppver/include")
target_link_libraries(bssf beatcppver)
if(WIN32)
target_link_libraries(bssf -static-libgcc -static-libstdc++)
endif(WIN32)

set_target_properties(bssf PROPERTIES VERSION ${PROJECT_VERSION})

install(TARGETS bssf RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}")
