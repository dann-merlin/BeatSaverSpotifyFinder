#include "bssf.h"
#include <algorithm>
#include <bs_search.h>
#include <bs_curl.h>
#include <iostream>
#include <fstream>
#include <string>
#include <thread>

std::vector<spotifyget::songdata> bssf::songs;
std::vector<BsSong> bssf::beatsaver_songs;
int bssf::song_counter = 0;
int bssf::all_song_count = 0;
static bool download = false;
std::set<std::string> bssf::song_set;
static BeatCver& beatcver = BeatCver::getInstance();
static std::vector<std::thread *> download_threads;

static std::string simplifystring(std::string s) {
	std::string new_str = "";
	for(char& character : s) {
		if((character <= 'z' && character >= 'a') || /*character == ' ' ||*/ (character >= '0' && character <= '9')) new_str.append(1, character);
		if(character <= 'Z' && character >= 'A') new_str.append(1, character - ('Z' - 'z'));
	}
	return new_str;
}

bool bssf::isDuplicate(BsSong& song) {
	return song_set.count(song.key);
}

void downloadToFile(std::string query) {
	std::cout << "Downloading song: " << query << std::endl;
	BsRequestOptions opt;
	opt.api_url = "https://beatsaver.com";
	opt.request_type = "GET";
	BsResponse res = BsCurl::sendRequest(query, opt);
	if(res.returnCode != "200") {
		std::cout << "Error Downloading song " << query << " with error code: " << res.returnCode << std::endl;
		std::cout << "Received: " << res.responseText << std::endl;
		return;
	}

	size_t filename_len = query.size() + 1;
	char filename[filename_len];
	strncpy(filename, query.c_str(), filename_len);
	std::ofstream file_out(std::string("downloaded/") + std::string(&(query[query.find_last_of("/")])), std::ios::binary);
	file_out.write(res.responseText.c_str(), sizeof(char) * res.responseText.size());
	/* file_out << res.responseText; */
	file_out.close();
}

void bssf::findBeatsaverSong(spotifyget::songdata songdata) {
	spotifyget::songdata searchfor = spotifyget::songdata{simplifystring(songdata.artist), simplifystring(songdata.songname)};
	int max_sites = 2;
	std::vector<BsSong> song_list = beatcver.searchText(songdata.songname + " " + songdata.artist, max_sites); 
	if(searchfor.artist == "" || searchfor.songname == "") return;
	std::vector<BsSong> filtered_song_list;
	int entry_count = 0;
	for(BsSong& song : song_list) { // TODO also compare with song.name
		if(isDuplicate(song)) continue;
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
		if(name && artist) {
			filtered_song_list.push_back(song);
			song_set.insert(song.key);
		}
	}
	if(filtered_song_list.size()) std::cout << "Found " << std::to_string(filtered_song_list.size()) << " Beatsaver Beatmaps for Song \"" << songdata.songname << "\" by \"" << songdata.artist << "\":" << std::endl;
	int count = 0;
	for(auto& song : filtered_song_list) {
		std::cout << "Entry " << std::to_string(++count) << ":" << std::endl;
		std::cout << "Song: " << song.metadata.songName << std::endl;
		std::cout << "Artist: " << song.metadata.songAuthorName << std::endl;
		std::cout << "Key: " << song.key << std::endl;
		std::cout << "Link: " << song.directDownload << std::endl << std::endl;
		if(download) {
			std::thread *t = new std::thread(downloadToFile, song.directDownload);
			download_threads.push_back(t);
		}
	}
}

void bssf::start(std::string p) {
	std::string playlistURI = p;
	while(playlistURI != "stop") {
		songs.clear();
		beatsaver_songs.clear();
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
				findBeatsaverSong(song); // TODO parallelize this
			}
			std::cerr << "Finished with the playlist. Give me the next one! (\"stop\" to exit)" << std::endl;
		}
		std::cin >> playlistURI;
	}
}


static void usage(int argc, char** argv) {
	std::cout << "Usage: " << std::string(argv[0]) << " spotifyplaylist_uri" << " [ --download | -d ]" << std::endl;
	exit(EXIT_SUCCESS);
}

int main(int argc, char** argv) {
	if(argc < 2 || argc > 3 || std::string(argv[1]) == "--help" || std::string(argv[1]) == "-h") {
		usage(argc, argv);
	}
	if(3 == argc) {
		if(std::string(argv[2]) == "--download" ||  std::string(argv[2]) == "-d" ) {
			download = true;
		} else {
			usage(argc, argv);
		}
	}
	beatcver.init();
	bssf::start(std::string(argv[1]));
	if(download) {
		std::cout << "Waiting for downloads to finish..." << std::endl;
		for(std::thread* t : download_threads) {
			t->join();
			delete t;
		}
	}
	std::cerr << "Have fun!" << std::endl;
	exit(EXIT_SUCCESS);
}
