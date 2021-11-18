#include "buggy.h"

int main()
{
    struct node_s *n = intiatetree(16);
    int begin;
    int end;
    printf("%d \n", reserve(3, n, &begin, &end));
    printf("%d \n", reserve(1, n, &begin, &end));
    printf("%d \n", reserve(2, n, &begin, &end));
    printf("%d \n", reserve(1, n, &begin, &end));
    printf("%d \n", reserve(6, n, &begin, &end));
    printf("%d %d\n", begin, end);
    begin = 0;
    end = 3;
    printf("%d \n", free_memory(n, &begin, &end));
    begin = 4;
    end = 4;
    printf("%d \n", free_memory(n, &begin, &end));
    printf("preorder:    \n");
    dummy(n, print);
    printf("\n");

    /*

    printf("level-order: ");
    levelorder(n, print);
    printf("\n");
*/
    destroy_tree(n);
    return 0;
}