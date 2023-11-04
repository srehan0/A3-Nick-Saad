#include <stdio.h>

struct threadinfo {
    int p_id;
    int arr_time;
    int burst_time;
    int completion_time;
    int turn_around_time;
    int waiting_time;
};

int main() {
    FILE *file = fopen("sample_in_schedule.txt", "r");
    if (file == NULL) {
        printf("Error opening file.\n");
        return 1;
    }

    int num_threads;
    fscanf(file, "%d", &num_threads);

    struct threadinfo threads[num_threads];
    int total_waiting_time = 0;
    int total_turn_around_time = 0;

    for (int i = 0; i < num_threads; i++) {
        fscanf(file, "%d,%d,%d", &threads[i].p_id, &threads[i].arr_time, &threads[i].burst_time);
    }

    fclose(file);

    // Calculate completion time, waiting time, and turn-around time
    int current_time = 0;
    for (int i = 0; i < num_threads; i++) {
        if (current_time < threads[i].arr_time) {
            current_time = threads[i].arr_time;
        }

        threads[i].completion_time = current_time + threads[i].burst_time;
        threads[i].turn_around_time = threads[i].completion_time - threads[i].arr_time;
        threads[i].waiting_time = threads[i].turn_around_time - threads[i].burst_time;

        current_time = threads[i].completion_time;

        total_waiting_time += threads[i].waiting_time;
        total_turn_around_time += threads[i].turn_around_time;
    }

    // Print results
    printf("Thread ID   Arrival Time   Burst Time   Completion Time   Turn-around Time   Waiting Time\n");
    for (int i = 0; i < num_threads; i++) {
        printf("%-13d%-16d%-14d%-19d%-18d%-14d\n",
               threads[i].p_id, threads[i].arr_time, threads[i].burst_time,
               threads[i].completion_time, threads[i].turn_around_time, threads[i].waiting_time);
    }

    printf("\nThe average waiting time: %.2f\n", (float)total_waiting_time / num_threads);
    printf("The average turn-around time: %.2f\n", (float)total_turn_around_time / num_threads);

    return 0;
}