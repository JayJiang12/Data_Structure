PROJ = Driver
CC = g++
CCFLAGS = -g -ansi -Wall

SOURCES = $(PROJ).cpp Graph.h Graph.cpp
OBJECTS = $(PROJ).o Graph.o

$(PROJ): $(OBJECTS)
	$(CC) $(CCFLAGS) -o $(PROJ).out $(OBJECTS)

$(PROJ).o: $(PROJ).cpp 
	$(CC) $(CCFLAGS) -c $(PROJ).cpp

Graph.o: Graph.cpp Graph.h
	$(CC) $(CCFLAGS) -c Graph.cpp

clean:
	rm -rf *o $(PROJ).out

run: $(PROJ)
	./$(PROJ).out
	
memory: $(PROJ)
	valgrind ./$(PROJ).out

memory2: $(PROJ)
	valgrind --leak-check=full ./$(PROJ).out