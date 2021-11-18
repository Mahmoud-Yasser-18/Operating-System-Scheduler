#include "headers.h"
#include "buggy.h"
heap_t *proccessTable;
struct Node *head;
struct Node *pointer1;
struct Node *pointer2;
struct node_s *memory_space;
FILE *output;
void cleanup(int signum)
{

    printf("Schedule terminating!\n");
    destroyClk(true);
    exit(0);
}
void child_handler(int signum)
{
    if (proccessTable->nodes[1].data->remainingTime <= 0)
    {
        printf("popping %d\n", proccessTable->nodes[1].data->pid);
        wait(NULL);
        free_memory(memory_space, &(proccessTable->nodes[1].data->memory_begin), &(proccessTable->nodes[1].data->memory_begin));
        //printf("At time %d freed %d from process %d from %d to %d\n", getClk(), proccessTable->nodes[1].data->memory_size, proccessTable->nodes[1].data->id, proccessTable->nodes[1].data->memory_begin, proccessTable->nodes[1].data->memory_end);
        fprintf(output, "At time %d freed %d from process %d from %d to %d\n", getClk(), proccessTable->nodes[1].data->memory_size, proccessTable->nodes[1].data->id, proccessTable->nodes[1].data->memory_begin, proccessTable->nodes[1].data->memory_end);
        //printf("At time %d freed %d from process %d from %d to %d\n", getClk(), proccessTable->nodes[1].data->memory_size, proccessTable->nodes[1].data->id, proccessTable->nodes[1].data->memory_begin, proccessTable->nodes[1].data->memory_end);
        printf("preorder:    ");
        preorder(memory_space, print);
        printf("\n");
        //fputs(sprintf("At time %d freed %d from process %d from %d to %d\n", getClk(), proccessTable->nodes[1].data->memory_size, proccessTable->nodes[1].data->id, proccessTable->nodes[1].data->memory_begin, proccessTable->nodes[1].data->memory_end),output);

        pop(proccessTable);
    }
    else
    {
     //   printf("stoped %d\n", proccessTable->nodes[1].data->pid);
    }
}

