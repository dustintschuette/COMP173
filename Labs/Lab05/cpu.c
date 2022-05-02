#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <errno.h>
#include <math.h>



struct pJob{
    int pid;
    int arrivalTime;
    int burstTime;
    int remainingTime;
    int waitTime;
    int turnAround;
    int startTime;
    int endTime;
    int runningTime;
};


//function to sort processes based on arrival time
void bubbleSort(struct pJob* jobs, int size){

    for(int i = 0; i < size; i++){
        int switchIndex = -1;
        for(int j = i; j < size; j++){
            if(jobs[j].arrivalTime < jobs[i].arrivalTime ){
                if(switchIndex == -1){
                    switchIndex = j;
                }else if(jobs[j].arrivalTime < jobs[switchIndex].arrivalTime){
                    switchIndex = j;
                }
            }
        }
        if(switchIndex != -1){
            struct pJob temp = jobs[i];
            jobs[i] = jobs[switchIndex];
            jobs[switchIndex] = temp;
        }
    }

}

//assigns runtime and prints all info for each process
void printResults(struct pJob * jobs, int size, int qTime){

    double totalWait = 0;
    for(int i = 0; i < size; i++){
        jobs[i].waitTime = jobs[i].turnAround - jobs[i].burstTime;
        totalWait += jobs[i].waitTime;
    }
    totalWait = totalWait / size;

    if(qTime != 0){
        printf("RR (Time Quantum = %d) \n", qTime);

        for(int i = 0; i < size; i++){
               jobs[i].runningTime = jobs[i].burstTime;   
        }


        
    }else{
        for(int i = 0; i < size; i++){
               jobs[i].runningTime = jobs[i].endTime - jobs[i].  startTime;   
        }
    }


    printf("PID             Arrival time           Time Start             Time End             Time Running             Waiting Time \n");
    printf("_________________________________________________________________________________________________________________________\n");

    for(int i = 0; i< size; i++){
        printf("  %d              %d                      %d                   %d                     %d                      %d\n",
             jobs[i].pid,   jobs[i].arrivalTime,     jobs[i].startTime,   jobs[i].endTime,         jobs[i].runningTime        , jobs[i].waitTime);
    }

    printf("average wait: %f \n", totalWait);
}
//first come first serve
void FCFS(struct pJob * jobs, int size){
    //printf("FCFS \n");
    int currentTime = 0;
    for(int i = 0; i < size; i++){
        jobs[i].startTime = currentTime;

        currentTime += jobs[i].burstTime;

        jobs[i].endTime = currentTime;
        jobs[i].turnAround = currentTime - jobs[i].arrivalTime;
    }
    printResults(jobs, size, 0);

    

    
}
//round robin, runs each process for qtime until all processes are done
void RR(struct pJob * jobs, int size, int qTime){
    //printf("RR \n");
    int currentTime = 0;
    bool taskDone = false;
    while(!taskDone){
        //run through the tasks one at a time for qtime amount of time
        int jobsDone = 0; // used to check when all jobs are completed
        int jobsStuck = 0;
        for(int i = 0; i < size; i++){
            if(jobs[i].arrivalTime > currentTime){
                jobsStuck++;
                //printf("jobstuck : %d, size: %d\n", jobsStuck, size);
            }
            else if(jobs[i].remainingTime <= qTime){
                if(jobs[i].startTime == -1){
                    jobs[i].startTime = currentTime;
                }
                currentTime += jobs[i].remainingTime;
                if(jobs[i].endTime == -1){

                    jobs[i].endTime = currentTime;
                    jobs[i].turnAround = currentTime - jobs[i].arrivalTime;
                    jobs[i].remainingTime = 0;
                }
                
                //printf("jobsdone : %d \n", jobsDone);
                jobsDone++;
                jobsStuck++;
            }else{
              
                if(jobs[i].startTime == -1){
                    jobs[i].startTime = currentTime;
                }
                currentTime += qTime;
                jobs[i].remainingTime -= qTime;
            }
        }
        if(jobsDone == size){
            taskDone = true;
        }else if(jobsStuck == size){
            currentTime++;
        }
        //printf("currentTime: %d \n", currentTime);
    }

    

    printResults(jobs, size, qTime);


}


//shortest job first
void SJF(struct pJob * jobs, int size){
    //printf("sjf \n");
    bool taskDone = false;
    int currentTime = 0;
    while(!taskDone){
        int shortestTask = 100000;
        int taskIndex = -1;
      for(int i = 0; i < size; i++){
          if(jobs[i].burstTime < shortestTask && jobs[i].remainingTime > 0 && jobs[i].arrivalTime <= currentTime){
                shortestTask = jobs[i].burstTime;
                taskIndex = i;
          }else if(jobs[i].startTime > currentTime && taskIndex == -1){
                taskIndex = -2;
          }
        }  

        if(taskIndex == -1){
            taskDone = true;
        }else if(taskIndex > -1){

            //start time
            if(jobs[taskIndex].startTime == -1){
                jobs[taskIndex].startTime = currentTime;
            }


            //update remaining time
            jobs[taskIndex].remainingTime = 0;
            currentTime += jobs[taskIndex].burstTime;


            //end time
            if(jobs[taskIndex].remainingTime == 0){
                jobs[taskIndex].endTime = currentTime;
                jobs[taskIndex].turnAround = currentTime - jobs[taskIndex].arrivalTime;
            }

        }else{
        currentTime += 1;
        }
                
    }
    printResults(jobs, size, 0);

    
}


int main(int argc, char *argv[]){
    FILE *file;
    struct pJob *jobs;
    //see if program is supplied a test file
    if(argc > 1){
        file = fopen(argv[1], "r");
        if(file == NULL){
            printf("Error opening file %s", argv[1]);
        }
        
    }
    else{
        printf("Too few arguments, exiting....\n");
        exit(1);
    }

    //read in one line at a time
    //first line is number of processes
    bool readinSize = false;
    int size;
    int index = 0;
    int pid;
    int burstTime;
    int arrivalTime;
    char fileIn [100];
    while(fgets(fileIn, 100, file) != NULL){
        if(!readinSize){
            readinSize = true;
            sscanf(fileIn, "%d", &size);
            jobs = calloc(size, sizeof(struct pJob));
            printf("size read in: %d\n", size);

        }else if(fileIn[0] == '#'){
            //skips line
        }else{
            //processes one pid at a time
            sscanf(fileIn, " %d %d %d", &pid, &arrivalTime, &burstTime );
            jobs[index].pid = pid;
            jobs[index].arrivalTime = arrivalTime;
            jobs[index].burstTime = burstTime;
            jobs[index].remainingTime = burstTime;
            jobs[index].startTime = -1;
            jobs[index].endTime = -1;

            //printf("P%d %d %d \n", jobs[index].pid, jobs[index].arrivalTime, jobs[index].burstTime );

            index++;
            

        }

    }

    //test bubblesort
    bubbleSort(jobs, size);
    

    //jobs read in now split path based on argument passed

    if(strcmp(argv[2], "FCFS") == 0)
        FCFS(jobs, size);
    else if(strcmp(argv[2], "RR") == 0)
        RR(jobs, size, atoi(argv[3]));
    else
        SJF(jobs, size);
}
