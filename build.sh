#!/bin/sh
dir=$(dirname "$(readlink -f $0)")

die() {
	echo "$1"
	exit 1
}

[ "$OS" == "Windows_NT" ] || [ -f "$dir/build/Makefile" ] || "$dir/configure.sh" $@

cmake --build "$dir/build" -j4
