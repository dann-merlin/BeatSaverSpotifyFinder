#include "bssf.h"
#include <beatcppver.h>
#include <algorithm>
#include <iostream>

std::vector<spotifyget::songdata> bssf::songs;

void bssf::start(std::string playlistURI) {
	spotifyget::get_playlist(playlistURI, &songs);
}


static void usage(int argc, char** argv) {
	std::cout << "Usage: " << std::string(argv[0]) << " spotifyplaylist_uri" << std::endl;
	exit(EXIT_SUCCESS);
}

int main(int argc, char** argv) {
	if(argc != 2 || std::string(argv[1]) == "--help" || std::string(argv[1]) == "-h") {
		usage(argc, argv);
	}
	bssf::start(std::string(argv[1]));
	std::cerr << "bssf start shouldn't exit!" << std::endl;
	exit(EXIT_FAILURE);
}
