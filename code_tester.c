#include "headers.h"

int main()
{
  struct processData D;
  struct Queue *processes_queue = createQueue();
  //Reading the file
  FILE *in_file = fopen("test.txt", "r"); // read only
  // test for files not existing.

  if (in_file == NULL)
  {
    printf("Error! Could not open file\n");
    exit(-1); // must include stdlib.h
  }
  int a, b, c, d;
  struct QNode *temp;
  while (fscanf(in_file, "%d\t%d\t%d\t%d", &a, &b, &c, &d) != EOF)
  {
    D.id = a;
    D.arrivaltime = b;
    D.runningtime = c;
    D.priority = d;
    //enQueue(processes_queue,D);
    temp = newNode(D);

    insert_priority_Sorted(processes_queue, temp);
  }
  struct QNode *p;
  p = processes_queue->front;
  /*
    while(processes_queue->front!=NULL)
    {
      printf("%d %d %d %d\n", processes_queue->front->key.id, processes_queue->front->key.arrivaltime, processes_queue->front->key.runningtime, processes_queue->front->key.priority);
      deQueue(processes_queue);
      }
  */
  printQueue(processes_queue);
}