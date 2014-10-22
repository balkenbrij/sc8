CC= 		gcc
CFLAGS= 	-O2 -Wall -Wextra `Wand-config --cflags`
LDFLAGS= 	`Wand-config --libs`


all:
	$(CC) $(CFLAGS) -s -o sc8 sc8.c $(LDFLAGS)

