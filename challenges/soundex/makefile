main: main.o soundex.o
	g++ -std=c++11 main.o soundex.o -o soundex

main.o: main.cpp soundex.h
	g++ -std=c++11 main.cpp -c

soundex.o: soundex.cpp soundex.h
	g++ -std=c++11 soundex.cpp -c

clean:
	rm -f *.o execute