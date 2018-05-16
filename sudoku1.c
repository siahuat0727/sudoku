#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <sys/time.h>
#include <unistd.h>

#define N 9

bool timing;
int ans_count;
int sudoku[N][N];

void bt(int r, int c);

void print_sudoku()
{
    if (timing)
        return;
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j)
            printf("%s%d", j==0 ? "" : " ", sudoku[i][j]);
        puts("");
    }
    puts("");
}

bool can_fill_row(int r, int n)
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

bool can_fill(int r, int c, int n)
{
    return can_fill_row(r, n) && can_fill_col(c, n) && can_fill_box(r, c, n);
}

void next_bt(int r, int c)
{
    c == N-1 ? bt(r+1, 0) : bt(r, c+1);
}

void bt(int r, int c)
{
    if(r == N) {
        ans_count++;
        print_sudoku();
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

void read_sudoku()
{
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            int t;
            scanf("%d", &t);
            if(t == 0)
                continue;
            if(t > 0 && t <= N && can_fill(i, j, t))
                sudoku[i][j] = t;
            else {
                fprintf(stderr, "Wrong input: row %d col %d value %d", i+1, j+1, t);
                exit(1);
            }
        }
    }
}

void test(int n_repeat)
{
    int cmp_func(const void *a, const void *b) {
        return (*(int*)a - *(int*)b);
    }

    int total = 100;
    int time_usage[total];
    int sudoku_tmp[N][N];
    memcpy(sudoku_tmp, sudoku, sizeof(sudoku_tmp));
    for (int i = 0; i < total; ++i) {
        struct timeval start, end;
        gettimeofday(&start, NULL);
        for (int j = 0; j < n_repeat; ++j) {
            memcpy(sudoku, sudoku_tmp, sizeof(sudoku_tmp));
            bt(0, 0);
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

    printf("%f\n", total_time / (1e3 * n_repeat * (total- (2*skip))));
}

int main(int argc, const char **argv)
{
    read_sudoku();
    if(argc == 2) {
        timing = true;
        test(atoi(argv[1]));
    } else {
        print_sudoku();
        bt(0, 0);
    }
    printf("%d  answer(s) found.\n", ans_count);
    return 0;
}
