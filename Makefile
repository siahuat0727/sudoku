all:
	gcc sudoku1.c -o sudoku1
	gcc sudoku2.c -o sudoku2
	gcc sudoku3.c -o sudoku3
	g++ sudoku4.cpp -o sudoku4
	g++ sudoku5.cpp -o sudoku5
	g++ sudoku6.cpp -o sudoku6

clean:
	rm sudoku1 sudoku2 sudoku3 sudoku4 sudoku5 sudoku6
