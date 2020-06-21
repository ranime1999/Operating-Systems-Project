
void Acquire(key_t msgqid)
{  
    struct msgSemaphoreofCertainRecord  messageRCV;
    int rec_val = msgrcv(msgqid, &messageRCV, sizeof(messageRCV.lock), getpid(), !IPC_NOWAIT);
    if(rec_val == -1)
	      perror("Errror in receive");
    else
	      printf("\nClient %d has received the lock \n",getpid());
    
    return ;  
}

void Release(key_t msgqid,int ManagerID,int key)
{
    struct msgKeyManagerToClient message;
    message.key = key ;
    message.mtype = ManagerID ;  
    int send_val = msgsnd(msgqid, &message, sizeof(message.key), !IPC_NOWAIT);
    printf("\nClient %d released the lock\n\n",getpid());
}



void Modify(key_t msgqid,int ManagerID,int key,int salary, char symb[10])
{
    struct msgModifyClientToManager message;
    message.key = key ;
    message.mtype = ManagerID ;
    strcpy(message.symb,symb); 
    message.change_salary=salary;
    int send_val = msgsnd(msgqid, &message, 3*sizeof(message.symb), !IPC_NOWAIT);
    printf("\nClient %d sent a modification request to manager %d\n\n",getpid(),ManagerID);

//printf("\n try modif %d %s %d \n",message.key,message.symb, message.change_salary);

}



void AddRecordRequest(key_t msgqid,int ManagerID,char name[],int salary)
{
    struct msgAddClientToManager  message;
    message.mtype = ManagerID;     	
    message.newRecord.Key = 0; //Dummy value
    message.newRecord.Salary = salary;
    strcpy(message.newRecord.Name, name);
    
    int send_val = msgsnd(msgqid, &message, sizeof(message.newRecord), !IPC_NOWAIT);
  
    if(send_val == -1)
  	perror("Errror in send");
    else
	printf("Record is sent successfully from the client %d to %d \n",getpid(),ManagerID);
	
}

void Requests_Sender(key_t msgqid,int myOrder,int managerPID,int key)
{
     struct Order_msg message; 
     message.order = myOrder;
     message.key = key;
     message.clientPid = getpid();
     message.mtype = managerPID;

    int send_val = msgsnd(msgqid, &message, 3*sizeof(message.order), !IPC_NOWAIT);
    if(send_val == -1)
	      perror("Errror in send");
    else
	      printf("\nClient %d  has sent the request to manager %ld\n",getpid(),message.mtype);
	
    switch(myOrder){
                  case modify:
                        printf("Client %d in modify \n",message.clientPid);
                        break;
                  case acquire:
                        printf("Client %d in acquire \n",message.clientPid);
			Acquire(msgqid);
			break;
                  case release:
                        printf("Client %d in release \n",message.clientPid);
			Release(msgqid,managerPID,key);
			break;
            }	
}

void Add_Request_Sender(key_t msgqid,int managerPID,char* name,int salary)
{
     struct Order_msg message; 
     message.order = add;
     message.key = 0;    // Default value
     message.clientPid = getpid();
     message.mtype = managerPID;

    int send_val = msgsnd(msgqid, &message, 3*sizeof(message.order), !IPC_NOWAIT);
    if(send_val == -1)
	      perror("Errror in send");
    else
	      printf("\nClient %d  has sent the request to manager %ld\n",getpid(),message.mtype);

    printf("Client %d in add \n",message.clientPid);	
    AddRecordRequest(msgqid,managerPID,name,salary);
                        
}


void Modify_Request_Sender(key_t msgqid,int managerPID,int key,int salary, char symb[10])
{
     struct Order_msg message; 
     message.order = modify;
     message.key = key;    
     message.clientPid = getpid();
     message.mtype = managerPID;

    int send_val = msgsnd(msgqid, &message, 3*sizeof(message.order), !IPC_NOWAIT);
    if(send_val == -1)
	      perror("Errror in send");
    else
	      printf("\nClient %d  has sent the request to manager %ld\n",getpid(),message.mtype);

    printf("Client %d in modify \n",message.clientPid);	
    Modify(msgqid,managerPID,key,salary,symb);
                        
}




