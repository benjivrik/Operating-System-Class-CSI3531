/**
 * 
 * Author : Benjamn Kataliko Viranga
 * ID : 8842942
 * 
 * Class : CSI3531 - Summer 2020 - Devoir 3
 * 
 * The Sleeping Teaching Assistant
 * 
 * University of Ottawa 
 * 
 * How to run from the command line ?
 * 
 * gcc ta-student.c -pthread -o ./homework
 * 
 * ./homework  <<< Will set the number of students to the default value 5
 * ./homework n_students <<< Will set the number of students to the value 'n_students'
 * 
 */

#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>

// only MAX_NUMBER_OF_CHAIRS chairs available
#define MAX_NUMBER_OF_CHAIRS 3

// stop the program after the TA helps MAX_NUMBER_OF_HELPS times
#define MAX_NUMBER_OF_HELPS 20


//initialize thread
pthread_t *students_thread; //pointer because we have n students available
pthread_t TA_thread; // only one TA

// declare a mutex for thread synchronization of the access to the chair
pthread_mutex_t mutex_for_chair;

//semaphores
sem_t ta;       //for waking or letting the TA sleep
sem_t student;  
sem_t chairs[MAX_NUMBER_OF_CHAIRS];

sem_t global_count;  // semaphore for global counter
sem_t global_count_incremented; // semaphore for confirming that the semaphore have been incremented.



//variable
int chair_used = -1;  // number of chair used by the students
int next_chair = 0;   // index of the next chair used


// global counter for stopping the program (from 0 to MAX_NUMBER_OF_HELPS-1)
int global_counter = 0;


void *role_of_TA()
{	
	for(;;)
	{
        sem_wait(&ta);  // waiting for the TA to be awakened by any student
        
        printf("\n>>>>>>>>>>>>>>>>>>>>>>>>>> TA awakened. <<<<<<<<<<<<<<<<<<<<<<<<<<<<\n");

        for(;;)
        {
            // accessing shared variable chair_used
            // locking the mutex_for_chair
            
            pthread_mutex_lock(&mutex_for_chair);

            // check the number of chair used
            if(chair_used == 0)
            {
                printf("\nThere is no student waiting in line.\n");
                printf("\n> The TA is helping the student who have just awakened him.\n");

                int delay = (rand() % 5) + 1 ; 
                // let the TA help the student during this delay
                sleep(delay);

                //signal the end of the help for the current student
                sem_post(&student);

                //wait for signal to increase the global counter
                sem_wait(&global_count);

                // increase the global counter
                global_counter++;
                printf("\n>>>>>>> TA helped students %d time(s). <<<<<<<\n",global_counter);
                if(global_counter == MAX_NUMBER_OF_HELPS)
                {
                    printf("\n>>> END OF PROGRAM <<<\n");
                    exit(0);
                }

                // confirm incrementation of the global count
                sem_post(&global_count_incremented);

                // no need to wait; TA can go back to sleep
                pthread_mutex_unlock(&mutex_for_chair);

                // end of the loop
                break;
            }
            else  //chair_used > 0
            {
                //next student available should come.
                sem_post(&chairs[next_chair]);
                //decrease the number of chair used
                chair_used = chair_used > -1 ? chair_used - 1 : -1;

                printf("\nThere are now %d chair(s) available. A Student left a chair.\n", MAX_NUMBER_OF_CHAIRS - chair_used);

                // move to the next index for the chairs.
                next_chair =  (next_chair + 1 ) % MAX_NUMBER_OF_CHAIRS;

                // unlock thread
                pthread_mutex_unlock(&mutex_for_chair);

                printf("The TA is currently helping a student.\n");
                // generate a random delay between 1 and 5 s for the TA helping the student
                int delay = (rand() % 10) + 1 ; 
                // let the TA help the student during this delay
                sleep(delay);
                // finish with student
                sem_post(&student);

                

                pthread_mutex_lock(&mutex_for_chair);


                //wait for signal to increase the global counter
                sem_wait(&global_count);
                // increase the global counter
                global_counter++;
                printf("\n>>>>>>> TA helped students %d time(s). <<<<<<<\n",global_counter);
                if(global_counter == MAX_NUMBER_OF_HELPS)
                {
                    printf("\n>>> END OF PROGRAM <<<\n");
                    exit(0);
                }
                
                // confirm incrementation of the global count
                sem_post(&global_count_incremented);

                pthread_mutex_unlock(&mutex_for_chair);
                
            } 
            // // adding this delay before the TA jump to the next student.
            // sleep(1);

        }

        printf("\nTA back to napping!\n");
        
	}

}

