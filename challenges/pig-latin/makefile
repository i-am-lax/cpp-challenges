main: main.o piglatin.o
	g++ -std=c++11 main.o piglatin.o -o piglatin

main.o: main.cpp piglatin.h
	g++ -std=c++11 main.cpp -c

piglatin.o: piglatin.cpp piglatin.h
	g++ -std=c++11 piglatin.cpp -c

clean:
	rm -f *.o execute