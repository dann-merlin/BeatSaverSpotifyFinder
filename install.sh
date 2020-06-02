#!/bin/sh
dir=$(dirname "$(readlink -f $0)")

[ -f "$dir/build/bssf" ] || "$dir/build.sh" && \
sudo cmake --install "$dir/build"
