#include "wrap.h"


int wrap_getattr_callback(const char *path, struct stat *stbuf) {
	return MyFuse::instance()->getattr_callback(path, stbuf);
}

int wrap_readdir_callback(const char *path, void *buf, fuse_fill_dir_t filler, off_t offset, struct fuse_file_info *fi) {
	return MyFuse::instance()->readdir_callback(path, buf, filler, offset, fi);
}

int wrap_open_callback(const char *path, struct fuse_file_info *fi) {
	return MyFuse::instance()->open_callback(path, fi);
}

int wrap_read_callback(const char *path, char *buf, size_t size, off_t offset, struct fuse_file_info *fi) {
	return MyFuse::instance()->read_callback(path, buf, size, offset, fi);
}

void wrap_createWad(const std::string &path) {
	return MyFuse::instance()->createWad(path);
}
