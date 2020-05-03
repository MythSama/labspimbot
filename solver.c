// code that need to be compiled into mips

struct LightsOut {
    int row;
    int column;
    int numColor;
    unsigned char board[MAXSIZE*MAXSIZE];
};

void lightsOutSolver(struct LightsOut* puzzle) {
    // Variable initialization
    int numRow = puzzle->row;
    int numCol = puzzle->column;
    int numColor = puzzle->numColor;
    unsigned char* board = puzzle->board;
    int stateSize = numRow * numCol;

    unsigned char boardForCheck[MAXSIZE*MAXSIZE];
    for (int i = 0; i < stateSize; i++) {
        boardForCheck[i] = board[i];
    }

    // Initial actionboard
    int actionState[stateSize][stateSize];
    for (int i = 0; i < stateSize; i++) {
        for (int j = 0; j < stateSize; j++) {
            int colIdx = j % numCol;
            actionState[i][j] = 
                (i == j) || (i == j - numCol) || (i == j + numCol) 
                || (i == j + 1 && colIdx != numCol - 1)
                || (i == j - 1 && colIdx != 0);
        }
    }

    // Set board to additive inverse
    for (int i = 0; i < stateSize; i++) {
        board[i] = (numColor - board[i]) % numColor;
    }

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
        // Swap rows if needed
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
        } else {
            answer[i] = 1;
            int solTemp2 = actionState[i][i];
            while (1 == 1) {
                if (solTemp2 == solTemp) break;
                solTemp2 += actionState[i][i];
                solTemp2 %= numColor;
                answer[i]++;
            } 
            answer[i] %= numColor;
        }
    }

}