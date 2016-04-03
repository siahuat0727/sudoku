#include "Sudoku.h"
    Sudoku::Sudoku(){
        maybe = 0;
        unsolvable = 0;
        recordStatus = 0;
    }
    void Sudoku::readIn(){
        for(int i = 0; i < 9; i++){
            for(int j = 0; j < 9; j++){
                cin >> arr[i][j];
            }
        }
    }

    void Sudoku::giveQuestion(){
        cout << "9 0 0 0 0 5 0 2 0 1 0 0 9 0 0 0 0 0 4 0 0 0 0 0 0 0 0 0 0 8 0 0 2 0 0 0 0 0 0 0 0 0 0 0 1 0 0 0 0 0 0 9 0 4 0 0 5 0 0 0 3 0 0 0 0 0 1 0 0 0 0 7 0 0 2 4 0 8 0 0 0 "<< endl;
    }

    void Sudoku::solve(){
        if(!firstJudge()){
            cout << 0 << endl;
            return;
        }
        if(lessThan17Elements()){
            unsolvableOrManySolution();
            return;
        }
        solveAccurately();
        //cout << "unsolvable = " << unsolvable << endl;
        if(unsolvable != 0){
            cout << 0 << endl;
            return;
        }
        for(int i = 0; i < 9; i++){
            for(int j = 0; j < 9; j++){
                if(arr[i][j] == 0){
                    copyAnsBeforeRecursive();
                    solveRecursively(i, j);
                    return;
                }
            }
        }
        cout << 1 << endl;
        print();
    }
    bool Sudoku::firstJudge(){
        for(int i = 0; i < 9; i++){
            int judge[10] = {0,0,0,0,0,0,0,0,0,0};
            for(int j = 0; j < 9; j++){
                ++judge[ arr[i][j] ];
                if(arr[i][j] < 0 || arr[i][j] > 9)
                    return false;
            }
            for(int k = 1; k < 10; k++){
                if(judge[k] > 1)
                    return false;
            }
        }
        for(int i = 0; i < 9; i++){
            int judge[10] = {0,0,0,0,0,0,0,0,0,0};
            for(int j = 0; j < 9; j++){
                ++judge[ arr[j][i] ];
            }
            for(int k = 1; k < 10; k++){
                if(judge[k] > 1)
                    return false;
            }
        }
        for(int term = 0, i = 0, j = 0; term <3; term++, i+=3, j-=9){
            for(int termm = 0; termm <3; termm++, i-=3, j+=3){
                int judge[10] = {0,0,0,0,0,0,0,0,0,0};
                for(int termmm = 0; termmm <3; termmm++, i++, j-=3){
                    for(int termmmm = 0; termmmm <3; termmmm++, j++){
                        ++judge[ arr[j][i] ];
                    }
                }
                for(int k = 1; k < 10; k++){
                    if(judge[k] > 1)
                        return false;
                }
            }
        }
        return true;
    }
    bool Sudoku::lessThan17Elements(){
        int nonZero = 0;
        for(int i = 0; i < 9; i++){
            for(int j = 0; j < 9; j++){
                if(arr[i][j] != 0)
                    nonZero++;
            }
        }
        if(nonZero < 17)
            return true;
        return false;
    }
    void Sudoku::unsolvableOrManySolution(){
        solveAccurately();
        if(unsolvable == 0)
             cout << 2 << endl;
        else cout << 0 << endl;
        return;
    }
    void Sudoku::solveAccurately(){
        for(nothingFound = 0; nothingFound <= 1; nothingFound++){//cout << "循环一周"  << endl;
            for(int i = 0; i < 9; i++){
                for(int j = 0; j < 9; j++){
                    if(arr[i][j] == 0){//cout << "横Find Zero   i j = " << i << " " << j << endl;
                        rowByRow(i, j);
                        break;
                    }
                }
            }
            for(int j = 0; j < 9; j++){
                for(int i = 0; i < 9; i++){
                    if(arr[i][j] == 0){//cout << "直Find Zero  i j = " << i << " " << j << endl;
                        columnByColumn(i, j);
                        break;
                    }
                }
            }
            for(int i = 0; i < 3;i++){
                for(int j = 0; j < 3; j++){
                    for(int m = 3*i, term = 0; term < 3; term++, m++){
                        int n = 3*j;
                        if(arr[m][n] == 0){//cout << "i j m n = "  << i << " " << j << " " << m << " " << n << endl;
                            cellByCell(m, n);
                            break;
                        }
                        n++;
                        if(arr[m][n] == 0){//cout << "i j m n = "  << i << " " << j << " " << m << " " << n << endl;
                            cellByCell(m, n);
                            break;
                        }
                        n++;
                        if(arr[m][n] == 0){//cout << "i j m n = "  << i << " " << j << " " << m << " " << n << endl;
                            cellByCell(m, n);
                            break;
                        }
                    }
                }
            }
        }
        return;
    }
    void Sudoku::rowByRow(int i, int j){//cout << "rowByRow i j = " << i << " " << j << endl;
        for(int num = 1; num <= 9; num++){
            for(int jj = 0; jj < 9; jj++){
                if(arr[i][jj] == num)
                    break;
                if(jj == 8){
                    rowCheckColumnAndCell(i, j, num);
                }
            }
            if(arr[i][j] != 0){//cout << "Found something then break" << endl;
                break;
            }
        }
    }
    void Sudoku::rowCheckColumnAndCell(int i, int j, int num){//cout << "rowcheckColumn i j num = " << i << " " << j  << " " << num << endl;
        for(int ii = 0; ii < 9; ii++){
            if(arr[ii][j] == num){//cout << "cant fill ! i j num ii = " << i << " " << j << " " << num << " " << ii << endl;
                for(int jj = j + 1; jj < 9; jj++){
                    if(arr[i][jj] == 0){//cout << "found next zero!" << endl;
                        rowCheckColumnAndCell(i, jj, num);
                        return;
                    }
                    if(jj == 8){//cout << "unsolvable!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << endl;
                        unsolvable++;
                    }
                }
            }
        }
        //cout << "rowcheckCell i j num = "<< i << " " << j  << " " << num << endl;
        for(int term = 1, ii = i/3*3, jj = j/3*3; term <= 3; term++, ii++, jj -= 3){
            for(int termm = 1; termm <= 3; termm++, jj++){
                if(arr[ii][jj] == num){//cout << "cant fill ! k kk ii jj = " << term << " " << termm << " " << ii << " " << jj << endl;
                    for(jj = j + 1; jj < 9; jj++){
                        if(arr[i][jj] == 0){//cout << "found next zero!" << endl;
                            rowCheckColumnAndCell(i, jj, num);
                            return;
                        }
                        if(jj == 8){//cout << "unsolvable!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << endl;
                            unsolvable++;
                        }
                    }
                }
            }
        }
        maybe++;
        doesRowNextZeroExist(i, j, num, j);
    }
    void Sudoku::rowCheckColumnAndCell(int i, int j, int num, int jj){//cout << "checkColumnSuper i j num jj = " << i << " " << j  << " " << num << " " << jj << endl;
        for(int ii = 0; ii < 9; ii++){
            if(arr[ii][jj] == num){//cout << "cant fill ! num & ii & jj = " << num << " " << ii << " " << jj << endl;
                doesRowNextZeroExist(i, j, num, jj);
                return;
            }
        }
        //cout << "checkCellSuper i j num = " << i << " " << j  << " " << num << endl;
        for(int term = 1, ii =  i/3*3, kk = jj/3*3; term <= 3; term++, ii++, kk -= 3){
            for(int termm = 1; termm <= 3; termm++, kk++){
                if(arr[ii][kk] == num){//cout << "cant fill ! k kk ii jj = " << term << " " << termm << " " << ii << " " << kk << endl;
                    doesRowNextZeroExist(i, j, num, jj);
                    return;
                }
            }
        }
        maybe = 0;
       // cout << "check next number~~~~~~~~" << endl;
    }
    void Sudoku::doesRowNextZeroExist(int i, int j, int num, int jj){//cout << "doesRowNextZeroExist i j num jj maybe = " << i << " " << j  << " " << num << " " << jj << " " << maybe << endl;
        if(maybe >= 2){
            return;
        }
        for(jj++; jj < 9; jj++){
            if(arr[i][jj] == 0){//cout<< "Next Zero Exist ! i j num jj = " << i << " " << j  << " " << num << " " << jj << " " << endl;
                rowCheckColumnAndCell(i, j, num, jj);
                return;
            }
        }
        found(i, j, num);
    }
    void Sudoku::columnByColumn(int i, int j){//cout << "columnByColumn i j = " << i << " " << j << endl;
        for(int num = 1; num <= 9; num++){
            for(int ii = 0; ii < 9; ii++){
                if(arr[ii][j] == num)
                    break;
                if(ii == 8){
                    columnCheckRowAndCell(i, j, num);
                }
            }
            if(arr[i][j] != 0)
                break;
        }
    }
    void Sudoku::columnCheckRowAndCell(int i, int j, int num){//cout << "columnCheckRowAndCell i j num = " << i << " " << j  << " " << num << endl;
        for(int jj = 0; jj < 9; jj++){
            if(arr[i][jj] == num){//cout << "cant fill ! i j num jj = " << i << " " << j << " " << num << " " << jj << endl;
                for(int ii = i + 1; ii < 9; ii++){
                    if(arr[ii][j] == 0){//cout << "found next zero!" << endl;
                        columnCheckRowAndCell(ii, j, num);
                        return;
                    }
                    if(ii == 8){//cout << "unsolvable!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << endl;
                        unsolvable++;
                    }
                }
            }
        }
        //cout << "columncheckCell i j num = "<< i << " " << j  << " " << num << endl;
        for(int term = 1, ii = i/3*3, jj = j/3*3; term <= 3; term++, jj++, ii -= 3){
            for(int termm = 1; termm <= 3; termm++, ii++){
                if(arr[ii][jj] == num){//cout << "cant fill ! k kk ii jj = " << term << " " << termm << " " << ii << " " << jj << endl;
                    for(ii = i + 1; ii < 9; ii++){
                        if(arr[ii][j] == 0){//cout << "found next zero!" << endl;
                            columnCheckRowAndCell(ii, j, num);
                            return;
                        }
                        if(ii == 8){//cout << "unsolvable!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << endl;
                            unsolvable++;
                        }
                    }
                }
            }
        }
        maybe++;
        doesColumnNextZeroExist(i, j, num, i);
    }
    void Sudoku::columnCheckRowAndCell(int i, int j, int num, int ii){//cout << "checkColumnSuper i j num jj = " << i << " " << j  << " " << num << " " << ii << endl;
        for(int jj = 0; jj < 9; jj++){
            if(arr[ii][jj] == num){//cout << "cant fill ! num & ii & jj = " << num << " " << ii << " " << jj << endl;
                doesColumnNextZeroExist(i, j, num, ii);
                return;
            }
        }
       // cout << "checkCellSuper i j num = " << i << " " << j  << " " << num << endl;
        for(int term = 1, jj = j/3*3, kk = ii/3*3; term <= 3; term++, jj++, kk -= 3){
            for(int termm = 1; termm <= 3; termm++, kk++){
                if(arr[kk][jj] == num){//cout << "cant fill ! k kk ii jj = " << term << " " << termm << " " << ii << " " << kk << endl;
                    doesColumnNextZeroExist(i, j, num, ii);
                    return;
                }
            }
        }
        maybe = 0;
        //cout << "check next number ~~~~~~~~ " << endl;
    }
    void Sudoku::doesColumnNextZeroExist(int i, int j, int num, int ii){//cout << "doesRowNextZeroExist i j num jj maybe = " << i << " " << j  << " " << num << " " << ii << " " << maybe << endl;
        if(maybe >= 2){
            return;
        }
        for(ii++; ii < 9; ii++){
            if(arr[ii][j] == 0){//cout<< "next zero exist ! i j num jj = " << i << " " << j  << " " << num << " " << ii << " " << endl;
                columnCheckRowAndCell(i, j, num, ii);
                return;
            }
        }
        found(i, j, num);
    }
    void Sudoku::cellByCell(int i, int j){//cout << "cellByCell i j = " << i << " " << j << endl;
        for(int num = 1; num <= 9; num++){
            for(int term = 1, ii = i/3*3, jj = j/3*3; term <= 3; term++, ii++, jj-=2){
                if(arr[ii][jj] == num){
                    break;
                }
                jj++;
                if(arr[ii][jj] == num){
                    break;
                }
                jj++;
                if(arr[ii][jj] == num){
                    break;
                }
                if(term == 3)
                    cellCheckRowAndColumn(i, j, num);
            }
            if(arr[i][j] != 0){//cout << "Found something then break" << endl;
                break;
            }
        }
    }
    void Sudoku::cellCheckRowAndColumn(int i, int j, int num){//cout << "cellCheckRow i j num = " << i << " " << j << " " << num << endl;
        for(int jj = 0; jj < 9; jj++){
            if(arr[i][jj] == num){//cout << "cant fill ! i j num jj = " << i << " " << j << " " << num << " " << jj << endl;
                for(int iii = i, jjj = j + 1, cellMaxI = i/3*3 + 2; iii <= cellMaxI; iii++, jjj-=3){
                    for(int cellMaxJ = j/3*3 + 2; jjj <= cellMaxJ; jjj++){
                        if(arr[iii][jjj] == 0){//cout << "found next zero!" << endl;
                            cellCheckRowAndColumn(iii, jjj, num);
                            return;
                        }
                    }
                    if(iii == cellMaxI){//cout << "unsolvable!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << endl;
                        unsolvable++;
                    }
                }
            }
        }
        //cout << "cellCheckColumn i j num = " << i << " " << j << " " << num << endl;
        for(int ii = 0; ii < 9; ii++){
            if(arr[ii][j] == num){//cout << "cant fill ! i j num ii = " << i << " " << j << " " << num << " " << ii << endl;
                for(int iii = i, jjj = j + 1, cellMaxI = i/3*3 + 2; iii <= cellMaxI; iii++, jjj-=3){
                    for(int cellMaxJ = j/3*3 + 2; jjj <= cellMaxJ; jjj++){
                        if(arr[iii][jjj] == 0){//cout << "found next zero!" << endl;
                            cellCheckRowAndColumn(iii, jjj, num);
                            return;
                        }
                    }
                    if(iii == cellMaxI){//cout << "unsolvable!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << endl;
                        unsolvable++;
                    }
                }
            }
        }
        maybe++;
        doesCellNextZeroExist(i, j, num, i, j);
    }
    void Sudoku::cellCheckRowAndColumn(int i, int j, int num, int ii, int jj){//cout << "checkColumnSuper i j num jj = " << i << " " << j  << " " << num << " " << jj << endl;
        for(int iii = 0; iii < 9; iii++){
            if(arr[iii][jj] == num){//cout << "cant fill ! num & iii & jj = " << num << " " << ii << " " << jj << endl;
                doesCellNextZeroExist(i, j, num, ii, jj);
                return;
            }
        }
        for(int jjj = 0; jjj < 9; jjj++){
            if(arr[ii][jjj] == num){//cout << "cant fill ! num & ii & jjj = " << num << " " << ii << " " << jj << endl;
                doesCellNextZeroExist(i, j, num, ii, jj);
                return;
            }
        }
        maybe = 0;
        //cout << "check next number ~~~~~~~~ " << endl;

    }
    void Sudoku::doesCellNextZeroExist(int i, int j, int num, int ii, int jj){//cout << "doesCellNextZeroExist i j num jj maybe = " << i << " " << j  << " " << num << " " << jj << " " << maybe << endl;
        if(maybe >= 2){
            return;
        }
        for(int iii = ii, jjj = jj + 1, cellMaxI = ii/3*3 + 2; iii <= cellMaxI; iii++, jjj-=3){
            for(int cellMaxJ = jj/3*3 + 2; jjj <= cellMaxJ; jjj++){
                if(arr[iii][jjj] == 0){//cout << "found next zero!" << endl;
                    cellCheckRowAndColumn(i, j, num, iii, jjj);
                    return;
                }
            }
        }
        found(i, j, num);
    }
    void Sudoku::found(int i, int j, int num){//cout << "found [" << i << "] [" << j << "] = " << num << endl;
        maybe = 0;
        nothingFound = 0;
        arr[i][j] = num;
        //cout << "*****************************************************************************"<<endl;
        whenSomethingIsFound(i, j);
    }
    void Sudoku::whenSomethingIsFound(int i, int j){//cout << "whenSomethingIsFound i j = "<< i << " " << j << endl;
        for(int jj = 0; jj < 9; jj++){
            if(arr[i][jj] == 0){//cout << "Find Zero  i jj = " << i << " " << jj << endl;
                rowByRow(i, jj);
                break;
            }
        }
        for(int ii = 0; ii < 9; ii++){
            if(arr[ii][j] == 0){//cout << "Find Zero  ii j = " << ii << " " << j << endl;
                columnByColumn(ii, j);
                break;
            }
        }
        for(int iii = i/3*3, jjj = j/3*3, term = 0; term < 3; term++, iii++, jjj-=3){
            for(int termm = 0; termm < 3; termm++, jjj++){
                if(arr[iii][jjj] == 0){
                    cellByCell(iii, jjj);
                    return;
                }
            }
        }
        //cout << "Finish found ++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl;
    }
    void Sudoku::copyAnsBeforeRecursive(){
        for(int i = 0; i < 9; i++){
            for(int j = 0; j < 9; j++){
                aaa[i][j] = arr[i][j];
            }
        }
    }
    void Sudoku::solveRecursively(int i, int j){
        tryToSolveSmall(i, j);
        copyAns();
        resetElement();
        tryToSolveBig(i, j);
        compareAns();
        return;
    }
    void Sudoku::tryToSolveSmall(int i, int j){//cout << "SmallNooooooooooooooooooooooooooooooooooooooooooooooooooooo" << endl;
        backUpElement(recordStatus);
        fillFirstZeroSmall(i, j);
        return;
    }
    void Sudoku::backUpElement(int r){//cout << "backUpElement" << endl;
        for(int i = 0; i < 9; i++){
            for(int j = 0; j < 9; j++){
                rar[i][j][r] = arr[i][j];
            }
        }
    }
    void Sudoku::fillFirstZeroSmall(int i, int j){//cout << "fillFirstZeroSmall recordStatus = " << i << " " << j << " " << recordStatus << endl;
        for(int num = 1; num <= 9; num ++){
            tryAll(i, j, num);
            if(arr[i][j] != 0){//cout << "try to fill " << num << "at " << "arr[" << i << "][" << j << "]" << endl;
                solveAccurately();
                if(unsolvable != 0){//cout << "fail ! try another num" << endl;
                    unsolvable = 0;
                        for(int m = 0; m < 9; m++){
                        for(int n = 0; n < 9; n++){
                            arr[m][n] = rar[m][n][recordStatus];
                        }
                    }
                    continue;
                }
                //cout << "finish 1 round" << endl;
                for(int m = 0; m < 9; m++){
                    for(int n = 0; n < 9; n++){
                        if(arr[m][n] == 0){
                            recordStatus++;
                            tryToSolveSmall(m, n);
                            recordStatus--;
                        }
                    }
                }
                return;
            }
        }
    }
    void Sudoku::tryAll(int i, int j, int num){
        for(int jj = 0; jj < 9; jj++){
            if(arr[i][jj] == num){
                return;
            }
        }
        for(int ii = 0; ii < 9; ii++){
            if(arr[ii][j] == num){
                return;
            }
        }
        for(int ii = i/3*3, jj = j/3*3, term = 1; term <= 3; term++, ii++, jj -= 3){
            for(int termm = 1; termm <= 3; termm++, jj++){
                if(arr[ii][jj] == num){
                    return;
                }
            }
        }

        arr[i][j] = num;
    }
    void Sudoku::copyAns(){//cout << "copyAns" << endl;
        for(int i = 0; i < 9; i++){
            for(int j = 0; j < 9; j++){
                rra[i][j] = arr[i][j];
            }
        }
    }
    void Sudoku::resetElement(){//cout << "resetElement" << endl;
        for(int i = 0; i < 9; i++){
            for(int j = 0; j < 9; j++){
                arr[i][j] = aaa[i][j];
            }
        }
    }
    void Sudoku::tryToSolveBig(int i, int j){//cout << "BigNooooooooooooooooooooooooooooooooooooooooooooooooooooo" << endl;
        backUpElement(recordStatus);
        fillFirstZeroBig(i, j);
    }
    void Sudoku::fillFirstZeroBig(int i, int j){//cout << "fillFirstZeroBig recordStatus = " << i << " " << j << " " << recordStatus << endl;
        for(int num = 9; num >= 1; num--){
            tryAll(i, j, num);
            if(arr[i][j] != 0){//cout << "try to fill " << num << "at " << "arr[" << i << "][" << j << "]" << endl;
                solveAccurately();
                if(unsolvable != 0){//cout << "fail ! try another num" << endl;
                    unsolvable = 0;
                        for(int m = 0; m < 9; m++){
                        for(int n = 0; n < 9; n++){
                            arr[m][n] = rar[m][n][recordStatus];
                        }
                    }
                    continue;
                }
                //cout << "finish 1 round" << endl;
                for(int m = 0; m < 9; m++){
                    for(int n = 0; n < 9; n++){
                        if(arr[m][n] == 0){
                            recordStatus++;
                            tryToSolveBig(m, n);
                            recordStatus--;
                        }
                    }
                }
                return;
            }
        }
    }
    void Sudoku::compareAns(){//cout << "compareAns" << endl;
        for(int i = 0; i < 9; i++){
            for(int j = 0; j < 9; j++){
                if(arr[i][j] != rra[i][j]){
                    cout << 2 << endl;
                    return;
                }
            }
        }
        cout << 1 << endl;
        print();
        return;
    }
    void Sudoku::print(){
        for(int i = 0; i < 9; i++){
            for(int j = 0; j < 9; j++){
                cout << arr[i][j] << " ";
            }
            cout << endl;
        }
    }
    void Sudoku::changeNum(int a, int b){
        for(int i = 0; i < 9; i++){
            for(int j = 0; j < 9; j++){
                if(arr[i][j] == a)
                    arr[i][j] = b;
                else if(arr[i][j] == b)
                    arr[i][j] = a;
            }
        }
    }
    void Sudoku::changeRow(int a, int b){
        if(a == b)
            return;
        else{
            int temp;
            for(int term = 0, i = 3*a, ii = 3*b; term < 3; term++, i++, ii++){
                for(int j = 0; j < 9; j++){
                    temp = arr[i][j];
                    arr[i][j] = arr[ii][j];
                    arr[ii][j] = temp;
                }
            }
        }
    }
    void Sudoku::changeCol(int a, int b){
        if(a == b)
            return;
        else{
            int temp;
            for(int term = 0, j = 3*a, jj = 3*b; term < 3; term++, j++, jj++){
                for(int i = 0; i < 9; i++){
                    temp = arr[i][j];
                    arr[i][j] = arr[i][jj];
                    arr[i][jj] = temp;
                }
            }
        }
    }
    void Sudoku::flip(int n){
        int temp;
        if(n == 0){
            for(int i = 0; i < 4; i++){
                for(int j = 0; j < 9; j++){
                    temp = arr[i][j];
                    arr[i][j] = arr[8-i][j];
                    arr[8-i][j] = temp;
                }
            }
        }
        else{
            for(int j = 0; j < 4; j++){
                for(int i = 0; i < 9; i++){
                    temp = arr[i][j];
                    arr[i][j] = arr[i][8-j];
                    arr[i][8-j] = temp;
                }
            }
        }
    }
    void Sudoku::rotate(int n){
        switch(n%4){
            case 0: break;
            case 1: int temp;
                    for(int i = 0, j = 0, term = 0, decrease2From8 = 8; term < 4; term++, i++, decrease2From8 -= 2){
                        for(int j = i, termm = 0; termm < decrease2From8; termm++, j++){
                            temp = arr[i][j];
                            arr[i][j] = arr[8-j][i];
                            arr[8-j][i] = arr[8-i][8-j];
                            arr[8-i][8-j] = arr[j][8-i];
                            arr[j][8-i] = temp;
                        }
                    }
                    break;
            case 2: flip(0);flip(1);break;
            case 3:for(int i = 0, j = 0, term = 0, decrease2From8 = 8; term < 4; term++, i++, decrease2From8 -= 2){
                        for(int j = i, termm = 0; termm < decrease2From8; termm++, j++){
                            temp = arr[i][j];
                            arr[i][j]     = arr[j][8-i];
                            arr[j][8-i]   = arr[8-i][8-j];
                            arr[8-i][8-j] = arr[8-j][i];
                            arr[8-j][i]   = temp;
                        }
                    }
                    break;
        }
    }
    void Sudoku::transform(){
        srand(time(0));
        changeNum(rand()%9 + 1, rand()%9 + 1);
        changeRow(rand()%3, rand()%3);
        changeCol(rand()%3, rand()%3);
        rotate(rand()%101);
        flip(rand()%2);
        print();
    }
