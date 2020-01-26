# Custom-Linux-File-System

Step 0: Setup 
$ sudo apt install libfuse-dev
$ sudo chmod 666 /dev/fuse

Step 1: Download the project
Option 1: Download wadfs.tar.gz and run
$ tar zxvf wadfs.tar.gz

Option 2 Download the entire repository

Step 2: Build
To build the program, run the following command in terminal.
$ cd libWad
$ make
$ cd ..
$ cd wadfs
$ make
$ cd ..

Step3: Run
To run the program, excute the following command(need to mount a directory first).
$ ./wadfs/wadfs somewadfile.wad /some/mount/directory


*Some sample wad files are provided.
