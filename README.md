#FreeLSS

FreeLSS is originally a laser scanning program for the Raspberry Pi. It allows a
Raspberry Pi to function as the core to a complete turn table laser scanning
system. It is designed to work with Atlas 3D laser scanner. 

This version is a modification of the original FreeLSS, adapted to
SardauScan hardware. It is designed to run on a classical Linux system (even a
Raspberry Pi ! :)), with an Arduino as the hardware interface with the laser
scanner. 

Sardauscan is great hardware, and the software (which is OpenSource) looks great
also. But it is written in C#, which makes it impractical for Linux development.
Since freelss is the original software behind Sardauscan, this is an attempt at
merging both programs, using a cross-platform environment instead of a Windows
only one. 


###COMPILE

These instructions assume you are running the latest version of Debian.  Other distros will likely require changes.

First, update the firmware to the latest version and reboot.
```
$ sudo apt-get update
$ sudo apt-get upgrade
```

Install the dependencies that are managed by the package manager.
```
$ sudo apt-get install libpng-dev libjpeg-dev cmake vlc git-core gcc build-essential unzip sqlite3 libsqlite3-dev libmicrohttpd-dev libcurl4-openssl-dev libiw-dev libssl-dev
```

Download and build FreeLSS
```
$ git clone https://github.com/phhorrein/freelss
$ cd freelss/src
$ make
```
###Running FreeLSS
The interface for FreeLSS is web based and by default runs on port 2980.  When running, access it by navigating to http://localhost:2980/ from the computer. Or access it from another machine on the network by the computer's IP or hostname.  For Example: http://raspberrypi:2980/

The following command starts FreeLSS.
```
$ ./freelss
```
