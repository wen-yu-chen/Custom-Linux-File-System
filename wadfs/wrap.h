#include "myfuse.h"
#include <fuse.h>
#include <string>


extern "C" {
	int wrap_getattr_callback(const char *path, struct stat *stbuf);
        int wrap_readdir_callback(const char *path, void *buf, fuse_fill_dir_t filler, off_t offset, struct fuse_file_info *fi);
        int wrap_open_callback(const char *path, struct fuse_file_info *fi);
        int wrap_read_callback(const char *path, char *buf, size_t size, off_t offset, struct fuse_file_info *fi);
	void wrap_createWad(const std::string &path);
}
