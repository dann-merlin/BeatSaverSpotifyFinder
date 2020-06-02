#pragma once

#include <cstring>
#include <vector>
#include <string>

class spotifyget {
	public: 
		struct songdata {
			std::string artist, songname;
		};
	private:
		static std::string access_token, token_type, scope;
		static int expires_in;

		static bool authorization();
		static bool retrievePlaylistArea(std::string playlistURI, std::vector<spotifyget::songdata> *songs, int offset, int limit);
		static void retrievePlaylist(std::string playlistURI, std::vector<spotifyget::songdata>* songs);

	public:

		static void get_playlist(std::string playlistURI, std::vector<songdata> *songs);

};
