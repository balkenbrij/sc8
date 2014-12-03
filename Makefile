CC= 		gcc
CFLAGS= 	-O2 -Wall -Wextra `pkg-config --cflags MagickWand`
LDFLAGS= 	`pkg-config --libs MagickWand`


all:
	$(CC) $(CFLAGS) -s -o sc8 sc8.c $(LDFLAGS)

