CFLAGS = -Wall

all:
	gcc ETP.c -o ETP $(CFLAGS)
	./ETP