int main(int argc, char *argv[])
{
    // signal(SIGCONT,Continue);
    signal(SIGINT, cleanup);
    signal(SIGCHLD, child_handler);
    int pid_childs;
    int pid_finished;
    printf("Schudler iniatiated\n");
    initClk();
    output = fopen("output.txt", "w");

    if (output == NULL)
    {
        fprintf(stderr, "Can't open output file !\n");
        exit(1);
    }
    //////////////////////////////////
    // memory handlers
    memory_space = intiatetree(1024);
    int begin = 0, end = 0;
    if (*argv[1] == 'a') // Non pre-emptive priority
    {

        printf("Non preemptive HPF is in action\n");
        proccessTable = (heap_t *)calloc(1, sizeof(heap_t));

        /////////////////////////////////////////////////////////////////
        //////// receive the messeage
        struct Gen_to_Sch message;
        int msgid;
        msgid = msgget(1, 0666);
        char remaining_char[7];
        // msgrcv to receive message
        struct processData *temp;
        struct processData current;
        int dummy_counter = 0;
        int busy = 0;

        while (1)
        {
            int rcvTag = msgrcv(msgid, &message, sizeof(message), 0, IPC_NOWAIT);
            // checking of the message recived
            if (rcvTag == -1)
            {
                ///// No proceeses at the message queue
                //printf("nothing at %d\n\n", getClk());
                if (!proccessTable->len && !dummy_counter)
                {
                    printf(" No scheduler is sleeping\n");

                    sleep(1);
                }
                else
                {
                    if (!busy)
                    {
                        //printf("%d\n", proccessTable->nodes[1].data->priority);
                        current.memory_begin = proccessTable->nodes[1].data->memory_begin;
                        current.memory_end = proccessTable->nodes[1].data->memory_end;
                        current.memory_size = end - begin + 1;
                        current.arrivaltime = proccessTable->nodes[1].data->arrivaltime;
                        current.remainingTime = proccessTable->nodes[1].data->remainingTime;
                        current.waitingTime = proccessTable->nodes[1].data->waitingTime;
                        current.priority = proccessTable->nodes[1].data->priority;
                        current.pid = proccessTable->nodes[1].data->pid;
                        current.id = proccessTable->nodes[1].data->id;
                        current.runningtime = proccessTable->nodes[1].data->runningtime;
                        if (kill(proccessTable->nodes[1].data->pid, SIGCONT) == 0)
                        {
                            busy = 1;
                            pop(proccessTable);

                            sleep(1);
                            (current.remainingTime)--;
                        }
                    }
                    else
                    {
                        if (current.remainingTime == 0)
                        {
                            int status;
                            pid_finished = wait(&status);
                            free_memory(memory_space, &current.memory_begin, &current.memory_end);
                            printf("preorder:    ");
                            preorder(memory_space, print);
                            printf("\n");
                           fprintf(output,"At time %d freed %d from process %d from %d to %d\n", getClk(), current.memory_size, current.id, current.memory_begin, current.memory_end);

                            dummy_counter--;
                            busy = 0;
                        }
                        else
                        {
                            sleep(1);
                            (current.remainingTime)--;
                        }
                    }
                }
            }
            else
            {
                //// There are processes at the message queue, so inserting them in the priority queue
                //printf("process recived at %d \n  ", getClk());
                ////// DO Stuff to to insert them in the priority queue
                pid_childs = fork();
                if (pid_childs == -1)
                    printf("error in forking Processes");

                else if (pid_childs == 0)
                {
                    char remaining_char[7];

                    sprintf(remaining_char, "%d", message.ProcessData.runningtime);

                    char *argv[] = {"./process.out", remaining_char, 0};
                    execve(argv[0], &argv[0], NULL); //// forking the processes
                }
                else
                {
                    message.ProcessData.pid = pid_childs;
                    reserve(message.ProcessData.memory_size, memory_space, &begin, &end);
                    // printf("At time %d allocated %d to process %d from %d to %d\n", getClk(), end - begin + 1, message.ProcessData.id, begin, end);
                      fprintf(output, "At time %d allocated %d to process %d from %d to %d\n", getClk(), end - begin + 1, message.ProcessData.id, begin, end);
                 //   printf("At time %d allocated %d to process %d from %d to %d\n", getClk(), (end - begin + 1), message.ProcessData.id, begin, end);
                    printf("preorder:    ");
                    preorder(memory_space, print);
                    printf("\n");
                    //fputs(sprintf("At time %d allocated %d to process %d from %d to %d\n", getClk(), end -begin +1, message.ProcessData.id, begin,end),output);

                    if (kill(pid_childs, SIGSTOP))
                        ;
                    //printf("process with %d recived at %d with running time %d \n  ", pid_childs,getClk(),message.ProcessData.runningtime);
                    temp = (struct processData *)malloc(sizeof(struct processData));
                    temp->memory_begin = begin;
                    temp->memory_end = end;
                    temp->memory_size = end - begin + 1;
                    temp->arrivaltime = message.ProcessData.arrivaltime;
                    temp->remainingTime = message.ProcessData.remainingTime;
                    temp->waitingTime = message.ProcessData.waitingTime;
                    temp->priority = message.ProcessData.priority;
                    temp->pid = message.ProcessData.pid;
                    temp->id = message.ProcessData.id;
                    temp->runningtime = message.ProcessData.runningtime;
                    push(proccessTable, message.ProcessData.priority, temp);
                   // printf("highest Priority is %d\n", proccessTable->nodes[1].priority);
                    dummy_counter++;
                    continue;
                }
            }
        }
        //Data structure of process Table forking and signaling
        //
        //////////////////////////////
        //always sleep for one second
    }
    else if (*argv[1] == 'b')
    {

        printf("Shortest Remaining Time Next is in action \n");
        proccessTable = (heap_t *)calloc(1, sizeof(heap_t));

        /////////////////////////////////////////////////////////////////
        //////// receive the messeage
        struct Gen_to_Sch message;
        int msgid;
        msgid = msgget(1, 0666);
        char remaining_char[7];
        // msgrcv to receive message
        struct processData *temp;
        /////
        int pid_of_currnet_process = 0;
        while (1)
        {
            int rcvTag = msgrcv(msgid, &message, sizeof(message), 0, IPC_NOWAIT);
            // checking of the message recived
            if (rcvTag == -1)
            {
                ///// No proceeses at the message queue
                //printf("nothing at %d\n\n", getClk());
                if (!proccessTable->len)
                {
                    printf(" No scheduler is sleeping\n");

                    sleep(1);
                }
                else
                {

                    if (proccessTable->nodes[1].data->pid == pid_of_currnet_process)
                    {
                      //  printf("remaining %d\n", proccessTable->nodes[1].data->pid);
                        sleep(1);
                        proccessTable->nodes[1].data->remainingTime = proccessTable->nodes[1].data->remainingTime - 1;
                        proccessTable->nodes[1].priority = proccessTable->nodes[1].priority - 1;
                    }
                    else if (pid_of_currnet_process == 0)
                    {
                        pid_of_currnet_process = proccessTable->nodes[1].data->pid;
                        kill(pid_of_currnet_process, SIGCONT);
                       // printf("started %d\n", proccessTable->nodes[1].data->pid);
                        sleep(1);
                        proccessTable->nodes[1].data->remainingTime = proccessTable->nodes[1].data->remainingTime - 1;
                        proccessTable->nodes[1].priority = proccessTable->nodes[1].priority - 1;
                    }
                    else
                    {

                        kill(pid_of_currnet_process, SIGSTOP);
                        pid_of_currnet_process = proccessTable->nodes[1].data->pid;
                        kill(pid_of_currnet_process, SIGCONT);
                     //   printf("continued %d\n", proccessTable->nodes[1].data->pid);
                        sleep(1);
                        proccessTable->nodes[1].data->remainingTime = proccessTable->nodes[1].data->remainingTime - 1;
                        proccessTable->nodes[1].priority = proccessTable->nodes[1].priority - 1;
                    }
                }
            }
            else
            {
                //// There are processes at the message queue, so inserting them in the priority queue
                //printf("process recived at %d \n  ", getClk());
                ////// DO Stuff to to insert them in the priority queue
                pid_childs = fork();
                if (pid_childs == -1)
                    printf("error in forking Processes");

                else if (pid_childs == 0)
                {
                    char remaining_char[7];

                   // sprintf(remaining_char, "%d", message.ProcessData.runningtime);

                    char *argv[] = {"./process.out", remaining_char, 0};
                    execve(argv[0], &argv[0], NULL); //// forking the processes
                }
                else
                {
                    message.ProcessData.pid = pid_childs;
                    kill(pid_childs, SIGSTOP);
                    reserve(message.ProcessData.memory_size, memory_space, &begin, &end);
                    // printf("At time %d allocated %d to process %d from %d to %d\n", getClk(), end - begin + 1, message.ProcessData.id, begin, end);
                    //  fprintf(output, "At time %d allocated %d to process %d from %d to %d\n", getClk(), end - begin + 1, message.ProcessData.id, begin, end);
               //     printf("At time %d allocated %d to process %d from %d to %d\n", getClk(), end - begin + 1, message.ProcessData.id, begin, end);
                    printf("preorder:    ");
                    preorder(memory_space, print);
                    printf("\n");
                    //fputs(sprintf("At time %d allocated %d to process %d from %d to %d\n", getClk(), end -begin +1, message.ProcessData.id, begin,end),output);

                    temp = (struct processData *)malloc(sizeof(struct processData));
                    temp->memory_begin = begin;
                    temp->memory_end = end;
                    temp->memory_size = end - begin + 1;
                    temp->arrivaltime = message.ProcessData.arrivaltime;
                    temp->remainingTime = message.ProcessData.remainingTime;
                    temp->waitingTime = message.ProcessData.waitingTime;
                    temp->priority = message.ProcessData.priority;
                    temp->pid = message.ProcessData.pid;
                    temp->id = message.ProcessData.id;
                    temp->runningtime = message.ProcessData.runningtime;
                    push(proccessTable, message.ProcessData.remainingTime, temp);
                 //   printf("%d highest Priority is %d\n", proccessTable->nodes[1].data->pid, proccessTable->nodes[1].data->priority);
                    continue;
                }
            }
        }
    }
    else
    {
        printf("Round Robin in action\n");
        /////////////////////////////////////////////////////////////////
        //////// receive the messeage

        struct Gen_to_Sch message;
        int msgid;
        msgid = msgget(1, 0666);
        char remaining_char[7];
        int Qunata = atoi(argv[2]);
        // msgrcv to receive message
        struct processData *temp;
        /////
        int pid_of_currnet_process = 0;
        pointer1 = head;
        while (1)
        {
            int rcvTag = msgrcv(msgid, &message, sizeof(message), 0, IPC_NOWAIT);
            // checking of the message recived
            if (rcvTag == -1)
            {
                ///// No proceeses at the message queue
                //printf("nothing at %d\n\n", getClk());
                if (head == NULL)
                {
                    printf(" No processes scheduler is sleeping\n");

                    sleep(1);
                }
                else
                {
                    kill(pointer1->data->pid, SIGCONT);
                    sleep(Qunata);
                    kill(pointer1->data->pid, SIGSTOP);
                    pointer1 = pointer1->next;
                    if (pointer1 == NULL)
                    {
                        pointer1 = head;
                    }
                }
            }
            else
            {
                //// There are processes at the message queue, so inserting them in the priority queue
                //printf("process recived at %d \n  ", getClk());
                ////// DO Stuff to to insert them in the priority queue
                pid_childs = fork();
                if (pid_childs == -1)
                    printf("error in forking Processes");

                else if (pid_childs == 0)
                {
                    char remaining_char[7];

                    sprintf(remaining_char, "%d", message.ProcessData.runningtime);

                    char *argv[] = {"./process.out", remaining_char, 0};
                    execve(argv[0], &argv[0], NULL); //// forking the processes
                }
                else
                {
                    message.ProcessData.pid = pid_childs;
                    if (kill(pid_childs, SIGSTOP))
                        ;
                    //printf("process with %d recived at %d with running time %d \n  ", pid_childs,getClk(),message.ProcessData.runningtime);
                    temp = (struct processData *)malloc(sizeof(struct processData));
                    temp->arrivaltime = message.ProcessData.arrivaltime;
                    temp->remainingTime = message.ProcessData.remainingTime;
                    temp->waitingTime = message.ProcessData.waitingTime;
                    temp->priority = message.ProcessData.priority;
                    temp->pid = message.ProcessData.pid;
                    temp->id = message.ProcessData.id;
                    temp->runningtime = message.ProcessData.runningtime;
                    if (head = NULL)
                    {
                        push_(&head, temp);
                    }
                    else
                    {
                        insertAfter(pointer1, temp);
                    }
                    printf("%d highest Priority is %d\n", proccessTable->nodes[1].data->pid, proccessTable->nodes[1].data->priority);
                    continue;
                }
            }
        }
    }

    // destroyClk(true);
}
