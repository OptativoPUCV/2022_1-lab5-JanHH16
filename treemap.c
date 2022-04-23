#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "treemap.h"

typedef struct TreeNode TreeNode;


struct TreeNode {
    Pair* pair;
    TreeNode * left;
    TreeNode * right;
    TreeNode * parent;
};

struct TreeMap {
    TreeNode * root;
    TreeNode * current;
    int (*lower_than) (void* key1, void* key2);
};

int is_equal(TreeMap* tree, void* key1, void* key2){
    if(tree->lower_than(key1,key2)==0 &&  
        tree->lower_than(key2,key1)==0) return 1;
    else return 0;
}


TreeNode * createTreeNode(void* key, void * value) {
    TreeNode * new = (TreeNode *)malloc(sizeof(TreeNode));
    if (new == NULL) return NULL;
    new->pair = (Pair *)malloc(sizeof(Pair));
    new->pair->key = key;
    new->pair->value = value;
    new->parent = new->left = new->right = NULL;
    return new;
}

TreeMap * createTreeMap(int (*lower_than) (void* key1, void* key2)) 
{
    TreeMap *tM = (TreeMap *) malloc(sizeof(TreeMap));
    tM -> root = NULL;
    tM -> current = NULL;
    tM -> lower_than = lower_than;
    
    return tM;
}

void insertTreeMap(TreeMap * tree, void* key, void * value) 
{
    if(tree->root == NULL) return;
    TreeNode *nodoAux;
    nodoAux = tree->root;
    TreeNode *padre = NULL;
    
    while(nodoAux != NULL)
    {
        padre = nodoAux;
        if(tree->lower_than(nodoAux->pair->key,key))
            nodoAux = nodoAux->right;
        else
        {
            if(tree->lower_than(key,nodoAux->pair->key))
                nodoAux = nodoAux->left;
            else return;
        }
        
    }
    TreeNode *nuevoNodo;
    nuevoNodo = createTreeNode(key,value);
    nuevoNodo->parent = padre;
    if(padre==NULL)
        tree->root = nuevoNodo;
    else
    {
        if(tree->lower_than(nuevoNodo->pair->key,padre->pair->key))
            padre->left = nuevoNodo;
        else    
            padre->right = nuevoNodo;
    }
    tree->current = nuevoNodo;
}

TreeNode * minimum(TreeNode * x){
    TreeNode *nodoAux;
    nodoAux = x;
    while(nodoAux->left != NULL)
    {
        nodoAux = nodoAux->left;
    }
    return nodoAux;
}

