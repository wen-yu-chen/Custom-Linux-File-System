#include <fuse.h>
#include <stdio.h>
#include <iostream>

#include "wrap.h"


struct fuse_operations fp;


int main(int argc, char *argv[]) {

	//pass the wad file to myfuse and read data using the library
	wrap_createWad(argv[1]);

	//adjust argv because the second element is a wad file, and we do not want to pass it to fuse_main()
	argv[1] = argv[2];
	argv++;
	argc--;

	fp.getattr = wrap_getattr_callback;
	fp.readdir = wrap_readdir_callback;
	fp.open = wrap_open_callback;
	fp.read = wrap_read_callback;

	return fuse_main(argc, argv, &fp, NULL);
}
