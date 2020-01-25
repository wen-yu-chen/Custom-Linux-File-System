#include <fuse.h>
#include <string>
#include "../libWad/Wad.h"


class MyFuse {
public:
	static MyFuse *instance();

        MyFuse();
        ~MyFuse();

        int getattr_callback(const char *path, struct stat *stbuf);
        int readdir_callback(const char *path, void *buf, fuse_fill_dir_t filler, off_t offset, struct fuse_file_info *fi);
        int open_callback(const char *path, struct fuse_file_info *fi);
        int read_callback(const char *path, char *buf, size_t size, off_t offset, struct fuse_file_info *fi);
	int write_callback(const char *path, const char *buf, size_t size, off_t offset, struct fuse_file_info *finfo);
	void createWad(const std::string &path);
};
