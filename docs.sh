#!/bin/bash

if [ $# -lt 1 ]; then
	echo "Must provide destination argument"
	exit 1
fi

DEST="$1"

# mkdir -p "$DEST"/{doxygen,doxyrest,doxy-xml,sphinx-rst,html}
mkdir -p "$DEST"

# if [ "$(uname)" == "Darwin" ]; then
#     # Do something under Mac OS X platform
#     wget -q -c -nc https://github.com/vovkos/doxyrest/releases/download/doxyrest-1.1.1/doxyrest-1.1.1-mac.tar.xz -O "$DEST/doxyrest/doxyrest-1.1.1-mac.tar.xz"
#     tar xf "$DEST/doxyrest/doxyrest-1.1.1-mac.tar.xz" -C "$DEST/doxyrest/"
#     export DOXYREST="$DEST/doxyrest/doxyrest-1.1.1-mac/"
# elif [ "$(expr substr $(uname -s) 1 5)" == "Linux" ]; then
#     # Do something under GNU/Linux platform
#     wget -q -c -nc https://github.com/vovkos/doxyrest/releases/download/doxyrest-1.1.1/doxyrest-1.1.1-linux-amd64.tar.xz -O "$DEST/doxyrest/doxyrest-1.1.1-linux-amd64.tar.xz"
#     tar xf "$DEST/doxyrest/doxyrest-1.1.1-linux-amd64.tar.xz" -C "$DEST/doxyrest/"
#     export DOXYREST="$DEST/doxyrest/doxyrest-1.1.1-linux-amd64/"
# fi

echo "Doxygen: $(doxygen --version)"
# $DOXYREST/bin/doxyrest --version
# sphinx-build --version
cp "docs/config/Doxyfile" "$DEST/Doxyfile"
echo "OUTPUT_DIRECTORY = $DEST" >> "$DEST/Doxyfile"
doxygen "$DEST/Doxyfile"
# $DOXYREST/bin/doxyrest "$DEST/doxy-xml/xml/index.xml" -o "$DEST/sphinx-rst/index.rst" -F "$DOXYREST/share/doxyrest/frame" -f c_index.rst.in -D g_introFile=page_index.rst -D g_escapeAsterisks -D "g_indexTitle=BVE-Reborn documentation and manual"
# echo
# cp $(find . -name '*.png' | grep -v dependencies) "$DEST/sphinx-rst/"
# cp docs/sphinx-conf.py "$DEST/sphinx-rst/conf.py"
# sphinx-build -q -j $(($(nproc) + 1)) -b html "$DEST/sphinx-rst" "$DEST/html"
