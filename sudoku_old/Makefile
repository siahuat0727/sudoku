all: Sudoku.o giveQuestion.cpp solve.cpp transform.cpp
	g++ -o giveQuestion giveQuestion.cpp Sudoku.o
	g++ -o solve solve.cpp Sudoku.o
	g++ -o transform transform.cpp Sudoku.o

Sudoku.o: Sudoku.cpp Sudoku.h
	g++ -c Sudoku.cpp -o Sudoku.o
