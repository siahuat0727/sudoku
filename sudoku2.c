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
bool row[N][N+1];
bool col[N][N+1];
bool box[N][N+1];

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

int get_box(int r, int c)
{
    return r/3*3 + c/3;
}

bool can_fill(int r, int c, int n)
{
    return !row[r][n] && !col[c][n] && !box[get_box(r,c)][n];
}

void _fill(int r, int c, int n, bool b)
{
    row[r][n] = b;
    col[c][n] = b;
    box[get_box(r,c)][n] = b;
    sudoku[r][c] = b ? n : 0;
}

void fill(int r, int c, int n)
{
    _fill(r, c, n, true);
}

void erase(int r, int c, int n)
{
    _fill(r, c, n, false);
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
                fill(r, c, n);
                next_bt(r, c);
                erase(r, c, n);
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
            if (t == 0)
                continue;
            if(t > 0 && t <= N && can_fill(i, j, t))
                fill(i, j, t);
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
