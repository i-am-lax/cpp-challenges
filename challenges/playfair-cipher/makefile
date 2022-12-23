main: main.o playfair.o
	g++ main.o playfair.o -o playfair

main.o: main.cpp playfair.h
	g++ main.cpp -c

playfair.o: playfair.cpp playfair.h
	g++ playfair.cpp -c

clean:
	rm -f *.o execute