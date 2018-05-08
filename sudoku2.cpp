#include <iostream>

using namespace std;

#define N 9

int ans_count;
int sudoku[N][N];
int raw[N][N+1];
int col[N][N+1];
int box[N][N+1];

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

inline int get_box(int r, int c)
{
    return r/3*3 + c/3;
}

inline bool can_fill(int r, int c, int n)
{
    return !raw[r][n] && !col[c][n] && !box[get_box(r,c)][n];
}

inline void _fill(int r, int c, int n, bool b)
{
    raw[r][n] = b;
    col[c][n] = b;
    box[get_box(r,c)][n] = b;
    sudoku[r][c] = b ? n : 0;
}

inline void fill(int r, int c, int n)
{
    _fill(r, c, n, true);
}

inline void erase(int r, int c, int n)
{
    _fill(r, c, n, false);
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
                fill(r, c, n);
                next_bt(r, c);
                erase(r, c, n);
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
                fill(i, j, t);
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
