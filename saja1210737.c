//saja asfour-1210737-sec2

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/wait.h>
#include <unistd.h>
#include <pthread.h>

#define MAX_ENTRIES_BMI 1000
#define NUM_THREADS 4


//this use in  multi threads approach
typedef struct {

    char gender[10];
    double height;
    double weight;

} Record_multi;

//this for threads
int num_records=0;
Record_multi data1[MAX_ENTRIES_BMI];

//function definition
void Naive_Approach ();
void Multi_Proccesor_Approach();
void Multithreading_Approach();
void *calculateBMIThread(void *arg) ;

//main function
int main() {

    printf("\nSaja Asfour 1210737-section 2\n\n");
    struct timespec start_naive;

    timespec_get(&start_naive, TIME_UTC);
    //call the Navie_Approach
    Naive_Approach ();
    // to store the current time in the end_naive variable.
    struct timespec end_naive;

    timespec_get(&end_naive, TIME_UTC);

    double time_naive = (end_naive.tv_sec - start_naive.tv_sec) + (end_naive.tv_nsec - start_naive.tv_nsec) /1000000000.0; //Calculate the time of naive approach in seconds

    printf("Execution time using naive approach =  %lf seconds.\n",time_naive);

    printf("\n**********************************************************\n\n");

    struct timespec begin_multi_process;

    // to store the current time in the begin_multi_process variable.
    timespec_get(&begin_multi_process, TIME_UTC);

    //call the multiproccessor approach
    Multi_Proccesor_Approach();

    struct timespec end_multi_process;

    // to store the current time in the end_multi_process variable.
    timespec_get(&end_multi_process, TIME_UTC);

    //to calculate execution time of multiprocessing part
    double time_multi_process;
    time_multi_process = (end_multi_process.tv_sec - begin_multi_process.tv_sec) + (end_multi_process.tv_nsec - begin_multi_process.tv_nsec) / 1000000000.0;

    printf("Execution time using multiProccesor approach =  %lf seconds.\n",time_multi_process);

    printf("\n**********************************************************\n\n");



    //call the multithread approach
    Multithreading_Approach();

    return 0;
}

//the Naive approach, a program that does not use any child processes or threads
void Naive_Approach (){

    FILE *in = fopen("bmi.csv", "r");
        if (in == NULL) {

            printf("Error opening file.\n");

        }
    char line[100];
    int height[MAX_ENTRIES_BMI];
    int weight[MAX_ENTRIES_BMI];
    float bmi[MAX_ENTRIES_BMI];
    int count = 0;
    float sum = 0.0;
    while (fgets(line, sizeof(line), in)) {
        char gender[10];
        sscanf(line, "%[^,],%d,%d", gender, &height[count], &weight[count]);

        // Check if height is not zero before calculating BMI
        if (height[count] != 0) {
            // Calculate BMI
            bmi[count] = (weight[count] / ((height[count] / 100.0) * (height[count] / 100.0)));
            sum += bmi[count];
            count++;
        }
    }

    fclose(in);

    // Check if any valid entries were found(this to not div about zero )
    if (count == 0) {

            printf("No valid entries found.\n");

    }

    // Calculate average BMI
    float average_bmi = sum / count;

    printf("Average BMI for Naive approach = %.2f\n", average_bmi);


}

//Multiprocessing approach: a program that uses multiple child processes running in parallel.
void Multi_Proccesor_Approach(){

    struct timespec start_read;

    timespec_get(&start_read, TIME_UTC);

    //open the file for read
    FILE *file = fopen("bmi.csv", "r");

    //if file does not exist:
    if (file == NULL) {
        perror("Error opening file");
    }

    //  array to store avg bmi from file
   char line[100];
    int height[MAX_ENTRIES_BMI];
    int weight[MAX_ENTRIES_BMI];
    //float bmi[MAX_ENTRIES_BMI];
    int count = 0;
    //float sum = 0.0;
    while (fgets(line, sizeof(line), file)) {
        char gender[10];
        sscanf(line, "%[^,],%d,%d", gender, &height[count], &weight[count]);

        // Check if height is not zero before calculating BMI
        if (height[count] != 0) {
            // Calculate BMI
            count++;
        }
    }


    //close the file
    fclose(file);
     struct timespec end_read;

    timespec_get(&end_read, TIME_UTC);

    double time_read = (end_read.tv_sec - start_read.tv_sec) + (end_read.tv_nsec - start_read.tv_nsec) /1000000000.0; //Calculate the time of naive approach in seconds

    printf("Execution time for read1 =  %lf seconds.\n",time_read);
    // Number of child processes to create
    int num_processes = 4;

    // Divide the dataset equally to dvide it into the child process
    int Size_for_each_child = count / num_processes;

    //array  to store the process IDs (PIDs) of the child processes created using fork()
    pid_t pids[num_processes];

    //array to store file descriptors for communication between parent and child processes via pipes.
    int file__descriptors[2*num_processes];

    // Create a pipe for each child process
    for (int i = 0; i < num_processes; i++) {

        //creates a pipe and stores the file descriptors for the i-th child process starting
        // at the calculated index.
        pipe(file__descriptors + i*2);
    }

    for (int i = 0; i < num_processes; i++) {

        // Child process
        if ((pids[i] = fork()) == 0) {

            // Close the read-end of the pipe
            close(file__descriptors[i*2]);

            //variable used within each child process to calculate the sum of the BMI values
            //for a subset of the records in the dataset
            double local_total = 0;

            //variable used to determine the starting index of the subset of records that
            //a particular child process will process
            int start_of_child = i * Size_for_each_child;

            //calculate the ending index of the subset of records that a particular child process will process.
            // If i represents the last process (i == num_processes - 1)
            // then n is used as the end index to ensure that the last process includes all remaining records
            int end_of_child = (i == num_processes - 1) ? count : (i + 1) * Size_for_each_child;
            double bmi;
            //This for loop is responsible for iterating over the subset of records assigned to the current
            //child process and calculating the BMI for each record
            for (int j = start_of_child; j < end_of_child; j++) {
                bmi = (weight[j] / ((height[j] / 100.0) * (height[j] / 100.0)));
                local_total += bmi;

            }

            // Write the local total to the pipe
            write(file__descriptors[i*2 + 1], &local_total, sizeof(local_total));

            // Close the write-end of the pipe
            close(file__descriptors[i*2 + 1]);

            //exit
            exit(0);
        }

       //if parent
        else {
            // Close the write-end of the pipe in the parent
            close(file__descriptors[i*2 + 1]);
        }
    }

    //variable used to accumulate the BMI values computed by each child process.
    double ALL_BMI = 0;

    //variable used to store the BMI calculated by each child process for its subset of records
    double BMI_FOR_CHILD;

    //this loop coordinates the synchronization and aggregation of results from the child processes
    //ensuring that the parent process can calculate the average BMI correctly based on
    // the contributions from each child process
    for (int i = 0; i < num_processes; i++) {

        // Wait for child processes to finish
        waitpid(pids[i], NULL, 0);

        // Read the local total from the pipe
        read(file__descriptors[i*2], &BMI_FOR_CHILD, sizeof(BMI_FOR_CHILD));
        ALL_BMI += BMI_FOR_CHILD;

        // Close the read-end of the pipe
        close(file__descriptors[i*2]);
    }
    struct timespec start_bmi;

    timespec_get(&start_bmi, TIME_UTC);

    double averageBMI = ALL_BMI / count;

    printf("Average BMI from multi_proccessor approach : %0.2f\n", averageBMI);
    struct timespec end_bmi;

    timespec_get(&end_bmi, TIME_UTC);

    double time_bmi = (end_bmi.tv_sec - start_bmi.tv_sec) + (end_bmi.tv_nsec - start_bmi.tv_nsec) /1000000000.0;
    printf("Execution time for avg =%lf\n",time_bmi);
}

