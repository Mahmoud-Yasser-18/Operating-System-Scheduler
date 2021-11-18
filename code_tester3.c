#include <stdio.h>
#include <stdlib.h>
#include "headers.h"
/*int main()
{
    struct heap_t *h = (struct heap_t *)calloc(1, sizeof(struct heap_t));
    struct processData D;

  
    FILE *in_file = fopen("test.txt", "r"); // read only
    // test for files not existing.

    if (in_file == NULL)
    {
        printf("Error! Could not open file\n");
        exit(-1); // must include stdlib.h
    }
    int a, b, c, d;
    
    while (fscanf(in_file, "%d\t%d\t%d\t%d", &a, &b, &c, &d) != EOF)
    {   
        
        D.id = a;
        D.arrivaltime = b;
        D.runningtime = c;
        D.priority = d;
        //enQueue(processes_queue,D);
       push(h,D.priority,&D);
    }
    struct processData *G;
     
    struct processData *M;
    G=pop(h);
    M=pop(h);
    printf("%d",M->id);
     
    return 0;
}
*/
/*typedef struct
{
    int priority;
    struct processData *data;
} node_t;

typedef struct
{
    node_t *nodes;
    int len;
    int size;
} heap_t;

void push(heap_t *h, int priority, struct processData *data)
{
    if (h->len + 1 >= h->size)
    {
        h->size = h->size ? h->size * 2 : 4;
        h->nodes = (node_t *)realloc(h->nodes, h->size * sizeof(node_t));
    }
    int i = h->len + 1;
    int j = i / 2;
    while (i > 1 && h->nodes[j].priority > priority)
    {
        h->nodes[i] = h->nodes[j];
        i = j;
        j = j / 2;
    }
    h->nodes[i].priority = priority;
    h->nodes[i].data = data;
    h->len++;
}

struct processData *pop(heap_t *h)
{
    int i, j, k;
    if (!h->len)
    {
        return NULL;
    }
        struct processData *data = h->nodes[1].data;

    h->nodes[1] = h->nodes[h->len];

    h->len--;

    i = 1;
    while (i != h->len + 1)
    {
        k = h->len + 1;
        j = 2 * i;
        if (j <= h->len && h->nodes[j].priority < h->nodes[k].priority)
        {
            k = j;
        }
        if (j + 1 <= h->len && h->nodes[j + 1].priority < h->nodes[k].priority)
        {
            k = j + 1;
        }
        h->nodes[i] = h->nodes[k];
        i = k;
    }
    return data;
}
*/
int main()
{
    heap_t *h = (heap_t *)calloc(1, sizeof(heap_t));
    

  struct processData *D;
    FILE *in_file = fopen("test.txt", "r"); // read only
    // test for files not existing.

    if (in_file == NULL)
    {
        printf("Error! Could not open file\n");
        exit(-1); // must include stdlib.h
    }
    int a, b, c, d;
    
    while (fscanf(in_file, "%d\t%d\t%d\t%d", &a, &b, &c, &d) != EOF)
    {   
      D = (struct processData *)calloc(1, sizeof(struct  processData));
    
        D->id = a;
        D->arrivaltime = b;
        D->runningtime = c;
        D->priority = d;
        //enQueue(processes_queue,D);
       push(h,D->priority,D);
    }
 
  
    while(1)
    {
        if (!h->len)break;
   
      printf("%d %d %d %d\n",h->nodes[1].data->id,h->nodes[1].data->arrivaltime,h->nodes[1].data->runningtime, h->nodes[1].data->priority);
    pop(h);  
    sleep(2);
      }
    return 0;
}
