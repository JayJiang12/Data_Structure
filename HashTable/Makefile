PROJ = Driver
CC = g++
CCFLAGS = -g -ansi -Wall

SOURCES = $(PROJ).cpp HashTable.h HashTable.cpp words.h primes.h
OBJECTS = $(PROJ).o HashTable.o

$(PROJ): $(OBJECTS)
	$(CC) $(CCFLAGS) -o $(PROJ).out $(OBJECTS)

$(PROJ).o: $(PROJ).cpp 
	$(CC) $(CCFLAGS) -c $(PROJ).cpp

HashTable.o: HashTable.cpp HashTable.h
	$(CC) $(CCFLAGS) -c HashTable.cpp

clean:
	rm -rf *o $(PROJ).out

run: $(PROJ)
	./$(PROJ).out

memory: $(PROJ)
	valgrind ./$(PROJ).out