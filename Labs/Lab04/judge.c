#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>


int main(int argc, char *argv[]){

    int nbytes, pipe1[2], pipe2[2];
    char input[10];
    int numJurors, numRounds;
    int maxL = 10;
    int ID = -1;
    srand(time(0));


    pid_t childpid;
    char    readbuffer[50];
    pipe(pipe1);
    pipe(pipe2);

    //getting input
    printf("Input number of jurors: \n > ");
    fgets(input, maxL, stdin);
    sscanf(input, "%d *[^\n]", &numJurors);
    printf("\n Input number of voting rounds: \n > ");
    fgets(input, maxL, stdin);
    sscanf(input, "%d *[^\n]", &numRounds);
    
    //setting up pipes

    int** pipes;
    pipes = calloc(numJurors, sizeof(int*));
    for(int i = 0; i < numJurors; i++){
        pipes[i] = calloc(2, sizeof(int));
    }

    for(int i = 0; i < numJurors; i++){
        for(int j = 0; j < 2; j++){
            pipes[i][j] = 1;
        }
    }
    for(int i = 0; i < numJurors; i++){
        pipe(pipes[i]);
    }


    //create juror threads
    int pid = 0;
    pid = fork();
    if(pid == 0){
        ID = 0;
    }
    
    for(int i = 1; i < numJurors; i++){
        if(pid != 0){
            pid = 0;
            pid = fork();
            if(pid == 0){
                ID = i;
            }
        }
    }

    //make sure there's correct number of unique jurors and only one judge
    /*
    if(ID == -1){
        printf("Judge is present \n");
    }else{
       printf("I'm juror number: %d \n", ID); 
    }
    */

    //start voting
    int     vote;
    char yesString[] = "yes";
    char noString[] = "no ";

    for(int i = 0; i < numRounds; i++){
        //juror process

        if(ID != -1){
            if(i == 0){

                /* Child process closes up input side of pipe */
                close(pipes[ID][0]);

            }
            vote = rand();
            //printf("vrandom number: %d\n", vote);
            for(int j = 0; j < ID; j++){
                vote = vote >> 2;
            }
            vote = vote % 2;
            //printf("voter %d voted %d in round %d\n", ID, vote,  i);
            
            
            if(vote == 0){
                //vote yes
               write(pipes[ID][1], yesString, (strlen(yesString)+1)); 
            }else{
               write(pipes[ID][1], noString, (strlen(noString)+1));
            }
            /* Send "string" through the output side of pipe */
            
        }else{
            if(i == 0){
                
                /* Judge process closes up output side of pipes */
                for(int j = 0; j < numJurors; j++){
                    close(pipes[j][1]);    
                }
                sleep(2);
            }

            int countNo = 0;
            int countYes = 0;

            printf("----Voting for Round %d------\n", i+1);
            

            /* Read in votes from the pipes */

            for(int j = 0; j < numJurors; j++){
                nbytes = read(pipes[j][0], readbuffer, 4);
                
                if(strcmp(readbuffer, yesString) == 0){
                    countYes++;
                }else{
                    countNo++;
                }

                printf("Juror %d voted: %s \n", j, readbuffer);
            }
            printf("Round %d results are: ", i+1);
            sleep(1);

            if(countYes > countNo){
                printf("Yes \n");
            }else if(countNo == countYes){
                printf("Hung Jury\n");
            }else{
                printf("No\n");
            }

        }


    }

    //end program by closing all threads
    if(pid == 0){
        exit(0);
    }else{
        wait(NULL);
        exit(0);
    }

    return 0;
}
