main: main.o tube.o
	g++ -std=c++11 main.o tube.o -o tube

main.o: main.cpp tube.h
	g++ -std=c++11 main.cpp -c

tube.o: tube.cpp tube.h
	g++ -std=c++11 tube.cpp -c

clean:
	rm -f *.o execute