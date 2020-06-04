#!/bin/sh
dir=$(dirname "$(readlink -f $0)" )
arg=""
echo $EDITOR | grep vim -q && arg="-DCMAKE_EXPORT_COMPILE_COMMANDS=1"
mkdir -p "$dir/build"
cmake -S "$dir" -B "$dir/build" $arg $@
