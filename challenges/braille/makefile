main: main.o braille.o
	g++ -std=c++11 main.o braille.o -o braille

main.o: main.cpp braille.h
	g++ -std=c++11 main.cpp -c

braille.o: braille.cpp braille.h
	g++ -std=c++11 braille.cpp -c

clean:
	rm -f *.o execute