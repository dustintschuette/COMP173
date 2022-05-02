#include <pthread.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>


#define NUM_THREADS   7
//edit this value to seat more customers
#define N 5

long chairs[N];
bool asleep;
int numCustomers;

//mutex to check the shop for open chairs and sit down if there is one
pthread_mutex_t signUp;
//mutex reprsenting someone getting a haircut
pthread_mutex_t buzzTime;
//conditional variable to wake up the barber
pthread_cond_t wakeBarber;
//conditional variable to tell people wiating to check who's next and get a haircut
pthread_cond_t haircutDone;
pthread_cond_t barberFree;
pthread_cond_t customerReady;


/*5 chairs
1 barber
if all chairs occupied leave
else if barber is busy but there are chairs wait
else if no one is in chair and barber is sleeping wake him up
1 thread for barber and 1 for each customer
counter for haircut duration (busy barber)
random number genrator to simulate new customers arriving (even means new arrival)*/

//checks to see if a chair is available and if the barber is sleeping
bool getChair(long id){
  if(asleep){
    pthread_cond_signal(&wakeBarber);
    //if they wake the barber here then they are the only customer and should setthe flag nextC
    asleep = false;
    chairs[0] = id;
    return true;
  }
  for(int i =0; i < N; i++){
    if(chairs[i] == -1){
     chairs[i] = id;
     return true;
      
    }
  }
  return false;
}
//When a haircut finishes the customer leaves
void leave(long pid){
  bool found = false;
  if(chairs[0]!= pid){
    printf("Error leaving, customer not in seat 0 \n");
  }
  chairs[0] = -1;
  for(int i =0; i < N - 1; i++){
    if(chairs[i] == -1){
      chairs[i] = chairs[i+1];
      chairs[i+1] = -1;
    }
    
  }
}

//function to see if someone (customer thread) is next to get their haircut
bool checkLine(long id){
  if(id == chairs[0]){
    return true;
  }else{
    return false;
  }
}

//barber thread
void *BarbFunc(void *threadid){
  long tid = (long)threadid;
  printf("Barber has arrived for work...\n");
  pthread_mutex_lock(&signUp);
  printf("Barber dozed off....\n");
  pthread_cond_wait(&wakeBarber, &signUp);
  printf("Barber woken up by %ld \n", chairs[0]);
  //loop for cutting hair
  pthread_mutex_unlock(&signUp);
  bool hasC = true;
  int allCust = 40;//# of potential customers
  while(allCust > 0){
    while(hasC){
      
      //cuts hair
      printf("(bzzzzz..... bzzzzzz...)\n");
      sleep(5);
      //let customer know thier haircut is done
      pthread_mutex_lock(&buzzTime);
      printf("Barber successfully cut hair\n");
      pthread_cond_signal(&haircutDone);
      pthread_cond_wait(&customerReady, &buzzTime);
      pthread_mutex_unlock(&buzzTime);
      pthread_cond_signal(&barberFree);
      sleep(1);
     
      pthread_mutex_lock(&signUp);
      hasC = false;
      
      if(chairs[0] != -1){
        hasC = true;
      }
      
      pthread_mutex_unlock(&signUp);
      //printf("barber successfully saw list");
    }
    //no customers left
    pthread_mutex_lock(&signUp);
    allCust = numCustomers;
    if(!(numCustomers < 1)){
      asleep = true;
      printf("Barber went back to sleep.... \n");

      pthread_cond_wait(&wakeBarber, &signUp);
      printf("Barber woken up by %ld \n", chairs[0]);
      pthread_mutex_unlock(&signUp);
      hasC = true;
    }
   
    
  }
  printf("The barber goes home for the day");
  pthread_exit(NULL);



}
//customer thread
void *Customer(void *threadid)
{
    long tid;
    tid = (long)threadid;
    printf("Customer %ld has arrived for a haircut...\n", tid);
    //get in line for singup
    pthread_mutex_lock(&signUp);
    bool gotChair = getChair(tid);
    bool first = false;
    
    if(gotChair){
      pthread_mutex_unlock(&signUp);
      pthread_mutex_lock(&signUp);
      first = checkLine(tid);
      //chair available
      printf("The customer found an open chair and sits down... \n");
      //wait for buzztime unless they are first in line
      while(!first){
        pthread_cond_wait(&barberFree, &signUp);
        first = checkLine(tid);
      }
      printf("Customer %ld gets into barber chair \n", tid);
      pthread_mutex_unlock(&signUp);
      pthread_mutex_lock(&buzzTime);
      //wait for haircut
      pthread_cond_wait(&haircutDone, &buzzTime);
      //barber is finished cutting hair
      printf("Customer %ld got thier haircut \n", tid);
      
      pthread_mutex_unlock(&buzzTime);
      pthread_mutex_lock(&signUp);
      //remove name from queue
      leave(tid);
      //let everyone know the barber is free
      pthread_mutex_unlock(&signUp);
      pthread_cond_signal(&barberFree);
      
      //let barber know to leave
      pthread_cond_signal(&customerReady);
      
      printf("Customer %ld left\n", tid);
      pthread_exit(NULL);

    }
    else{
      pthread_mutex_unlock(&signUp);
      printf("There's no open chairs, so the customer leaves... \n");
      pthread_exit(NULL);
    }


  //
    
 
}



int main (int argc, char *argv[])
{
  for(int i = 0; i < N; i ++){
    chairs[i] = -1;
  }
  srand(time(NULL));
  int r = (rand () % 20) + 21; //generate random number between 20-40 for the number of possible customers
  r = 15;
  numCustomers = r;
  pthread_t threads[NUM_THREADS];
  pthread_t barber;
  asleep = true;
  long t = 0;
  pthread_create(&barber, NULL, BarbFunc, (void *)t);
  pthread_mutex_init(&signUp, NULL);
  pthread_mutex_init(&buzzTime, NULL);

  pthread_cond_init(&wakeBarber, NULL);
  pthread_cond_init(&barberFree, NULL);
  pthread_cond_init(&haircutDone, NULL);
  pthread_cond_init(&customerReady, NULL);
  int rc;
  printf("Barbershop opens for business \n");
  //given barber time to open the store
  sleep(1);
  int startNum = numCustomers;
  while(r > 0){
    //generate a random customer
    
    int newC = rand() %2;
    
    //create barber thread
    
    //returns non zero on failure
    //start creating customers
    if(newC == 1){
      printf("Person %d walked by \n", (numCustomers - r));
      //if even a new customer walks in
      //no customer added
      
    }else{
      //customer added
      pthread_mutex_lock(&signUp);
      for(int i = 0; i < N + 3; i++){
        if(chairs[i] == -1){
          if(chairs[(i+ 1) % N] == -1){
            pthread_create(&threads[i], NULL, Customer, (void *)t);
            break;
          }
        } 
      }
      t++;
      pthread_mutex_unlock(&signUp);
    }
    sleep(1);
    r--;
  }
  //No more new customers for the day, wait for remaining haircuts to finish
  pthread_mutex_lock(&signUp);
  numCustomers = 0;
  pthread_mutex_unlock(&signUp);
  sleep(5);
  pthread_join(barber, NULL);
  //clean up
  pthread_mutex_destroy(&signUp);
  pthread_mutex_destroy(&buzzTime);
  pthread_cond_destroy(&wakeBarber);
  pthread_cond_destroy(&haircutDone);
  pthread_cond_destroy(&customerReady);
  //exit
  pthread_exit (NULL);
}