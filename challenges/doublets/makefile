main: main.o doublets.o
	g++ -std=c++11 main.o doublets.o -o doublets

main.o: main.cpp doublets.h
	g++ -std=c++11 main.cpp -c

doublets.o: doublets.cpp doublets.h dictionary.h
	g++ -std=c++11 doublets.cpp -c

clean:
	rm -f *.o execute