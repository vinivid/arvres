#pragma once

#include <stdbool.h>

typedef struct rbt_ rbt;

typedef struct rbt_node_ rbt_node;

#define BLACK 0
#define RED 1

struct rbt_node_ {
    rbt_node* parent;
    rbt_node* left_child;
    rbt_node* right_child;
    int key;
    bool color;
};

struct rbt_ {
    rbt_node* root;
    rbt_node* nil;
};

rbt* rbt_construct();
void rbt_destroy(rbt* rbt);
rbt_node* rbt_search(const rbt* rbt, int search_key);
rbt_node* rbt_minimun(const rbt* rbt);
rbt_node* rbt_maximum(const rbt* rbt);
int rbt_insert(rbt* rbt, int key);
void rbt_print(rbt* rbt);
void rbt_delete(rbt* rbt, int key);