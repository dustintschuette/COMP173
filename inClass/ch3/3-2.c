#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>


int main(int argc, char *argv[]){

    //child to parent
    int pipe1[2];
    //parent to child
    int pipe2[2], nbytes;


    pid_t childpid;
    char    string[] = "HelLo, WorLd!\n";
    char    readbuffer[80];
    pipe(pipe1);
    pipe(pipe2);


    //child process
    if((childpid = fork()) == -1){
        perror("fork");
        exit(1);

    }
    //fork successful

    if(childpid == 0){
        /* Child process closes up input side of pipe */
        close(pipe1[0]);
        /* Child process closes up output side of pipe */
        close(pipe2[1]);

        /* Send "string" through the output side of pipe */
        write(pipe1[1], string, (strlen(string)+1));
        nbytes = read(pipe2[0], readbuffer, sizeof(readbuffer));

        printf("Response received: %s \n", readbuffer);
        exit(0);
    }else{
        /* Parent process closes up output side of pipe */
        close(pipe1[1]);
        /* Parent process closes up input side of pipe */
        close(pipe2[0]);

        /* Read in a string from the pipe */
        nbytes = read(pipe1[0], readbuffer, sizeof(readbuffer));
        printf("Received request: %s", readbuffer);

        
        int range = sizeof(readbuffer);
        //flip case of letters
        for(int i = 0; i < range; i++){
            char temp = readbuffer[i];
            if(temp < 123 && temp > 96){
                readbuffer[i] -= 32;
            }else if(temp < 91 && temp > 64){
                readbuffer[i] += 32;
            }else{

            }
        }
        //send message back
        write(pipe2[1], readbuffer, (strlen(readbuffer)+1));
        wait(NULL);
        


    }
        
        return(0);

}
