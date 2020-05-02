#include <stdio.h>
#include <stdlib.h>

#define MAXSIZE 16

struct LightsOut {
    int row;
    int column;
    int numColor;
    unsigned char board[MAXSIZE*MAXSIZE];
};

void lightsOutSolver(struct LightsOut* puzzle) {
    // variable initialization
    int num_row = puzzle->row;
    int num_col = puzzle->column;
    int numColor = puzzle->numColor;
    unsigned char* board = puzzle->board;
    int stateSize = num_row * num_col;

    //  initial actionboard
    int actionState[stateSize][stateSize];
    for (int i = 0; i < stateSize; i++) {
        for (int j = 0; j < stateSize; j++) {
            int colIdx = j % num_col;
            int rowIdx = j / num_col;
            actionState[i][j] = 
                (i == j) || (i == j - num_col) || (i == j + num_col) 
                || (i == j + 1 && colIdx != num_col - 1)
                || (i == j - 1 && colIdx != 0);
            // printf("%d", actionState[i][j]);
        }
        // printf("\n");
    }
    // printf("\n");
    // for (int i = 0; i < stateSize; i++) {
    //     board[i] = (numColor - board[i]) % numColor;
    //     printf("%d", board[i]);
    // }
    // printf("\n");
    // printf("\n");

    // Gaussian Elimination to REF
    int pivot = 0;
    for (int i = 0; i < stateSize; i++) {
        while (1 == 1) {
            for (int j = pivot + 1; j < stateSize; j++) {
                if (actionState[j][i] != 0) {
                    for (int k = 0; k < stateSize; k++) {
                        actionState[j][k] += (numColor - actionState[pivot][k]) % numColor;
                        actionState[j][k] %= numColor;
                    }
                    board[j] += (numColor - board[pivot]) % numColor;
                    board[j] %= numColor;
                }
            }
            int needBreak = 1;
            for (int j = pivot + 1; j < stateSize; j++) {
                if (actionState[j][i] != 0) { needBreak = 0; break; }
            }
            if (needBreak) break;
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
            printf("NO SOLUTION!\n");
            break;
        } else (answer[i]) = solTemp;
    }
    for (int i = 0; i < stateSize; i++) {
        printf("%d", answer[i]);
    }
    printf("\n");
}

int main() {
    struct LightsOut game;
    game.column = 4;
    game.row = 16;
    game.numColor = 2;
    for (int i = 0; i < 64; i++) {
        game.board[i] = 1;
    }
    lightsOutSolver(&game);
    return 0;
}