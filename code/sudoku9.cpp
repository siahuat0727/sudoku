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
#define FOR_3(k, start) for (int kk = start, k = kk; k < kk+3; ++k)
#define FOR_R_C_IN_BOX(r, c, i) FOR_3(r, get_r(i)) FOR_3(c, get_c(i))

struct Cell {
    int r, c, n;
};

typedef struct _Sudoku {
    int grid[N][N];
    bool row[N][N+1];
    bool col[N][N+1];
    bool box[N][N+1];
    bool can_fill[N][N][N+1];
    int num_can_fill_row[N][N+1];
    int num_can_fill_col[N][N+1];
    int num_can_fill_box[N][N+1];
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
        FOR_EACH_NUM(n) {
            thiz->num_can_fill_row[i][n] = 9;
            thiz->num_can_fill_col[i][n] = 9;
            thiz->num_can_fill_box[i][n] = 9;
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
    return !thiz->grid[r][c] && !thiz->row[r][n] && !thiz->col[c][n] && !thiz->box[get_box(r,c)][n];
}

void sudoku_check(Sudoku *thiz)
{
    FOR_EACH_NUM(n) {
        FOR_N(i) {
            FOR_N(j) {
                assert(thiz->can_fill[i][j][n] == sudoku_can_fill_2(thiz, i, j, n));
            }
        }
        FOR_N(i) {
            int t_row = 0;
            int t_col = 0;
            int t_box = 0;
            FOR_N(j) {
                t_row += thiz->grid[i][j] == 0 && sudoku_can_fill_2(thiz, i, j, n);
                t_col += thiz->grid[j][i] == 0 && sudoku_can_fill_2(thiz, j, i, n);
            }
            FOR_R_C_IN_BOX(rr, cc, i) {
                t_box += thiz->grid[rr][cc] == 0 && sudoku_can_fill_2(thiz, rr, cc, n);
            }
            assert(thiz->row[i][n] || t_row == thiz->num_can_fill_row[i][n]);
            assert(thiz->col[i][n] || t_col == thiz->num_can_fill_col[i][n]);
            assert(thiz->box[i][n] || t_box == thiz->num_can_fill_box[i][n]);
        }
    }
    FOR_N(r) {
        FOR_N(c) {
            int t_cell = 0;
            FOR_EACH_NUM(n) {
                t_cell += thiz->grid[r][c] == 0 && sudoku_can_fill_2(thiz, r, c, n);
            }
            assert(thiz->grid[r][c] || t_cell == thiz->num_can_fill_cell[r][c]);
        }
    }
}

void sudoku_maybe_update_can_fill(Sudoku *thiz, int r, int c, int n, bool fill)
{
    if (fill) {
        if (thiz->can_fill[r][c][n]) {
            thiz->can_fill[r][c][n] = false;
            thiz->num_can_fill_cell[r][c]--;
            thiz->num_can_fill_row[r][n]--;
            thiz->num_can_fill_col[c][n]--;
            thiz->num_can_fill_box[get_box(r, c)][n]--;
        }
    } else {
        if (!thiz->can_fill[r][c][n]
            && sudoku_can_fill_2(thiz, r, c, n)) {
            thiz->can_fill[r][c][n] = true;
            thiz->num_can_fill_cell[r][c]++;
            thiz->num_can_fill_row[r][n]++;
            thiz->num_can_fill_col[c][n]++;
            thiz->num_can_fill_box[get_box(r, c)][n]++;
        }
    }
}

void _sudoku_fill(Sudoku *thiz, int r, int c, int n, bool fill)
{
    thiz->grid[r][c] = fill ? n : 0;

    const int box_index = get_box(r, c);
    thiz->row[r][n] = fill;
    thiz->col[c][n] = fill;
    thiz->box[box_index][n] = fill;

    FOR_N(rr) {
        sudoku_maybe_update_can_fill(thiz, rr, c, n, fill);
    }
    FOR_N(cc) {
        sudoku_maybe_update_can_fill(thiz, r, cc, n, fill);
    }
    FOR_R_C_IN_BOX(rr, cc, box_index) {
        sudoku_maybe_update_can_fill(thiz, rr, cc, n, fill);
    }
    FOR_EACH_NUM(nn) {
        sudoku_maybe_update_can_fill(thiz, r, c, nn, fill);
    }
    // sudoku_check(thiz);
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

#define FILL_IF_EXACTLY_ONE(LOOP1, LOOP2, num_can_fill) \
    do { \
        LOOP1 { \
            if (num_can_fill == 1) { \
                LOOP2 { \
                    if (sudoku_can_fill(thiz, r, c, n)) { \
                        sudoku_fill(thiz, r, c, n); \
                        ++*num_filled; \
                        if (record) { \
                            Cell cell = {.r=r, .c=c, .n=n}; \
                            filled_cells->push(cell); \
                        } \
                        break; \
                    } \
                } \
            } else if (num_can_fill == 0) { \
                return false; /* pruning */ \
            } else if (count_unsolved) { \
                ++*num_unsolved; \
            } \
        } \
        return true; \
    } while (0)

bool sudoku_solve_row(Sudoku *thiz, int *num_filled, int *num_unsolved, 
        bool count_unsolved, stack<Cell> *filled_cells, bool record)
{
    FILL_IF_EXACTLY_ONE(
        FOR_N(r) FOR_EACH_NUM(n) if(!thiz->row[r][n]),
        FOR_N(c),
        thiz->num_can_fill_row[r][n]
    );
}

bool sudoku_solve_col(Sudoku *thiz, int *num_filled, int *num_unsolved, 
        bool count_unsolved, stack<Cell> *filled_cells, bool record)
{
    FILL_IF_EXACTLY_ONE(
        FOR_N(c) FOR_EACH_NUM(n) if(!thiz->col[c][n]),
        FOR_N(r),
        thiz->num_can_fill_col[c][n]
    );
}

bool sudoku_solve_box(Sudoku *thiz, int *num_filled, int *num_unsolved, 
        bool count_unsolved, stack<Cell> *filled_cells, bool record)
{
    FILL_IF_EXACTLY_ONE(
        FOR_N(i) FOR_EACH_NUM(n) if(!thiz->box[i][n]),
        FOR_R_C_IN_BOX(r, c, i),
        thiz->num_can_fill_box[i][n]
    );
}

bool sudoku_solve_cell(Sudoku *thiz, int *num_filled, int *num_unsolved, 
        bool count_unsolved, stack<Cell> *filled_cells, bool record)
{
    FILL_IF_EXACTLY_ONE(
        FOR_FOR_EACH_EMPTY_CELL(r, c, thiz),
        FOR_EACH_NUM(n),
        thiz->num_can_fill_cell[r][c]
    );
}

int NUM;
void sudoku_solve(Sudoku *thiz, bool record=false)
{
    stack<Cell> filled_cells;
    NUM++;
    int num_filled, num_unsolved;
    bool no_ans = false;

    do {
        num_filled = 0;
        num_unsolved = 0;

        if (!sudoku_solve_row(thiz, &num_filled, &num_unsolved, false, &filled_cells, record) ||
                !sudoku_solve_col(thiz, &num_filled, &num_unsolved, false, &filled_cells, record) ||
                !sudoku_solve_box(thiz, &num_filled, &num_unsolved, false, &filled_cells, record) ||
                !sudoku_solve_cell(thiz, &num_filled, &num_unsolved, true, &filled_cells, record)) {
            no_ans = true;
            break;
        }

    } while (num_filled != 0);

    do {
        if (no_ans)
            break;
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
                    sudoku_fill(thiz, r_min, c_min, n);
                    sudoku_solve(thiz, true);
                    sudoku_erase(thiz, r_min, c_min, n);
                }
            }
        }
    } while (0);

    while(!filled_cells.empty()) {
        Cell cell = filled_cells.top();
        sudoku_erase(thiz, cell.r, cell.c, cell.n);
        filled_cells.pop();
    }
}

int cmp_func(const void *a, const void *b) {
    return (*(int*)a - *(int*)b);
}

void record_time(Sudoku s, const int num_repeat)
{
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
    printf("num = %d\n", NUM);
    return 0;
}
