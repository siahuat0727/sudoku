all:
	gcc sudoku1.c -o sudoku1
	gcc sudoku2.c -o sudoku2
	gcc sudoku3.c -o sudoku3
	g++ sudoku4.cpp -o sudoku4
	gcc sudoku5.c -o sudoku5

clean:
	rm sudoku1 sudoku2 sudoku3 sudoku4 sudoku5
