main: main.o gogen.o
	g++ -std=c++11 main.o gogen.o -o gogen

main.o: main.cpp gogen.h
	g++ -std=c++11 main.cpp -c

gogen.o: gogen.cpp gogen.h
	g++ -std=c++11 gogen.cpp -c

clean:
	rm -f *.o execute