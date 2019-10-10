.SUFFIXES : .c .o
CC = gcc

INC =
LIBS = 
CFLAGS = 

OBJS_1 = server.o
TARGET_1 = server

all : 
	$(CC) -g -o server *.c #-lcrypto  $(LIBS)

clean :
	rm -rf $(TARGET_1) $(TARGET_1).exe core
new : 
	$(MAKE) clean 
	$(MAKE) 

