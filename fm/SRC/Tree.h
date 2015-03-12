#ifndef TREE_H
#define TREE_H

typedef char Data;

struct Node_tree
{
  Data *data;
  unsigned int count;
  struct Node_tree *left, *right;
};

typedef struct
{
  struct Node_tree *root;
}Tree;

struct Node_tree* add_tree(struct Node_tree *node, Data *data);
Data *get_tree(Tree *tree, int key);
void print_tree(struct Node_tree *node);
void init_tree(Tree *tree);

#endif
