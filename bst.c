#include <stdlib.h>
#include <stdio.h>

typedef struct bst_node_ bst_node;

typedef struct bst_ bst;

struct bst_node_{
    bst_node* parent;
    bst_node* right_child;
    bst_node* left_child;

    int key;
};

struct bst_ {
    bst_node* root;
};

static bst_node* minimum (bst_node* start);

bst* bst_construct () {
    bst* new_bst = malloc(sizeof(bst));
    if (!new_bst) {printf("\n\nERROR::BAD ALLOC\n\n"); return NULL;}
    
    new_bst->root = NULL;

    return new_bst;
}

bst_node* bst_search(bst* bst, int key) {
    bst_node* current_node = bst->root;

    while (current_node != NULL && current_node->key != key)
        if (key < current_node->key)
            current_node = current_node->left_child;
        else
            current_node = current_node->right_child;
    
    return current_node;
}

int bst_minimun (bst* bst) {
    bst_node* res = minimum(bst->root);
    return res == NULL ? 0 : res->key;
}

static bst_node* minimum (bst_node* start) {
    bst_node* current_node = start;

    while (current_node->left_child != NULL)
        current_node = current_node->left_child;

    return current_node;
}

int bst_maximum (bst* bst) {
    if (!bst) {printf("\n\nERROR::TREE DOES NOT EXIST\n\n"); return 0;}

    bst_node* current_node = bst->root;

    if (!current_node)
        return 0;

    while (current_node->right_child != NULL) 
        current_node = current_node->right_child;

    return current_node->key;
}

int bst_insert (bst* bst, int key) {
    if (!bst) {printf("\n\nERROR::TREE DOES NOT EXIST\n\n"); return 1;}

    bst_node* new_node = malloc(sizeof(bst_node));

    if (!new_node) {printf("\n\nERROR::BAD ALLOC\n\n"); return 1;}

    new_node->key = key;
    new_node->left_child = NULL;
    new_node->right_child = NULL;

    bst_node* search_node = bst->root;
    bst_node* trailing_node = NULL;

    while (search_node != NULL) {
        trailing_node = search_node;

        if (new_node->key < search_node->key)
            search_node = search_node->left_child;
        else
            search_node = search_node->right_child;
    }

    new_node->parent = trailing_node;

    if (trailing_node == NULL) 
        bst->root = new_node;
    else if (new_node->key < trailing_node->key) 
        trailing_node->left_child = new_node;
    else 
        trailing_node->right_child = new_node;

    return 0;
}