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

int main(int argc, char const *argv[])
{
    // It is left up to you how you use shared memory and multiple processes 
    // to solve the conway problem
    return 0;
}
