main: main.o soundex.o
	g++ main.o soundex.o -o main

main.o: main.cpp soundex.h
	g++ main.cpp -c

soundex.o: soundex.cpp soundex.h
	g++ soundex.cpp -c

clean:
	rm -f *.o execute