void removeNode(TreeMap * tree, TreeNode* node) 
{
    // Caso 1: sin hijos.
    if(node->left == NULL && node->right == NULL)
    {
        node->parent->left = NULL;
        node->parent->right = NULL;
        node->parent = NULL;
        free(node);
        return;
    }
    // Caso 3: con dos hijos.
    if(node->left != NULL && node->right != NULL)
    {
        TreeNode *nodoAux;
        TreeNode *padre;
        nodoAux = node->left;
        if(nodoAux->right != NULL)
        {
            while(nodoAux->right != NULL)
            {
                nodoAux =  nodoAux->right;
            }
            padre =  node;
            node = nodoAux;
            node->parent = padre->parent;
            node->right = padre->right;
            node->left = padre->left;
            if(nodoAux->left != NULL)
            {
                nodoAux->parent->right = nodoAux->left;
            }
            else
                nodoAux->parent->right = NULL;
            free(nodoAux);
            return;
        }
        else
        {
            padre = nodoAux->parent;
            node = nodoAux;
            node->left = nodoAux->left;
            node->parent = padre->parent;
            node->right = padre->right;
            /*if(nodoAux->left == NULL)
            {
                padre->right = NULL;
            }
            else
                padre->right = nodoAux->left;*/
            free(nodoAux);
            //free(padre);
            return;
        }
        
    }
    // Caso 2: con solo un hijo.
    // Caso 2.1: hijo izquierdo.
    if(node->left != NULL && node->right == NULL)
    {
        if(node->pair->key > node->parent->pair->key)
        {
            if(node->left != NULL){
                node->parent->right = node->left;
                node->left->parent = node->parent;
                free(node);
                return;
            }
            if(node->right != NULL){
                node->parent->right = node->right;
                node->left->parent = node->parent;
                free(node);
                return;
            }       
        }
        else
        {
            if(node->left != NULL){
                node->parent->right = node->left;
                node->left->parent = node->parent;
                free(node);
                return;
            }
            if(node->right != NULL){
                node->parent->right = node->right;
                node->left->parent = node->parent;
                free(node);
                return;
            }
        }
    }
    // Caso 2.2: hijo derecho.
    if(node->right != NULL && node->left == NULL)
    {
        if(node->pair->key > node->parent->pair->key)
        {
            if(node->left != NULL){
                node->parent->right = node->left;
                node->left->parent = node->parent;
                free(node);
                return;
            }
            if(node->right != NULL){
                node->parent->right = node->right;
                node->left->parent = node->parent;
                free(node);
                return;
            }       
        }
        else
        {
            if(node->left != NULL){
                node->parent->right = node->left;
                node->left->parent = node->parent;
                free(node);
                return;
            }
            if(node->right != NULL){
                node->parent->right = node->right;
                node->left->parent = node->parent;
                free(node);
                return;
            }
        }
    }
}

void eraseTreeMap(TreeMap * tree, void* key){
    if (tree == NULL || tree->root == NULL) return;

    if (searchTreeMap(tree, key) == NULL) return;
    TreeNode* node = tree->current;
    removeNode(tree, node);
}

Pair * searchTreeMap(TreeMap * tree, void* key) 
{
    if(tree != NULL && tree->root != NULL)
    {
        TreeNode *nodoAux;
        nodoAux = tree->root;
        while(nodoAux)
        {
            tree->current = nodoAux;
            if(is_equal(tree,nodoAux->pair->key,key) == 1) return nodoAux->pair;
            else
            {
                if(tree->lower_than(key,nodoAux->pair->key) == 1)
                {
                    if(nodoAux->left != NULL)
                        nodoAux = nodoAux->left;
                    else return NULL;
                }
                else
                {
                    if(nodoAux->left != NULL)
                        nodoAux = nodoAux->right;
                    else return NULL;
                }
            }
        }
    }
    else return NULL;
    return NULL;
}

Pair * upperBound(TreeMap * tree, void* key) 
{
    /*TreeNode *nodoAux;
    nodoAux = tree->current;
    while(nodoAux)
    {
        if(is_equal(tree,nodoAux->pair->key,key) == 1) return nodoAux->pair;
            else
            {
                if(tree->lower_than(key,nodoAux->pair->key) == 1)
                {
                    if(nodoAux->left != NULL)
                        nodoAux = nodoAux->left;
                    //else return NULL;
                }
                else
                {
                    if(nodoAux->left != NULL)
                        nodoAux = nodoAux->right;
                    //else return NULL;
                }
            }
    }*/
    return NULL;
}

Pair * firstTreeMap(TreeMap * tree) 
{
    TreeNode *first;
    first = tree->root;
    while(first->left != NULL)
    {
        first = first->left;
    }
    return first->pair;
}

Pair * nextTreeMap(TreeMap * tree) 
{
    TreeNode *nodoAux;
    nodoAux = tree->current;
    if(nodoAux->right != NULL)
    {
        nodoAux = nodoAux->right;
        tree->current = nodoAux;
        return nodoAux->pair;
    }
    else
    {
        if(nodoAux->right ==  NULL)
        {
            if(nodoAux->parent != NULL)
            {
                nodoAux = nodoAux->parent;
                tree->current = nodoAux;
                return nodoAux->pair;
            }
                   
        }
    }
    return NULL;
}
