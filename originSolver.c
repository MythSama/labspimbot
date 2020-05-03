#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <unistd.h>
#include <sys/time.h>
#include <assert.h>

#define MAXSIZE 16

static FILE *fptr;

const int MAX_GRIDSIZE = 16;
struct LightsOut {
    int num_rows;
    int num_cols;
    int num_color;
    unsigned char board[MAX_GRIDSIZE*MAX_GRIDSIZE];
    bool clue[MAX_GRIDSIZE*MAX_GRIDSIZE]; //(using bytes in SpimBot)
};

void toggle_light(int row, int col, struct LightsOut* puzzle, int action_num){
    int num_rows = puzzle->num_rows;
    int num_cols = puzzle->num_cols;
    int num_colors = puzzle->num_color;
    unsigned char* board = (puzzle-> board);
    board[row*num_cols + col] = (board[row*num_cols + col] + action_num) % num_colors;
    if (row > 0){
        board[(row-1)*num_cols + col] = (board[(row-1)*num_cols + col] + action_num) % num_colors;
    }
    if (col > 0){
        board[(row)*num_cols + col-1] = (board[(row)*num_cols + col-1] + action_num) % num_colors;
    }
    
    if (row < num_rows - 1){
        board[(row+1)*num_cols + col] = (board[(row+1)*num_cols + col] + action_num) % num_colors;
    }

    if (col < num_cols - 1){
        board[(row)*num_cols + col+1] = (board[(row)*num_cols + col+1] + action_num) % num_colors;
    }
}

bool board_done(int num_rows, int num_cols, unsigned char* board){ 
    for (int row = 0; row < num_rows; row++) {
        for (int col = 0; col < num_cols; col++) {
            if (board[(row)*num_cols + col] != 0) {
                return false;
            }
        }
    }
    return true;
}

bool solve(struct LightsOut* puzzle, unsigned char* solution, int row, int col){
    int num_rows = puzzle->num_rows; 
    int num_cols = puzzle->num_cols;
    int num_colors = puzzle->num_color;
    int next_row = ((col == num_cols-1) ? row + 1 : row);
    if (row >= num_rows || col >= num_cols) {
         return board_done(num_rows,num_cols,puzzle->board);
    }
    if (row != 0) {
        int actions = (num_colors - puzzle->board[(row-1)*num_cols + col]) % num_colors;
        solution[row*num_cols + col] = actions;
        toggle_light(row, col, puzzle, actions);
        if (solve(puzzle,solution, next_row, (col + 1) % num_cols)) {
            return true;
        }
        solution[row*num_cols + col] = 0;
        toggle_light(row, col, puzzle, num_colors - actions);
        return false;
    }

    for(char actions = 0; actions < num_colors; actions++) {
        solution[row*num_cols + col] = actions;
        toggle_light(row, col, puzzle, actions);
        if (solve(puzzle,solution, next_row, (col + 1) % num_cols)) {
            return true;
        }
        toggle_light(row, col, puzzle, num_colors - actions); 
        solution[row*num_cols + col] = 0;
    }
    return false;
}

struct choice {
    int row;
    int column;
    int numColor;
    int solCount;
    int noAnsCount;
    int faultCount;
};

double rtclock()
{
    struct timeval Tp;
    int stat;
    stat = gettimeofday (&Tp, NULL);
    if (stat != 0) printf("Error return from gettimeofday: %d",stat);
    return(Tp.tv_sec + Tp.tv_usec*1.0e-6);
}

int main() {

    fptr = fopen("lightsOutOri.txt", "a");

    struct LightsOut game;
    struct choice choices[10] = {
        {12, 4, 2, 0, 0, 0},
        {15, 4, 2, 0, 0, 0},
        {16, 4, 2, 0, 0, 0},
        {6, 5, 2, 0, 0, 0},
        {13, 5, 2, 0, 0, 0},
        {5, 6, 2, 0, 0, 0},
        {7, 3, 3, 0, 0, 0},
        {12, 3, 3, 0, 0, 0},
        {13, 3, 3, 0, 0, 0},
        {11, 5, 3, 0, 0, 0}
    };
    double t_start, t_end;
    time_t t;
    for (int i = 0; i < 10000; i++) {
        
        int idx = i % 10;
        game.num_cols = choices[idx].column;
        game.num_rows = choices[idx].row;
        game.num_color = choices[idx].numColor;

        srand((unsigned) time(&t));
        fprintf(fptr, "Game Status:\n");
        for (int i = 0; i < game.num_cols * game.num_rows; i++) {
            game.board[i] = rand() % game.num_color;
            fprintf(fptr, "%d", game.board[i]);
            if ((i+1) % game.num_cols == 0) fprintf(fptr, "\n");
        }
        fprintf(fptr, "\n");
        
        t_start = rtclock();
        unsigned char solution[4096];
        bool flag = solve(&game, solution, 0, 0);
        t_end = rtclock();
        fprintf(stdout,"Time: %7.5lf ms\n", (t_end - t_start)*1.0e4);

        if (flag == 1) choices[idx].solCount++;
        if (flag == 0) choices[idx].noAnsCount++;
        // if (flag == -1) choices[idx].faultCount++;
    }
    
    for (int i = 0; i < 10; i++) {
        printf("%d %d %d\n", choices[i].solCount, choices[i].noAnsCount, choices[i].faultCount);
    }

    return 0;
}