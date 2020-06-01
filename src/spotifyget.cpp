#include "spotifyget.h"
#include <vector>
#include <bs_curl.h>
#include <iostream>



bool spotifyget::authenticate() {

}

void spotifyget::get_playlist(std::string playlistURI, std::vector<spotifyget::songdata>* songs) {
	if(curl_global_init(CURL_GLOBAL_ALL)) {
		std::cerr << "curl_global_init return errorcode! in get_playlist" << std::endl;
	}
}
