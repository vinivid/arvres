#include <stdlib.h>
#include <stdio.h>

#include "rbt.h"

static void left_rotate(rbt* rbt, rbt_node* x);
static void right_rotate(rbt* rbt, rbt_node* x);
static void insert_fixup(rbt* rbt, rbt_node* z);
static void inorder_walk(rbt* rbt, rbt_node* x);
static void destroy_nodes(rbt* rbt, rbt_node* x);
static void transplant(rbt* rbt, rbt_node* u, rbt_node* v);
static void delete_fixup(rbt* rbt, rbt_node* x);
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

    rbt_node* z = rbt_search(rbt, key);

    if (!z) return;

    rbt_node* x = rbt->nil;
    rbt_node* y = z;
    bool y_og_color = y->color;

    if (z->left_child == rbt->nil) {
        x = z->right_child;
        transplant(rbt, z, z->right_child);
    } else if (z->right_child == rbt->nil) {
        x = z->left_child;
        transplant(rbt, z, z->left_child);
    } else {
        y = minimun(rbt, z->right_child);
        y_og_color = y->color;
        x = y->right_child;

        if (y->parent == z) {
            x->parent = y;
        } else {
            transplant(rbt, y, y->right_child);
            y->right_child = z->right_child;
            y->right_child->parent = y;
        }

        transplant(rbt, z, y);
        y->left_child = z->left_child;
        y->left_child->parent = y;
        y->color = z->color;
    }

    if (y_og_color == BLACK)
        delete_fixup(rbt, x);

    free(z);
}

static void delete_fixup(rbt* rbt, rbt_node* x) {
    while (x != rbt->root && x->color == BLACK) {
        if (x == x->parent->left_child) {
            rbt_node* w = x->parent->right_child;

            if (w->color == RED) {
                w->color = BLACK;
                x->parent->color = RED;
                left_rotate(rbt, x->parent);
                w = x->parent->right_child;
            }

            if (w->right_child->color == BLACK && w->left_child->color == BLACK) {
                w->color = RED;
                x = x->parent;
            } else {
                if (w->right_child->color == BLACK) {
                    w->left_child->color = BLACK;
                    w->color = RED;
                    right_rotate(rbt, w);
                    w = x->parent->right_child;
                }

                w->color = x->parent->color;
                x->parent->color = BLACK;
                w->right_child->color = BLACK;
                left_rotate(rbt, x->parent);
                x = rbt->root;
            }
        } else {
            rbt_node* w = x->parent->left_child;

            if (w->color == RED) {
                w->color = BLACK;
                x->parent->color = RED;
                right_rotate(rbt, x->parent);
                w = x->parent->left_child;
            }

            if (w->right_child->color == BLACK && w->left_child->color == BLACK) {
                w->color = RED;
                x = x->parent;
            } else {
                if (w->left_child->color == BLACK) {
                    w->right_child->color = BLACK;
                    w->color = RED;
                    left_rotate(rbt, w);
                    w = x->parent->left_child;
                }

                w->color = x->parent->color;
                x->parent->color = BLACK;
                w->left_child->color = BLACK;
                right_rotate(rbt, x->parent);
                x = rbt->root;
            }
        }
    }
    x->color = BLACK;
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