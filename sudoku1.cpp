#include <iostream>

using namespace std;

#define N 9

int ans_count;
int sudoku[N][N];

void bt(int r, int c);

void sudoku_print()
{
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j)
            cout << sudoku[i][j] << ' ';
        cout << endl;
    }
    cout << endl;
}

bool can_fill_raw(int r, int n)
{
    for (int c = 0; c < N; ++c)
        if (sudoku[r][c] == n)
            return false;
    return true;
}

bool can_fill_col(int c, int n)
{
    for (int r = 0; r < N; ++r)
        if (sudoku[r][c] == n)
            return false;
    return true;
}

bool can_fill_box(int r, int c, int n)
{
    int r_start = r/3 * 3;
    int c_start = c/3 * 3;
    for (int i = r_start; i < r_start+3; ++i)
        for (int j = c_start; j < c_start+3; ++j)
            if(sudoku[i][j] == n)
                return false;
    return true;
}

inline bool can_fill(int r, int c, int n)
{
    return can_fill_raw(r, n) && can_fill_col(c, n) && can_fill_box(r, c, n);
}

inline void next_bt(int r, int c)
{
    c == N-1 ? bt(r+1, 0) : bt(r, c+1);
}

void bt(int r, int c)
{
    if(r == N) {
        ans_count++;
        sudoku_print();
        return;
    }
    if(sudoku[r][c] != 0)
        next_bt(r, c);
    else {
        for(int n = 1; n <= N; ++n) {
            if(can_fill(r, c, n)) {
                sudoku[r][c] = n;
                next_bt(r, c);
                sudoku[r][c] = 0;
            }
        }
    }
}

void sudoku_read()
{
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            int t;
            cin >> t;
            if(t == 0)
                continue;
            if(t > 0 && t <= N && can_fill(i, j, t))
                sudoku[i][j] = t;
            else {
                fprintf(stderr, "Wrong input: raw %d col %d value %d", i+1, j+1, t);
                exit(1);
            }
        }
    }
}

int main()
{
    sudoku_read();
    sudoku_print();
    bt(0, 0);
    cout << ans_count << " answer(s) found." << endl;
    return 0;
}
