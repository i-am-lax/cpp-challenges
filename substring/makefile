main: main.o substring.o
	g++ main.o substring.o -o main

main.o: main.cpp substring.h
	g++ main.cpp -c

substring.o: substring.cpp substring.h
	g++ substring.cpp -c

clean:
	rm -f *.o execute