#define add 1
#define modify 2
#define acquire 3
#define release 4

#include "queue.h" 
#include "DBManager.h"
#include "DBClient.h"


//FILE *open=fopen("/home/omar/Documents/Final/config.txt","r");
//open = fopen("/home/omar/Documents/Final/config.txt","r");
void do_child(int other_proc,int looping_num,int *pids,key_t msgqid)
{
				printf("\n inside do_child %d %d \n",other_proc,looping_num);
				int child_number=looping_num-other_proc;
				char child_name[50];
				char child_name_temp[50];
				sprintf(child_name,"Child%d",child_number);
				printf("\n try child %s \n",child_name);
				FILE *open;
open = fopen("/home/omar/Documents/Final/config_children.txt","r");
				char order[50];
				fscanf(open, "%s", &child_name_temp);
				printf("\n try %s %s \n",child_name,child_name_temp);

				while(strcmp(child_name_temp, child_name)!=0)
				{
					//printf("\n error %s %d \n",child_name,looping_num);
					fscanf(open, "%s", &child_name_temp);
				}
				printf("\n Child name is %s \n",child_name);
				
				printf("\n inside do_child \n");
				fscanf(open, "%s", &order);
				printf("\n order = %s \n",order);
				while(strcmp(order, "#")!=0)
				{
				if(strcmp(order, "Add")==0)
				{
				char employee_name[50];
				fscanf(open, "%s", &employee_name);
				printf("\n employee name = %s \n",employee_name);
				int employee_salary=0;
				fscanf(open, "%d", &employee_salary);
				Add_Request_Sender(msgqid,pids[1],employee_name,employee_salary);
				}


				if(strcmp(order, "Acquire")==0)
				{
				int acq_key=0;
				fscanf(open, "%d", &acq_key);
				Requests_Sender(msgqid,acquire,pids[1],acq_key);
				}
				if(strcmp(order, "Sleep")==0)
				{
				int sleep_value=0;
				fscanf(open, "%d", &sleep_value);
				sleep(sleep_value);
				}
				if(strcmp(order, "Modify")==0)
				{
				int mod_key=0;
				fscanf(open, "%d", &mod_key);
				char mod_symb[50];
				fscanf(open, "%s", &mod_symb);
				int mod_salary=0;
				fscanf(open, "%d", &mod_salary);

				Modify_Request_Sender(msgqid,pids[1],mod_key,mod_salary,mod_symb);
				}

				if(strcmp(order, "Release")==0)
				{
				int rel_key=0;
				fscanf(open, "%d", &rel_key);
				Requests_Sender(msgqid,release,pids[1],rel_key);
				}


				if(strcmp(order, "Query")==0)
				{
				char which_query[50];
				fscanf(open, "%s", &which_query);
				if(strcmp(which_query, "ExactName")==0)
				{
				char searched_name[50];
				fscanf(open, "%s", &searched_name);
				printf("\n query of Exact_Name %s \n",searched_name);
				}
				}
				fscanf(open, "%s", &order);
				}
				if(strcmp(order, "#")==0)
				{
				printf("\n This child has finished his tasks \n\n");
				}
				//return(50);
				//while(1);

}

int main()
{       
	key_t msgqid;
	msgqid = msgget(IPC_PRIVATE, 0644); 

	key_t Semaphores_key = ftok("shmfile",65); 
	int Semaphores_shmid = shmget(Semaphores_key,1000 * sizeof(struct semaphore),0666|IPC_CREAT); 
	
	key_t PIDs_key = ftok("shm",200);
	int PIDs_shmid = shmget(PIDs_key,1024,0666|IPC_CREAT);

        key_t key = ftok("shmfile",65); 

        int shmid = shmget(key,1000 * sizeof(struct Record),0666|IPC_CREAT); 

struct Record * DatabaseTable = (struct Record *) shmat(shmid,(void*)0,0); 

/////////////////////////////////////////////////

FILE *open;
open = fopen("/home/omar/Documents/Final/config.txt","r");
char child_name[50];
char order[50];
int No_children_forked=0;
if(open ==NULL)
{
printf("\n empty file \n");

}

fscanf(open, "%d", &No_children_forked);

        
	////////////////////////////////////////////////////////////////////////////////////////////////

	int  PID;
	int i;
	for(i=1 ;i<4 ;i++)
	{
		PID=fork();
 		if(PID == 0)  //child
	 	{
			fscanf(open, "%s", &child_name);
	        	if (i==1)
	      		{
				///Manager///
				struct semaphore * sem = (struct semaphore *) shmat(Semaphores_shmid,(void*)0,0);
				//struct Record * DatabaseTable = (struct Record *) shmat(shmid,(void*)0,0); 
				int* pids = (int*) shmat(PIDs_shmid,(void*)0,0);
				pids[1]=getpid();
				Requests_Handler(msgqid,sem,DatabaseTable);
              		}

	        	else if(i==2)
             		{   
				/*while(strcmp(child_name, "Child1")!=0)
				{
					fscanf(open, "%s", &child_name);
				}*/
				//printf("\n Child name is %s \n",child_name);
           			///Client 1 ///
				int* pids = (int*) shmat(PIDs_shmid,(void*)0,0);
				pids[2]=getpid(); 
				sleep(3);
				do_child(1,i,pids,msgqid);
				//printf("\n inside do_child \n");
				//printf("\n inside do_child \n");
				
				
				
             		}

		        else if(i==3)
		     	{
				/*while(strcmp(child_name, "Child2")!=0)
				{
					fscanf(open, "%s", &child_name);
				}*/
				//printf("\n Child name is %s \n",child_name);
				printf("\n I'm the second child \n");
		       		///Client 2 ///
				int* pids = (int*) shmat(PIDs_shmid,(void*)0,0);
				pids[3]=getpid(); 
				sleep(3);
				do_child(1,i,pids,msgqid);
				
			
		     	}
		    
		        // To prevent children from forking
		        break;

		}
  		else if(PID > 0 ) // parent 
	      	{    
			 printf("\nI am the parent who's pid is  = %d\n\n", getpid());
			 if(i==3){while(1);}
	      	}
	 
		else  // error
		      
		{ 	
			 perror("Error in fork");  	  	
			 exit(-1);
		}

    	} 

	return 0;
}
