#include "headers.h"
#include "buggy.h"
void clearResources(int);

int msgid;
int pid;
int pid2;
int main(int argc, char *argv[])
{
    signal(SIGINT, clearResources);

    //////////////////////////////////////////////////////////////////

    //////////////////////////////////////////////////////////////

    //Reading the file and populating the queue of processes

    struct processData D;                          //Dummy variable for data transfer
    struct Queue *processes_queue = createQueue(); // creating a queue for saving the info of the procceses

    FILE *in_file = fopen("test.txt", "r"); // read only
    // test for files not existing.
    if (in_file == NULL)
    {
        printf("Error! Could not open file\n");
        exit(-1); // must include stdlib.h
    }

    int a, b, c, d,e; // Dummy variables for data transfer
    while (fscanf(in_file, "%d\t%d\t%d\t%d\t%d", &a, &b, &c, &d,&e) != EOF)
    {
        D.id = a;
        D.arrivaltime = b;
        D.runningtime = c;
        D.priority = d;
        D.memory_size=e;
        enQueue(processes_queue, D);
    }
    //////////////////////////////////////////////////////////////////

    //////////////////////////////////////////////////////////////

    /////////////////////////////////////
    // 2. Ask the user for the chosen scheduling algorithm and its parameters, if there are any.
    printf("Please, Choose the scheduling algorithm\na. Non-preemptive Highest Priority First (HPF).\nb. Shortest Remaining time Next (SRTN).\nc. Round Robin (RR).\n");
    char alogrithmType;
    int RoundRobin = 0;
    char RoundRobinChars[5];
    fscanf(stdin, "%c", &alogrithmType);

    if (alogrithmType == 3)
    {
        printf("Please, choose the Quanta of the Round Robin Algorithm\n");
        fscanf(stdin, "%d", &RoundRobin);
        sprintf(RoundRobinChars, "%d", RoundRobin);
    }

    //////////////////////////////////////////////////////////////////

    //////////////////////////////////////////////////////////////

    /////////////////////////
    //// 3. Initiate and create the scheduler and clock processes.
    pid = fork();

    if (pid == -1)
        perror("error in forking Clock");

    else if (pid == 0)
    {
        char *argv[] = {"./clock.o", 0};
        execve(argv[0], &argv[0], NULL); //// forking the Clock
    }
    else
    {

        pid2 = fork();
        if (pid2 == -1)
            perror("error in forking Scheduler");

        else if (pid2 == 0)
        {
            char *argv[] = {"./scheduler.out", &alogrithmType, RoundRobinChars, 0};
            execve(argv[0], &argv[0], NULL); //// forking the Scheduler
        }
        else
        {

            //////////////////////////////////////////////////////////////////
            //Back to process Genrator
            //////////////////////////////////////////////////////////////

            /////// 4. initialize clock
            initClk();

            int Current_time = getClk();

            ////////////////////////////
            // // 5. Send the information to the scheduler at the appropriate time.
            // This is down by a messeage queue created by the process genrator

            /////////////////////////////////////////////////////////////////////////
            /// Generating a unquie Key for the queue
            msgid = msgget(1, 0666 | IPC_CREAT);
            key_t msgqid;
            int send_val;
            struct Gen_to_Sch message_processDate; //message to be sent from the Genrator
            //                                       to the Scheduler
            /////////////////////////////////////////////////////////////////////////
            struct QNode *N; //pointer to the node to be sent
            // The node contains a struct that carry info about the process
            /////////////////////////////////////////////////////////////////////////
            message_processDate.mtype = 1;               // can be ignored
             // can be ignored
            /////////////////////////////////////////////////////////////////////////
            int Next_process_time;

            while (processes_queue->front != NULL)
            {
                /////////////////////////////////////////////////////////////////////////
                //Calculating the sleep time of the process genrator
                // geting the time of the next process to be sent form the queue of prceeses
                // to the message queue
                Next_process_time = processes_queue->front->key.arrivaltime;
                Current_time = getClk(); // getting the time now
                //The process genrator should sleep untill the arrival time of the process in turn
                sleep(Next_process_time - Current_time);
                /////////////////////////////////////////////////////////////////////////
                 // extracting the front node from the queue
                /////////////////////////////////////////////////////////////////////////
                /// initializing the message
                message_processDate.ProcessData.arrivaltime =processes_queue->front->key.arrivaltime;
                message_processDate.ProcessData.id = processes_queue->front->key.id;
                message_processDate.ProcessData.runningtime = processes_queue->front->key.runningtime;
                message_processDate.ProcessData.priority =  processes_queue->front->key.priority;
                message_processDate.ProcessData.pid = 0;
                message_processDate.ProcessData.remainingTime =  processes_queue->front->key.runningtime;
                message_processDate.ProcessData.waitingTime = 0;
                 message_processDate.ProcessData.memory_size =   processes_queue->front->key.memory_size;
                /// Sending the message
                deQueue(processes_queue);
                int send_val = msgsnd(msgid, &message_processDate, sizeof(message_processDate.ProcessData), !IPC_NOWAIT);
                if (send_val == -1)
                    printf("Errror in send at %d", getClk());
                else
                {
                    ///printf("message sent at %d\n ", getClk());
                }
            }
            printf("Process genrator waiting\n");
            
            int status;
            int pid_wait = wait(&status);
            wait(&status);
            printf("Process genrator stoped waiting\n");
            // 7. Clear clock resources
            destroyClk(true);
        }
    }
}

void clearResources(int signum)
{
     destroyClk(true);
    msgctl(msgid, IPC_RMID, NULL);
    kill(pid,SIGINT);
    kill(pid2,SIGINT);
    destroyClk(true);
    printf("Process genrator exits\n");
    exit(0);
    //TODO Clears all resources in case of interruption
}
