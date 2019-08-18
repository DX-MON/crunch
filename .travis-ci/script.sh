#!/bin/bash -e
export PS4="$ "
set -x

if [ "$ENGINE" == "make" ]; then
	make test
	make check
elif [ "$ENGINE" == "meson" ]; then
	cd build
	ninja test
	ninja install
fi
