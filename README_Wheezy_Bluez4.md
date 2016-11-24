# usbkb2bt - Bluetooth Keyboard Emulation on Raspberry PI - Raspbian Wheezy

The program works with multiple versions of linux for Raspberry Pi. Using Raspbian, the Wheezy version is the best choice.   

Jessie Raspbian works equally, but the pairing process is sometimes problematic bluez5 in Raspbian Jessie. 
This alternative README is for Bluez4 users, for example Raspbian Wheezy.     

![USB keyboard as a Bluetooth keyboard](images/usbkb2bt_20161121.jpg)


## Download the Raspbian Wheezy image 

Download Raspbian Wheezy from here
```
https://downloads.raspberrypi.org/raspbian/images/2013-07-26-wheezy-raspbian/
```

## Unzip the image

```
$ unzip 2013-07-26-wheezy-raspbian.zip 

-rw-r--r-- 1 rene rene  518475358 Nov 22 14:11 2013-07-26-wheezy-raspbian.zip
-rw-r--r-- 1 rene rene 1939865600 Jul 26  2013 2013-07-26-wheezy-raspbian.img
```

## Login as root, and copy the image to and SDCard (or use sudo)

```
dd if=2013-07-26-wheezy-raspbian.img of=/dev/mmcblk0 bs=1M
1325+0 records in
1325+0 records out
1389363200 bytes (1.4 GB) copied, 134.876 s, 10.3 MB/s
```


## Boot the Raspberry Pi
Insert the card into a Raspberry Pi, provide power, a USB keyboard, a bluetooth dongle, network, and boot the Raspberry Pi

Find the  address of the Raspberry Pi and ssh to it. I configured eth0 as "Shared to other computers" and the raspberry Pi get address 10.42.0.172
ssh to the Raspberry Pi.    
Initial password is "raspberry"


```
$ ssh pi@10.42.0.172
```

From now on, you must be connected to the internet.   

## Install git. 

```
$ sudo apt-get update
$ sudo apt-get install git
```

## Install the Program
###Clone the program into a new repository.   


```
$ git clone https://github.com/rdubois440/usbkb2bt 
Cloning into 'usbkb2bt'...
remote: Counting objects: 10, done.
remote: Compressing objects: 100% (9/9), done.
remote: Total 10 (delta 0), reused 7 (delta 0), pack-reused 0
Unpacking objects: 100% (10/10), done.
Checking connectivity... done.
```

###Change directory to the newly created directory, and check the directory content

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

###Install the required package libbluetooth-dev, and bluez libraries and utilities (quite large)


```
$ sudo apt-get install libbluetooth-dev
$ sudo apt-get install bluez bluez-utils
```

###Run make to create the program

```
$ make

$ ls -lrt uskb2bt
-rwxr-xr-x 1 pi pi 20744 Nov 22 11:22 usbkb2bt

```

## Startup Bluetooth on Raspberry PI

```
$ sudo hciconfig hci0 up
$ sudo hciconfig hci0 piscan
$ sudo hciconfig 
hci0:   Type: BR/EDR  Bus: USB
        BD Address: 00:1B:DC:0F:EC:E5  ACL MTU: 310:10  SCO MTU: 64:8
        UP RUNNING PSCAN ISCAN 
        RX bytes:1256 acl:0 sco:0 events:46 errors:0
        TX bytes:453 acl:0 sco:0 commands:45 errors:0
```


## Pair the dongle with the device

### Notice
If the dongle is already paired with the device, you can use it immediately.    

If the dongle is already paired with the device, and you want to unpair and pair again, make sure to unpair from both ends: from the Raspberry Pi and from the device.   

* To unpair from Raspberry
sudo bluez-test-device remove F0:5A:09:33:9D:ED

* To unpair from the device, use the graphical user interface on your device

### Pairing
Start the program

```
$ sudo ./bluez4-simple-agent 
Agent registered
```

Start pairing from the device. Pin code is 1234

When paing is successful from the device, stop the agent program.   

Enter the target bluetooth address in the program and rebuild


## Run the program


```
sudo ./usbkb2bt /dev/input/event0
```

Press Alt + Control + Left Shift + 2	(assuming device is associated with 2)


##autostart

Copy the file etc_init.d_usbkb2bt as below, and make it executable


```
$ sudo cp etc_init.d_usbkb2bt /etc/init.d/usbkb2bt
$ sudo chmod +x /etc/init.d/usbkb2bt
```

At this point, the program can be started and stopped as a service.   
```
$ sudo /etc/init.d/usbkb2bt start
$ sudo /etc/init.d/usbkb2bt stop
```

The program must be registered to autostart

```
sudo update-rc.d usbkb2bt defaults
```


After reboot, the program should start automatically, and does not require any ssh session anymore !    

## Happy work !


