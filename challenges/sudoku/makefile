main: main.o sudoku.o
	g++ -std=c++11 main.o sudoku.o -o sudoku

main.o: main.cpp sudoku.h
	g++ -std=c++11 sudoku.cpp -c

sudoku.o: sudoku.cpp sudoku.h
	g++ -std=c++11 sudoku.cpp -c

clean:
	rm -f *.o execute
