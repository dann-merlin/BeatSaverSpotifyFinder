#include "spotifyget.h"
#include "b64_encode.h"
#include <cstdlib>
#include <string>
#include <vector>
#include <bs_curl.h>
#include <iostream>
#include <cstddef>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

const char *client_id = "137f5edef1b04c99904ed360362928fe";
const char *client_secret = "a11788d798b84614b4dde7485fdb1beb";
std::string spotifyget::access_token, spotifyget::token_type, spotifyget::scope;
int spotifyget::expires_in;

void die(std::string exit_msg) {
	std::cerr << exit_msg << std::endl;
	exit(EXIT_FAILURE);
}

bool spotifyget::authorization() {
	BsRequestOptions opt;
	std::string client_info = std::string(client_id) + ":" + std::string(client_secret);
	opt.headers = {"Authorization: Basic " + base64_encode((unsigned char *) client_info.c_str(), client_info.size())};
	/* std::cout << opt.headers[0] << std::endl; */
	opt.api_url = "https://accounts.spotify.com/api";
	opt.request_type = "POST";
	opt.data = "grant_type=client_credentials";
	BsResponse response = BsCurl::sendRequest("/token", opt);
	if(response.returnCode != "200") return false;

	json j = json::parse(response.responseText);
	/* std::cout << "Received Authorization Token:" << std::endl; */
	if(!j["access_token"].is_null()) access_token = j["access_token"];
	/* std::cout << "access_token:" << std::endl; */
	/* std::cout << access_token << std::endl; */
	if(!j["token_type"].is_null()) token_type = j["token_type"];
	/* std::cout << "token_type:" << std::endl; */
	/* std::cout << token_type << std::endl; */
	if(!j["expires_in"].is_null()) expires_in = j["expires_in"];
	/* std::cout << "expires_in:" << std::endl; */
	/* std::cout << std::to_string(expires_in) << std::endl; */
	if(!j["scope"].is_null()) scope = j["scope"];
	/* std::cout << "scope:" << std::endl; */
	/* std::cout << scope << std::endl; */
	return true;
}

bool spotifyget::retrievePlaylistArea(std::string playlistURI, std::vector<spotifyget::songdata> *songs, int offset, int limit) {
	BsRequestOptions opt;
	std::string client_info = std::string(client_id) + ":" + std::string(client_secret);
	opt.headers = {"Authorization: " + token_type + " " + access_token};
	opt.api_url = "https://api.spotify.com";
	opt.request_type = "GET";
	std::string query = "?limit=" + std::to_string(limit) + "&offset=" + std::to_string(offset*limit) + "&fields=items(track(artists%2Cname))%2Cnext";
	std::cerr << query << std::endl;
	BsResponse response = BsCurl::sendRequest("/v1/playlists/" + playlistURI + "/tracks" + query, opt);
	if(response.returnCode != "200") {
		std::cerr << "Spotify request sent return code: " << response.returnCode << std::endl;
		return false;
	}
	json j = json::parse(response.responseText);

	if(j["items"].is_null()) {
		std::cerr << "items not found" << std::endl;
		return false;
	}
	auto items = j["items"];
	for(auto& el : items) {
		auto track = el["track"];
		if(track["artists"].is_null() || track["name"].is_null()) {
			std::cerr << "artists or name was null" << std::endl;
			continue;
		}
		songs->push_back(spotifyget::songdata{track["artists"][0]["name"], track["name"]});
	}
	if(j["next"].is_null()) {
		std::cerr << "next is null" << std::endl;
		return false;
	}
	return true;
}

void spotifyget::retrievePlaylist(std::string playlistURI, std::vector<spotifyget::songdata>* songs) {
	const int pagesize = 100;
	int pageindex = 0;
	while(retrievePlaylistArea(playlistURI, songs, pageindex++, pagesize));

	std::cerr << "Retrieved Songs from Spotify Playlist: " << songs->size() << std::endl;
}

void spotifyget::get_playlist(std::string playlistURI, std::vector<spotifyget::songdata>* songs) {
	if(curl_global_init(CURL_GLOBAL_ALL)) {
		die("curl_global_init return errorcode! in get_playlist");
	}
	if(!authorization()) {
		die("Could not authenticate with spotify");
	}
	retrievePlaylist(playlistURI, songs);
}