void *role_of_student(void *student_id) 
{
	// void  32  bits -- long 32 bits
	for(;;)
	{
        printf("\n> Student %ld is programming.\n", (long) student_id);
        // generate any delay from 1 to 10 s
        int delay =  rand() % 10 + 1;
        // let the student program for that delay
        sleep(delay);

        // lock mutex for accessing shared resources
        pthread_mutex_lock(&mutex_for_chair);
        int chairs_currently_used =  chair_used;
        pthread_mutex_unlock(&mutex_for_chair);

        if(chairs_currently_used < MAX_NUMBER_OF_CHAIRS)
        {
            // Student requesting help from the TA
            printf("\n> Student %ld is heading to request help from the TA.\n", (long) student_id);

            //access shared resources next_chair and chair_used
            pthread_mutex_lock(&mutex_for_chair);
            int current_chair =  (next_chair + chair_used) % MAX_NUMBER_OF_CHAIRS; // get the current index of the chair used
            //increment the chair used; // increment the chair used | -1  means there is no one
            chair_used = chair_used < MAX_NUMBER_OF_CHAIRS && chair_used >= -1 ? chair_used + 1 : MAX_NUMBER_OF_CHAIRS; 

            // print the number of chair left.
            printf("\n> There are currently %d chair(s) left available.\n", MAX_NUMBER_OF_CHAIRS - chair_used);
            pthread_mutex_unlock(&mutex_for_chair);

        
            if( chairs_currently_used > 0 ) // student should be sitting on a chair
            {

                printf("\nStudent %ld is sitting on a chair while waiting for the TA.\n", (long) student_id);

                //student waiting to be called by the TA
                sem_wait(&chairs[current_chair]);

                //student waiting for the TA to finish the explanation
                sem_wait(&student);
                printf("\n> The Student %ld have received help from the TA and is going back to programming.\n",(long) student_id);

        
                // confirm counter incrementation : student has been helped
                sem_post(&global_count);

                // wait for incrementation confirmation
                sem_wait(&global_count_incremented);

            }
            else // chair_used is zero
            {
                // wake the TA up
                sem_post(&ta);

                //student waiting for the TA to finish the explanation
                sem_wait(&student);
                printf("\n> The Student %ld have received help from the TA and is going back to programming.\n",(long) student_id);

                
                 // confirm counter incrementation : student has been helped
                sem_post(&global_count);

                // wait for incrementation confirmation
                sem_wait(&global_count_incremented);

            } 
        }
        else
        {
            // Student requesting help from the TA
            printf("\n> Student %ld is heading to request help from the TA.\n", (long) student_id);

            printf("\n> Student %ld is heading back to programming.\n"
            "Because the TA is busy. And there is no chairs available for waiting.\n", (long) student_id);
        }
        
	}

}

int main(int ac, char **av)
{

    int n_students; 

    if(ac == 2) // get the number of students from the command line
    {
       if(sscanf(av[1],"%d",&n_students) == 1)
       {
           printf("\nNumber of students set to %d\n",n_students);
       }
       else fprintf(stderr,"You must enter an integer for the number of students.\n");
    }
    else    // request the user to enter an integer
    {
        printf("\nYou did not specify any value from the command line.");
        n_students = 5 ; // default value for students as discussed in the tutorial
        printf("\nNumber of students set to %d by default.\n",n_students);
    }

	// Inform the user about when the program will stop.
    printf("\n>> This program will stop after the TA have helped students %d times. <<\n", MAX_NUMBER_OF_HELPS);

    // initialize semaphores
    sem_init(&student, 0, 0); // semaphore for students
	sem_init(&ta, 0, 0);      // semaphore for ta
    sem_init(&global_count, 0, 0); // semaphore for students
	sem_init(&global_count_incremented, 0, 0);      // semaphore for ta

    // initialize the semaphores for the MAX_NUMBER_OF_CHAIRS available
	for(int sem = 0; sem < MAX_NUMBER_OF_CHAIRS; sem++)	
    {
        sem_init(&chairs[sem], 0, 0);
    }		
		
    // initialize mutex
    if (pthread_mutex_init(&mutex_for_chair, NULL) != 0) { 
        printf("\nFailed to initialized mutex_for_chair\n"); 
        return 1; //error
    } 

    // use a different seed for every simulation
    srand(time(NULL));

    //initialize students threads
    //allocate memory for the number of students threads
    students_thread = (pthread_t*) malloc(n_students * sizeof(pthread_t));

    // initialize the thread of the TA
    pthread_create(&TA_thread, NULL, role_of_TA, NULL);

    // initialize the threads of the students

    for(int student_id = 0; student_id < n_students; student_id++)
    {
        // 32 bits for void type -- long - 32 bits also (void*) (long)
        pthread_create(&students_thread[student_id], NULL, role_of_student, (void*) (long) student_id);
    }
    // joining threads

    // TA thread
    pthread_join(TA_thread,NULL);

    // Students threads
    for(int student_id = 0; student_id < n_students; student_id++)
    {
        pthread_join(students_thread[student_id],NULL);
    }
    
	return 0;

}