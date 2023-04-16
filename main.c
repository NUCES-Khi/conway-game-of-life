/**
 * @author     21k-3079 Rayyan Ahmed
 * @date       2023
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <pthread.h>

#define NUM_THREADS 4
#define CHUNK_SIZE 25
int ROWS  = 10;
int COLS =  10;
int matrix[][10] = {
    {0, 0, 1, 1, 0, 0, 0, 1, 1, 1},
    {0, 0, 0, 1, 0, 0, 0, 1, 0, 0},
    {1, 1, 1, 0, 1, 0, 0, 0, 1, 1},
    {1, 0, 0, 0, 1, 0, 1, 1, 1, 0},
    {0, 1, 1, 1, 0, 0, 1, 1, 0, 0},
    {0, 1, 0, 1, 0, 1, 1, 0, 0, 1},
    {1, 0, 0, 0, 1, 1, 1, 0, 0, 0},
    {1, 1, 0, 0, 0, 1, 0, 1, 1, 1},
    {0, 0, 1, 0, 1, 0, 0, 0, 1, 1},
    {1, 0, 0, 1, 1, 0, 0, 1, 0, 0}
};


void print_board(int board[][10]) {
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {
            printf("%d ", board[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}


int count_neighbors(int (*board)[10], int row, int col) {
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

void update_board(int (*board)[COLS], int (*new_board)[COLS], int rows, int cols) {
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


int main() {
int rc = 10;
int cc = 10;



    
        // Open the file for writing
    FILE *fp = fopen("matrix.txt", "w");
    if (fp == NULL) {
        perror("fopen");
        return 1;
    }

    // Write the matrix to the file
    for (int i = 0; i < rc; i++) {
        for (int j = 0; j < cc; j++) {
            fprintf(fp, "%d ", matrix[i][j]);
        }
        fprintf(fp, "\n");
    }

    // Close the file
    fclose(fp);
    
    // Open the file for reading
    FILE *fp2 = fopen("matrix.txt", "r");
    if (fp2 == NULL) {
        perror("fopen");
        exit(1);
    }

    // Read the matrix from the file into a two-dimensional array
    int matrixx[rc][cc];
    for (int i = 0; i < rc; i++) {
        for (int j = 0; j < cc; j++) {
            fscanf(fp, "%d", &matrixx[i][j]);
        }
    }

    // Close the file
    fclose(fp2);
    
         int shmid;
    key_t key = IPC_PRIVATE;
    int (*board)[COLS];

    shmid = shmget(key, ROWS * COLS * sizeof(int), IPC_CREAT | 0666);
    if (shmid < 0) {
        perror("shmget");
        exit(1);
    }

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

int new_board[ROWS][COLS];
int count = 1;
int num_processes = 4;
for (int k = 0; k < 10; k++){
for (int i = 0; i < num_processes; i++) {
        pid = fork();

        if (pid == -1) {
            // Error handling
            perror("fork");
            exit(1);
        } 
        else if (pid == 0) {
        int start_row = i * 10 / num_processes;
            int end_row = (i + 1) * 10 / num_processes;

 for (int j = 0; j < 100; j++) {
                // Update 25 cells in each iteration
                for (int k = start_row; k < end_row; k++) {
                    for (int l = 0; l < COLS && (k * COLS + l) < ((i + 1) * 25); l++) {
                        update_board(board, new_board, k, l);
                        
                    }
                }
                // Wait for all child processes to finish updating the board
                wait(NULL);
            }
                        exit(0);
        }
    }
    print_board(board);
    }
    

    // Parent process
    for (int i = 0; i < num_processes; i++) {
        // Wait for all child processes to finish
        wait(NULL);
    }


for (int i = 0; i < 10; i++) {
    wait(&status);
}


shmdt(board);
shmctl(shmid, IPC_RMID, NULL);

return 0;

}


