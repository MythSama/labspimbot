#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include <sys/time.h>
#include <assert.h>

#define MAXSIZE 16
// #include "originSolver.c"

static FILE *fptr;

double rtclock()
{
    struct timeval Tp;
    int stat;
    stat = gettimeofday (&Tp, NULL);
    if (stat != 0) printf("Error return from gettimeofday: %d",stat);
    return(Tp.tv_sec + Tp.tv_usec*1.0e-6);
}

struct LightsOut {
    int row;
    int column;
    int numColor;
    unsigned char board[MAXSIZE*MAXSIZE];
};

struct choice {
    int row;
    int column;
    int numColor;
    int solCount;
    int noAnsCount;
    int faultCount;
};

int lightsOutSolver(struct LightsOut* puzzle) {
    // variable initialization
    int numRow = puzzle->row;
    int numCol = puzzle->column;
    int numColor = puzzle->numColor;
    unsigned char* board = puzzle->board;
    int stateSize = numRow * numCol;

    unsigned char boardForCheck[MAXSIZE*MAXSIZE];
    for (int i = 0; i < stateSize; i++) {
        boardForCheck[i] = board[i];
    }

    //  initial actionboard
    int actionState[stateSize][stateSize];
    for (int i = 0; i < stateSize; i++) {
        for (int j = 0; j < stateSize; j++) {
            int colIdx = j % numCol;
            actionState[i][j] = 
                (i == j) || (i == j - numCol) || (i == j + numCol) 
                || (i == j + 1 && colIdx != numCol - 1)
                || (i == j - 1 && colIdx != 0);
            // printf("%d", actionState[i][j]);
        }
        // printf("\n");
    }
    //printf("\n");
    for (int i = 0; i < stateSize; i++) {
        board[i] = (numColor - board[i]) % numColor;
        // printf("%d", board[i]);
    }
    // printf("\n");
    // printf("\n");

    // Gaussian Elimination to REF
    int pivot = 0;
    for (int i = 0; i < stateSize; i++) {
        for (int j = pivot + 1; j < stateSize; j++) {
            while (actionState[j][i] != 0) {
                for (int k = 0; k < stateSize; k++) {
                    actionState[j][k] += actionState[pivot][k];
                    actionState[j][k] %= numColor;
                }
                board[j] += board[pivot];
                board[j] %= numColor;
            }
        }
        pivot++;
        // swap rows if needed
        if (actionState[pivot][i+1] == 0) {
            for (int j = pivot + 1; j < stateSize; j++) {
                if (actionState[j][i+1] != 0) {
                    int swapTemp;
                    for (int i = 0; i < stateSize; i++) {
                        swapTemp = actionState[j][i];
                        actionState[j][i] = actionState[pivot][i];
                        actionState[pivot][i] = swapTemp;
                    }
                    swapTemp = board[j];
                    board[j] = board[pivot];
                    board[pivot] = swapTemp;
                    break;
                }
            }
        }
    }

    // print for debugging
    // for (int i = 0; i < stateSize; i++) {
    //     for (int j = 0; j < stateSize; j++) {
    //         printf("%d", actionState[i][j]);
    //     }
    //     printf("\n");
    // }
    // printf("\n");
    // for (int i = 0; i < stateSize; i++) {
    //     printf("%d", board[i]);
    // }
    // printf("\n");

    // Back Subsititution
    int answer[stateSize];
    for (int i = stateSize - 1; i > -1; i--) {
        int backSum = 0;
        for (int j = i + 1; j < stateSize; j++) {
            backSum += actionState[i][j] * answer[j];
        }
        backSum %= numColor;
        int solTemp = board[i];
        solTemp += (numColor - backSum) % numColor;
        solTemp %= numColor;
        if (solTemp != 0 && actionState[i][i] == 0) {
            printf("NO SOLUTION!\n"); return 0;
            break;
        } else {
            // printf("\n%d %d %d\n", i, backSum, solTemp);
            answer[i] = 1;
            int solTemp2 = actionState[i][i];
            while (1 == 1) {
                if (solTemp2 == solTemp) break;
                solTemp2 += actionState[i][i];
                solTemp2 %= numColor;
                // printf("%d", solTemp2);
                answer[i]++;
            } 
            answer[i] %= numColor;
        }
    }

    // Answer check
    // char valid = 1;
    // int tempSum = 0;
    // for (int i = 0; i < stateSize; i++) {
    //     tempSum = boardForCheck[i];
    //     // printf("%d ", tempSum);
    //     int up = i - numCol;
    //     int down = i + numCol;
    //     int left = i - 1;
    //     int right = i + 1;
    //     tempSum += answer[i]; // printf("%d ", tempSum);
    //     if (up >= 0) tempSum += answer[up]; // printf("%d upTol ", tempSum);
    //     if (down < stateSize) tempSum += answer[down]; // printf("%d downTol ", tempSum);
    //     if (i % numCol != 0) tempSum += answer[left]; // printf("%d leftTol ", tempSum);
    //     if ((i + 1) % numCol != 0) tempSum += answer[right]; // printf("%d rightTol ", tempSum);
    //     tempSum %= numColor;
    //     // printf("%d\n", tempSum);
    //     if (tempSum != 0) { valid = 0; break;}
    // }

    // if (valid) printf("yes\n");
    // else { printf("no\n"); return -1;}

    // fprintf(fptr, "Solution:\n");
    // for (int i = 0; i < stateSize; i++) {
    //     fprintf(fptr, "%d", answer[i]);
    //     if ((i+1) % numCol == 0) fprintf(fptr, "\n");
    // }
    // fprintf(fptr, "\n");
    return 1;
}

