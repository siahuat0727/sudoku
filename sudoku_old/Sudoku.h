#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
using namespace std;

class Sudoku{
public:
    Sudoku();
    void giveQuestion();
    void readIn();
    void solve();
    bool firstJudge();
    bool lessThan17Elements();
    void unsolvableOrManySolution();
    void solveAccurately();
    void rowByRow(int i, int j);
    void rowCheckColumnAndCell(int i, int j, int num);
    void rowCheckColumnAndCell(int i, int j, int num, int jj);
    void doesRowNextZeroExist(int i, int j, int num, int jj);
    void columnByColumn(int i, int j);
    void columnCheckRowAndCell(int i, int j, int num);
    void columnCheckRowAndCell(int i, int j, int num, int ii);
    void doesColumnNextZeroExist(int i, int j, int num, int ii);
    void cellByCell(int i, int j);
    void cellCheckRowAndColumn(int i, int j, int num);
    void cellCheckRowAndColumn(int i, int j, int num, int ii, int jj);
    void doesCellNextZeroExist(int i, int j, int num, int ii, int jj);
    void found(int i, int j, int num);
    void whenSomethingIsFound(int i, int j);
    void copyAnsBeforeRecursive();
    void solveRecursively(int i, int j);
    void tryToSolveSmall(int i, int j);
    void backUpElement(int r);
    void fillFirstZeroSmall(int i, int j);
    void tryAll(int i, int j, int num);
    void copyAns();
    void resetElement();
    void tryToSolveBig(int i, int j);
    void fillFirstZeroBig(int i, int j);
    void compareAns();
    void print();
    void changeNum(int a, int b);
    void changeRow(int a, int b);
    void changeCol(int a, int b);
    void flip(int n);
    void rotate(int n);
    void transform();
private:
    int arr[9][9], rar[9][9][20], rra[9][9], aaa[9][9];
    int maybe, nothingFound, unsolvable, recordStatus;
};
