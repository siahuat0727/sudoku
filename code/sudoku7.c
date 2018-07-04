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
#define FOR_3(k, start) for (int kk = start, k = kk; k < kk+3; ++k)
#define FOR_R_C_IN_BOX(r, c, i) FOR_3(r, get_r(i)) FOR_3(c, get_c(i))

typedef struct _Sudoku {
    int grid[N][N];
    bool row[N][N+1];
    bool col[N][N+1];
    bool box[N][N+1];
    bool can_fill[N][N][N+1];
    int num_can_fill_cell[N][N];
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

void sudoku_init(Sudoku *thiz)
{
    memset(thiz->grid, 0, sizeof(thiz->grid));
    memset(thiz->row, 0, sizeof(thiz->row));
    memset(thiz->col, 0, sizeof(thiz->col));
    memset(thiz->box, 0, sizeof(thiz->box));
    memset(thiz->can_fill, true, sizeof(thiz->can_fill));
    FOR_N(i) {
        FOR_N(j) {
            thiz->num_can_fill_cell[i][j] = 9;
        }
    }
}

int get_r(int box_i)
{
    return box_i/3*3;
}

int get_c(int box_i)
{
    return (box_i%3)*3;
}

int get_box(int r, int c)
{
    return r/3*3 + c/3;
}

bool sudoku_can_fill(Sudoku *thiz, int r, int c, int n)
{
    return thiz->can_fill[r][c][n];
}

bool sudoku_can_fill_2(Sudoku *thiz, int r, int c, int n)
{
    return !thiz->row[r][n] && !thiz->col[c][n] && !thiz->box[get_box(r,c)][n];
}

bool sudoku_maybe_update_can_fill(Sudoku *thiz, int r, int c, int n, bool filled)
{
    if (filled) {
        if (thiz->can_fill[r][c][n] == true) {
            thiz->can_fill[r][c][n] = false;
            thiz->num_can_fill_cell[r][c]--;
        }
    } else {
        if (thiz->can_fill[r][c][n] == false) {
            if (sudoku_can_fill_2(thiz, r, c, n)) {
                thiz->can_fill[r][c][n] = true;
                thiz->num_can_fill_cell[r][c]++;
            }
        }
    }
}

void _sudoku_fill(Sudoku *thiz, int r, int c, int n, bool b)
{
    const int box_index = get_box(r, c);
    thiz->row[r][n] = b;
    thiz->col[c][n] = b;
    thiz->box[box_index][n] = b;
    thiz->grid[r][c] = b ? n : 0;

    FOR_N(rr) {
        sudoku_maybe_update_can_fill(thiz, rr, c, n, b);
    }
    FOR_N(cc) {
        sudoku_maybe_update_can_fill(thiz, r, cc, n, b);
    }
    FOR_R_C_IN_BOX(rr, cc, box_index) {
        sudoku_maybe_update_can_fill(thiz, rr, cc, n, b);
    }
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

#define FILL_IF_EXACTLY_ONE(LOOP1, LOOP2, save_tmp, sudoku_fill_stmt) \
    do { \
        LOOP1 { \
            int num_can_fill = 0; \
            int t; \
            LOOP2 { \
                if (sudoku_can_fill(thiz, r, c, n)) { \
                    num_can_fill++; \
                    save_tmp; \
                    if (num_can_fill > 1) /* pruning */ \
                        break; \
                } \
            } \
            if (num_can_fill == 0) \
                return false; \
            if (num_can_fill == 1) { \
                sudoku_fill_stmt; \
                ++*num_filled; \
            } else if (count_unsolved) \
                ++*num_unsolved; \
        } \
        return true; \
    } while (0)

bool sudoku_solve_row(Sudoku *thiz, int *num_filled, int *num_unsolved, bool count_unsolved)
{
    int cc;
    FILL_IF_EXACTLY_ONE(
        FOR_N(r) FOR_EACH_NUM(n) if(!thiz->row[r][n]),
        FOR_N(c) if(!thiz->grid[r][c]),
        cc = c,
        sudoku_fill(thiz, r, cc, n)
    );
}

bool sudoku_solve_col(Sudoku *thiz, int *num_filled, int *num_unsolved, bool count_unsolved)
{
    int rr;
    FILL_IF_EXACTLY_ONE(
        FOR_N(c) FOR_EACH_NUM(n) if(!thiz->col[c][n]),
        FOR_N(r) if(!thiz->grid[r][c]),
        rr = r,
        sudoku_fill(thiz, rr, c, n)
    );
}

bool sudoku_solve_box(Sudoku *thiz, int *num_filled, int *num_unsolved, bool count_unsolved)
{
    int rr, cc;
    FILL_IF_EXACTLY_ONE(
        FOR_N(i) FOR_EACH_NUM(n) if(!thiz->box[i][n]),
        FOR_R_C_IN_BOX(r, c, i) if(!thiz->grid[r][c]),
        rr = r; cc = c,
        sudoku_fill(thiz, rr, cc, n)
    );
}

bool sudoku_solve_cell(Sudoku *thiz, int *num_filled, int *num_unsolved, bool count_unsolved)
{
    FOR_FOR_EACH_EMPTY_CELL(r, c, thiz) {
        if (thiz->num_can_fill_cell[r][c] == 1) {
            FOR_EACH_NUM(n) {
                if (sudoku_can_fill(thiz, r, c, n)) {
                    sudoku_fill(thiz, r, c, n);
                    ++*num_filled;
                    break;
                }
            }
        } else if (thiz->num_can_fill_cell[r][c] == 0) {
            return false; // pruning
        } else if (count_unsolved) {
            ++*num_unsolved;
        }
    }
    return true;
}

void sudoku_solve(Sudoku *thiz)
{
    int num_filled, num_unsolved;
    do {
        num_filled = 0;
        num_unsolved = 0;

        if (!sudoku_solve_row(thiz, &num_filled, &num_unsolved, false) ||
            !sudoku_solve_col(thiz, &num_filled, &num_unsolved, false) ||
            !sudoku_solve_box(thiz, &num_filled, &num_unsolved, false) ||
            !sudoku_solve_cell(thiz, &num_filled, &num_unsolved, true))
            return;

    } while (num_filled != 0);
    if (num_unsolved == 0) {
        ans_count++;
        sudoku_print(thiz);
    } else {
        int Min = N;
        int r_min = -1, c_min = -1;
        // find the empty cell with fewest possible ans, try to fill and solve it
        FOR_FOR_EACH_EMPTY_CELL(r, c, thiz) {
            if (thiz->num_can_fill_cell[r][c] < Min) {
                Min = thiz->num_can_fill_cell[r][c];
                r_min = r;
                c_min = c;
            }
        }
        FOR_EACH_NUM(n) {
            if (sudoku_can_fill(thiz, r_min, c_min, n)) {
                // copy and solve
                Sudoku sudoku_tmp;
                memcpy(&sudoku_tmp, thiz, sizeof(Sudoku));

                sudoku_fill(&sudoku_tmp, r_min, c_min, n);
                sudoku_solve(&sudoku_tmp);
            }
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
    Sudoku s;
    sudoku_init(&s);
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
