CC = gcc
#COMPILERFLAGS = -g
SRCDIRECTORY = src
TARGET = master worker periodicasterik
all: $(TARGET)

master:$(SRCDIRECTORY)/master.c
	$(CC) -o master $<

worker:$(SRCDIRECTORY)/worker.c
	$(CC) -o worker $<

periodicasterik: $(SRCDIRECTORY)/periodicasterik.c
	$(CC) -o periodicasterik $<
clean:
	 /bin/rm -f *.o $(TARGET)

