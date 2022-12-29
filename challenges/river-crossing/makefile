main: main.o river.o
	g++ -std=c++11 main.o river.o -o river

main.o: main.cpp river.h
	g++ -std=c++11 main.cpp -c

river.o: river.cpp river.h
	g++ -std=c++11 river.cpp -c

clean:
	rm -f *.o execute