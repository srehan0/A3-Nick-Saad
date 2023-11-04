#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define SIZE 9

int sudoku[SIZE][SIZE];
int result[11];

// Data structure for passing parameters to threads
typedef struct {
    int start_row;
    int start_col;
} ThreadData;

void *checkRow(void *param) {
    ThreadData *data = (ThreadData *)param;
    int row = data->start_row;
    int col;

    for (col = 0; col < SIZE; col++) {
        int value = sudoku[row][col];
        if (value < 1 || value > SIZE || result[value] == 1) {
            result[value] = 0;
        }
    }

    pthread_exit(0);
}

void *checkColumn(void *param) {
    ThreadData *data = (ThreadData *)param;
    int row;
    int col = data->start_col;

    for (row = 0; row < SIZE; row++) {
        int value = sudoku[row][col];
        if (value < 1 || value > SIZE || result[value] == 1) {
            result[value] = 0;
        }
    }

    pthread_exit(0);
}

void *checkSubgrid(void *param) {
    ThreadData *data = (ThreadData *)param;
    int start_row = data->start_row;
    int start_col = data->start_col;
    int row, col;

    int count[SIZE + 1] = {0};

    for (row = start_row; row < start_row + 3; row++) {
        for (col = start_col; col < start_col + 3; col++) {
            int value = sudoku[row][col];
            if (value < 1 || value > SIZE || count[value] == 1) {
                result[value] = 0;
            }
            count[value] = 1;
        }
    }

    pthread_exit(0);
}

int main() {
    // Read Sudoku puzzle from file
    FILE *file = fopen("sample_in_sudoku.txt", "r");
    if (file == NULL) {
        perror("Error opening the file");
        return 1;
    }

    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            fscanf(file, "%d", &sudoku[i][j]);
        }
    }

    fclose(file);

    pthread_t threads[11];
    ThreadData data[11];

    for (int i = 0; i <= SIZE; i++) {
        result[i] = 1;
    }

    // Create and run threads to check rows and columns
    for (int i = 0; i < SIZE; i++) {
        data[i].start_row = i;
        data[i].start_col = i;
        pthread_create(&threads[i], NULL, checkRow, &data[i]);
        pthread_create(&threads[i + SIZE], NULL, checkColumn, &data[i]);
    }

    // Create and run threads to check 3x3 sub-grids
    int subgrid_thread_index = 2 * SIZE;
    for (int row = 0; row < SIZE; row += 3) {
        for (int col = 0; col < SIZE; col += 3) {
            data[subgrid_thread_index].start_row = row;
            data[subgrid_thread_index].start_col = col;
            pthread_create(&threads[subgrid_thread_index], NULL, checkSubgrid, &data[subgrid_thread_index]);
            subgrid_thread_index++;
        }
    }

    // Wait for all threads to finish
    for (int i = 0; i < 11; i++) {
        pthread_join(threads[i], NULL);
    }

    // Check if the Sudoku puzzle is valid
    for (int i = 1; i <= SIZE; i++) {
        if (result[i] == 0) {
            printf("The Sudoku puzzle is not valid.\n");
            return 0;
        }
    }

    printf("The Sudoku puzzle is valid.\n");

    return 0;
}

