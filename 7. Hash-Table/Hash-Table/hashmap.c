//
//  hashmap.c
//  Hash-Table
//
//  Created by Amin Benarieb on 01/12/15.
//  Copyright © 2015 Amin Benarieb. All rights reserved.
//

#include "hashmap.h"
#include <stdlib.h>
#include <string.h>
#include "core.h"


#pragma mark Common
unsigned int hashtab_hash(char *key, size_t size) {
    
    unsigned int h = 0;
    for (char *p = key; *p != '\0'; p++)
        h = h * HASHTAB_MUL + (unsigned int)*p;
    
    return h % size;
}

#pragma mark Closed Address
void hashtab_closed_init(hashmap_closed *hashmap, size_t size) {
    
    hashmap->col_count = 0;
    hashmap->size = size;
    hashmap->res_count = 0;
    
    for (int i = 0; i < size; i++)
        strncpy(hashmap->table[i], "\0", BUFFER_MAX_SIZE+1);
}
void hashtab_closed_reinit(hashmap_closed *hashtab){
    
    hashmap_closed new_hashmap;
    hashtab_closed_init(&new_hashmap, hashtab->size*2);
    
    for (int i = 0; i < hashtab->size; i++)
        if (hashtab->table[i][0] != '\0')
                hashtab_closed_add(&new_hashmap, hashtab->table[i]);
    
    hashtab->col_count = 0;
    hashtab->size *= 2;
    hashtab->res_count++;
    for (int i = 0; i < hashtab->size; i++)
        strncpy(hashtab->table[i], "\0", BUFFER_MAX_SIZE+1);
    
    for (int i = 0; i < new_hashmap.size; i++)
        if (new_hashmap.table[i][0] != '\0')
                hashtab_closed_add(hashtab, new_hashmap.table[i]);
    
}
void hashtab_closed_add(hashmap_closed *hashmap, char *key){
    
    int index = hashtab_hash(key, hashmap->size);
    
    if (hashmap->table[index][0] != '\0')
    {
        hashmap->col_count++;
        
        int i = index+1;
        do{
            
            if (hashmap->table[i][0] == '\0')
            {
                strncpy(hashmap->table[i], key, BUFFER_MAX_SIZE+1);
                return;
            }
            
            if (++i == hashmap->size)
                i = 0;
        }
        while (index != i);
        
        if (i == index)
            hashtab_closed_reinit(hashmap);
        index = hashtab_hash(key, hashmap->size);
    
    }
    
    strncpy(hashmap->table[index], key, BUFFER_MAX_SIZE+1);
    
}
bool hashtab_closed_lookup(hashmap_closed hashmap, char *key, unsigned long long *tb,  unsigned long long *te, unsigned long long *kcmp){
    
    int index;
    *kcmp = 0;
    
    *tb = tick();
    index = hashtab_hash(key, hashmap.size);
    
    (*kcmp)++;
    if (strncmp(hashmap.table[index], key, BUFFER_MAX_SIZE+1) == 0) {
        *te = tick();
        return true;
    }
    else
    {
        int i = index+1;
        do{
            (*kcmp)++;
            if ( strncmp(hashmap.table[i], key, BUFFER_MAX_SIZE+1) == 0)
            {
                *te = tick();
                return true;
            }
            

            if (++i == hashmap.size)
                i = 0;
            
        }
        while (index != i);
        
    }
    *te = tick();
    
    
    return false;
}
void hashtab_closed_delete_all(hashmap_closed *hashmap, char letter){
    
    for (int i = 0; i < hashmap->size; i++)
        if (hashmap->table[i][0] != '\0')
        {
            if (hashmap->table[i][0] == letter)
                hashmap->table[i][0] = '\0';
        }
}


