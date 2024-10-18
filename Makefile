a.exe: amp.o ams.o frame.o usb.o audisen_sim.o audisen_usb.o
	gcc amp.o ams.o frame.o usb.o ftd2xx.lib audisen_sim.o -o audisen_sim.exe
	gcc amp.o ams.o frame.o usb.o ftd2xx.lib audisen_usb.o -o audisen_usb.exe

amp.o: amp.c amp.h
	gcc -c amp.c

audisen_sim.o: audisen_sim.c
	gcc -c audisen_sim.c

audisen_usb.o: audisen_usb.c
	gcc -c audisen_usb.c

ams.o: ams.c ams.h
	gcc -c ams.c

frame.o: frame.c frame.h
	gcc -c frame.c

usb.o: usb.c usb.h
	gcc -c usb.c


