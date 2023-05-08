OBJS	= main.o NFA.o State.o
SOURCE	= main.cpp NFA.cpp State.cpp
HEADER	= NFA.hpp State.hpp
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

State.o: State.cpp
	$(CC) $(FLAGS) State.cpp 


clean:
	rm -f $(OBJS) $(OUT)