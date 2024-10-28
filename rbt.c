#include <stdlib.h>
#include <stdio.h>

#include "rbt.h"

static void left_rotate(rbt* rbt, rbt_node* x);
static void right_rotate(rbt* rbt, rbt_node* x);
static void insert_fixup(rbt* rbt, rbt_node* current);
static void inorder_walk(rbt* rbt, rbt_node* x);
static void destroy_nodes(rbt* rbt, rbt_node* x);
static void transplant(rbt* rbt, rbt_node* u, rbt_node* v);
static void delete_fixup(rbt* rbt, rbt_node* current);
static rbt_node* minimun(const rbt* rbt, rbt_node* iterate);

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

void rbt_destroy(rbt* rbt) {
    if (!rbt) {printf("\n\nERROR::TREE DOES NOT EXIST\n\n"); return;}

    destroy_nodes(rbt, rbt->root);

    free(rbt->nil);
    free(rbt); rbt = NULL;
}

static void destroy_nodes(rbt* rbt, rbt_node* x) {
    if (x != rbt->nil) {
        destroy_nodes(rbt, x->left_child);
        destroy_nodes(rbt, x->right_child);
        free(x);
    } else
        return;
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

int rbt_minimun(const rbt* rbt) {
    if (!rbt) {printf("\n\nERROR::TREE DOES NOT EXIST\n\n"); return 0;}

    rbt_node* rtn = minimun(rbt, rbt->root);

    return rtn == rbt->nil ? 0 : rtn->key;
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

static void insert_fixup(rbt* rbt, rbt_node* current) {
    while (current->parent->color == RED) {
        if (current->parent == current->parent->parent->left_child) {
            rbt_node* uncle = current->parent->parent->right_child;

            if (uncle->color == RED) {
                current->parent->color = BLACK;
                uncle->color = BLACK;
                current->parent->parent->color = RED;
                current = current->parent->parent;
            } else {
                if (current == current->parent->right_child) {
                    current = current->parent;
                    left_rotate(rbt, current);
                }

                current->parent->color = BLACK;
                current->parent->parent->color = RED;
                right_rotate(rbt, current->parent->parent);
            }
        } else {
            rbt_node* uncle = current->parent->parent->left_child;

            if (uncle->color == RED) {
                current->parent->color = BLACK;
                uncle->color = BLACK;
                current->parent->parent->color = RED;
                current = current->parent->parent;
            } else {
                if (current == current->parent->left_child) {
                    current = current->parent;
                    right_rotate(rbt, current);
                }

                current->parent->color = BLACK;
                current->parent->parent->color = RED;
                left_rotate(rbt, current->parent->parent);
            }
        }
    }

    rbt->root->color = BLACK;
}

static void transplant(rbt* rbt, rbt_node* u, rbt_node* v) {
    if (u->parent == rbt->nil)
        rbt->root = v;
    else if (u == u->parent->left_child)
        u->parent->left_child = v;
    else u->parent->right_child = v;

    v->parent = u->parent;
}

static rbt_node* minimun(const rbt* rbt, rbt_node* iterate) {
    rbt_node* current_node = iterate;

    while (current_node->right_child != rbt->nil)
        current_node = current_node->right_child;

    return current_node;
}

void rbt_delete(rbt* rbt, int key) {
    if (!rbt) {printf("\n\nERROR::TREE DOES NOT EXIST\n\n"); return;}

    rbt_node* node_to_delete = rbt_search(rbt, key);

    if (!node_to_delete) return;

    //Subtree that remains after when doing an operation
    rbt_node* subtree_remaing = rbt->nil;
    //Minimum of a subtree to separete and merge into main
    rbt_node* mini_of_subtree = node_to_delete;
    bool mini_og_color = mini_of_subtree->color;

    if (node_to_delete->left_child == rbt->nil) {
        subtree_remaing = node_to_delete->right_child;
        transplant(rbt, node_to_delete, subtree_remaing);
    } else if (node_to_delete->right_child == rbt->nil) {
        subtree_remaing = node_to_delete->left_child;
        transplant(rbt, node_to_delete, subtree_remaing);
    } else {
        mini_of_subtree = minimun(rbt, node_to_delete->right_child);
        mini_og_color = mini_of_subtree->color;
        subtree_remaing = mini_of_subtree->right_child;

        if (mini_of_subtree->parent == node_to_delete) {
            subtree_remaing->parent = mini_of_subtree;
        } else {
            transplant(rbt, mini_of_subtree, mini_of_subtree->right_child);
            mini_of_subtree->right_child = node_to_delete->right_child;
            mini_of_subtree->right_child->parent = mini_of_subtree;
        }

        transplant(rbt, node_to_delete, mini_of_subtree);
        mini_of_subtree->left_child = node_to_delete->left_child;
        mini_of_subtree->left_child->parent = mini_of_subtree;
        mini_of_subtree->color = node_to_delete->color;
    }

    if (mini_og_color == BLACK)
        delete_fixup(rbt, subtree_remaing);

    free(node_to_delete);
}

static void delete_fixup(rbt* rbt, rbt_node* current) {
    while (current != rbt->root && current->color == BLACK) {
        if (current == current->parent->left_child) {
            rbt_node* sibling = current->parent->right_child;

            if (sibling->color == RED) {
                sibling->color = BLACK;
                current->parent->color = RED;
                left_rotate(rbt, current->parent);
                sibling = current->parent->right_child;
            }

            if (sibling->right_child->color == BLACK && sibling->left_child->color == BLACK) {
                sibling->color = RED;
                current = current->parent;
            } else {
                if (sibling->right_child->color == BLACK) {
                    sibling->left_child->color = BLACK;
                    sibling->color = RED;
                    right_rotate(rbt, sibling);
                    sibling = current->parent->right_child;
                }

                sibling->color = current->parent->color;
                current->parent->color = BLACK;
                sibling->right_child->color = BLACK;
                left_rotate(rbt, sibling->parent);
                current = rbt->root;
            }
        } else {
            rbt_node* sibling = current->parent->left_child;

            if (sibling->color == RED) {
                sibling->color = BLACK;
                current->parent->color = RED;
                right_rotate(rbt, current->parent);
                sibling = current->parent->left_child;
            }

            if (sibling->right_child->color == BLACK && sibling->left_child->color == BLACK) {
                sibling->color = RED;
                current = current->parent;
            } else {
                if (sibling->left_child->color == BLACK) {
                    sibling->right_child->color = BLACK;
                    sibling->color = RED;
                    left_rotate(rbt, sibling);
                    sibling = current->parent->left_child;
                }

                sibling->color = current->parent->color;
                current->parent->color = BLACK;
                sibling->left_child->color = BLACK;
                right_rotate(rbt, current->parent);
                current = rbt->root;
            }
        }
    }
    current->color = BLACK;
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