#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <errno.h>
#include <math.h>

struct process{
    int *allocated;
    int *requested;
    bool finished;

};

int main(){

    char buffer[100];
    char fileName[100];

    while(true){
        int size;
        int types;
        int *currResources;

        struct process * processes;

        printf("input file to open (quit to exit) > ");
        fgets(buffer, 100, stdin);
        sscanf(buffer, "%s", fileName);
        printf("\n");
        if(strcmp(fileName, "quit") == 0){
            printf("exiting... \n");
            exit(0);
        }
        //read in requested file
        FILE *file;
        file = fopen(fileName, "r");
        if(file == NULL){
            printf("Error opening file %s", buffer);
        }
        else{
            //read in requested file
            char fileIn [100];
            int index = 0;
            bool readinSize = false;
            bool readinResourcses = false;
            bool readinStartState = false;
            while(true){
                if(!readinSize){
                    fgets(fileIn, 100, file);
                    if(fileIn == NULL){
                        printf("error reading line 1 \n");
                        exit(1);
                    } 
                    readinSize = true;
                    sscanf(fileIn, "%d", &size);
                    processes = calloc(size, sizeof(struct process));
                    printf("size read in: %d\n", size);

                }else if(fileIn[0] == '#'){
                    //skips line
                }else if(!readinResourcses){
                    fgets(fileIn, 100, file);
                    if(fileIn == NULL){
                        printf("error reading line 2 \n");
                        exit(1);
                    }  
                    readinResourcses = true;
                    sscanf(fileIn, "%d", &types);
                    for(int i = 0; i < size; i++){
                        processes[i].allocated = calloc(types, sizeof(int));
                        processes[i].requested = calloc(types, sizeof(int));
                    }
                    printf("types of resources read in: %d\n", types);
                }else if(!readinStartState){
                    readinStartState = true;
                    currResources = calloc(types, sizeof (int));
                    for(int i = 0; i < types; i++){
                        fscanf(file, " %d", &currResources[i]);
                    }
                }else{
                    //processes one pid at a time
                    if(index < size){
                        for(int i = 0; i < types;i++){
                            fscanf(file, " %d", &processes[index].allocated[i] );
                            
                        }
                    }else{
                        for(int i = 0; i < types;i++){
                            fscanf(file, " %d", &processes[index%size].requested[i] );
                        }
                    }
                   if(index == 0){
                        printf("Allocated: \n");
                    }else if(index == size){
                            printf("Requested: \n");
                        }

                    printf("P%d ", index%size);
                    

                    if(index < size){
                       for(int i = 0;i < types; i++){
                            printf(" %d ", processes[index].allocated[i]);
                        }
                        printf("\n"); 
                    }
                    else{
                        
                        for(int i = 0;i < types; i++){
                            printf(" %d ", processes[index%size].requested[i]);
                        }
                        printf("\n"); 
                    }
                    //exit loop when all data is read in
                    index++;
                    if(index == size*2){
                        break;
                    }
                    
                    

                }

            }
            for(int i = 0; i < size; i++){
                processes[i].finished = false;
            }
            printf("Current Resources: ");
            for(int i = 0; i < types; i++){
                printf("%d ", currResources[i]);
            }
            printf("\n");


            //processes read in
            //now see if there is a deadlock
            int counter = 0;
            int lastIncrement = 0;
            while(true){
                //checks next process
                if(processes[counter%size].finished != true){
                    //flag to see if this process is completable
                    bool canFinish = true;
                    for(int i = 0; i < types; i++){
                        if(processes[counter%size].requested[i] > currResources[i]){
                            canFinish = false;
                        }
                    }
                    //process can finish, add its resoucres to the pool, mark as done
                    if(canFinish){
                        processes[counter%size].finished = true;
                        for(int i = 0; i< types; i++){
                            currResources[i] += processes[counter%size].allocated[i];
                        }
                        printf("P%d finished, ", counter%size);
                        printf("Current Resources: ");
                        for(int i = 0; i < types; i++){
                            printf("%d ", currResources[i]);
                        }
                        printf("\n");
                        lastIncrement = 0;
                    }else{
                        lastIncrement++;
                        //printf("li:%d counter:%d \n", lastIncrement, counter%size);
                        if(lastIncrement == size){
                            //deadlocked
                            printf("deadlock detected. printing data... \n");
                            printf("Current Resources: ");
                            for(int i = 0; i < types; i++){
                                printf("%d ", currResources[i]);
                            }
                            printf("\nRemaining processes and their requests: \n");

                            for(int j = 0; j < size; j++){
                                if(!processes[j].finished){
                                    printf("P%d ", j);
                                    for(int k = 0; k < types; k++){
                                        printf(" %d ", processes[j].requested[k]);
                                    }
                                }
                                printf("\n");
                            }
                            break;
                            
                        }
                    }
                }else{

                    
                }
                bool checkDone = true;
                for(int i = 0; i < size; i++){
                    if(!processes[i].finished){
                        checkDone = false;
                    }
                }

                if(checkDone){
                    printf("Task is safe to run \n");
                    break;
                }
                counter++;
            }

        }

    }
}