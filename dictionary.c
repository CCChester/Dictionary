/**
 * File: q4/dictionary.c
 * Enter a description of this file.
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include "dictionary.h"

struct bstnode{
    void *key;
    void *value;
    struct bstnode *left;
    struct bstnode *right;
};

struct dictionary{
    struct bstnode *root;
    CompareFunction comp_func;
    FreeFunction free_key;
    FreeFunction free_value;
};

Dictionary create_Dictionary(CompareFunction comp_k,
                             FreeFunction free_k, FreeFunction free_v){
    assert(comp_k != NULL);
    assert(free_k != NULL);
    assert(free_v != NULL);
    Dictionary new = malloc(sizeof(struct dictionary));
    new->root = NULL;
    new->comp_func = comp_k;
    new->free_key = free_k;
    new->free_value = free_v;
    return new;
}

void destroy_helper(Dictionary dict, struct bstnode *data){
    if (data != NULL){
        dict->free_key(data->key);
    	dict->free_value(data->value);
    	destroy_helper(dict, data->left);
    	destroy_helper(dict, data->right);
    }
    else{return;}
    free(data);
}

void destroy_Dictionary(Dictionary dict){
    assert(dict != NULL);
    if (dict->root != NULL){
        destroy_helper(dict, dict->root);
    }
    else{return;}
    free(dict);
}

void insert(Dictionary dict, void *k, void *v){
    assert(dict != NULL);
    assert(k != NULL);
    assert(v != NULL);
    struct bstnode *cur = dict->root;
    struct bstnode *prev = NULL;
    int result = 0;
    while (cur != NULL){
        result = dict->comp_func(k, cur->key);
        if (result == 0){
            dict->free_value(cur->value);
            cur->value = v;
            return;
        }
        prev=cur;
        if (result > 0){
            cur=cur->right;
        }
        else if (result < 0){
            cur=cur->left;
        }
    }
    struct bstnode *new = malloc(sizeof(struct bstnode));
    new->key = k;
    new->value = v;
    new->left = NULL;
    new->right = NULL;
    if (prev == NULL){
        dict->root = new;
    }
    else if (result > 0){
        prev->right = new;
    }
    else if(result < 0){
        prev->left = new;
    }
}

void *lookup(Dictionary dict, void *k){
    assert(dict != NULL);
    assert(k != NULL);
    struct bstnode *new = dict->root;
    int result = 0;
    while(new){
        result = dict->comp_func(k,new->key);
        if (result > 0){
            new=new->right;
        }
        else if (result < 0){
            new=new->left;
        }
        else{
            return new->value;
        }
    }
        return NULL;
}

