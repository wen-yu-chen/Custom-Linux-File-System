#define FUSE_USE_VERSION 26
#include <errno.h>
#include <sys/stat.h>
#include<stdio.h>
#include <cstring>
#include <vector>
#include <iostream>

#include "myfuse.h"
#include "../libWad/Wad.h"


static Wad* temp;


MyFuse* MyFuse::instance() {
	MyFuse* mf = new MyFuse();
	return mf;
}

MyFuse::MyFuse() {
}

MyFuse::~MyFuse() {
	delete temp;
}

void MyFuse::createWad(const std::string &path) {
	temp = Wad::loadWad(path);
}

int MyFuse::getattr_callback(const char *path, struct stat *stbuf) {
	memset(stbuf, 0, sizeof(struct stat));

	//path represents a directory
	if (temp->isDirectory(path) || (strcmp(path, "..") == 0) || (strcmp(path, ".") == 0)) {
		stbuf->st_mode = S_IFDIR | 0755;
		stbuf->st_nlink = 2;
		stbuf->st_size = 0;
		return 0;
	}

	//path represents a file
	if (temp->isContent(path)) {
		stbuf->st_mode = S_IFREG | 0777;
		stbuf->st_nlink = 1;
		stbuf->st_size = temp->getSize(path);
		return 0;
	}

	return -ENOENT;
}

int MyFuse::readdir_callback(const char *path, void *buf, fuse_fill_dir_t filler, off_t offset, struct fuse_file_info *fi) {
	(void) offset;
	(void) fi;
	std::vector<std::string> v;
        int num = temp->getDirectory(path, &v);

	filler(buf, ".", NULL, 0);
	filler(buf, "..", NULL, 0);

	for (int i = 0; i < num; i++) {
		filler(buf, v[i].c_str(), NULL, 0);
	}

	return 0;
}

int MyFuse::open_callback(const char *path, struct fuse_file_info *fi) {
  	return 0;
}

int MyFuse::read_callback(const char *path, char *buf, size_t size, off_t offset, struct fuse_file_info *fi) {
	if (temp->isContent(path)) {
		size_t len = temp->getSize(path);
		int num;

		if (offset >= len) {
			return 0;
		}

		if ((offset + size) > len) {
			num = temp->getContents(path, buf, len, offset);
			return num;
		}

		num = temp->getContents(path, buf, len, size);
		return num;
	}

	return -ENOENT;
}
