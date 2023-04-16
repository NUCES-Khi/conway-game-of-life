[![Review Assignment Due Date](https://classroom.github.com/assets/deadline-readme-button-24ddc0f5d75046c5622901739e7c5dd533143b0c8e959d652212380cedb1ea36.svg)](https://classroom.github.com/a/OiiB2wMi)
[![Open in Visual Studio Code](https://classroom.github.com/assets/open-in-vscode-718a45dd9cf7e7f842a935f5ebbe5719a5e09af4491e668f4dbf3b35d5cca122.svg)](https://classroom.github.com/online_ide?assignment_repo_id=10789362&assignment_repo_type=AssignmentRepo)
# Assign 02 - Multiprocess Conway Game
|Name|Id|
|-|-|
|Rayyan Ahmed|21k-3079|


## Intro
Operating System assignment where we have to implement Conway Game Of Life using filing, shared memory, fork() system calls 

## Approach
1) Created a file
2) Created grid in shared memory
3) Stored that grid in file
4) Created count_neighbour function to count the neighbour of a cell.
5) Created update_cells function to update the cells according to the conway rules

## Problems Faced
Converting the program from single process to multiple process

## Online / Chat GPT Help
From ChatGPT
Q1: C filing structure
Ans: #include <stdio.h>

int main() {
   FILE *fp;   // declaring a file pointer
   char str[100];

   // opening file for writing
   fp = fopen("myfile.txt", "w");

   // writing to file
   fprintf(fp, "This is my file.\n");

   // closing file
   fclose(fp);

   // opening file for reading
   fp = fopen("myfile.txt", "r");

   // reading from file
   fgets(str, 100, fp);
   printf("File content: %s", str);

   // closing file
   fclose(fp);
   
   return 0;
}
Q2:Creating a matrix in shared memory 
Ans:#include <sys/ipc.h>
#include <sys/shm.h>

#define ROWS 10
#define COLS 10

int main() {
    int shmid;
    int (*matrix)[COLS];
    
    // create shared memory segment
    shmid = shmget(IPC_PRIVATE, ROWS * COLS * sizeof(int), IPC_CREAT | 0666);
    if (shmid == -1) {
        perror("shmget");
        exit(EXIT_FAILURE);
    }
    
    // attach shared memory segment to program's address space
    matrix = shmat(shmid, NULL, 0);
    if (matrix == (int (*)[COLS]) -1) {
        perror("shmat");
        exit(EXIT_FAILURE);
    }
    
    // now you can use 'matrix' as a 10x10 array in shared memory
    
    // detach shared memory segment
    shmdt(matrix);
    
    // remove shared memory segment
    shmctl(shmid, IPC_RMID, NULL);
    
    return 0;
}


## Screenshots
![image](https://user-images.githubusercontent.com/124567636/232287439-822ba1e7-1182-4a52-af8f-366e2f81d90e.png)
![image](https://user-images.githubusercontent.com/124567636/232287469-7992d93b-0343-409d-9582-729444d1883f.png)
![image](https://user-images.githubusercontent.com/124567636/232287492-ed6beb7e-5fcf-410d-9081-8dd725e35499.png)

