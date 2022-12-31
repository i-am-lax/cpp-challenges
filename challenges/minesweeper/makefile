main: main.o minesweeper.o
	g++ -std=c++11 main.o minesweeper.o -o minesweeper

main.o: main.cpp minesweeper.h
	g++ -std=c++11 main.cpp -c

minesweeper.o: minesweeper.cpp minesweeper.h
	g++ -std=c++11 minesweeper.cpp -c

clean:
	rm -f *.o execute