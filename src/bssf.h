#pragma once

#include "spotifyget.h"
#include <beatcppver.h>
#include <set>

class bssf {
	private:
		static std::vector<spotifyget::songdata> songs;
		static std::vector<BsSong> beatsaver_songs;
		static int song_counter, all_song_count;
		static std::set<std::string> song_set;

		static bool isDuplicate(BsSong& song);
		static void findBeatsaverSong(spotifyget::songdata songdata);
	public:
		static void start(std::string playlistURI);
};
