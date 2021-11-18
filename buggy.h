#include <stdlib.h>
#include <stdio.h>
#include <math.h>

struct node_s
{
    int value;
    int range_begin;
    int range_end;
    int ocupied;
    struct node_s *left;
    struct node_s *right;
};

struct node_s *tree(int v, struct node_s *l, struct node_s *r)
{
    struct node_s *n = malloc(sizeof(struct node_s));
    n->value = v;
    n->left = l;
    n->right = r;
    return n;
}

void destroy_tree(struct node_s *n)
{
    if (n->left)
        destroy_tree(n->left);
    if (n->right)
        destroy_tree(n->right);
    free(n);
}

void preorder(struct node_s *n, void (*f)(int))
{
   /* printf("%d (%d) {%d , %d}\n", n->value, n->ocupied, n->range_begin, n->range_end);
    if (n->left)
        preorder(n->left, f);
    if (n->right)
        preorder(n->right, f);*/
         if (n->ocupied == 1)
    {
        printf("%d (%d) {%d , %d}\n", n->value, n->ocupied, n->range_begin, n->range_end);
    }
    else
    {
        if (n->left)
            preorder(n->left, f);
        if (n->right)
            preorder(n->right, f);
    }
}

void dummy(struct node_s *n, void (*f)(int))
{
    if (n->ocupied == 1)
    {
        printf("%d (%d) {%d , %d}\n", n->value, n->ocupied, n->range_begin, n->range_end);
    }
    else
    {
        if (n->left)
            preorder(n->left, f);
        if (n->right)
            preorder(n->right, f);
    }
}

void inorder(struct node_s *n, void (*f)(int))
{
    if (n->left)
        inorder(n->left, f);
    f(n->value);
    if (n->right)
        inorder(n->right, f);
}

void postorder(struct node_s *n, void (*f)(int))
{
    if (n->left)
        postorder(n->left, f);
    if (n->right)
        postorder(n->right, f);
    f(n->value);
}

///////////////////////////////
// create the buggy system tree
struct node_s *retree(struct node_s *n)
{
    if (n->value == 1)
        return n;

    n->left = tree(n->value / 2, NULL, NULL);
    n->left->range_begin = n->range_begin;
    n->left->range_end = n->range_begin + n->value / 2 - 1;
    n->left->ocupied = 0;
    n->right = tree(n->value / 2, NULL, NULL);
    n->right->range_begin = n->range_end - n->value / 2 + 1;
    n->right->range_end = n->range_end;
    n->right->ocupied = 0;
    retree(n->left);
    retree(n->right);
}
struct node_s *intiatetree(int N)
{
    struct node_s *tr = tree(N, NULL, NULL);
    tr->range_begin = 0;
    tr->range_end = N - 1;
    tr->ocupied = 0;
    retree(tr);
    return tr;
}

////////////////////////////////
// Buggy mechanims
int is_free(struct node_s *tr)
{

    if (tr->ocupied)
        return 0;
    if (tr->value == 1)
        return 1;
    if (!is_free(tr->left))
        return 0;
    if (!is_free(tr->right))
        return 0;
    return 1;
}
int reserve(int size, struct node_s *tr, int *begin, int *end)
{
    if (tr->ocupied)
        return 0;
    if ((size <= tr->value) && (size > tr->value / 2) /*&& is_free(tr)*/)
    {
        if (is_free(tr))
        {
            *begin = tr->range_begin;
            *end = tr->range_end;
            tr->ocupied = 1;
            return 1; // reseve successed
        }
        else
            return 0;
    }
    if (reserve(size, tr->left, begin, end))
        return 1;
    if (reserve(size, tr->right, begin, end))
        return 1;
    return 0; // reseve failed
}
int free_memory(struct node_s *tr, int *begin, int *end)
{
    if (tr->value == 1)
    {
        if ((tr->range_begin == *begin) && (tr->range_end == *end))
        {
            if (tr->ocupied == 1)
            {
                tr->ocupied = 0;
                return 1; // memory freed
            }
            printf("memory already free \n");
            return 0; // somthing wrong : tring to free already freed memory
        }
        else
        {
            return 0;
            printf("Not here \n");
        }
    }
    else
    {
        if ((tr->range_begin == *begin) && (tr->range_end == *end))
        {
            if (tr->ocupied == 1)
            {
                tr->ocupied = 0;
                return 1; // memory freed
            }
            printf("memory already free \n");
            return 0; // somthing wrong : tring to free already freed memory
        }
    }
    if (free_memory(tr->left, begin, end))
        return 1;
    if (free_memory(tr->right, begin, end))
        return 1;
    return 0; //  segment not found
}

void print(int n)
{
    printf("%d ", n);
}