/**
 * 
 * Author : Benjamn Kataliko Viranga
 * 
 * Class : CSI3531 - Devoir 4
 * 
 * The consumer
 * 
 * University of Ottawa 
 * 
 * How to run from the command line ?
 * 
 * gcc catalan-consumer.c -o ./consumer
 * 
 * When you run this program, It will wait for 
 * the catalan-producer.c to finish to fill the shared memory 
 * 
 * 
 */
#include<stdio.h>
#include<string.h>
#include<errno.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<sys/ipc.h>
#include<sys/shm.h>
#include<sys/types.h>

// key for the shared memory
#define MEM_KEY 0x1111

// 1K for the memory segment assigned
#define DATA_SIZE 1024 

// structure of the memory segment
struct mem_seg {
   long catalan_numbers;     // for checking the number of catalan numbers in memory
   int completed;           // shows when the producer have completed the catalan number
   long data[DATA_SIZE];    // array of data
};

int main(int argc, char *argv[]) 
{
   // for memory id
   int mem_id;
   // initialize memory segment pointer
   struct mem_seg *mem_ptr;

   // pointer to the array of data in the shared memory
   long *data_ptr;
   
   // get the identifier of the shared memory segment
   mem_id = shmget(MEM_KEY, sizeof(struct mem_seg), 0666|IPC_CREAT);

   // print memory segment id
   // printf("> %d\n",mem_id);
   // in case of an error while creating the memory
   if (mem_id == -1) {
      perror("Shared memory failed to be created.");
      return 1;
   }
   
   // get the address the memory segment created above
   mem_ptr = shmat(mem_id, NULL, 0);
   
   if (mem_ptr == (void *) -1) {
      perror("Shared memory attachment failed.");
      return 1;
   }


   // printf(">>> %p\n", mem_ptr);

   
   printf("> Consumer waiting for the producer to finish.\n");
   while (mem_ptr->completed != 1) {} // wait for the producer to complete all the data
   printf("> Number of Catalan numbers requested : %ld\n", mem_ptr -> catalan_numbers);
   // initialize pointer value
   data_ptr = mem_ptr -> data;

   /* Get data from the memory */
   printf("\n> Producer done. Consumer proceeding with the memory lecture.\n");
   printf("Your Catalan numbers are : ");
   for(int n = 1; n <= mem_ptr -> catalan_numbers; n++)
    {
        // get the catalan value
        long catalan = *data_ptr;

        // print the value
        printf("%ld ",catalan);

        // move the pointer
        data_ptr++;

        // printf("\n> Address : %p",data_ptr); < DEBUG purpose 
    }
   printf("\n");

   // detach the memory segment
   if (shmdt(mem_ptr) == -1) {
      perror("shmdt failed to be detached");
      return 1;
   }

   // perform control operations on the memory segment. - Destroying the segment at the end
   // IPC_RMID -  segment to be destroyed after the last process detached it
   if (shmctl(mem_id, IPC_RMID, 0) == -1) {
      perror("shmctl failed to be executed");
      return 1;
   }

   printf("Consumer Status: Done.\n");
   return 0;
}
