# usbkb2bt - Bluetooth Keyboard Emulation on Raspberry PI - Raspbian Wheezy

https://www.raspberrypi.org/downloads/raspbian/
Download Raspbian Wheezy from here
https://downloads.raspberrypi.org/raspbian/images/2013-07-26-wheezy-raspbian/

Unzip the image

rene@Sony:/home/DoNotBackup$ unzip 2013-07-26-wheezy-raspbian.zip 
Archive:  2013-07-26-wheezy-raspbian.zip
  inflating: 2013-07-26-wheezy-raspbian.img  


-rw-r--r-- 1 rene rene  518475358 Nov 22 14:11 2013-07-26-wheezy-raspbian.zip
-rw-r--r-- 1 rene rene 1939865600 Jul 26  2013 2013-07-26-wheezy-raspbian.img

Login as root, and copy the image to and SDCard (or use sudo)

```
dd if=2016-09-23-raspbian-jessie-lite.img of=/dev/mmcblk0 bs=1M
1325+0 records in
1325+0 records out
1389363200 bytes (1.4 GB) copied, 134.876 s, 10.3 MB/s
```

Insert the card into a Raspberry Pi, provide power, a USB keyboard, a bluetooth dongle, network, and boot the Raspberry Pi

Find the  address of the Raspberry Pi and ssh to it. In my case, I configured my eth0 as "Shared to other computers" and the raspberry Pi get address 10.42.0.172
ssh to the Raspberry Pi. 
Initial password is "raspberry"


```
rene@Sony:/home/DoNotBackup$ ssh pi@10.42.0.172
The authenticity of host '10.42.0.172 (10.42.0.172)' can't be established.
ECDSA key fingerprint is db:0b:8a:2d:a1:1c:da:d7:52:ed:dd:9b:04:f2:b1:b3.
Are you sure you want to continue connecting (yes/no)? yes
Warning: Permanently added '10.42.0.172' (ECDSA) to the list of known hosts.
pi@10.42.0.172's password: 

The programs included with the Debian GNU/Linux system are free software;
the exact distribution terms for each program are described in the
individual files in /usr/share/doc/*/copyright.

Debian GNU/Linux comes with ABSOLUTELY NO WARRANTY, to the extent
permitted by applicable law.
pi@raspberrypi:~ $ 
```

From now on, you must be connected to the internet.   
Install git. 

```
pi@raspberrypi:~ $ sudo apt-get update
pi@raspberrypi:~ $ sudo apt-get install git
```

Clone the program into a new repository.   


```
pi@raspberrypi:~ $ git clone https://github.com/rdubois440/usbkb2bt 
Cloning into 'usbkb2bt'...
remote: Counting objects: 10, done.
remote: Compressing objects: 100% (9/9), done.
remote: Total 10 (delta 0), reused 7 (delta 0), pack-reused 0
Unpacking objects: 100% (10/10), done.
Checking connectivity... done.
```

Change directory to the newly created directory, and check the directory content

```
$ cd usbkb2bt
$ ls -lrt
total 40
-rw-r--r-- 1 pi pi   56 Nov 22 11:18 README.md
-rw-r--r-- 1 pi pi  259 Nov 22 11:18 Makefile
-rw-r--r-- 1 pi pi 4256 Nov 22 11:18 hid.h
-rw-r--r-- 1 pi pi 6798 Nov 22 11:18 btkbdemu.c
-rwxr-xr-x 1 pi pi 3620 Nov 22 11:18 bluez-simple-agent
-rw-r--r-- 1 pi pi 9327 Nov 22 11:18 usbkb2bt.c

```

Install the required package libbluetooth-dev   


```
$ sudo apt-get install libbluetooth-dev
```

Build with make to create the program

```
$ make

$ ls -lrt uskb2bt
-rwxr-xr-x 1 pi pi 20744 Nov 22 11:22 usbkb2bt

```

Configure the BT address in the program, run make again, and connect to a device that is already paired with the dongle


sudo ./usbkb2bt /dev/input/event0


##autostart





Login as root, and copy the image to and SDCard (or use sudo)

```
dd if=2016-09-23-raspbian-jessie-lite.img of=/dev/mmcblk0 bs=1M
1325+0 records in
1325+0 records out
1389363200 bytes (1.4 GB) copied, 134.876 s, 10.3 MB/s
```

Insert the card into a Raspberry Pi, provide power, a USB keyboard, a bluetooth dongle, network, and boot the Raspberry Pi
ssh to the Raspberry Pi. 

Find the  address of the Raspberry Pi and ssh to it. In my case, I configured my eth0 as "Shared to other computers" and the raspberry Pi get address 10.42.0.172
Initial password is "raspberry"


```
rene@Sony:/home/DoNotBackup$ ssh pi@10.42.0.172
The authenticity of host '10.42.0.172 (10.42.0.172)' can't be established.
ECDSA key fingerprint is db:0b:8a:2d:a1:1c:da:d7:52:ed:dd:9b:04:f2:b1:b3.
Are you sure you want to continue connecting (yes/no)? yes
Warning: Permanently added '10.42.0.172' (ECDSA) to the list of known hosts.
pi@10.42.0.172's password: 

The programs included with the Debian GNU/Linux system are free software;
the exact distribution terms for each program are described in the
individual files in /usr/share/doc/*/copyright.

Debian GNU/Linux comes with ABSOLUTELY NO WARRANTY, to the extent
permitted by applicable law.
pi@raspberrypi:~ $ 
```