#pragma mark Open Address
bool hashtab_checklist(listnode *node){
    unsigned list_size = 0;
    for (listnode *inode = node; inode != NULL; inode = inode->next, list_size++);
    if (list_size >= HASHTAB_LIST_SIZE)
        return true;
    
    return  false;
    
}
void hashtab_init(hashmap *hashmap, size_t size) {
    
    hashmap->col_count = 0;
    hashmap->size = size;
    hashmap->res_count = 0;
    
    for (int i = 0; i < size; i++)
        hashmap->table[i] = NULL;
}
void hashtab_reinit(hashmap *hashtab, Error *error){
    
    hashmap new_hashmap;
    hashtab_init(&new_hashmap, hashtab->size*2);
    
    for (int i = 0; i < hashtab->size; i++)
        if (hashtab->table[i] != NULL)
            for (listnode *node = hashtab->table[i]; node != NULL; node = node->next)
            {
                hashtab_add(&new_hashmap, node->key, error);
                if (error->code != 0)
                {
                    hashtab_free(&new_hashmap);
                    return;
                }
            }
    
    hashtab_free(hashtab);
    
    hashtab->col_count = 0;
    hashtab->size *= 2;
    hashtab->res_count++;
    for (int i = 0; i < hashtab->size; i++)
        hashtab->table[i] = NULL;
    
    for (int i = 0; i < new_hashmap.size; i++)
        if (new_hashmap.table[i] != NULL)
            for (listnode *node = new_hashmap.table[i]; node != NULL; node = node->next)
            {
                hashtab_add(hashtab, node->key, error);
                if (error->code != 0)
                {
                    hashtab_free(&new_hashmap);
                    return;
                }
            }
    
    hashtab_free(&new_hashmap);
    
}
void listnode_free(listnode *head){
    listnode *next;
    
    for (; head; head = next)
    {
        next = head->next;
        free(head);
    }
}

void hashtab_add(hashmap *hashmap, char *key, Error *error){
    
    listnode *node;
    int index = hashtab_hash(key, hashmap->size);
    bool flag_reinit = false;
    
    node = malloc(sizeof(*node));
    if (node != NULL) {
        strncpy(node->key, key, BUFFER_MAX_SIZE+1);
        
        if (hashmap->table[index] != NULL)
        {
            hashmap->col_count++;
            flag_reinit = hashtab_checklist(hashmap->table[index]);
        }
        
        node->next = hashmap->table[index];
        hashmap->table[index] = node;
    }
    else
    {
        error->code = 1;
        error->message = MESSAGE_MEMORY_ERROR;
    }
    
    if (flag_reinit)
    {
        hashtab_reinit(hashmap, error);
        if (error->code != 0)
            return ;
    }
    
}
void hashtab_delete(hashmap *hashmap, char *key){
    
    int index;
    listnode *p, *prev = NULL;
    
    index = hashtab_hash(key, hashmap->size);
    for (p = hashmap->table[index]; p != NULL; p = p->next) {
        if (strcmp(p->key, key) == 0) {
            if (prev == NULL)
                hashmap->table[index] = p->next;
            else
                prev->next = p->next;
            free(p);
            return;
        }
        prev = p;
    }
}
void hashtab_free(hashmap *hashmap){
    for (int i = 0; i < hashmap->size; i++)
        if (hashmap->table[i] != NULL)
            listnode_free(hashmap->table[i]);
    
}
listnode *hashtab_lookup( hashmap hashmap, char *key, unsigned long long *tb,  unsigned long long *te, unsigned long long *kcmp){
    
    int index;
    listnode *node;
    
    *kcmp = 0;
    
    *tb = tick();
    index = hashtab_hash(key, hashmap.size);
    
    for (node = hashmap.table[index]; node != NULL; node = node->next)
    {
        (*kcmp)++;
        if (strncmp(node->key, key, BUFFER_MAX_SIZE+1) == 0)
        {
            *te = tick();
            return node;
        }
    }
    *te = tick();
    
    return NULL;
}
void hashtab_delete_all(hashmap *hashmap, char letter){
    
    for (int i = 0; i < hashmap->size; i++)
        if (hashmap->table[i] != NULL)
        {
            for (listnode *node = hashmap->table[i], *prev = NULL; node != NULL; node = node->next)
            {
                if (node->key[0] == letter)
                {
                    if (prev == NULL)
                        hashmap->table[i] = node->next;
                    else
                        prev->next = node->next;
                    free(node);
                }
                else
                    prev = node;
            }
        }
}
