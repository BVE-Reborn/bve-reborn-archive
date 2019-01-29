#!/bin/bash

if [ $# -lt 1 ]; then
	echo "Must provide destination argument"
	exit 1
fi

DEST="$1"

mkdir -p "$DEST"

echo "Doxygen: $(doxygen --version)"
cp "docs/config/Doxyfile" "$DEST/Doxyfile"
echo "OUTPUT_DIRECTORY = $DEST" >> "$DEST/Doxyfile"
doxygen "$DEST/Doxyfile"
