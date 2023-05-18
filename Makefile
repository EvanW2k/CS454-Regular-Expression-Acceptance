OBJS	= main.o NFA.o
SOURCE	= main.cpp NFA.cpp
HEADER	= NFA.hpp
OUT	= main
CC	 = g++
FLAGS	 = -g -c -Wall
LFLAGS	 = 

all: $(OBJS)
	$(CC) -g $(OBJS) -o $(OUT) $(LFLAGS)

main.o: main.cpp
	$(CC) $(FLAGS) main.cpp 

NFA.o: NFA.cpp
	$(CC) $(FLAGS) NFA.cpp 
	
clean:
	rm -f $(OBJS) $(OUT)