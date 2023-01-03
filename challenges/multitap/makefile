main: main.o multitap.o
	g++ -std=c++11 main.o multitap.o -o multitap

main.o: main.cpp multitap.h
	g++ -std=c++11 main.cpp -c

multitap.o: multitap.cpp multitap.h
	g++ -std=c++11 multitap.cpp -c

clean:
	rm -f *.o execute