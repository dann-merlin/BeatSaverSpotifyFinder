#!/bin/sh
dir=$(dirname "$(readlink -f $0)")

die() {
	echo "$1"
	exit 1
}

[ -f "$dir/build/Makefile" ] || "$dir/configure.sh"

cmake --build "$dir/build" --parallel
