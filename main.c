/**
 * @defgroup   MAIN main
 *
 * @brief      This file implements main. Please Comment you code
 *
 * @author     21k-3079 Rayyan Ahmed
 * @date       2023
 */

#include <sys/mman.h>
#include <fcntl.h>
#include <semaphore.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>

// This function prints the board
// take board , rows and columns as arguments
void print_board(int board[][10], int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%d ", board[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}
// This function counts the neighbors
// Some conditions to check if the row , col is in bounds
// returns count which is basically the number of neighbors alive or labelled as one in matrix
int count_neighbors(int board[][10], int row, int col) {
	int ROWS = 10;
	int COLS = 10;
    int count = 0;
    for (int i = row - 1; i <= row + 1; i++) {
        for (int j = col - 1; j <= col + 1; j++) {
            if (i < 0 || i >= ROWS || j < 0 || j >= COLS || (i == row && j == col)) {
                continue;
            }
            if (board[i][j] == 1) {
                count++;
            }
        }
    }
    return count;
}
// This function uses count_neighbors functions to count the neighbour of a particular cell and apply the needed operation to it
void update_board(int board[][COLS], int rows, int cols) {
    int new_board[ROWS][COLS];
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            int neighbors = count_neighbors(board, i, j);
            if (board[i][j] == 1 && (neighbors < 2 || neighbors > 3)) {
                new_board[i][j] = 0;
            } else if (board[i][j] == 0 && neighbors == 3) {
                new_board[i][j] = 1;
            } else {
                new_board[i][j] = board[i][j];
            }
        }
    }
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            board[i][j] = new_board[i][j];
        }
    }
}

int main(int argc, char const *argv[])
{
	//share memory
    int shmid;
    key_t key = IPC_PRIVATE;
    int (*board)[COLS];
	// creating shared memory
    shmid = shmget(key, ROWS * COLS * sizeof(int), IPC_CREAT | 0666);
    if (shmid < 0) {
        perror("shmget");
        exit(1);
    }
	// assigning memory block to it
    board = shmat(shmid, NULL, 0);
    if (board == (int (*)[]) -1) {
        perror("shmat");
        exit(1);
    }

    // Initialize the board
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            board[i][j] = rand() % 2;
        }
    }

    int pid;
    int status;

    for (int i = 0; i < 10; i++) {
        pid = fork();
        if (pid == 0) {
            int start_row = i * ROWS / 10;
            int end_row = (i + 1) * ROWS / 10;
            for (int j = 0; j < 100; j++) {
            update_board(board, end_row - start_row, COLS);
        }
        exit(0);
    } else if (pid < 0) {
        perror("fork");
        exit(1);
    }
}

for (int i = 0; i < 10; i++) {
    wait(&status);
}

print_board(board, ROWS, COLS);

shmdt(board);
shmctl(shmid, IPC_RMID, NULL);

return 0;
}
