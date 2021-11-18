#include "headers.h"

void clearResources(int);
int msgid; 
int main(int argc, char * argv[])
{   
       // TODO Initialization
    // 1. Read the input files.

    struct processData D;//Dummy for data transfer
    struct Queue* processes_queue = createQueue(); 
    //Reading the file 
    FILE *in_file  = fopen("test.txt", "r"); // read only 
    // test for files not existing. 
    if (in_file == NULL) 
    {   
          printf("Error! Could not open file\n"); 
          exit(-1); // must include stdlib.h 
    }
  int a,b,c,d;
    while(fscanf(in_file,"%d\t%d\t%d\t%d",&a,&b,&c,&d)!= EOF)
    {
        D.id=a;
        D.arrivaltime=b;
        D.runningtime=c;
        D.priority=d;
        enQueue(processes_queue,D);
    
    }
    
/////////////////////////////////////
// 2. Ask the user for the chosen scheduling algorithm and its parameters, if there are any.
printf("Please, Choose the scheduling algorithm\na. Non-preemptive Highest Priority First (HPF).\nb. Shortest Remaining time Next (SRTN).\nc. Round Robin (RR).\n");
char alogrithmType;
int RoundRobin =0;
char RoundRobinChars[5];
fscanf(stdin,"%c",&alogrithmType);

if(alogrithmType==3)
{
    printf("Please, choose the Quanta of the Round Robin Algorithm\n");
    fscanf(stdin,"%d",&RoundRobin);
    sprintf(RoundRobinChars,"%d",RoundRobin);
}
printf("this one is fine ");
}
void clearResources(int signum)
{
    msgctl(msgid, IPC_RMID, NULL); 
    destroyClk(true);
    //TODO Clears all resources in case of interruption
}