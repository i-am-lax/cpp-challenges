sudoku: main.o spell.o
	g++ -Wall -g -std=c++11 main.o spell.o -o spell

main.o: main.cpp spell.h
	g++ -Wall -g -std=c++11 main.cpp -c

spell.o: spell.cpp spell.h
	g++ -Wall -g -std=c++11 spell.cpp -c

clean:
	rm -f *.o execute
