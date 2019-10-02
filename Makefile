OBJS = bitcoin.o mainfunctions.o functions.o lists.o tree.o
SOURCE = bitcoin.h mainfunctions.h functions.h lists.h tree.h
HEADER = structs.h functions.h
OUT = bitcoin
CC = gcc
FLAGS = -g3 -c

all: $(OBJS)
	$(CC) -g $(OBJS) -o $(OUT)

bitcoin.o: bitcoin.c
	$(CC) $(FLAGS) bitcoin.c

mainfunctions.o: mainfunctions.c
	$(CC) $(FLAGS) mainfunctions.c

functions.o: functions.c
	$(CC) $(FLAGS) functions.c

lists.o: lists.c
	$(CC) $(FLAGS) lists.c

tree.o: tree.c
	$(CC) $(FLAGS) tree.c

clean:
	rm -f $(OBJS) $(OUT)
