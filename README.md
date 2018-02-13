INSTALLING
==
	
2. install docker and all the shabang

OPTION A
--------

3. clone this repository

4. docker build <path/to/Dockerfile-directory> -t <docker_image_name>

5. 5. docker run -it -d --privileged -p 8080:8080 -p 6901:6901 -p 5901:5901 -v <your_local_workspace_dir>:/workspace -v /dev:/dev <docker_image_name>

OPTION B
---------

3. docker run -it -d --privileged -p 8080:8080 -p 6901:6901 -p 5901:5901 -v <your_local_workspace_dir>:/workspace -v /dev:/dev jeanlego/os2_cs_unb


BUILDING and RUNNING kernel
==
I've updated Ken makefile so that it can do the following for you automaticaly


make all        //build the kernel from the source and run in qemu

make clean      //clean the directory	

make test       //build the kernel and run in qemu

make debug 	//build the kernel and run in qemu using gdb, stops at BRK macro (in common.h)

USING
==
	
I've provided you with the base image Ken provided us with.

you can connect to your web IDE via http://localhost:8080

you can connect to your web desktop via http://localhost:6901

you can connect to your x11 desktop via vnc client through port 5901

user: root

pass: letmein

PS. you can use --restart=always if you want the image to keep on booting with your machine

PPS. You can overwrite some or all of the default values for Cloud9 username, Cloud9 and VNC password, and Cloud9 root folder; with the following options: -e "USERNAME=<username>" -e "PASSWORD=<password>" -e "CLOUD9_ROOT_FOLDER=<path in the docker image file system you want to mount to (e.g. /workspace or /)>"
