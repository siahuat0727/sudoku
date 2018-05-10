#include <iostream>
#include <sys/time.h>
#include <unistd.h>
#include <cstring>
#include <cstdlib>
#include <vector>
#include <algorithm>

using namespace std;

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
            cout << sudoku[i][j] << ' ';
        cout << endl;
    }
    cout << endl;
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

inline bool can_fill(int r, int c, int n)
{
    return can_fill_row(r, n) && can_fill_col(c, n) && can_fill_box(r, c, n);
}

inline void next_bt(int r, int c)
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
            cin >> t;
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

void test(int n_repeat){
    vector <int> v;
    int total = 100;
    int tmp[N][N];
    memcpy(tmp, sudoku, sizeof(tmp));
    for (int i = 0; i < total; ++i){
        struct timeval start, end;
        gettimeofday(&start, NULL);
        for (int j = 0; j < n_repeat; ++j){
            memcpy(sudoku, tmp, sizeof(tmp));
            bt(0, 0);
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
    read_sudoku();
    if(argc == 2){
        timing = true;
        test(atoi(argv[1]));
    }else{
        print_sudoku();
        bt(0, 0);
    }
    cout << ans_count << " answer(s) found." << endl;
    return 0;
}
