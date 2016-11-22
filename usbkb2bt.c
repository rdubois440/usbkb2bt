/* Todo


	Suppport multiple targets - See how to use it practically
	Pair device and associate with key n - Left Alt + Left Control + Right Shift + Numeric key n
   	Read config from /proc/bus/input/devices

Later
	usb by pass mode - using an arduino micro with i2c
	Port to bluez5
	Port to openwrt or buildroot
	Mouse support (on combined mouse / keyboard)

Done
	Full keyboard coverage
	Modifier keys

	if no target paired, wait for one target to be paired, and connect to it (identify the address of the paired device)
	automatic connect to a device if already paired

	Pairing process
	Easy paring from device
	Alt Control Shift - unpair all devices - wait for new device

*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <dirent.h>
#include <linux/input.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/select.h>
#include <sys/time.h>
#include <termios.h>
#include <signal.h>

#define MODE_SINGLE 0
#define MODE_DOUBLE 1
#define MODE_NUMERIC 2

#define TARGET1	 "FC:19:10:FE:DE:9F"
#define TARGET2	 "F0:5A:09:33:9D:ED"
#define TARGET3	 "6C:F3:73:54:98:C7"
#define TARGET4	 "24:FD:52:33:D4:26"
#define TARGET5	 
#define TARGET6	 
#define TARGET7	 
#define TARGET8	 
#define TARGET9	 
#define TARGET0	 



void handler (int sig)
{
	printf ("nexiting...(%d)n", sig);
	//	exit (0);
}

void perror_exit (char *error)
{
	perror (error);
	handler (9);
}

int main (int argc, char *argv[])
{
	struct input_event ev[64];
	int fdk, rd, size = sizeof (struct input_event);
	char name[256] = "Unknown";
	char *kbdDevice = NULL;
	int i;

	fd_set fds;

	int isLeftShiftDown = 0;
	int isRightShiftDown = 0;
	int isShiftDown = 0;
	int isControlDown = 0;
	int isAltDown = 0;
	int isWinDown = 0;

	int  target;
 	int deviceCount; 

	//signal(SIGKILL, SIG_IGN);
	//signal(SIGSTOP, SIG_IGN);
	//signal(SIGHUP, SIG_IGN);
	//signal(SIGINT, SIG_IGN);

	unsigned char buf[12];
	unsigned char paired_device_name[20];

	int connected = 0;

	//Setup check
	if (argv[1] == NULL){
		printf("Usage : fpad /dev/input/event_number\n");
		printf("Please specify (on the command line) the path to the dev event interface device\n");
		printf("See /proc/bus/input/devices\n");
		exit (0);
	}

	kbdDevice = argv[1];

	if ((getuid ()) != 0)
	{
		printf ("You are not root! This will not work...\n");
		exit (0);
	}



	//Open Keyboard
	if ((fdk = open (kbdDevice, O_RDONLY)) == -1)
		printf ("%s is not a vaild device.n", kbdDevice);

	printf ("After open\n");
	ioctl (fdk, EVIOCGNAME (sizeof (name)), name);
	printf ("Reading From : %d %s (%s)\n", fdk, kbdDevice, name);



	/*while ((deviceCount = getPairedDevices(paired_device_name)) == 0)
	{
		printf("Waiting for a device to connect\n");
		sleep(5);
	}

	printf("%d devices currently paired\n");

	if (deviceCount > 1)
	{
		printf("Multiple devices paired. Don't know which one to use\n");
		exit(1);
	}

	if (deviceCount == 1)
	{

	memset(paired_device_name, 0, sizeof(paired_device_name));
	getPairedDevices(paired_device_name);


	//bt_connect("FC:19:10:FE:DE:9F"); // duo
	//bt_connect("F0:5A:09:33:9D:ED"); // tab27
	//bt_connect("6C:F3:73:54:98:C7"); // mini S3
	//bt_connect("24:FD:52:33:D4:26"); // Atos PC
	paired_device_name[17] = 0;
	printf("Trying to connect to %s \n", paired_device_name);
	bt_connect(paired_device_name);
*/

	while (1)
	{
		FD_ZERO(&fds);
		FD_SET(fdk, &fds);

		buf[0] = 0;	
		buf[2] = 1;

		select (fdk + 1, &fds, NULL, NULL, NULL);
//		printf("Select returns\n");
	
		for (i=0; i<6; i++)
		{
			ev[i].type = 0;
			ev[i].code = 0;
			ev[i].value = 0;
		}

		if (FD_ISSET(fdk, &fds))
		{
			//printf("kbd has something to say\n");	
			if ((rd = read (fdk, ev, size * 64)) < size)
				perror_exit ("read()");     
			//kbd_action = 1;
		}

		for (i=0; i<(rd / size); i++)
		{
			printf ("%d of %d - Code[%x %x %x]\n", i, rd/size, ev[i].type, ev[i].code, ev[i].value);
			if (ev[i].type == 1) 
			{
			unsigned char bt_code;
			unsigned char modifier = 0;

			if (ev[i].code == 0x2a)
			{
				isLeftShiftDown = ev[i].value;
				isShiftDown = ev[i].value;
				continue;
			}

			if (ev[i].code == 0x36)
			{
				isRightShiftDown = ev[i].value;
				isShiftDown = ev[i].value;
				continue;
			}

			if ((ev[i].code == 0x1d) ||  (ev[i].code == 0x61))
			{
				isControlDown = ev[i].value;
				continue;
			}

			if ((ev[i].code == 0x38) ||  (ev[i].code == 0x64))
			{
				isAltDown = ev[i].value;
				continue;
			}

			if ((ev[i].code == 0x7d) ||  (ev[i].code == 0x7e))
			{
				isWinDown = ev[i].value;
				continue;
			}

			if (isControlDown && isAltDown && isShiftDown && (ev[i].code == 0x03))	
			//if (ev[i].code == 0x62)
			{
				if (target == 2)
				{
					printf("Already connected to tab27 - nothing to do\n");
				}
				else
				{
				bt_disconnect();
				bt_connect("F0:5A:09:33:9D:ED"); // tab27
				printf("Connecting to tab27\n");
				connected = 1;
				target = 2;
				}
			}

			if (isControlDown && isAltDown && isShiftDown && (ev[i].code == 0x04))	
			//if (ev[i].code == 0x37)	
			{
				if (target == 3)
				{
					printf("Already connected to mini S3 - nothing to do\n");
				}
				else
				{
				bt_disconnect();
				bt_connect("6C:F3:73:54:98:C7"); // mini S3
				printf("Connecting to mini S3\n");
				connected = 1;
				target = 3;
				}
			}

			if (isControlDown && isAltDown && isShiftDown && (ev[i].code == 0x05))	
			{
				if (target == 4)
				{
					printf("Already connected to Atos PC - nothing to do\n");
				}
				else
				{
				bt_disconnect();
				bt_connect("24:FD:52:33:D4:26"); // Atos PC
				printf("Connecting to  Atos PC");
				connected = 1;
				target = 4;
				}
			}
	
			// numeric keypads throw tons of 0x45. they must be ignored
			if (ev[i].code == 0x45)	continue;


			if (ev[i].code == 0x01)	bt_code = 0x29; //Esc

			if (ev[i].code == 0x3b)	bt_code = 0x3a; //F1
			if (ev[i].code == 0x3c)	bt_code = 0x3b; //F2 
			if (ev[i].code == 0x3d)	bt_code = 0x3c; //F3 
			if (ev[i].code == 0x3e)	bt_code = 0x3d; //F4 
			if (ev[i].code == 0x3f)	bt_code = 0x3e; //F5 
			if (ev[i].code == 0x40)	bt_code = 0x3f; //F6 
			if (ev[i].code == 0x41)	bt_code = 0x40; //F7 
			if (ev[i].code == 0x42)	bt_code = 0x41; //F8 
			if (ev[i].code == 0x43)	bt_code = 0x42; //F9 
			if (ev[i].code == 0x44)	bt_code = 0x43; //F10
			if (ev[i].code == 0x57)	bt_code = 0x44; //F11
			if (ev[i].code == 0x58)	bt_code = 0x45; //F12

			if (ev[i].code == 0x29)	bt_code = 0x35; //~

			if (ev[i].code == 0x02)	bt_code = 0x1e; //1
			if (ev[i].code == 0x03)	bt_code = 0x1f; //2 
			if (ev[i].code == 0x04)	bt_code = 0x20; //2 
			if (ev[i].code == 0x05)	bt_code = 0x21; //4 
			if (ev[i].code == 0x06)	bt_code = 0x22; //5 
			if (ev[i].code == 0x07)	bt_code = 0x23; //6 
			if (ev[i].code == 0x08)	bt_code = 0x24; //7 
			if (ev[i].code == 0x09)	bt_code = 0x25; //8 
			if (ev[i].code == 0x0a)	bt_code = 0x26; //9 
			if (ev[i].code == 0x0b)	bt_code = 0x27; //0 

			if (ev[i].code == 0x0c)	bt_code = 0x2d; //-
			if (ev[i].code == 0x0d)	bt_code = 0x2e; //+
			if (ev[i].code == 0x0e)	bt_code = 0x2a; //Back space

			if (ev[i].code == 0x0f)	bt_code = 0x2b; //Tab 

			if (ev[i].code == 0x10)	bt_code = 0x14; //q
			if (ev[i].code == 0x11)	bt_code = 0x1a; //w 
			if (ev[i].code == 0x12)	bt_code = 0x08; //e 
			if (ev[i].code == 0x13)	bt_code = 0x15; //r 
			if (ev[i].code == 0x14)	bt_code = 0x17; //t 
			if (ev[i].code == 0x15)	bt_code = 0x1c; //y 
			if (ev[i].code == 0x16)	bt_code = 0x18; //u 
			if (ev[i].code == 0x17)	bt_code = 0x0c; //i 
			if (ev[i].code == 0x18)	bt_code = 0x12; //o 
			if (ev[i].code == 0x19)	bt_code = 0x13; //p 

			if (ev[i].code == 0x1a)	bt_code = 0x2f; //[ 
			if (ev[i].code == 0x1b)	bt_code = 0x30; //] 
			if (ev[i].code == 0x1c)	bt_code = 0x28; //Enter 


			if (ev[i].code == 0x1e)	bt_code = 0x04; //a
			if (ev[i].code == 0x1f)	bt_code = 0x16; //s 
			if (ev[i].code == 0x20)	bt_code = 0x07; //d 
			if (ev[i].code == 0x21)	bt_code = 0x09; //f 
			if (ev[i].code == 0x22)	bt_code = 0x0a; //g 
			if (ev[i].code == 0x23)	bt_code = 0x0b; //h 
			if (ev[i].code == 0x24)	bt_code = 0x0d; //j 
			if (ev[i].code == 0x25)	bt_code = 0x0e; //k 
			if (ev[i].code == 0x26)	bt_code = 0x0f; //l 
			if (ev[i].code == 0x27)	bt_code = 0x33; //; 

			if (ev[i].code == 0x28)	bt_code = 0x34; //\'
			if (ev[i].code == 0x2b)	bt_code = 0x31; //right back slash 

			if (ev[i].code == 0x56)	bt_code = 0x64; //left  back slash

			if (ev[i].code == 0x2c)	bt_code = 0x1d; //z
			if (ev[i].code == 0x2d)	bt_code = 0x1b; //x 
			if (ev[i].code == 0x2e)	bt_code = 0x06; //c 
			if (ev[i].code == 0x2f)	bt_code = 0x19; //v 
			if (ev[i].code == 0x30)	bt_code = 0x05; //b 
			if (ev[i].code == 0x31)	bt_code = 0x11; //n 
			if (ev[i].code == 0x32)	bt_code = 0x10; //m 

			if (ev[i].code == 0x33)	bt_code = 0x36; //, 
			if (ev[i].code == 0x34)	bt_code = 0x37; //. 
			if (ev[i].code == 0x35)	bt_code = 0x38; //\/ 

			if (ev[i].code == 0x39)	bt_code = 0x2c; //Space 

			if (ev[i].code == 0x7f)	bt_code = 0x65; //App 



			if (ev[i].code == 0x67)	bt_code = 0x52; //Up 
			if (ev[i].code == 0x6c)	bt_code = 0x51; //Down
			if (ev[i].code == 0x69)	bt_code = 0x50; //Left 
			if (ev[i].code == 0x6a)	bt_code = 0x4f; //Right 

			// Numeric Keypad
			if (ev[i].code == 0x4f)	bt_code = 0x1e; //1
			if (ev[i].code == 0x50)	bt_code = 0x1f; //2 
			if (ev[i].code == 0x51)	bt_code = 0x20; //2 
			if (ev[i].code == 0x4b)	bt_code = 0x21; //4 
			if (ev[i].code == 0x4c)	bt_code = 0x22; //5 
			if (ev[i].code == 0x4d)	bt_code = 0x23; //6 
			if (ev[i].code == 0x47)	bt_code = 0x24; //7 
			if (ev[i].code == 0x48)	bt_code = 0x25; //8 
			if (ev[i].code == 0x49)	bt_code = 0x26; //9 
			if (ev[i].code == 0x52)	bt_code = 0x27; //0 

			if (ev[i].code == 0x1c)	bt_code = 0x28; //Enter 
			if (ev[i].code == 0x0e)	bt_code = 0x2a; //Back space 
			if (ev[i].code == 0x53)	bt_code = 0x27; //Delete
			if (ev[i].code == 0x4a)	bt_code = 0x2d; //- 
			if (ev[i].code == 0x4e)	bt_code = 0x2e; //+ 


	if(isControlDown)
	{
		printf("Control is down\n");
		modifier += 1;
	}
	if (isShiftDown) 	
	{
		printf("Shift is down\n");
		modifier += 2;
	}
	if(isAltDown)
	{
		printf("Alt is down\n");
		modifier += 4;
	}

	if(isWinDown)
	{
		printf("Win is down\n");
		modifier += 8;
	}

	if (connected)
	{
		printf("Sending to bt %x %x %x\n", modifier, bt_code, ev[i].value);			
		bt_write(modifier, bt_code, ev[i].value);
	}
	else
	{
		printf("Not connected - just ignore\n");			
	}
	}	// usb message is a keystroke


	}	//loop on keys in usb message
	}	// while(1)	
} 
