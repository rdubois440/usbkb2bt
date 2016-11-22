CFLAGS=-Wall
LIBS=-lbluetooth

ALL: usbkb2bt.o btkbdemu.o 
	$(CC) $(CFLAGS) $(LIBS) -o usbkb2bt usbkb2bt.o btkbdemu.o 

clean:
	rm usbkb2bt *.o


usbkb2bt.o: usbkb2bt.c
	$(CC) $(CFLAGS) -c usbkb2bt.c

btkbdemu.o: btkbdemu.c
	$(CC) $(CFLAGS) -c btkbdemu.c





