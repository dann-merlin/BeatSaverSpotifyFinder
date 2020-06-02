#include "bssf.h"
#include <algorithm>
#include <bs_search.h>
#include <iostream>
#include <string>

std::vector<spotifyget::songdata> bssf::songs;
std::vector<BsSong> bssf::beatsaver_songs;
static BeatCver& beatcver = BeatCver::getInstance();
static int song_counter = 0;
static int all_song_count = 0;

static std::string simplifystring(std::string s) {
	std::string new_str = "";
	for(char& character : s) {
		if((character <= 'z' && character >= 'a') || /*character == ' ' ||*/ (character >= '0' && character <= '9')) new_str.append(1, character);
		if(character <= 'Z' && character >= 'A') new_str.append(1, character - ('Z' - 'z'));
	}
	return new_str;
}

void find_beatsaver_song(spotifyget::songdata songdata) {
	spotifyget::songdata searchfor = spotifyget::songdata{simplifystring(songdata.artist), simplifystring(songdata.songname)};
	int max_sites = 2;
	std::vector<BsSong> song_list = beatcver.searchText(songdata.songname + " " + songdata.artist, max_sites);
	if(searchfor.artist == "" || searchfor.songname == "") return;
	std::vector<BsSong> filtered_song_list;
	int entry_count = 0;
	for(BsSong& song : song_list) {
		std::cerr << "[" << std::to_string(song_counter) << "/" << std::to_string(all_song_count) << "] ... " << "Beatsaver entries: [" << std::to_string(++entry_count) << "/" << std::to_string(song_list.size()) << "]\r";
		std::flush(std::cerr);
		std::string bs_simple_song = simplifystring(song.metadata.songName);
		std::string bs_simple_artist = simplifystring(song.metadata.songAuthorName);
		if(bs_simple_song == "" || bs_simple_artist == "") continue;
		bool name, artist;
		name = (bs_simple_song.find(searchfor.songname) != std::string::npos) ||
			(searchfor.songname.find(bs_simple_song) != std::string::npos);
		artist = (bs_simple_artist.find(searchfor.artist) != std::string::npos) ||
			(searchfor.artist.find(bs_simple_artist) != std::string::npos);
		if(name && artist) filtered_song_list.push_back(song);
	}
	if(filtered_song_list.size()) std::cout << "Found " << std::to_string(filtered_song_list.size()) << " Beatsaver Beatmaps for Song \"" << songdata.songname << "\" by \"" << songdata.artist << "\":" << std::endl;
	int count = 0;
	for(auto& song : filtered_song_list) {
		std::cout << "Entry " << std::to_string(++count) << ":" << std::endl;
		std::cout << "Song: " << song.metadata.songName << std::endl;
		std::cout << "Artist: " << song.metadata.songAuthorName << std::endl;
		std::cout << "Key: " << song.key << std::endl;
		std::cout << "Link: " << song.directDownload << std::endl << std::endl;
	}
}

void bssf::start(std::string playlistURI) {
	while(playlistURI != "") {
		songs.clear();
		spotifyget::get_playlist(playlistURI, &songs);
		if(!songs.size()) {
			std::cerr << "Playlist is either empty or invalid." << std::endl;
		} else {
			song_counter = 0;
			all_song_count = songs.size();
			for(auto song : songs) {
				song_counter++;
				std::cerr << "[" << std::to_string(song_counter) << "/" << std::to_string(songs.size()) << "] ...\r";
				std::flush(std::cerr);
				find_beatsaver_song(song);
			}
			std::cerr << "Finished with the playlist. Give me the next one! (Empty line to exit)" << std::endl;
		}
		std::cin >> playlistURI;
	}
}


static void usage(int argc, char** argv) {
	std::cout << "Usage: " << std::string(argv[0]) << " spotifyplaylist_uri" << std::endl;
	exit(EXIT_SUCCESS);
}

int main(int argc, char** argv) {
	if(argc != 2 || std::string(argv[1]) == "--help" || std::string(argv[1]) == "-h") {
		usage(argc, argv);
	}
	beatcver.init();
	bssf::start(std::string(argv[1]));
	std::cerr << "Have fun!" << std::endl;
	exit(EXIT_SUCCESS);
}
