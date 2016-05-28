APP_DIR = /tftp
CC = arm-none-linux-gnueabi-gcc

CFLAGS := -Wall

all : Btest insins.sh
	cp -f $^ $(APP_DIR)

Btest : main.o dotmatrix.o 7segment.o buzzer.o keypad.o cled.o oled.o dipsw.o
	$(CC) -o Btest main.o dotmatrix.o 7segment.o buzzer.o keypad.o cled.o oled.o dipsw.o -lpthread

main.o: main.c dotmatrix.h 7segment.h buzzer.h keypad.h cled.h oled.h dipsw.h
	arm-none-linux-gnueabi-gcc -c main.c -lpthread

dotmatrix.o: dotmatrix.c
	arm-none-linux-gnueabi-gcc -c dotmatrix.c

7segment.o: 7segment.c
	arm-none-linux-gnueabi-gcc -c 7segment.c

buzzer.o: buzzer.c
	arm-none-linux-gnueabi-gcc -c buzzer.c

keypad.o: keypad.c
	arm-none-linux-gnueabi-gcc -c keypad.c

cled.o: cled.c
	arm-none-linux-gnueabi-gcc -c cled.c

oled.o: oled.c
	arm-none-linux-gnueabi-gcc -c oled.c

dipsw.o: dipsw.c
	arm-none-linux-gnueabi-gcc -c dipsw.c

clean:
	rm -f *.o

