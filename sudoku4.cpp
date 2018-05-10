#include <iostream>
#include <assert.h>
#include <sys/time.h>
#include <unistd.h>
#include <cstring>
#include <cstdlib>
#include <vector>
#include <algorithm>

using namespace std;

#define N 9
#define FOR_N(i) for (int i = 0; i < N; ++i)
#define FOR_EACH_EMPTY_CELL(r, c, s) FOR_N(r) FOR_N(c) if(s.grid[r][c] == 0)
#define FOR_EACH_NUM(n) for (int n = 1; n <= N; ++n)

struct Sudoku{
    int grid[N][N];
    bool row[N][N+1];
    bool col[N][N+1];
    bool box[N][N+1];
};

struct Cell{
    int r, c, n;
};

bool timing;
int ans_count;

void bt(int r, int c);

void sudoku_print(Sudoku &thiz)
{
    if(timing)
        return;
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
    return !thiz.row[r][n] && !thiz.col[c][n] && !thiz.box[get_box(r,c)][n];
}

inline void _sudoku_fill(Sudoku &thiz, int r, int c, int n, bool b)
{
    assert(thiz.row[r][n] != b && thiz.col[c][n] != b && thiz.box[get_box(r, c)][n] != b);
    assert(n > 0 && n <= N);
    thiz.row[r][n] = b;
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
                fprintf(stderr, "Wrong input: row %d col %d value %d", i+1, j+1, t);
                exit(1);
            }
        }
    }
}

void sudoku_solve(Sudoku &thiz){
    vector<Cell> v;
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
                goto end;
            if (n_can_fill == 1){
                sudoku_fill(thiz, r, c, t);
                Cell cell = {.r=r, .c=c, .n=t};
                v.push_back(cell);
                n_filled++;
            }else
                n_unsolved++;
        }
    } while (n_filled != 0);
    if (n_unsolved == 0){
        ans_count++;
        sudoku_print(thiz);
    }else{
        // find the first empty cell, try to fill and solve it
        FOR_EACH_EMPTY_CELL(r, c, thiz){
            FOR_EACH_NUM(n){
                if (sudoku_can_fill(thiz, r, c, n)){
                    sudoku_fill(thiz, r, c, n);
                    sudoku_solve(thiz);
                    sudoku_erase(thiz, r, c, n);
                }
            }
            for (Cell cell : v)
                sudoku_erase(thiz, cell.r, cell.c, cell.n);
            goto end;
        }
    }
end:
    for (Cell cell : v)
        sudoku_erase(thiz, cell.r, cell.c, cell.n);
}

void test(Sudoku s, int n_repeat){
    vector <int> v;
    int total = 100;
    Sudoku tmp;
    for (int i = 0; i < total; ++i){
        struct timeval start, end;
        gettimeofday(&start, NULL);
        for (int j = 0; j < n_repeat; ++j){
            memcpy(&tmp, &s, sizeof(tmp));
            sudoku_solve(tmp);
        }
        gettimeofday(&end, NULL);
        int usec = (1e6) * (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec);
        v.push_back(usec);
    }
    sort(v.begin(), v.end());

    double total_time = 0;
    for (int i = 20; i < v.size() - 20; ++i){
        total_time += v[i];
    }

    cout << total_time / (1e3 * n_repeat * v.size()-40) << endl;
}

int main(int argc, const char **argv)
{
    Sudoku s = {0};
    sudoku_read(s);
    if(argc == 2){
        timing = true;
        test(s, atoi(argv[1]));
    }else{
        sudoku_print(s);
        sudoku_solve(s);
    }
    cout << ans_count << " answer(s) found." << endl;
    return 0;
}
