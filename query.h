//#include "common.h"

void queryEqualSalary(struct Record * Database,int salary)
{
    int i =0 ;
    printf(" Row                        Name                           Salary\n");
        while(Database[i].Key > 0)
    {
        if (Database[i].Salary == salary)
        {
            printf("%4d|%50s|%10d \n",Database[i].Key,Database[i].Name,Database[i].Salary);
        }
        
        i++;
    }
}

 
void queryMoreThanSalary(struct Record * Database,int salary)
{
    int i =0 ;
    printf(" Row                        Name                           Salary\n");
        while(Database[i].Key > 0)
    {
        if (Database[i].Salary > salary)
        {
            printf("%4d|%50s|%10d \n",Database[i].Key,Database[i].Name,Database[i].Salary);
        }
        
        i++;
    }
}

void queryLessSalary(struct Record * Database,int salary)
{
    int i =0 ;
    printf(" Row                        Name                           Salary\n");
        while(Database[i].Key > 0)
    {
        if (Database[i].Salary < salary)
        {
            printf("%4d|%50s|%10d \n",Database[i].Key,Database[i].Name,Database[i].Salary);
        }
        
        i++;
    }
}

void queryLessThanOrEqualSalary(struct Record * Database,int salary)
{
    int i =0 ;
    printf(" Row                        Name                           Salary\n");
        while(Database[i].Key > 0)
    {
        if (Database[i].Salary <= salary)
        {
            printf("%4d|%50s|%10d \n",Database[i].Key,Database[i].Name,Database[i].Salary);
        }
        
        i++;
    }
}

void queryMoreThanOrEqualSalary(struct Record * Database,int salary)
{
    int i =0 ;
    printf(" Row                        Name                           Salary\n");
        while(Database[i].Key > 0)
    {
        if (Database[i].Salary >= salary)
        {
            printf("%4d|%50s|%10d \n",Database[i].Key,Database[i].Name,Database[i].Salary);
        }
        
        i++;
    }
}




/*

bool addRecordToDatabase(char* name , int salary,struct Record * Database, int key)
{   
    int NumberofRecords = *IDs;
    if (NumberofRecords == 1000 )
    {
        return false ;
    }
    else 
    {   
        (Database+NumberofRecords)->Key = NumberofRecords +1;
        Database[NumberofRecords].Salary = salary;

        //Database[NumberofRecords].Key = NumberofRecords + 1; 
        //Database[NumberofRecords].Salary = salary;
        strcpy(Database[NumberofRecords].Name, name);
        *IDs = *IDs +1;
        return true ; 
    }

if(key==1001)
{
return false;
}
else
{
}
}
*/
void printTable (struct Record * Database)
{   int i =0;
    //int NumberofRecords = getLengthofDatabase(Database);
    printf(" Row                        Name                           Salary\n");
    //for (int i = 0 ; i < 2 ; i++)
    while(Database[i].Key > 0)
    {
        printf("%4d|%50s|%10d \n",Database[i].Key,Database[i].Name,Database[i].Salary);
        i++;
    }
}
