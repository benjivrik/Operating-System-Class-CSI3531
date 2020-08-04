/*------------------------------------------------------------
Fichier: cpr.c

Nom:  BENJAMIN KATALIKO VIRANGA
Numero d'etudiant: x

Description: Ce programme contient le code pour la creation
             d'un processus enfant et y attacher un tuyau.
	     L'enfant envoyera des messages par le tuyau
	     qui seront ensuite envoyes a la sortie standard.

Explication du processus zombie
(point 5 de "A completer" dans le devoir):

	(s.v.p. completez cette partie);

	En exécutant ps -u ~ durant le délai sleep(10) 
	après la termination du processus, 
	celui reste affiché parmi les ressources disponibles
	(processus actif) alors qu'il est déjà fini.
	Son PID reste visible durant la période du sleep(10)

	Celui devient donc un processus zombie.

-------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <sys/select.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>

/* Prototype */
void creerEnfantEtLire(int);

/*-------------------------------------------------------------
Function: main
Arguments: 
	int ac	- nombre d'arguments de la commande
	char **av - tableau de pointeurs aux arguments de commande
Description:
	Extrait le nombre de processus a creer de la ligne de
	commande. Si une erreur a lieu, le processus termine.
	Appel creerEnfantEtLire pour creer un enfant, et lire
	les donnees de l'enfant.
-------------------------------------------------------------*/

char **argv; //arguments used in user defined functions

int main(int ac, char **av)
{
    int numeroProcessus; 

    if(ac == 2)
    {
       if(sscanf(av[1],"%d",&numeroProcessus) == 1)
       {
		   argv = av;  //initialize argv
           creerEnfantEtLire(numeroProcessus);

       }
       else fprintf(stderr,"Ne peut pas traduire argument\n");
    }
    else fprintf(stderr,"Arguments pas valide\n");
    return(0);
}


/*-------------------------------------------------------------
Function: creerEnfantEtLire
Arguments: 
	int prcNum - le numero de processus
Description:
	Cree l'enfant, en y passant prcNum-1. Utilise prcNum
	comme identificateur de ce processus. Aussi, lit les
	messages du bout de lecture du tuyau et l'envoie a 
	la sortie standard (df 1). Lorsqu'aucune donnee peut
	etre lue du tuyau, termine.
-------------------------------------------------------------*/

void creerEnfantEtLire(int prcNum)
{

	int  fd[2];			//pipes
	char buffer[32];    //char to be printed
	int  pid;			//process PID

	//pipe initialization

    if (pipe(fd) == -1) 
    {
       printf("Pipe creation failed.\n");  /* Erreur */
       exit(-1);
    }

    //id of process
	int id_of_process =  prcNum;

	if(id_of_process < 1 )
	{
		printf("Please enter a positive number. (value > 0)\n");
		fflush(stdout);
	}

	//Process 1;
	if(id_of_process == 1)
	{
		sprintf(buffer,"> Processus %d commence\n", id_of_process);
		write(1, buffer, strlen(buffer));
		sleep(5);
		sprintf(buffer,"> Processus %d termine\n", id_of_process);
		write(1, buffer, strlen(buffer));
		sleep(10);
	}

   
    //starting processes in the following loop
	//all the processes above 1
	if(id_of_process > 1) 
	{
	    sprintf(buffer,"> Processus %d commence\n", id_of_process);
		write(1, buffer, strlen(buffer));

		pid = fork(); //creating child;
 
		if ( pid == -1)  /* Error */
		{
			printf("fork failed for children %d\n" , id_of_process);
			exit(-1);
		} 
		else if (pid == 0)  /* Enfant - on part le programme ><  Child */
		{
		    /*

					Child

			*/
			close(fd[1]);   //close previous pipe (write)
			dup(fd[1]); 	//clone pipe

			//initialize argument to be sent to the next process
		    char data[12];
            sprintf(data,"%d", id_of_process-1);

			//call child
			execlp(argv[0],argv[0],data,NULL);

			/* In case execlp not working */
			exit(-1); 		
			
		}
		else  
		{
			/*
					Parent
			*/
			close(fd[1]);  // close the writing output

			//clear buffer
			memset(buffer,0,sizeof buffer);


			read(fd[0], buffer,32);
			write(1, buffer, strlen(buffer));

			//wait for the child to finish the process
			wait(NULL);

            //clear buffer
			memset(buffer,0,sizeof buffer);

			//terminate the process
			sprintf(buffer,"> Processus %d termine\n", id_of_process);
			write(1, buffer, strlen(buffer));

			//Adding 10s after terminating the process
			sleep(10);
			
		}
	}
    
}
