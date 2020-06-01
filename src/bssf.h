#pragma once

#include "spotifyget.h"

class bssf {
	private:
		static std::vector<spotifyget::songdata> songs;

	public:
		static void start(std::string playlistURI);
};
