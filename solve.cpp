#include <iostream>
#include "Sudoku.h"
using namespace std;
int main(){
    Sudoku ss;
    ss.readIn();
    ss.solve();
    return 0;
}
