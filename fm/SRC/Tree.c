#include "Tree.h"
#include <stdio.h>
#include <stdlib.h>

void init_tree(Tree *tree)
{
    tree->root=0;
}

struct Node_tree* add_tree(struct Node_tree *node, Data *data)
{
    int comparision;

    if(node==0)
    {
        node=(struct Node_tree*)malloc(sizeof(struct Node_tree));
        node->data=data;
        node->left=node->right=0;
        node->count=1;
    }
    else if(comparision = strcmp(data, node->data) == 0)
        node->count++;
    else if(comparision<0) node->left=add_tree(node->left, data);
    else node->right=add_tree(node->right, data);

    return node;
}

void delete_tree(struct Node_tree *delete)
{
    /*
    struct Node_tree *temp=delete->root, *temp_left, *temp_right;

    if(temp==0) return;

    if(temp->left->key < temp->previous->key)
    {

    }*/
}

void print_tree(struct Node_tree *node)
{
    if(node!=0)
    {
        print_tree(node->left);
        printf(node->data); printf(" %d\n", node->count);
        print_tree(node->right);
    }
}
