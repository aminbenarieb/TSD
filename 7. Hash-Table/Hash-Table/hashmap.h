//
//  hashmap.h
//  Hash-Table
//
//  Created by Amin Benarieb on 01/12/15.
//  Copyright © 2015 Amin Benarieb. All rights reserved.
//

#ifndef hashmap_h
#define hashmap_h

#include <stdio.h>
#include "config.h"


#define HASHTAB_SIZE 10000
#define HASHTAB_MUL  31
#define HASHTAB_LIST_SIZE 4

typedef struct listnode listnode;
typedef struct hashmap hashmap;
typedef struct hashmap_closed hashmap_closed;

struct listnode {
    char key[BUFFER_MAX_SIZE+1];
    listnode *next;
};

struct hashmap_closed{
    char table[HASHTAB_SIZE][BUFFER_MAX_SIZE+1];
    size_t size;
    unsigned res_count;
    unsigned col_count;
};

struct hashmap{
    listnode *table[HASHTAB_SIZE];
    size_t size;
    unsigned res_count;
    unsigned col_count;
};


#pragma mark Closed Address
void hashtab_closed_init(hashmap_closed *, size_t );
void hashtab_closed_add(hashmap_closed *, char *);
bool hashtab_closed_lookup(hashmap_closed, char *, unsigned long long *, unsigned long long *, unsigned long long *);
void hashtab_closed_delete_all(hashmap_closed*, char);

#pragma mark Open Address
void hashtab_init(hashmap *, size_t);
void hashtab_reinit(hashmap *, Error *);
void hashtab_add(hashmap *, char *, Error *error);
void hashtab_delete(hashmap *, char *);
void hashtab_free(hashmap *);
listnode *hashtab_lookup( hashmap, char *, unsigned long long*, unsigned long long*, unsigned long long *);
void hashtab_delete_all(hashmap *, char );

#endif /* hashmap_h */
