#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <sys/time.h>
#include <unistd.h>
#include <assert.h>
#include <stack>
using namespace std;

#define N 9
#define FOR_N(i) for (int i = 0; i < N; ++i)
#define FOR_FOR_EACH_EMPTY_CELL(r, c, s) FOR_N(r) FOR_N(c) if(s->grid[r][c] == 0)
#define FOR_EACH_NUM(n) for (int n = 1; n <= N; ++n)

typedef struct _Sudoku {
    int grid[N][N];
    bool row[N][N+1];
    bool col[N][N+1];
    bool box[N][N+1];
} Sudoku;

struct Cell {
    int r, c, n;
};

bool timing;
int ans_count;

void sudoku_solve(Sudoku *thiz, bool record);

void sudoku_print(Sudoku *thiz)
{
    if(timing)
        return;
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j)
            printf("%s%d", j==0 ? "" : " ", thiz->grid[i][j]);
        puts("");
    }
    puts("");
}

int get_box(int r, int c)
{
    return r/3*3 + c/3;
}

bool sudoku_can_fill(Sudoku *thiz, int r, int c, int n)
{
    return !thiz->row[r][n] && !thiz->col[c][n] && !thiz->box[get_box(r,c)][n];
}

void _sudoku_fill(Sudoku *thiz, int r, int c, int n, bool b)
{
    assert(thiz->row[r][n] != b && thiz->col[c][n] != b && thiz->box[get_box(r, c)][n] != b);
    assert(n > 0 && n <= N);
    thiz->row[r][n] = b;
    thiz->col[c][n] = b;
    thiz->box[get_box(r,c)][n] = b;
    thiz->grid[r][c] = b ? n : 0;
}

void sudoku_fill(Sudoku *thiz, int r, int c, int n)
{
    _sudoku_fill(thiz, r, c, n, true);
}

void sudoku_erase(Sudoku *thiz, int r, int c, int n)
{
    _sudoku_fill(thiz, r, c, n, false);
}

void sudoku_read(Sudoku *thiz)
{
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            int t;
            scanf("%d", &t);
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

void _sudoku_solve(Sudoku *thiz, stack<Cell> *s, bool record)
{
    int n_filled, n_unsolved;
    do {
        n_filled = 0;
        n_unsolved = 0;
        FOR_FOR_EACH_EMPTY_CELL(r, c, thiz) {
            int n_can_fill = 0;
            int t;
            FOR_EACH_NUM(n) {
                if(sudoku_can_fill(thiz, r, c, n)) {
                    n_can_fill++;
                    t = n;
                    if (n_can_fill > 1) // pruning
                        break;
                }
            }
            if (n_can_fill == 0)
                return;
            if (n_can_fill == 1) {
                sudoku_fill(thiz, r, c, t);
                n_filled++;
                if (record) {
                    Cell cell = {.r=r, .c=c, .n=t};
                    s->push(cell);
                }
            } else
                n_unsolved++;
        }
    } while (n_filled != 0);
    if (n_unsolved == 0) {
        ans_count++;
        sudoku_print(thiz);
    } else {
        // find the first empty cell, try to fill and solve it
        FOR_FOR_EACH_EMPTY_CELL(r, c, thiz) {
            FOR_EACH_NUM(n) {
                if (sudoku_can_fill(thiz, r, c, n)) {
                    sudoku_fill(thiz, r, c, n);
                    sudoku_solve(thiz, true);
                    sudoku_erase(thiz, r, c, n);
                }
            }
            return;
        }
    }
}

void sudoku_solve(Sudoku *thiz, bool record=false)
{
    stack<Cell> s;
    _sudoku_solve(thiz, &s, record);
    if(record) {
        while(!s.empty()) {
            Cell cell = s.top();
            sudoku_erase(thiz, cell.r, cell.c, cell.n);
            s.pop();
        }
    }
}

int cmp_func(const void *a, const void *b)
{
    return (*(int*)a - *(int*)b);
}

void test(Sudoku s, const int n_repeat)
{
    const int total = 100;
    int time_usage[total];
    Sudoku tmp;
    for (int i = 0; i < total; ++i) {
        struct timeval start, end;
        gettimeofday(&start, NULL);
        for (int j = 0; j < n_repeat; ++j) {
            memcpy(&tmp, &s, sizeof(tmp));
            sudoku_solve(&tmp);
        }
        gettimeofday(&end, NULL);
        int usec = (1e6) * (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec);
        time_usage[i] = usec;
    }
    qsort(time_usage, total, sizeof(time_usage[0]), cmp_func);

    double total_time = 0;
    const int skip = total / 5;
    for (int i = skip; i < total - skip; ++i)
        total_time += time_usage[i];

    printf("%f\n", total_time / (1e3 * n_repeat * (total - (2*skip))));
}

int main(int argc, const char **argv)
{
    Sudoku s = {0};
    sudoku_read(&s);
    if(argc == 2) {
        timing = true;
        test(s, atoi(argv[1]));
    } else {
        sudoku_print(&s);
        sudoku_solve(&s);
    }
    printf("%d  answer(s) found.\n", ans_count);
    return 0;
}
