#pragma once

#include <vector>
#include <string>

class spotifyget {
	private:
		static bool authenticate();

	public:
		struct songdata {
			std::string artist, songname;
		};

		static void get_playlist(std::string playlistURI, std::vector<songdata> *songs);

};
