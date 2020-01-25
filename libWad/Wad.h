#include <vector>
#include <string>

#pragma once

//class that stores data of file or directory
struct FileNode {
	unsigned int offset;
	unsigned int length;
	std::string name;
	unsigned char* contents;
	bool isDirectory;

	FileNode* prev;
	std::vector<FileNode> children;
};

class Wad {

public:
	//variables I added
	std::string magic;
	unsigned int numOfDescriptor;
	unsigned int descriptorOffset;
	std::vector<FileNode> v;
	FileNode fn;

	//required functions
	static Wad* loadWad(const std::string &path);
	std::string getMagic();
	bool isContent(const std::string &path);
	bool isDirectory(const std::string &path);
	int getSize(const std::string &path);
	int getContents(const std::string &path, char *buffer, int length, int offset = 0);
	int getDirectory(const std::string &path, std::vector<std::string> *directory);

	//functions I added
	unsigned int handleLittleEndian(unsigned char* data);
	bool isEMDirectory(FileNode &fn);
	bool isRegularDirectory(FileNode &fn, int x);
	void constructFileSystem(FileNode &fn, std::vector<FileNode> v);
	void printDirStruct(FileNode &fn);
	FileNode isValidPath(const std::string &path);
};
