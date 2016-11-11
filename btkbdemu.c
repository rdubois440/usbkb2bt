// Todo
//

// Notes
// input character is read from stdin in non canonical mode. See set_conio_terminal_mode
// input can be 1 or more bytes
// output is the usb standard defined in USB - HID Usage Tables




#include <errno.h>
#include <unistd.h>
#include <malloc.h>
#include <syslog.h>
#include <signal.h>
#include <getopt.h>
#include <sys/poll.h>
#include <sys/ioctl.h>
#include <sys/socket.h>

#include <linux/input.h>

#include <bluetooth/bluetooth.h>
#include <bluetooth/hci.h>
#include <bluetooth/hci_lib.h>
#include <bluetooth/l2cap.h>
//#include <bluetooth/sdp.h>
#include <bluetooth/hidp.h>

#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/select.h>
#include <termios.h>

#include "hid.h"
//#include "sdp.h"


#include <fcntl.h>
//#include <cstdlib>
#include <stdio.h>
#include <string.h>
//#include "usb.h"

struct termios orig_termios;

#define DEBUG_LEVEL 0

#define PROTOCOL_MOUSE 2
#define PROTOCOL_KEYBOARD 1

//using namespace std;

struct usb_bus *busses,*bus,*dbus;
char translateKey(char);
char getRepeatedKey(char);

int cs;
int is;

void reset_terminal_mode()
{
	tcsetattr(0, TCSANOW, &orig_termios);
}

void set_conio_terminal_mode()
{
	struct termios new_termios;

	/* take two copies - one for now, one for later */
	tcgetattr(0, &orig_termios);
	memcpy(&new_termios, &orig_termios, sizeof(new_termios));

	/* register cleanup handler, and set the new terminal mode */
	atexit(reset_terminal_mode);
	cfmakeraw(&new_termios);
	tcsetattr(0, TCSANOW, &new_termios);
}

int kbhit()
{
	struct timeval tv = { 0L, 10000L };
	fd_set fds;
	FD_ZERO(&fds);
	FD_SET(0, &fds);
	return select(1, &fds, NULL, NULL, &tv);
}

int getch()
{
	int r;
	unsigned char c;
	if ((r = read(0, &c, sizeof(c))) < 0) {
		{
			//printf("Error - getch returns %x \r\n", c);	
			return r;
		}
	} else {
		//printf("getch returns %02x", c);	
		return c;
	}
}

/*
 *  taken from BlueZ
 */
static int l2cap_listen(const bdaddr_t *bdaddr, unsigned short psm, int lm, int backlog)
{
	struct sockaddr_l2 addr;
	struct l2cap_options opts;
	int sk;

	perror("before socket:");
	if ((sk = socket(PF_BLUETOOTH, SOCK_SEQPACKET, BTPROTO_L2CAP)) < 0)
	{
		perror("after socket error:");
		return -1;
	}
	perror("after socket success:");
	fprintf(stderr, "socket returns %d\n", sk);
	memset(&addr, 0, sizeof(addr));
	addr.l2_family = AF_BLUETOOTH;
	bacpy(&addr.l2_bdaddr, bdaddr);
	addr.l2_psm = htobs(psm);
	//fprintf(stderr, "before bind %s\n", bdaddr);

	if (bind(sk, (struct sockaddr *) &addr, sizeof(addr)) < 0) {
		perror("after bind error:");
		close(sk);
		return -1;
	}
	perror("after bind success:");

	setsockopt(sk, SOL_L2CAP, L2CAP_LM, &lm, sizeof(lm));

	memset(&opts, 0, sizeof(opts));
	opts.imtu = HIDP_DEFAULT_MTU;
	opts.omtu = HIDP_DEFAULT_MTU;
	opts.flush_to = 0xffff;

	setsockopt(sk, SOL_L2CAP, L2CAP_OPTIONS, &opts, sizeof(opts));

	if (listen(sk, backlog) < 0) {
		close(sk);
		return -1;
	}
	perror("after listen:");

	return sk;
}

/*
 *  taken from BlueZ
 */
static int l2cap_connect(bdaddr_t *src, bdaddr_t *dst, unsigned short psm)
{
	struct sockaddr_l2 addr;
	struct l2cap_options opts;
	int sk;

	if ((sk = socket(PF_BLUETOOTH, SOCK_SEQPACKET, BTPROTO_L2CAP)) < 0)
		return -1;

	memset(&addr, 0, sizeof(addr));
	addr.l2_family  = AF_BLUETOOTH;
	bacpy(&addr.l2_bdaddr, src);

	if (bind(sk, (struct sockaddr *) &addr, sizeof(addr)) < 0) {
		close(sk);
		return -1;
	}
	perror("after bind:");

	memset(&opts, 0, sizeof(opts));
	opts.imtu = HIDP_DEFAULT_MTU;
	opts.omtu = HIDP_DEFAULT_MTU;
	opts.flush_to = 0xffff;

	setsockopt(sk, SOL_L2CAP, L2CAP_OPTIONS, &opts, sizeof(opts));

	memset(&addr, 0, sizeof(addr));
	addr.l2_family  = AF_BLUETOOTH;
	bacpy(&addr.l2_bdaddr, dst);
	addr.l2_psm = htobs(psm);

	if (connect(sk, (struct sockaddr *) &addr, sizeof(addr)) < 0) {
		close(sk);
		return -1;
	}
	perror("after connect:");

	return sk;
}

