#CC = gcc or g++
CC = g++
#CFLAGS are compile flags such as -lm to include <math.h>
#or -std=c++0x for C++11
CFLAGS = -std=c++0x -D ALGO_DFS
#TARGET is the name of your main file
TARGET = template
#OBJECTS = $(TARGET).o library1.o library.o, all the header files here
OBJECTS = $(TARGET).o space.o

#Do not modify
$(TARGET) : $(OBJECTS)
	$(CC) $(CFLAGS) $(EXTRA) $(OBJECTS) -o $(TARGET).bin 

#%.c or %.cpp
%.o : %.cpp
	$(CC) $(CFLAGS) -c $<

clean:
	rm -rvf $(OBJECTS) $(TARGET).bin *~
