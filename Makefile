CC = gcc
#COMPILERFLAGS = -g
SRCDIRECTORY = src
TARGET = master worker 
all: $(TARGET)

master:$(SRCDIRECTORY)/master.c
	$(CC) -o master $<

worker:$(SRCDIRECTORY)/worker.c
	$(CC) -o worker $<

clean:
	 /bin/rm -f *.o $(TARGET)

