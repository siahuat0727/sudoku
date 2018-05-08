#include <iostream>
#include <cstring>
#include <assert.h>


using namespace std;

#define N 9
#define FOR_N(i) for (int i = 0; i < N; ++i)
#define FOR_EACH_EMPTY_CELL(r, c, s) FOR_N(r) FOR_N(c) if(s.grid[r][c] == 0)
#define FOR_EACH_NUM(n) for (int n = 1; n <= N; ++n)

struct Sudoku{
    int grid[N][N];
    bool raw[N][N+1];
    bool col[N][N+1];
    bool box[N][N+1];
};

int ans_count;

void bt(int r, int c);

void sudoku_print(Sudoku &thiz)
{
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j)
            cout << thiz.grid[i][j] << ' ';
        cout << endl;
    }
    cout << endl;
}

inline int get_box(int r, int c)
{
    return r/3*3 + c/3;
}

inline bool sudoku_can_fill(Sudoku &thiz, int r, int c, int n)
{
    return !thiz.raw[r][n] && !thiz.col[c][n] && !thiz.box[get_box(r,c)][n];
}

inline void _sudoku_fill(Sudoku &thiz, int r, int c, int n, bool b)
{
    assert(thiz.raw[r][n] != b && thiz.col[c][n] != b && thiz.box[get_box(r, c)][n] != b);
    assert(n > 0 && n <= N);
    thiz.raw[r][n] = b;
    thiz.col[c][n] = b;
    thiz.box[get_box(r,c)][n] = b;
    thiz.grid[r][c] = b ? n : 0;
}

inline void sudoku_fill(Sudoku &thiz, int r, int c, int n)
{
    _sudoku_fill(thiz, r, c, n, true);
}

inline void sudoku_erase(Sudoku &thiz, int r, int c, int n)
{
    _sudoku_fill(thiz, r, c, n, false);
}

void sudoku_read(Sudoku &thiz)
{
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            int t;
            cin >> t;
            if(t == 0)
                continue;
            if(t > 0 && t <= N && sudoku_can_fill(thiz, i, j, t))
                sudoku_fill(thiz, i, j, t);
            else {
                fprintf(stderr, "Wrong input: raw %d col %d value %d", i+1, j+1, t);
                exit(1);
            }
        }
    }
}

void sudoku_solve(Sudoku &thiz){
    int n_filled, n_unsolved;
    do{
        n_filled = 0;
        n_unsolved = 0;
        FOR_EACH_EMPTY_CELL(r, c, thiz){
            int n_can_fill = 0;
            int t;
            FOR_EACH_NUM(n){
                if(sudoku_can_fill(thiz, r, c, n)){
                    n_can_fill++;
                    t = n;
                    if (n_can_fill > 1) // pruning
                        break;
                }
            }
            if (n_can_fill == 0)
                return;
            if (n_can_fill == 1){
                sudoku_fill(thiz, r, c, t);
                n_filled++;
            }else
                n_unsolved++;
        }
    } while (n_filled != 0);
    if (n_unsolved == 0){
        ans_count++;
        sudoku_print(thiz);
    }else{
        // find the first empty and try to fill and solve it
        FOR_EACH_EMPTY_CELL(r, c, thiz){
            FOR_EACH_NUM(n){
                if (sudoku_can_fill(thiz, r, c, n)){
                    Sudoku s;
                    memcpy(&s, &thiz, sizeof(Sudoku));

                    sudoku_fill(s, r, c, n);
                    sudoku_solve(s);
                    sudoku_erase(s, r, c, n);
                }
            }
            return;
        }
    }
}

int main()
{
    Sudoku s = {0};

    sudoku_read(s);
    sudoku_print(s);

    sudoku_solve(s);

    cout << ans_count << " answer(s) found." << endl;
    return 0;
}