int main() {

    fptr = fopen("lightsOut.txt", "a");

    // set initial board
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
        game.column = choices[idx].column;
        game.row = choices[idx].row;
        game.numColor = choices[idx].numColor;

        srand((unsigned) time(&t));
        fprintf(fptr, "Game Status:\n");
        for (int i = 0; i < game.column * game.row; i++) {
            game.board[i] = rand() % game.numColor;
            fprintf(fptr, "%d", game.board[i]);
            if ((i+1) % game.column == 0) fprintf(fptr, "\n");
        }
        fprintf(fptr, "\n");
        
        t_start = rtclock();
        int flag = lightsOutSolver(&game);
        t_end = rtclock();
        fprintf(stdout,"Time: %7.5lf ms\n", (t_end - t_start)*1.0e4);

        if (flag == 1) choices[idx].solCount++;
        if (flag == 0) choices[idx].noAnsCount++;
        if (flag == -1) choices[idx].faultCount++;
    }
    for (int i = 0; i < 10; i++) {
        printf("%d %d %d\n", choices[i].solCount, choices[i].noAnsCount, choices[i].faultCount);
    }
    
    // for (int i = 0; i < game.column * game.row; i++) {
    //     if (i == 0 || i == 2 || i == 5 || i == 6) game.board[i] = 0;
    //     else game.board[i] = 1;
    //     fprintf(fptr, "%d", game.board[i]);
    //     if ((i+1) % game.column == 0) fprintf(fptr, "\n");
    // }
    // for (int i = 0; i < game.column * game.row; i++) {
    //     if (i == 0 || i == 1 || i == 2 || i == 5 || i == 6 || i == 7 || i == 16 || i == 18) game.board[i] = 1;
    //     else if (i == 4 || i == 10 || i == 11 || i == 12 || i == 15 || i == 17 || i == 20)  game.board[i] = 0;
    //         else game.board[i] = 2; 
    //     fprintf(fptr, "%d", game.board[i]);
    //     if ((i+1) % game.column == 0) fprintf(fptr, "\n");
    // }
    // fprintf(fptr, "\n");

    return 0;
}