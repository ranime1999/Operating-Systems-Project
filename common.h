#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <sys/ipc.h> 
#include <sys/shm.h> 
#include <stdbool.h> 


int DatabaseLength = 0;

struct msgKeyManagerToClient
{
   long mtype;
   int key ;
};

struct Record 
{ 
   int Key; 
   char Name[50];  
   int Salary;  
};

struct msgAddClientToManager
{
   long mtype;
   struct Record newRecord;
};



struct msgModifyClientToManager
{
long mtype;
int key;
char symb[10];
int change_salary;

};


struct Order_msg  

{
    long mtype;
    int order; 
    int clientPid;
    int key;
};
struct semaphore 
{
    volatile int lock;
    struct Queue * arrayBlockedPids ;
    
};

struct msgSemaphoreofCertainRecord
{
   long mtype;
   int lock;
};


int GenerateKey()
{
    printf("\nManager %d generates the key \n\n", getpid());
    int key= DatabaseLength + 1;
    DatabaseLength = key;
    return key ;
}


/*bool addRecordToDatabase(char* name , int salary,struct Record * Database, int key)
{   

	printf("\n trying add and key=%d \n",key);
    int NumberofRecords = key;
    if (NumberofRecords == 1000 )
    {
        return false ;
    }
    else 
    {   
	printf("\n trying add and number of records =%d \n",NumberofRecords);
        (Database+NumberofRecords)->Key = NumberofRecords +1;
	printf("\n dummy 1\n");
	Database[NumberofRecords].Key=key;
	printf("\n dummy 2\n");
        Database[NumberofRecords].Salary = salary;

        //Database[NumberofRecords].Key = NumberofRecords + 1; 
        //Database[NumberofRecords].Salary = salary;
        strcpy(Database[NumberofRecords].Name, name);
        //*IDs = *IDs +1;
        return true ; 
    }
}*/


bool addRecordToDatabase(char* name , int salary,struct Record * Database, int key)
{   

    printf("\n try add %d \n",key);
    int NumberofRecords = key;
    //int NumberofRecords = key;
    if (NumberofRecords == 1000 )
    {
        return false ;
    }
    else 
    {   printf("\n try add again %d \n",key);
	Database[NumberofRecords-1].Key = NumberofRecords;
        //(Database+NumberofRecords)->Key = NumberofRecords+1;
	printf("\n try add again 2 %d \n",key);
        Database[NumberofRecords-1].Salary = salary;

        //Database[NumberofRecords].Key = NumberofRecords + 1; 
	printf("\n try add again 3 %d \n",key);
        //Database[NumberofRecords].Salary = salary;
        strcpy(Database[NumberofRecords-1].Name, name);
        //*IDs = *IDs +1;
        return true ; 
    }

}

