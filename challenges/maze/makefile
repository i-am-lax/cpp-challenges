main: main.o maze.o
	g++ -std=c++11 main.o maze.o -o maze

main.o: main.cpp maze.h
	g++ -std=c++11 main.cpp -c

maze.o: maze.cpp maze.h
	g++ -std=c++11 maze.cpp -c

clean:
	rm -f *.o execute