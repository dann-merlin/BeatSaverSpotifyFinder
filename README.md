<h1>Beatsaver Spotify Finder/h1>
Find and download all your Spotify songs on beatsaver.com!

This is a simple command-line application for Linux/Windows, that allows you to automatically search for and download all Songs in a given Spotify Playlist

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

cd <folder_where_beatcppver_should_be>

git clone https://gitlab.cs.fau.de/yp98ocaw/beatcppver.git --recurse-submodules

# You can also pass -G "MinGW Makefiles" to configure to compile the library using mingw. Both MSVC and MinGW work as of now.
./configure.sh "-DCMAKE_TOOLCHAIN_FILE=<insert the path from above>"  "-DCMAKE_BUILD_TYPE=Release" -G "MinGW Makefiles"

# In case you forgot to replace the <vcpkg_root> or just called ./configure without any arguments
# it will probably tell you it doesn't find some libraries in the next step.
# In that case you have to remove the build directory: rm -rf build/
# Then you would have to rerun configure.
./build.sh

# The binary is now in the build directory. Note that if you want to copy the file somewhere else, you also need to copy the dlls, that are also in the bin/ directory.
```


<h2>Thanks to:</h2>
nlohmann's json for the greates json header

beatsaver.com

Spotify Web API
