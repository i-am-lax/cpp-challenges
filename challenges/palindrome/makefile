main: main.o words.o
	g++ -std=c++11 main.o words.o -o palindrome

main.o: main.cpp words.h
	g++ -std=c++11 main.cpp -c

words.o: words.cpp words.h
	g++ -std=c++11 words.cpp -c

clean:
	rm -f *.o execute