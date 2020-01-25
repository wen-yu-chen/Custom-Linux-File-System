wadfs: main.cpp myfuse.cpp wrap.cpp
	g++ -D_FILE_OFFSET_BITS=64 -DFUSE_USE_VERSION=26 main.cpp -o wadfs myfuse.cpp wrap.cpp -L ../libWad -lWad -lfuse
