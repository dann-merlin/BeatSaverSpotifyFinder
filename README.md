<h1>Beatsaver Spotify Finder</h1>
Find and download all your Spotify songs on beatsaver.com!

This is a simple command-line application for Linux/Windows, that allows you to automatically search for and download all Songs in a given Spotify Playlist

<h2>Usage:</h2>
Download the bin folder (click on it in the structure above and then on the
download icon next to "clone" and select zip under "Download this directory"). 
Open a commandline (for example with Win+R and enter cmd) and navigate to the
directory where the bssf.exe is located (you can change directory with cd)
Example (Say the bssf.exe is located in your Downloads directory):

`cd C:\Users\Peter\Downloads`

or as you probably are already in C:\Users\Peter just run:

`cd Downloads`

Then run the application with 

`.\bssf.exe`

It will show you the options you have available and how to use the program.

The following command will search the Playlist with the URI 5tucYhsJP4eThEX6ILK0Yb
and download the beatmaps, whose like to dislike ratio is greater than 0.3 (30%) and more than 60000 Downloads.
You can find out the URI of a Playlist by right-clicking it in spotify -> Share -> Copy Spotify URI
Be sure to remove the preceding "spotify:playlist:"
It will save the zip files to the directory "C:\Users\Peter\Documents".
To select the directory in your command prompt type: "." for the directory

`.\bssf.exe --download=C:\Users\Peter\Documents --like-ratio=0.3 --filter-min-downloads=60000 5tucYhsJP4eThEX6ILK0Yb`

The Program won't exit by itself. Instead it will say "Finished with the playlist. Give me the next one! ("stop" to exit)"

This means you can paste the URI of another playlist and press enter or just type "stop" to let the program exit.

When running without the download option, it won't download any zip files and just print the results it found to the commandline.

Default behaviour without options is not to download any zip files and to just
print every song regardless of the like-ratio or the download count.

Don't use invert-results. Just don't.

<h2>Dependencies:</h2>

BeatC++ver library (gets pulled automatically, if not already installed)

libcurl

nlohmann_json (header only)

Build deps:

cmake

gcc/g++

git

idk, lol. Normal build stuff...

To build for Windows:

MinGW-w64
Visual Studio Build Tools for C++

<h2>Build Instructions</h2>
<h3>Linux (Maybe also Mac and BSD)</h3>

```
# Get git, cmake, make, libcurl from your Distro. libcurl might have to be libcurl-dev or something depending on your Distro
# You can also manually install the BeatC++Ver library. If you did, you can leave out --recurse-submodules
git clone https://gitlab.cs.fau.de/yp98ocaw/beatsaverspotifyfinder.git --recurse-submodules
cd beatsaverspotifyfinder

# In case you want to install the program, you can also skip this step.
./build.sh

./install.sh  #(asks for sudo password)
```

That's it. The Binary is now installed under /usr/local/bin and the beatc++ver library under /usr/local/lib and /usr/local/include.

<h3>Windows</h3>
You could probably also build this without vcpkg. This is just how I managed to build the program on Windows 10.

You will need to install MinGW-w64: https://sourceforge.net/projects/mingw-w64/files/Toolchains%20targetting%20Win64/

Because I use getopt_long, which is not implemented in the windows libc. If someone wants to implement a windows version of parsing the commandline options, I'm happy to accept a pull request. Then you would be able to build with MSVC

Install download and install vcpkg somewhere. I will reference this folder as <vcpkg_root> from now on.

I did all of this in git-bash, but you can also use Powershell or something.

IMPORTANT:
Add this path to the PATH Environment Variable:


```
# If dumpbin is not found you have to install MSBuildTools and add the path to dumpbin to the PATH variable.
# In my case it could be found in this folder: C:\Program Files (x86)\Microsoft Visual Studio\2019\BuildTools\VC\Tools\MSVC\14.26.28801\bin\Hostx64\x64
dumpbin

cd <vcpkg_root>

# This next line might has to be run as root
./vcpkg.exe integrate install
# Copy the -DCMAKE_TOOLCHAIN_FILE Path in the output. You will need it later.

./vcpkg.exe install nlohmann-json:x64-windows

./vcpkg.exe install curl:x64-windows

# if you want to build the test:
./vcpkg.exe install catch2:x64-windows

cd <folder_where_beatsaverspotifyfinder_should_be>

git clone https://gitlab.cs.fau.de/yp98ocaw/beatsaverspotifyfinder.git --recurse-submodules

cd beatsaverspotifyfinder

./configure.sh "-DCMAKE_TOOLCHAIN_FILE=<insert the path from above>"  "-DCMAKE_BUILD_TYPE=Release" -G "MinGW Makefiles" "-DBEATCPPVER_STATIC=1"

# In case you forgot to replace the <vcpkg_root> or just called ./configure without any arguments
# it will probably tell you it doesn't find some libraries in the next step.
# In that case you have to remove the build directory: rm -rf build/
# Then you would have to rerun configure.
./build.sh

# The binary is now in the build directory. Note that if you want to copy the file somewhere else, you might also need to copy the dlls, that are also in the bin/ directory.
```


<h2>Thanks to:</h2>
nlohmann's json for the greates json header

beatsaver.com

Spotify Web API
