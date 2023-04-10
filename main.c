/**
 * @defgroup   MAIN main
 *
 * @brief      This file implements main. Please Comment you code
 *
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

struct ThreadArgs{
int chunk_num;
int (*board)[COLS];
};

// function that each thread will execute
void* apply_rules(void* arg) {
   struct ThreadArgs* thread_args = arg;
    int chunk_num = thread_args->chunk_num;
    int (*board)[COLS] = thread_args->board;
    int start_row = chunk_num * CHUNK_SIZE;
    int end_row = (chunk_num + 1) * CHUNK_SIZE;
    if (end_row > ROWS){
     end_row = 10;
     }
    // loop over the chunk of the matrix and count neighbors for each cell
    for (int i = start_row; i < end_row; i++) {
        for (int j = 0; j < COLS; j++) {
            // update the cell based on the number of neighbors
            update_board(board, i, j);
        }
    }
    pthread_exit(NULL);
}


// for printing board or array
void print_board(int board[][10]) {
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {
            printf("%d ", board[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

// to count neighbor of a cell
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
// for applying rules of conway for a cell
void update_board(int (*board)[COLS], int rows, int cols) {
    int (*new_board)[COLS];
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
    int shmid;
    pthread_t threads[NUM_THREADS];
    struct ThreadArgs thread_args[NUM_THREADS];
    key_t key = IPC_PRIVATE;
    int (*board)[cc];
    
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
    
        shmid = shmget(key, rc * cc * sizeof(int), IPC_CREAT | 0666);
	    if (shmid < 0) {
	        perror("shmget");
	        exit(1);
	    }

    // Create a shared memory segment to hold the matrix
    board = shmat(shmid, NULL, 0);
	    if (board == (int (*)[]) -1) {
	        perror("shmat");
	        exit(1);
	    }
	
	    // Initialize the board
	    for (int i = 0; i < rc; i++) {
	        for (int j = 0; j < cc; j++) {
	            board[i][j] = matrixx[i][j];
	        }
	    }
	
 	for (int i = 0; i < NUM_THREADS; i++) {
          thread_args[i].chunk_num = i;
         thread_args[i].board = board;
        pthread_create(&threads[i], NULL, apply_rules, &thread_args[i]);
    }

    // wait for all threads to finish
    for (int i = 0; i < NUM_THREADS; i++) {

        pthread_join(threads[i], NULL);
    }

	
	print_board(board);
	
	shmdt(board);
	shmctl(shmid, IPC_RMID, NULL);
	
	return 0;

}

