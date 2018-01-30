1. Enable nested virtualization if docker is running as a VM else skip to 2.
	follow this guide for windows
	https://www.timothygruber.com/hyper-v-2/run-a-nested-vm-on-kvm-qemu-vm-in-hyper-v/
	
	follow this guide for linux
	https://lalatendu.org/2015/11/01/kvm-nested-virtualization-in-fedora-23/
	
2. install docker and all the shabang

3. clone this repository

4. docker build -t <path/to/Dockerfile>/<docker_image_name>

5. docker run -it -d --privileged -p 8080:8080 -p 6901:6901 -p 5901:5901 -v <your_local_workspace_dir>:/workspace <docker_image_name>

	PS. you can use --restart=always if you want the image to keep on booting with your machine

	
