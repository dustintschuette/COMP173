/**
 * This program illustrates the functionality of the ipcs command on POSIX systems.
 *
 * sm.c
 *
 * Usage:
 *      gcc -o sm sm.c
 *
 *      ./sm 
 */

#include <stdio.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>


int main(int argc, char *argv[])
{
	int segment_id;  			// identifier of the shared memory segment
	unsigned short mode;		// permissions of the segment
	struct shmid_ds shmbuffer; 	// the shared memory segment 


// Step 1: Create a new shared memory segment using shmget
	segment_id = shmget(IPC_PRIVATE, 200*sizeof(int), IPC_CREAT | S_IRUSR | S_IWUSR);	
	printf("new segment created: %d \n",  segment_id);		
		
// Step 2: Retrieve the information of the segment and store in shmbuffer		
	if (shmctl(segment_id, IPC_STAT, &shmbuffer) == -1) {
		fprintf(stderr,"Unable to access segment %d\n",segment_id);
		return -1;
	}

// Step 3: output information about the segment in the required format
	
	//top line for catagories

	printf("owner          group          world          Size          Attaches \n");
	printf("_____________________________________________________________________\n");


	/** report on the permission */
	mode = shmbuffer.shm_perm.mode;
	
	/** OWNER */
	if (mode & 0400)
		printf("r");
	else
		printf("-");
	if (mode & 0200)
		printf("w");
	else
		printf("-");
	if (mode & 0100)
		printf("x");
	else
		printf("-");

	printf("             ");
	/** GROUP */
	if (mode & 0040)
		printf("r");
	else
		printf("-");
	if (mode & 0020)
		printf("w");
	else
		printf("-");
	if (mode & 0010)
		printf("x");
	else
		printf("-");



	printf("            ");
	/** WORLD */
	if (mode & 0004)
		printf("r");
	else
		printf("-");
	if (mode & 0002)
		printf("w");
	else
		printf("-");
	if (mode & 0001)
		printf("x");
	else
		printf("-");


	//Size
	printf("             %d          %d", shmbuffer.shm_segsz, shmbuffer.shm_nattch);
	printf("\n");

// Step 4: Create a new process using fork
	int pid_t = fork();




// Step 5: The child process sends a message to the parent process via the 
//         shared memory segment created in Step 1 and the parent prints out 
//         the message it received from the child process
	char *shared_memory = (char *)shmat(segment_id, NULL, 0);
	//child process
	if(pid_t == 0){
		//write message
		sprintf(shared_memory, "Reading from shared memory \n");
		
		shmdt(shared_memory);
		exit(1);

	}else{
		wait(NULL);
		printf(shared_memory);
		shmdt(shared_memory);
		exit(1);

	}
		

    return 0;
}
