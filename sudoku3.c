#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <sys/time.h>
#include <unistd.h>
#include <assert.h>

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

bool recording;
int ans_count;

void sudoku_print(Sudoku *thiz)
{
    if (recording)
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
    // assert(thiz.row[r][n] != b && thiz.col[c][n] != b && thiz.box[get_box(r, c)][n] != b);
    // assert(n > 0 && n <= N);
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

void sudoku_solve(Sudoku *thiz)
{
    int num_filled, num_unsolved;
    do {
        num_filled = 0;
        num_unsolved = 0;
        FOR_FOR_EACH_EMPTY_CELL(r, c, thiz) {
            int num_can_fill = 0;
            int t;
            FOR_EACH_NUM(n) {
                if(sudoku_can_fill(thiz, r, c, n)) {
                    num_can_fill++;
                    t = n;
                    if (num_can_fill > 1) // pruning
                        break;
                }
            }
            if (num_can_fill == 0)
                return;
            if (num_can_fill == 1) {
                sudoku_fill(thiz, r, c, t);
                num_filled++;
            } else
                num_unsolved++;
        }
    } while (num_filled != 0);
    if (num_unsolved == 0) {
        ans_count++;
        sudoku_print(thiz);
    } else {
        // find the first empty cell, try to fill and solve it
        FOR_FOR_EACH_EMPTY_CELL(r, c, thiz) {
            FOR_EACH_NUM(n) {
                if (sudoku_can_fill(thiz, r, c, n)) {
                    sudoku_fill(thiz, r, c, n);

                    // copy and solve
                    Sudoku sudoku_tmp;
                    memcpy(&sudoku_tmp, thiz, sizeof(Sudoku));
                    sudoku_solve(&sudoku_tmp);

                    sudoku_erase(thiz, r, c, n);
                }
            }
            return;
        }
    }
}

void record_time(Sudoku s, const int num_repeat)
{
    int cmp_func(const void *a, const void *b) {
        return (*(int*)a - *(int*)b);
    }

    const int total = 100;
    int time_usage[total];
    Sudoku tmp;
    for (int i = 0; i < total; ++i) {
        struct timeval start, end;
        gettimeofday(&start, NULL);
        for (int j = 0; j < num_repeat; ++j) {
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

    printf("%f\n", total_time / (1e3 * num_repeat * (total - (2*skip))));
}

int main(int argc, const char **argv)
{
    Sudoku s = {0};
    sudoku_read(&s);
    if(argc == 2) {
        recording = true;
        record_time(s, atoi(argv[1]));
    } else {
        sudoku_print(&s);
        sudoku_solve(&s);
    }
    printf("%d  answer(s) found.\n", ans_count);
    return 0;
}
