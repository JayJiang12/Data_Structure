TEST = driver1
PROJ = Sally
CC = g++
CCFLAGS = -g -ansi -Wall

SOURCES = $(TEST).cpp $(PROJ).h $(PROJ).cpp
OBJECTS = $(TEST).o $(PROJ).o

$(TEST): $(OBJECTS)
	$(CC) $(CCFLAGS) -o $(TEST).out $(OBJECTS)

$(TEST).o: $(TEST).cpp 
	$(CC) $(CCFLAGS) -c $(TEST).cpp

$(PROJ).o: $(PROJ).cpp $(PROJ).h
	$(CC) $(CCFLAGS) -c $(PROJ).cpp

clean:
	rm -rf *o $(TEST).out

run: $(TEST)
	./$(TEST).out

memory: $(TEST)
	valgrind ./$(TEST).out