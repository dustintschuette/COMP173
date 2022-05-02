# COMP173
Projects from my operating systems class

# Lab 0. Get familiar with Linux
 
not really relevant as I ran Pop! OS at the time I took this class.

# Lab 1. Monitoring Task Manager

Goal: Get familiar with a process monitoring tool. 

# Lab 2. Simple Shell

This project consists of modifying a C program which serves as a shell interface that accepts user 
commands and then executes each command in a separate process. A shell interface provides the 
user  a  prompt  after  which  the  next  command  is  entered.  The  example  below  illustrates  the 
prompt  ```sh>```   and  the  user's  next  command:  ``` cat prog.c``` .  This  command  displays  the  file 
prog.c on the terminal using the UNIX cat command.  

# Lab 3. Shared Memory

Used ```shmget``` to create a new shared memory segment, then wrote to it and finally read from it.

# Lab 4. Majority Voting

In this lab we will use pipes to implement a majority voting system that involves one judge and 
N  voters.  N  is  specified  by  the  user  and  should  be  equal  to  or  greater  than  3.  The  judge  is  the 
parent  process,  and  the  voters  are  child  processes.  At  each  round,  every  voter  will  randomly 
choose to vote yes or no, show the vote on the screen, and send the vote to the judge. The judge 
will then declare  the final decision following  the majority rule  and display it on the screen. The 
voting  continues  until  it  reaches  the  maximal  number  of  rounds,  and all  processes  should  be  terminated 
properly before exit. The same group votes for all the rounds.   


# Lab 5. CPU Scheduling Simulator

This  programming  project  is  to  simulate  a  few  CPU  scheduling  policies  discussed  in  the  class. 
You  will  write  a  program  to  implement  a  simulator  with  different  scheduling  algorithms.  The 
simulator  selects  a  task  to  run  from  ready  queue  based  on  the  scheduling  algorithm.  Since  the 
project  intends  to  simulate  a  CPU  scheduler,  it  does  not  require  any  actual  process  creation  or 
execution. When a task is scheduled, the simulator will simply print out what task is selected to 
run at a time.  
 
# Lab 6. The Sleeping-Barber Problem

A barbershop consists of a waiting room with n chairs and a barber room with one barber 
chair. If there are no customers to be served, the barber goes to sleep. If a customer enters the 
barbershop and all chairs are occupied, then the customer leaves the shop. If the barber is busy 
but chairs are available, then the customer sits in one of the free chairs. If the barber is asleep, 
the customer wakes up the barber. Describe your multi-threaded solution to coordinate the 
barber  and  the  customers.  There  should  be  one  thread  representing  the  barber  and  one 
additional  thread  for  each  new  customer.  Implement  your  solution  using  only  Pthreads 
functions.  

# Lab 7. Deadlock Detection

In this lab, you are asked to apply Banker’s Algorithm to deadlock detection. Please refer 
to the course slides on Deadlock Detection. Your program should ask the user to specify 
an  input  file.  The  format  of  the  input  file  is  described  below.  The  program  will  run  the 
algorithm and display a message stating either that there is a deadlock or that there is not. 
If there is a deadlock, list the processes that are involved in the deadlock. Then you will 
give the user a chance to run another set of data.

input files format:
Number of processes (N) (in black) 
Number of Resource Types (M) (in teal) 
Available (unallocated) instances of each resource - 1 line of M numbers
Allocation matrix - N lines of M numbers 
Request Matrix - N lines of M numbers 


