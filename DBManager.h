#include "common.h"
#include "query.h"
int Down (struct semaphore * s)
{
	 printf("\nLock is set to zero \n");
	 s->lock = 0;
	 return Dequeue(s->arrayBlockedPids);
}

void Up (struct semaphore * s,int ClientID) 
{
         int compare = (__sync_lock_test_and_set(&s->lock, 1)) ;
         if(compare ==0) 
         { 
               printf("\nProcess %d is allowed to enter critical section \n",ClientID);
               return; 
         }
         else if (compare ==1)
         {                  
              Enqueue(s->arrayBlockedPids,ClientID); 
              printf("\nProcess %d has been blocked \n",s->arrayBlockedPids->elements[s->arrayBlockedPids->front]);
              kill(ClientID,SIGSTOP);   
              return; 
         }
}

void managerAcquire(int ClientID,key_t msgqid ,struct semaphore * sem, int key)
{
	    Up(&sem[key],ClientID);

            struct msgSemaphoreofCertainRecord message;
            message.lock = sem[key].lock;
            message.mtype=ClientID ; 
 
            int send_val = msgsnd(msgqid, &message, sizeof(message.lock), !IPC_NOWAIT); 
	    if(send_val == -1)
	      	perror("Errror in send");
	    else
		printf("%d has sent the lock to %ld \n",getpid(),message.mtype); 
}

void managerRelease(key_t msgqid,struct semaphore * sem, int key)
{
         struct msgKeyManagerToClient message; 
         int rec_val = msgrcv(msgqid, &message, sizeof(message.key), getpid(), !IPC_NOWAIT);   
         if(rec_val == -1)
  	        perror("Error in receive");
         else
         {
		printf("\nManager receives the key of the released lock \n"); 
		int processId = Down(&sem[key]);

		if(processId==-1)
		      printf("\nThe queue is empty no process waiting \n");      

		else 
		{
		      kill(processId,SIGCONT); 
	              printf("Process %d is unblocked \n",processId);
		}
         }     
}

void Add_ReceiveRequestFromClient(key_t msgqid, int ClientId, struct semaphore * Sem, struct Record * DatabaseTable) 
{
     struct msgAddClientToManager  message;
     int rec_val = msgrcv(msgqid, &message, sizeof(message.newRecord), getpid(), !IPC_NOWAIT);   
  
        if(rec_val == -1)
  	        perror("Error in receive");

        else
        {
	  	printf("\nRecord received by manager \n");    
		
		int salary=message.newRecord.Salary ;
		char name[50];
		strcpy(name, message.newRecord.Name);
                int key = GenerateKey();
		printf("Salary is %d   \n",salary);
		printf("Name is %s   \n",name);
		printf("Generated key is  %d   \n",key);
		//printTable(DatabaseTable);
		addRecordToDatabase(name,salary,DatabaseTable,key);



		// call logger 
		// call add to database
		//if(addRecordToDatabase(name, salary,key, DatabaseTable))
		//{
			
			printf("\nRecord added successfully\n");
			printTable(DatabaseTable);
			Sem[key].lock=0;
			Sem[key].arrayBlockedPids = createQueue(5);
		//}
		//else 
		//	printf("\nRecord addition failure");

		struct msgKeyManagerToClient KeyMessage;
		KeyMessage.key = key;
		KeyMessage.mtype = ClientId;
		int send_val = msgsnd(msgqid, &KeyMessage, sizeof(KeyMessage.key), !IPC_NOWAIT);  
		if(send_val == -1)
	      		perror("Error in send");
	        else
			printf("\nManager has sent the key to %ld \n",KeyMessage.mtype); 
		
        }     

}



void Modify_ReceiveRequestFromClient(key_t msgqid, int ClientId, struct Record * DatabaseTable) 
{
struct msgModifyClientToManager  message;
     int rec_val = msgrcv(msgqid, &message, 3*sizeof(message.symb), getpid(), !IPC_NOWAIT);   
  
        if(rec_val == -1)
  	        perror("Error in receive");

        else
        {
	printf("\nModify request received by manager through client %d \n",ClientId);
	printf("\n\n try %d %s %d \n",message.key,message.symb,message.change_salary);
	int key=message.key;
	char symb[10];
	strcpy(symb,message.symb);
	int change_salary=message.change_salary;
	bool found=0;
	int i=0;
	//printf("\n\n try %d \n",DatabaseTable[0].Key);
	while(DatabaseTable[i].Key > 0)
		{
			printf("\n\n try %d \n",DatabaseTable[i].Key);

			if(DatabaseTable[i].Key ==key)
				{
					found=1;
					printf("\n Key is found %d \n",i);
					break;
					
				}



		i++;

		}


	if (found==0)
	{
	printf("\n Key is NOT found \n");
	
	}
	if(strcmp(symb,"+")==0)
	{

	DatabaseTable[i].Salary=DatabaseTable[i].Salary+change_salary;
	printf("\n Salary changed successfully by + \n");

	}

	else if(strcmp(symb,"-")==0)
	{

	DatabaseTable[i].Salary=DatabaseTable[i].Salary - change_salary;
	printf("\n salaries %d   %d \n\n",DatabaseTable[i].Salary,change_salary);
	printf("\n Salary changed successfully by - \n");
	}

	else
	{

	printf("\n Wrong symbol !! \n");
	}
	//printTable(DatabaseTable);


	}



}



void Requests_Handler(key_t msgqid,struct semaphore * Sem, struct Record * DatabaseTable)
{
     printf("\n Requests handler \n"); 
     while(1)
     {
	struct Order_msg message; 
        int rec_val = msgrcv(msgqid, &message, 3*sizeof(message.order), getpid(), !IPC_NOWAIT);  
        if(rec_val == -1)
  	    perror("Error in receive");
        else
        {
	    printf("\nNew request is received \n"); 
	    int order = message.order;
	    int key = message.key; 
            switch(order){
                  case add:
                        printf("Manager in add by client -> %d\n",message.clientPid);
			Add_ReceiveRequestFromClient(msgqid,message.clientPid,Sem,DatabaseTable);
                        break;
                  case modify:
                        printf("Manager in modify by client -> %d\n",message.clientPid);

Modify_ReceiveRequestFromClient(msgqid,message.clientPid,DatabaseTable);
                        break;
                  case acquire:
                        printf("Manager in acquire by client -> %d\n",message.clientPid);
			managerAcquire(message.clientPid,msgqid ,Sem,key);
			break;
                  case release:
                        printf("Manager in release by client -> %d\n",message.clientPid);
			managerRelease(msgqid,Sem,key);
			break;

            }
        }

     }
}
