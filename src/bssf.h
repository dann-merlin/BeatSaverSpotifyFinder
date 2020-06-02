#pragma once

#include "spotifyget.h"
#include <beatcppver.h>

class bssf {
	private:
		static std::vector<spotifyget::songdata> songs;
		static std::vector<BsSong> beatsaver_songs;

	public:
		static void start(std::string playlistURI);
};
