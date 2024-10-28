#include <stdlib.h>
#include <stdio.h>

#include "rbt.h"

static void left_rotate(rbt* rbt, rbt_node* x);
static void right_rotate(rbt* rbt, rbt_node* x);
static void insert_fixup(rbt* rbt, rbt_node* z);
static void inorder_walk(rbt* rbt, rbt_node* x);

rbt* rbt_construct(){
    rbt* new_rbt = malloc(sizeof(rbt));
    if (!new_rbt) {printf("\n\nERROR::BAD ALLOC\n\n"); return NULL;}

    new_rbt->nil = malloc(sizeof(rbt_node));
    if (!new_rbt->nil) {printf("\n\nERROR::BAD ALLOC\n\n"); return NULL;}

    new_rbt->nil->color = BLACK;
    new_rbt->nil->parent = NULL;
    new_rbt->nil->left_child = NULL;
    new_rbt->nil->right_child = NULL;
    new_rbt->nil->key = -451;

    new_rbt->root = new_rbt->nil;

    return new_rbt;
}

rbt_node* rbt_search(const rbt* rbt, int search_key) {
    if (!rbt) {printf("\n\nERROR::TREE DOES NOT EXIST\n\n"); return NULL;}

    rbt_node* current_node = rbt->root;

    while (current_node != rbt->nil && current_node->key != search_key) {
        if (search_key < current_node->key)
            current_node = current_node->left_child;
        else current_node = current_node->right_child;
    }
    return current_node == rbt->nil ? NULL : current_node;
}

rbt_node* rbt_minimun(const rbt* rbt) {
    if (!rbt) {printf("\n\nERROR::TREE DOES NOT EXIST\n\n"); return NULL;}

    rbt_node* current_node = rbt->root;

    while (current_node->left_child != rbt->nil)
        current_node = current_node->left_child;

    return current_node == rbt->nil ? NULL : current_node;
}

rbt_node* rbt_maximum(const rbt* rbt) {
    if (!rbt) {printf("\n\nERROR::TREE DOES NOT EXIST\n\n"); return NULL;}

    rbt_node* current_node = rbt->root;

    while (current_node->right_child != rbt->nil)
        current_node = current_node->right_child;

    return current_node == rbt->nil ? NULL : current_node;
}

static void left_rotate(rbt* rbt, rbt_node* x) {
    rbt_node* y = x->right_child;
    x->right_child = y->left_child;

    if (y->left_child != rbt->nil)
        y->left_child->parent = x;

    y->parent = x->parent;

    if (x->parent == rbt->nil)
        rbt->root = y;
    else if (x == x->parent->left_child)
        x->parent->left_child = y;
    else 
        x->parent->right_child = y;

    y->left_child = x;
    x->parent = y;
}

static void right_rotate(rbt* rbt, rbt_node* x) {
    rbt_node* y = x->left_child;
    x->left_child = y->right_child;

    if (y->right_child != rbt->nil)
        y->right_child->parent = x;

    y->parent = x->parent;

    if (x->parent == rbt->nil)
        rbt->root = y;
    else if (x == x->parent->left_child)
        x->parent->left_child = y;
    else 
        x->parent->right_child = y;

    y->right_child = x;
    x->parent = y;
}

int rbt_insert(rbt* rbt, int key) {
    if (!rbt) {printf("\n\nERROR::TREE DOES NOT EXIST\n\n"); return 1;}

    rbt_node* new_node = malloc(sizeof(rbt_node));
    if (!new_node) {printf("\n\nERROR::BAD ALLOC\n\n"); return 1;}

    new_node->key = key;

    rbt_node* trailing_node = rbt->nil;
    rbt_node* search_node = rbt->root;

    while (search_node != rbt->nil) {
        trailing_node = search_node;

        if (new_node->key < search_node->key)
            search_node = search_node->left_child;
        else
            search_node = search_node->right_child;
    }

    new_node->parent = trailing_node;

    if (trailing_node == rbt->nil)
        rbt->root = new_node;
    else if (new_node->key < trailing_node->key)
        trailing_node->left_child = new_node;
    else 
        trailing_node->right_child = new_node;

    new_node->left_child = rbt->nil;
    new_node->right_child = rbt->nil;
    new_node->color = RED;
    insert_fixup(rbt, new_node);

    return 0;
}

static void insert_fixup(rbt* rbt, rbt_node* z) {
    while (z->parent->color == RED) {
        if (z->parent == z->parent->parent->left_child) {
            rbt_node* y = z->parent->parent->right_child;

            if (y->color == RED) {
                z->parent->color = BLACK;
                y->color = BLACK;
                z->parent->parent->color = RED;
                z = z->parent->parent;
            } else {
                if (z == z->parent->right_child) {
                    z = z->parent;
                    left_rotate(rbt, z);
                }

                z->parent->color = BLACK;
                z->parent->parent->color = RED;
                right_rotate(rbt, z->parent->parent);
            }
        } else {
            rbt_node* y = z->parent->parent->left_child;

            if (y->color == RED) {
                z->parent->color = BLACK;
                y->color = BLACK;
                z->parent->parent->color = RED;
                z = z->parent->parent;
            } else {
                if (z == z->parent->left_child) {
                    z = z->parent;
                    right_rotate(rbt, z);
                }

                z->parent->color = BLACK;
                z->parent->parent->color = RED;
                left_rotate(rbt, z->parent->parent);
            }
        }
    }

    rbt->root->color = BLACK;
}

void rbt_print(rbt* rbt){
    if (!rbt) {printf("\n\nERROR::TREE DOES NOT EXIST\n\n"); return;}

    inorder_walk(rbt, rbt->root);
    printf("\n");
}

static void inorder_walk(rbt* rbt, rbt_node* x) {
    if (x != rbt->nil) {
        inorder_walk(rbt, x->left_child);
        printf("%d ", x->key);
        inorder_walk(rbt, x->right_child);
    }
}