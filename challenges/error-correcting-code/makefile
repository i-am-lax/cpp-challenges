main: main.o correct.o
	g++ -std=c++11 main.o correct.o -o correct

main.o: main.cpp correct.h
	g++ -std=c++11 main.cpp -c

correct.o: correct.cpp correct.h
	g++ -std=c++11 correct.cpp -c

clean:
	rm -f *.o execute