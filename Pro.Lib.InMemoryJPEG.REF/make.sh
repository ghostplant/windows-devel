#/bin/sh
rm -rf dist
mkdir dist
gcc *.cpp -shared -o dist/memjpeg.so
cp memjpeg.h dist/memjpeg.h