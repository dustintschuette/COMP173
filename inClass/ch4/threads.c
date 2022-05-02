#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <stdlib.h>


int length;
long* fib;


long getFib(int num);
void *genFib(void *threadid);

int main(){

    char input[10];
    int maxL = 5;
    length = 0;
    long t = 0;

    //child thread for generating fib sequence
    pthread_t thread;

    //getting input
    printf("Input how long of a fibonacci sequence you want : \n > ");
    fgets(input, maxL, stdin);
    sscanf(input, "%d *[^\n]", &length);
    

    //make array to store results
    fib = calloc(length + 1, sizeof(long));

    //fill array with -1
    for(int i = 0; i <= length; i++){

       fib[i] = -1;
    }

    //start thread to calculate fib
    pthread_create(&thread, NULL, genFib, (void *)t);

    //wait for child to finish and print results;
    pthread_join(thread, NULL);

    //print the results

    for(int i = 0; i <= length; i++){

        if(i == length -1){
            printf("%d \n", fib[i]);
        }else{
            printf("%d ", fib[i]);
        }
        

    }
    return 0;


}


//main thread for customers
void *genFib(void *threadid){
    fib[0] = 0;
    fib[1] = 1;
    fib[2] = 1;
    fib[3] = 2;

    getFib(length);

    pthread_exit (NULL);
        
}

long getFib(int num){
    //printf("Fib of %d is %d", num, fib[num]);
   if(fib[num] != -1){
        return fib[num];
    }
    fib[num] = (getFib(num -1) + getFib(num - 2));

    return fib[num];
}
