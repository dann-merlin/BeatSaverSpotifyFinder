#include "bssf.h"
#include <algorithm>
#include <bs_search.h>
#include <bs_curl.h>
#include <cctype>
#include <iostream>
#include <fstream>
#include <string>
#include <thread>
#include <getopt.h>

#ifdef _WIN32
#define PATH_SEPERATOR "\\"
#else
#define PATH_SEPERATOR "/"
#endif

bssf::cmdline_args bssf::args;

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

	std::cout << "Download Folder: " << bssf::args.download_folder << std::endl;
	std::string filename = std::string(&(query[query.find_last_of("/")+1]));
	std::string filepath = bssf::args.download_folder + PATH_SEPERATOR + filename;
	/*
	size_t filename_len = filepath.size() + 1;
	char *path = new char[filename_len];
#ifdef _WIN32
	strncpy_s(path, filename_len, filepath.c_str(), filename_len);
#else
	strncpy(path, filepath.c_str(), filename_len);
#endif
	*/
	std::cout << "trying to open path: " << filepath << std::endl;
	std::ofstream file_out(filepath, std::ios::binary);
	file_out.write(res.responseText.c_str(), sizeof(char) * res.responseText.size());
	file_out.flush();
	file_out.close();
	//delete path;
}

bool bssf::apply_filters(BsSong& song) {
	if(args.like_ratio > song.stats.rating) return false;
	if(args.filter_min_downloads > song.stats.downloads) return false;
	return true;
}

void bssf::findBeatsaverSong(spotifyget::songdata songdata) {
	spotifyget::songdata searchfor = spotifyget::songdata{simplifystring(songdata.artist), simplifystring(songdata.songname)};
	if(searchfor.artist == "" || searchfor.songname == "") return;
	int max_sites = 2;
	std::vector<BsSong> song_list = beatcver.searchText(songdata.songname + " " + songdata.artist, max_sites); 
	std::vector<BsSong> filtered_song_list;
	int entry_count = 0;
	for(BsSong& song : song_list) { // TODO also compare with song.name
		std::string simple_entry_name = simplifystring(song.name);
		bool name = simple_entry_name.find(searchfor.songname) != std::string::npos;
		bool artist = simple_entry_name.find(searchfor.artist) != std::string::npos;
		if(isDuplicate(song)) continue;
		song_set.insert(song.key);
		std::cerr << "[" << std::to_string(song_counter) << "/" << std::to_string(all_song_count) << "] ... " << "Beatsaver entries: [" << std::to_string(++entry_count) << "/" << std::to_string(song_list.size()) << "]\r";
		std::flush(std::cerr);
		std::string bs_simple_song = simplifystring(song.metadata.songName);
		std::string bs_simple_artist = simplifystring(song.metadata.songAuthorName);
		if(bs_simple_song == "" || bs_simple_artist == "") continue;
		name |= (bs_simple_song.find(searchfor.songname) != std::string::npos) ||
			(searchfor.songname.find(bs_simple_song) != std::string::npos);
		artist |= (bs_simple_artist.find(searchfor.artist) != std::string::npos) ||
			(searchfor.artist.find(bs_simple_artist) != std::string::npos);
		if(name && artist) {
			if(apply_filters(song) != args.invert_results) {
				filtered_song_list.push_back(song);
			}
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
		if(args.download_songs) {
			std::thread *t = new std::thread(downloadToFile, song.directDownload);
			download_threads.push_back(t);
		}
	}
}

void bssf::start() {
	std::string playlistURI = args.playlistURI;
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
	std::cout << "Usage: " << std::string(argv[0]) << " [ <OPTIONS> ] <SpotifyURI of a playlist>" << std::endl << std::endl;

	std::cout << "OPTIONS:" << std::endl;

	std::cout << "\t--download <path to download folder>" << std::endl;
	std::cout << "\t\tDirectly download the zip files to the given path. Default: current directory" << std::endl << std::endl;

	std::cout << "\t--like-ratio <ratio>" << std::endl;
	std::cout << "\t\tFilter out songs, whose like/dislike ratio is worse than \'ratio\'. Default: 0.00 (no songs get filtered out)" << std::endl << std::endl;

	std::cout << "\t--filter-min-downloads <downloadcount>" << std::endl;
	std::cout << "\t\tFilter out songs, that don't have more than 'downloadcount' songs." << std::endl << std::endl;

	std::cout << "\t--invert-results" << std::endl;
	std::cout << "\t\tInvert the filters provided by -r and -m" << std::endl << std::endl;

	exit(EXIT_SUCCESS);
}

static void parse_args(int argc, char** argv) {
	if(1 == argc) usage(argc, argv);
	std::vector<option> options;
	std::string options_short = "";
	options.push_back(option{"download", required_argument, nullptr, 'd'});
	options.push_back(option{"like-ratio", optional_argument, nullptr, 'r'});
	options.push_back(option{"invert-results", no_argument, nullptr, 'i'});
	options.push_back(option{"filter-min-downloads", required_argument, nullptr, 'm'});
	options.push_back(option{"help", no_argument, nullptr, 'h'});
	options.push_back(option{nullptr, 0, nullptr, 0});

	for(option& o : options) {
		if(o.val) options_short.append(1, (char) o.val);
	}

	int opt_code, indexptr;
	while(-1 != ( opt_code = getopt_long(argc, argv, options_short.c_str(), options.data(), &indexptr))) {
		switch(opt_code) {
			case 'h':
				usage(argc, argv);
				break;
			case 'm':
				bssf::args.filter_min_downloads = std::stoul(std::string(optarg));
				break;
			case 'd':
				bssf::args.download_songs = true;
				if(optarg) std::cout << "settings the download_folder to: " << std::string(optarg) << std::endl;
				if(optarg) bssf::args.download_folder = std::string(optarg);
				break;
			case 'i':
				bssf::args.invert_results = true;
				break;
			case 'r':
				{
				float ratio = (nullptr == optarg) ? 0.f : std::stof(std::string(optarg));
				bssf::args.like_ratio = ratio;
				}
				break;
			default:
				std::cerr << "This is weird... the option \"" << "-" << std::to_string((char)opt_code) << "\" was recognized, I can't handle it. Whatever I'll just forget this..." << std::endl;
		}
	}

	bssf::args.playlistURI = std::string(argv[argc-1]);
	std::cout << "PlaylistURI: " << bssf::args.playlistURI << " was detected" << std::endl;
}

int main(int argc, char** argv) {
	parse_args(argc, argv);
	beatcver.init();
	bssf::start();
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