void Multithreading_Approach(){
    struct timespec start;
    timespec_get(&start, TIME_UTC);

    struct timespec start_read2,end_read2;
    timespec_get(&start_read2, TIME_UTC);

    //open the file for read
    FILE *file = fopen("bmi.csv", "r");

    //if file does not exist:
    if (file == NULL) {
        perror("Error opening file");
    }

    //keep read from file until we reach the end of file
    while (fscanf(file, "%9[^,],%lf,%lf\n", data1[num_records].gender, &data1[num_records].height, &data1[num_records].weight) != EOF) {
        //count the number of bmi only if the hight not eqaul 0
        if (data1[num_records].height!=0){

            //increament the counter
            num_records++;

        }
    }

    //close the file
    fclose(file);

    timespec_get(&end_read2, TIME_UTC);

    double time_read2 = (end_read2.tv_sec - start_read2.tv_sec) + (end_read2.tv_nsec - start_read2.tv_nsec) /1000000000.0;
    printf("Execution time for read2 =%lf\n",time_read2);
    pthread_t threads[NUM_THREADS];
    int thread_ids[NUM_THREADS];
    pthread_attr_t attr;
    void *status;
    double totalBMI = 0;

    // Initialize and set thread joinable
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

    // Create threads
    for (int i = 0; i < NUM_THREADS; i++) {
        thread_ids[i] = i;
        int rc = pthread_create(&threads[i], &attr, calculateBMIThread, (void *)&thread_ids[i]);
        if (rc) {
            printf("ERROR: Return code from pthread_create() is %d\n", rc);
            exit(EXIT_FAILURE);
        }
    }

     // Wait for all threads to complete
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], &status);
        totalBMI += *(double *)status;
        free(status);
    }

    pthread_attr_destroy(&attr);
    struct timespec start_bmi2,end_bmi2;
    timespec_get(&start_bmi2, TIME_UTC);
    double averageBMI = totalBMI / num_records;

     struct timespec end;
       timespec_get(&end, TIME_UTC);

    double timeTaken;
    timeTaken = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1000000000.0;

    printf("Average BMI from multithreads approach : %0.2f\n", averageBMI);
    printf("Execution time using multithreads approach =  %lf seconds.\n",timeTaken);
    timespec_get(&end_bmi2, TIME_UTC);

    double timebmi2;
    timebmi2 = (end_bmi2.tv_sec - start_bmi2.tv_sec) + (end_bmi2.tv_nsec - start_bmi2.tv_nsec) / 1000000000.0;
    printf("Execution time for avg2=  %lf seconds.\n",timebmi2);

    printf("\n**********************************************************\n\n");


    pthread_exit(NULL);


}

void *calculateBMIThread(void *arg) {
    int thread_id = *((int *)arg);
    int start_index = thread_id * (num_records / NUM_THREADS);
    int end_index = (thread_id == NUM_THREADS - 1) ? num_records : (thread_id + 1) * (num_records / NUM_THREADS);
    double *totalBMI = malloc(sizeof(double));
    *totalBMI = 0;

    // Calculate BMI for the assigned portion of data
    for (int i = start_index; i < end_index; i++) {
        double bmi= data1[i].weight / ((data1[i].height/100.0) * (data1[i].height/100.0));
        *totalBMI += bmi;
    }

    pthread_exit(totalBMI);
}
