//
//  binarytree.c
//  Hash-Table
//
//  Created by Amin Benarieb on 01/12/15.
//  Copyright © 2015 Amin Benarieb. All rights reserved.
//

#include "binarytree.h"

#pragma mark Avl-Tree

unsigned char avl_height(tnode* p){
    return p?p->height:0;
}
int bfactor(tnode* p){
    return avl_height(p->right)-avl_height(p->left);
}
void fixheight(tnode* p){
    unsigned char hl = avl_height(p->left);
    unsigned char hr = avl_height(p->right);
    p->height = ( hl > hr ? hl : hr)+1;
}

tnode* rotateright(tnode* p){
    tnode* q = p->left;
    p->left = q->right;
    q->right = p;
    fixheight(p);
    fixheight(q);
    return q;
}
tnode* rotateleft(tnode* q){
    tnode* p = q->right;
    q->right = p->left;
    p->left = q;
    fixheight(q);
    fixheight(p);
    return p;
}
tnode* balance(tnode* p){
    fixheight(p);
    if( bfactor(p)==2 )
    {
        if( bfactor(p->right) < 0 )
            p->right = rotateright(p->right);
        return rotateleft(p);
    }
    if( bfactor(p)==-2 )
    {
        if( bfactor(p->left) > 0  )
            p->left = rotateleft(p->left);
        return rotateright(p);
    }
    return p;
}

tnode* avl_insert(tnode* p, tnode *node, Error *error){
    if( !p )
        return node;
    
    int cmp = strncmp( node->file.name, p->file.name, BUFFER_MAX_SIZE+1);
    
    if( cmp < 0 )
        p->left = avl_insert(p->left, node, error);
    else if (cmp > 0)
        p->right = avl_insert(p->right, node, error);
    else
    {
        error->code = 2;
        error->message = "Node is already exist in tree!";
        return p;
    }
    
    return balance(p);
}

#pragma mark Task

void remove_nodes_predate(tnode **tree, tnode **newtree, char letter, Error *error, tnode* (*_insert)(tnode *, tnode *, Error *)){
    if (*tree == NULL )
        return;
    
    if((*tree)->file.name[0] != letter)
    {
        tnode *enode = create_node((*tree)->file, error);
        *newtree = _insert(*newtree, enode, error);
    }
    remove_nodes_predate(&(*tree)->left, newtree, letter, error, _insert);
    remove_nodes_predate(&(*tree)->right, newtree, letter, error, _insert);
    
}
tnode *copy_tree(tnode *tree, Error *error){
    if (tree == NULL)
        return tree;
    
    tnode *temp = create_node(tree->file, error);
    temp->left = copy_tree(tree->left, error);
    temp->right = copy_tree(tree->right, error);
    return temp;
    
}


#pragma mark Tree Node

tnode* create_node(tfile file, Error *error){
    tnode *node = malloc(sizeof(tnode));
    if (node == NULL)
    {
        error->code = 1;
        error->message = MESSAGE_MEMORY_ERROR;
        return NULL;
    }
    
    node->file = file;
    node->left = NULL;
    node->right = NULL;
    node->height = 0;
    
    
    return node;
}
void free_node(tnode *node, void *param){
    free(node);
}
tnode* insert(tnode *tree, tnode *node, Error *error){
    
    
    if (tree == NULL)
        return node;
    
    int cmp = strncmp( node->file.name, tree->file.name, BUFFER_MAX_SIZE+1);
    
    if (cmp == 0)
    {
        error->code = 2;
        error->message = "Node is already exist in tree!";
        return tree;
    }
    else if (cmp < 0)
    {
        tree->left = insert(tree->left, node, error);
    }
    else
    {
        tree->right = insert(tree->right, node, error);
    }
    
    return tree;
}
tnode* lookup(tnode *tree, tfile file, int (*compare)(tfile, tfile), unsigned long long *cmr_count){
    
    int cmp;
    (*cmr_count) = 0;
    
    while (tree != NULL)
    {
        cmp = compare(file, tree->file);
        (*cmr_count)++;
        if (cmp == 0)
            return tree;
        else if (cmp < 0)
            tree = tree->left;
        else
            tree = tree->right;
    }
    
    return NULL;
}
tfile rightmost(tnode *root){
    while (root->right != NULL)
        root = root->right;
    return root->file;
}
tnode *eject(tnode *root, tfile file, int (*compare)(tfile, tfile), Error *error){
    
    if (root == NULL)
    {
        error->code = 4;
        error->message = "Node to eject does not exist in tree!";
        return NULL;
    }
    
    int cmp = compare(file, root->file);
    
    if (cmp == 0) {
        if (root->left == NULL && root->right == NULL) {
            free(root);
            return NULL;
        }
        if (root->right == NULL && root->left != NULL) {
            tnode *temp = root->left;
            free(root);
            return temp;
        }
        if (root->right != NULL && root->left == NULL) {
            tnode *temp = root->right;
            free(root);
            return temp;
        }
        root->file = rightmost(root->left);
        root->left = eject(root->left, root->file, compare, error);
        return root;
    }
    if (cmp < 0) {
        root->left = eject(root->left, file, compare, error);
        return root;
    }
    if (cmp > 0) {
        root->right = eject(root->right, file, compare, error);
        return root;
    }
    
    
    return root;
}

#pragma mark Traversal
void apply_pre(tnode *tree, void (*f)(tnode*, void*), void *arg){
    if (tree == NULL)
        return;
    
    f(tree, arg);
    apply_pre(tree->left, f, arg);
    apply_pre(tree->right, f, arg);
}
void apply_post(tnode *tree, void (*f)(tnode*, void*), void *arg){
    if (tree == NULL)
        return;
    
    apply_post(tree->left, f, arg);
    apply_post(tree->right, f, arg);
    f(tree, arg);
}
void apply_in(tnode *tree, void (*f)(tnode*, void*), void *arg){
    if (tree == NULL)
        return;
    
    apply_post(tree->left, f, arg);
    f(tree, arg);
    apply_post(tree->right, f, arg);
}
