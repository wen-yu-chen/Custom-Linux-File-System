#include <iostream>
#include <stack>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

#include "Wad.h"

using namespace std;

Wad* Wad::loadWad(const string &path) {
	Wad* w = new Wad();
	w->fn.name = "/";//initialize root directory
	int fd = open(path.c_str(), O_RDONLY);

	if (fd == -1) {
		cout << "File not foud" << endl;
		return nullptr;
	}

	//buffer
	unsigned char* temp = new unsigned char[4];//maybe size of 5 instead?

	//read magic
	read(fd, temp, 4);
	w->magic = string(reinterpret_cast<char*>(temp));

	//read number of descriptors
	read(fd, temp, 4);
	w->numOfDescriptor = w->handleLittleEndian(temp);

	//read descriptor offset
	read(fd, temp, 4);
	w->descriptorOffset = w->handleLittleEndian(temp);


	lseek(fd, w->descriptorOffset, SEEK_SET);
	//buffer 2
	unsigned char* temp2 = new unsigned char[8];//maybe size of 9 instead?

	for (int i = 0; i < w->numOfDescriptor; i++) {
		FileNode node;

		//read element offset
		read(fd, temp, 4);
		node.offset = w->handleLittleEndian(temp);

		//read element length
		read(fd, temp, 4);
		node.length = w->handleLittleEndian(temp);

		//read element name
		read(fd, temp2, 8);
		node.name = string(reinterpret_cast<char*>(temp2));

		w->v.push_back(node);
	}

	for (int i = 0; i < w->numOfDescriptor; i++) {
		if (w->v[i].length == 0) {
			w->v[i].isDirectory = true;
			continue;
		}

		//buffer 3
		unsigned char* temp3 = new unsigned char[w->v[i].length + 1]();//make a room for '\0'

		lseek(fd, w->v[i].offset, SEEK_SET);
		read(fd, temp3, w->v[i].length);

		w->v[i].contents = temp3;
	}

	w->constructFileSystem(w->fn, w->v);
	close(fd);

	return w;
}

unsigned int Wad::handleLittleEndian(unsigned char* data) {
	unsigned int x = 0;

        return x += data[0] | (data[1] << 8) | (data[2] << 16) | (data[3] << 24);
}

bool Wad::isEMDirectory(FileNode &fn) {
	if ((fn.name.length() == 4) && (fn.name[0] == 'E') && (fn.name[1] > 48 || fn.name[1] < 57) && (fn.name[2] == 'M') && (fn.name[3] > 48 || fn.name[3] < 57)) {
		return true;
	}

	return false;
}

bool Wad::isRegularDirectory(FileNode &fn, int x) {
	string s;
	string temp = "";

	if (x == 1) {
		s = "TRATS_";
	}
	else {
		s = "DNE_";
	}

	for (int i = fn.name.length() - 1; i > 0; i--) {
		temp += fn.name[i];

		if (temp == s) {
			return true;
		}
	}

	return false;
}

void Wad::constructFileSystem(FileNode &fn, vector<FileNode> v) {
	FileNode* temp = &fn;
	stack <int> st;//stores counters for number of elements following after E#M#

	for (int i = 0; i < v.size(); i++) {

		//end of E#M#
		if ((st.size() != 0) && (st.top() == 0) && (temp->prev != nullptr)) {
			temp = temp->prev;
			st.pop();
		}

		//_END
        	if (isRegularDirectory(v[i], 2)) {
                	temp = temp->prev;
			continue;
        	}

		//E#M#
		if (isEMDirectory(v[i])) {
			temp->children.push_back(v[i]);
                	temp->children.back().prev = temp;

			temp = &(temp->children.back());
			int count = 11;
			st.push(count);
		}
		//_START
		else if (isRegularDirectory(v[i], 1)) {
			string s = "";
			for (int j = 0; j < v[i].name.length() - 6; j++) {
				s += v[i].name[j];
			}
			v[i].name = s;
			temp->children.push_back(v[i]);
                	temp->children.back().prev = temp;

			temp = &(temp->children.back());
		}
		//file
		else {
			temp->children.push_back(v[i]);
                	temp->children.back().prev = temp;
		}

                if ((st.size() != 0) && (st.top() != 0)) {
                        st.top()--;
                }
	}
}

void Wad::printDirStruct(FileNode &fn) {

	cout << "Children of " << fn.name << endl;
	for (int i = 0; i < fn.children.size(); i++) {
		cout << fn.children[i].name << endl;
	}


	for (int i = 0; i < fn.children.size(); i++) {
		if (fn.children[i].length == 0) {
			printDirStruct(fn.children[i]);
		}
	}
}

FileNode Wad::isValidPath(const string &path) {
	FileNode* temp = &fn;
	FileNode invalid;//a FileNode object returned when the path is invalid
	bool validPath = true;
	int count = 0;
	int x;

	//empty path or path not starting with /
	if ((path.length() == 0) || (path[count] != '/')) {
		return invalid;
	}

	//root directory
	if ((path.length() == 1) && (path[count] == '/')) {
		return *temp;
	}

	count++;

	while(count != path.length()) {
		string s = "";

		while (path[count] != '/') {
			s += path[count];
			count++;

			if (count == path.length()) {
				count--;
				break;
			}
		}

		count++;

		for (int i = 0; i < temp->children.size(); i++) {
			if (temp->children[i].name == s) {
				temp = &(temp->children[i]);
				validPath = true;
				break;
			}
			else {
				validPath = false;
			}
		}

		if ((temp->children.size() == 0) && (count != path.length())) {
			return invalid;
		}
	}

	if (validPath) {
		return *temp;
	}

	return invalid;
}

string Wad::getMagic() {
        return magic;
}

bool Wad::isContent(const string &path) {
	FileNode temp = isValidPath(path);

	//not a valid path
	if (temp.name == "") {
		return false;
	}

	//is a directory
	else if (temp.length == 0) {
		return false;
	}

	//is a file
	else {
		return true;
	}
}

bool Wad::isDirectory(const string &path) {
	FileNode temp = isValidPath(path);

	//not a valid path
	if (temp.name == "") {
		return false;
	}

	//is a file
	else if (temp.length != 0) {
		return false;
	}

	//is a direcotry
	else {
		return true;
	}
}

int Wad::getSize(const string &path) {
	if (!isContent(path)) {
		return -1;
	}

	FileNode temp = isValidPath(path);
	return temp.length;
}

int Wad::getContents(const string &path, char *buffer, int length, int offset) {
	if (!isContent(path)) {
		return -1;
	}

	FileNode temp = isValidPath(path);
	int newLength;
	int count = 0;

	if (offset >= temp.length) {
		offset = 0;
	}

	newLength = offset + length;

	if (temp.length < newLength) {
		newLength = temp.length;
	}

	for (int i = offset; i < newLength; i++) {
		buffer[count] = temp.contents[i];
		count++;
	}

	return count;
}

int Wad::getDirectory(const string &path, vector<string> *directory) {
	if (!isDirectory(path)) {
		return -1;
	}

	FileNode temp = isValidPath(path);

	for (int i = 0; i < temp.children.size(); i++) {
		(*directory).push_back(temp.children[i].name);
	}

	return (*directory).size();
}
