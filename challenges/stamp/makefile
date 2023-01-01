main: main.o stamp.o
	g++ -std=c++11 main.o stamp.o -o stamp

main.o: main.cpp stamp.h
	g++ -std=c++11 main.cpp -c

stamp.o: stamp.cpp stamp.h
	g++ -std=c++11 stamp.cpp -c

clean:
	rm -f *.o execute