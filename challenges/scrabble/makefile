main: main.o scrabble.o
	g++ -std=c++11 main.o scrabble.o -o scrabble

main.o: main.cpp scrabble.h
	g++ -std=c++11 main.cpp -c

scrabble.o: scrabble.cpp scrabble.h
	g++ -std=c++11 scrabble.cpp -c

clean:
	rm -f *.o execute