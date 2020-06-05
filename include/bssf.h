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
		static bool apply_filters(BsSong& song);
		static void findBeatsaverSong(spotifyget::songdata songdata);
	public:
		struct cmdline_args {
			std::string download_folder = ".";
			std::string playlistURI = "";
			unsigned long filter_min_downloads = 0;
			float like_ratio = 0.f;
			bool download_songs = false;
			bool invert_results = false;
		};
		static cmdline_args args;

		static void start();
};
