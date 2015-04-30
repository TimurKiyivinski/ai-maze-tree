#CC = gcc or g++
CC = g++
#CFLAGS are compile flags such as -lm to include <math.h>
#or -std=c++0x for C++11
CFLAGS = -std=c++11 -lsfml-graphics -lsfml-window -lsfml-system
#TARGET is the name of your main file
TARGET = template
#OBJECTS = $(TARGET).o library1.o library.o, all the header files here
OBJECTS = $(TARGET).o space.o shared.o node_space.o bfs.o dfs.o gbfs.o ass.o bs.o hs.o
#EXTRA is for command line arguments
EXTRA? = ""

#Do not modify
$(TARGET) : clean $(OBJECTS)
	$(CC) $(CFLAGS) $(EXTRA) $(OBJECTS) -o maze_solver

#%.c or %.cpp
%.o : %.cpp
	$(CC) $(CFLAGS) $(EXTRA) -c $<

clean:
	rm -rvf $(OBJECTS) $(TARGET).bin maze_solver *~