/*
 *  taken from BlueZ
 */
static int l2cap_accept(int sk, bdaddr_t *bdaddr)
{
	struct sockaddr_l2 addr;
	socklen_t addrlen;
	int nsk;

	fprintf(stderr, "Inside l2cap_accept socket %d\n", sk);

	memset(&addr, 0, sizeof(addr));
	addrlen = sizeof(addr);

	if ((nsk = accept(sk, (struct sockaddr *) &addr, &addrlen)) < 0)
	{
		perror("accept returns error");
		return -1;
	}

	fprintf(stderr, "accept returns success\n");

	if (bdaddr)
		bacpy(bdaddr, &addr.l2_bdaddr);

	return nsk;
}



void bt_write(unsigned char modifier, unsigned char c, char action)
{

	unsigned char pkg[12];


	//unsigned char pkg[12];
		pkg[0] = 0xa1;
		pkg[1] = 0x01;
		pkg[2] = modifier;
		pkg[3] = 0x00;
	//	pkg[4] = 0x4; // Clear the key code
		pkg[5] = 0x00;
		pkg[6] = 0x00;
		pkg[7] = 0x00;
		pkg[8] = 0x00;
		pkg[9] = 0x00;

		if (action == 1)
		{
			printf("Pressing key\n\r");
			pkg[4] = c; // Set the key code
		}

		else
		{
			printf("Releasing key\n\r");
			pkg[4] = 0; // Clear the key code
		}

		if (write(is, pkg, 10) <= 0) 
		{
			perror("write");
			exit(-1);
		}
		printf("Press F12 to exit\n\n\r");
}

//mode	0	-s server mode. listens for connections
//mode	1	-c connect mode. initiates connections

int bt_listen(char *target)
{
	//unsigned char pkg[12];
	bdaddr_t dst;
	//char event_file[256] = {0};

	printf("\nbtkbdemu DEM0 (c) Collin R. Mulliner <collin@betaversion.net> http://www.mulliner.org/bluetooth/\n\n");

	//sdp_open();
	//sdp_add_keyboard();
	//printf("sdp record created\n\n");



	cs = l2cap_listen(BDADDR_ANY, L2CAP_PSM_HIDP_CTRL, 0, 1);
	//cs = l2cap_listen(&dst, L2CAP_PSM_HIDP_CTRL, 0, 1);
	perror("HID listen control channel");
	is = l2cap_listen(BDADDR_ANY, L2CAP_PSM_HIDP_INTR, 0, 1);
	//is = l2cap_listen(&dst, L2CAP_PSM_HIDP_INTR, 0, 1);
	perror("HID listen interrupt channel");
	cs = l2cap_accept(cs, NULL);
	perror("HID control channel");
	is = l2cap_accept(is, NULL);
	perror("HID interupt channel");
	printf("Connection received ! ");

	return EXIT_SUCCESS;
}


int bt_connect(char *target)
{
	bdaddr_t dst;

	printf("\nbtkbdemu DEM0 (c) Collin R. Mulliner <collin@betaversion.net> http://www.mulliner.org/bluetooth/\n\n");

	//sdp_open();
	//sdp_add_keyboard();
	//printf("sdp record created\n\n");


	fprintf(stderr, "Trying to connect to %s \n", target);
	str2ba(target, &dst);

	cs = l2cap_connect(BDADDR_ANY, &dst, L2CAP_PSM_HIDP_CTRL);
	perror("HID control channel\n");
	is = l2cap_connect(BDADDR_ANY, &dst, L2CAP_PSM_HIDP_INTR);
	perror("HID interupt channel");
	printf("Connection created ! \n");

	return EXIT_SUCCESS;
}

int bt_disconnect()
{
	close(cs);
	close(is);
}

// Change for playing around with git

void usage()
{
	fprintf(stderr,	"\nbtkbdemu v0.1 Dec. 2005 Collin R. Mulliner <collin@betaversion.net>\n\n"\
			"syntax: btkbdemu -hsce\n\n"\
			"\th\t\thelp\n"\
			"\ts\t\tserver mode\n"\
			"\tc <BD_ADDR>\tclient mode - connect to addr\n"\
			"\te <FILE>\tevent file\n");
}
