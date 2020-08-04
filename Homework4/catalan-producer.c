/**
 * 
 * Author : Benjamn Kataliko Viranga
 * 
 * Class : CSI3531 - Devoir 4
 * 
 * The producer
 * 
 * University of Ottawa 
 * 
 * How to run from the command line ?
 * 
 * gcc catalan-producer.c -o ./producer
 * 
 * './producer' will set the default value for the Catalan numbers to '5'
 * 
 * './producer val' will set the default value for the Calalan number to 'val'
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
   long catalan_numbers;    // for checking the number of catalan numbers in memory
   int completed;           // shows when the producer have completed the catalan number
   long data[DATA_SIZE];    // array of data
};

void fill_data(long *data_ptr, long catalan_number); // function for filling the catalan numbers
long factorial(long value);   // calculate the factorial of the number

int main(int argc, char *argv[]) {

    long catalan_value; 

    if(argc == 2) // get the number of students from the command line
    {
       if(sscanf(argv[1],"%ld",&catalan_value) == 1)
       {
           printf("\nNumber of Catalan numbers set to %ld\n",catalan_value);
       }
       else fprintf(stderr,"You must enter an integer for the number of Catalan number.\n");
    }
    else    // request the user to enter an integer
    {
        printf("\nYou did not specify any value from the command line.");
        catalan_value = 5 ; // default value for students as discussed in the tutorial
        printf("\nNumber of Catalan number requested set to %ld by default.\n",catalan_value);
    }

    
   int mem_id;
   // pointer to the memory segment
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
   // printf("\n> Address : %p \n",mem_ptr);
   // initialize the number of catalan numbers to be added in the mem segment
   mem_ptr -> catalan_numbers = catalan_value;
   // Access to the shared memory data array with a pointer 
   data_ptr = mem_ptr->data;
   printf("> Producer writing data in the shared  memory.\n");
   fill_data(data_ptr, catalan_value);  
   printf("> The producer have written %ld catalan numbers in the memory.\n", catalan_value);
   mem_ptr -> completed = 1;
 
   // detach the memory segment
   if (shmdt(mem_ptr) == -1) {
      perror("shmdt failed to be detached");
      return 1;
   }


   printf("Producer Status: Done.\n");
   return 0;
}
/**
 * 
 * This function filling the memory with the corresponding catalan numbers
 * 
 * */
void fill_data(long *data_ptr, long catalan_number) 
{
    // C_n = (2n)!/(n+1)!n!
    // initializing catalan numbers starting from zero
    long catalan = 0;

    for(int n = 1; n <= catalan_number; n++)
    {
        //get the catalan value
        catalan = factorial(2*n)/ ( factorial(n+1) * factorial(n));

        //printf("\n> Address : %p \n",data_ptr);
        // add value in memory
        *data_ptr = (long) catalan;
        //printf("\n> Address value : %ld\n", (long) *data_ptr);
  
        // move the pointer
        data_ptr++;

        //data_ptr++;
        //printf("%ld ", catalan);

        printf("%d item(s) has(have) been added in the memory.\n",n);

        //printf("\n> Address : %p",data_ptr); < DEBUG purpose
        sleep(3); 
    }
    printf("\nData filled.\n");

}

// function for calculating the n factorial of a number
long factorial(long value)
{
    long result = 1; //initiliaze the value of the result
    // n factorial
    for(long init = 1; init <= value; init++)
    {
        result *= init;  
    }
    return result;
}