From now on, you must be connected to the internet.   
Install git. 

```
pi@raspberrypi:~ $ sudo apt-get update
pi@raspberrypi:~ $ sudo apt-get install git
```

Clone the program into a new repository.   


```
pi@raspberrypi:~ $ git clone https://github.com/rdubois440/usbkb2bt 
Cloning into 'usbkb2bt'...
remote: Counting objects: 10, done.
remote: Compressing objects: 100% (9/9), done.
remote: Total 10 (delta 0), reused 7 (delta 0), pack-reused 0
Unpacking objects: 100% (10/10), done.
Checking connectivity... done.
```

Change directory to the newly created directory, and check the directory content

```
$ cd usbkb2bt
$ ls -lrt
total 40
-rw-r--r-- 1 pi pi   56 Nov 22 11:18 README.md
-rw-r--r-- 1 pi pi  259 Nov 22 11:18 Makefile
-rw-r--r-- 1 pi pi 4256 Nov 22 11:18 hid.h
-rw-r--r-- 1 pi pi 6798 Nov 22 11:18 btkbdemu.c
-rwxr-xr-x 1 pi pi 3620 Nov 22 11:18 bluez-simple-agent
-rw-r--r-- 1 pi pi 9327 Nov 22 11:18 usbkb2bt.c

```

Install the required package libbluetooth-dev   


```
$ sudo apt-get install libbluetooth-dev
```

Build with make to create the program

```
$ make

$ ls -lrt uskb2bt
-rwxr-xr-x 1 pi pi 20744 Nov 22 11:22 usbkb2bt

```

Configure the BT address in the program, run make again, and connect to a device that is already paired with the dongle


sudo ./usbkb2bt /dev/input/event0


##autostart









# Jessie Version
## Download a fresh image

Download Jessie Light from the web site



unzip the img file

-rw-r--r-- 1 rene rene 1389363200 Sep 27 06:45 2016-09-23-raspbian-jessie-lite.img
-rw-r--r-- 1 rene rene  306010843 Nov 22 11:53 2016-09-23-raspbian-jessie-lite.zip

Login as root, and copy the image to and SDCard (or use sudo)

```
dd if=2016-09-23-raspbian-jessie-lite.img of=/dev/mmcblk0 bs=1M
1325+0 records in
1325+0 records out
1389363200 bytes (1.4 GB) copied, 134.876 s, 10.3 MB/s
```

Insert the card into a Raspberry Pi, provide power, a USB keyboard, a bluetooth dongle, network, and boot the Raspberry Pi
ssh to the Raspberry Pi. 

Find the  address of the Raspberry Pi and ssh to it. In my case, I configured my eth0 as "Shared to other computers" and the raspberry Pi get address 10.42.0.172
Initial password is "raspberry"


```
rene@Sony:/home/DoNotBackup$ ssh pi@10.42.0.172
The authenticity of host '10.42.0.172 (10.42.0.172)' can't be established.
ECDSA key fingerprint is db:0b:8a:2d:a1:1c:da:d7:52:ed:dd:9b:04:f2:b1:b3.
Are you sure you want to continue connecting (yes/no)? yes
Warning: Permanently added '10.42.0.172' (ECDSA) to the list of known hosts.
pi@10.42.0.172's password: 

The programs included with the Debian GNU/Linux system are free software;
the exact distribution terms for each program are described in the
individual files in /usr/share/doc/*/copyright.

Debian GNU/Linux comes with ABSOLUTELY NO WARRANTY, to the extent
permitted by applicable law.
pi@raspberrypi:~ $ 
```

From now on, you must be connected to the internet.   
Install git. 

```
pi@raspberrypi:~ $ sudo apt-get update
pi@raspberrypi:~ $ sudo apt-get install git
```

Clone the program into a new repository.   


```
pi@raspberrypi:~ $ git clone https://github.com/rdubois440/usbkb2bt 
Cloning into 'usbkb2bt'...
remote: Counting objects: 10, done.
remote: Compressing objects: 100% (9/9), done.
remote: Total 10 (delta 0), reused 7 (delta 0), pack-reused 0
Unpacking objects: 100% (10/10), done.
Checking connectivity... done.
```

Change directory to the newly created directory, and check the directory content

```
$ cd usbkb2bt
$ ls -lrt
total 40
-rw-r--r-- 1 pi pi   56 Nov 22 11:18 README.md
-rw-r--r-- 1 pi pi  259 Nov 22 11:18 Makefile
-rw-r--r-- 1 pi pi 4256 Nov 22 11:18 hid.h
-rw-r--r-- 1 pi pi 6798 Nov 22 11:18 btkbdemu.c
-rwxr-xr-x 1 pi pi 3620 Nov 22 11:18 bluez-simple-agent
-rw-r--r-- 1 pi pi 9327 Nov 22 11:18 usbkb2bt.c

```

Install the required package libbluetooth-dev   


```
$ sudo apt-get install libbluetooth-dev
```

Build with make to create the program

```
$ make

$ ls -lrt uskb2bt
-rwxr-xr-x 1 pi pi 20744 Nov 22 11:22 usbkb2bt

```

Configure the BT address in the program, run make again, and connect to a device that is already paired with the dongle


sudo ./usbkb2bt /dev/input/event0


##autostart



