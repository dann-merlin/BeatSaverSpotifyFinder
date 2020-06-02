#!/bin/sh
dir=$(dirname "$(readlink -f $0)" )
mkdir -p "$dir/build"
cmake -S "$dir" -B "$dir/build" -DCMAKE_EXPORT_COMPILE_COMMANDS=1
