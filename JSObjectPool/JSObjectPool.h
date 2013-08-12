//
//  JSObjectPool.h
//  JSObjectPool
//
//  Created by Johan Sundhage on 2013-08-12.
//  Copyright (c) 2013 Johan Sundhage. All rights reserved.
//

#ifndef JSObjectPool_JSObjectPool_h
#define JSObjectPool_JSObjectPool_h

#include <stdbool.h>
#include "JSHashmap.h"

typedef void* (*jspool_create_func)();
typedef void* (*jspool_init_func)();
typedef void (*jspool_destroy_func)();


typedef struct jspool_item {
    void* object;
    struct jspool_item* next;
    struct jspool_item* prev;
    bool busy;
    
} jspool_item;


typedef struct jspool_t {
    jspool_item* free_items;
    jspool_item* busy_items;
    jshash_t* all_items;
    
    jspool_create_func create_func;
    jspool_destroy_func destroy_func;
    jspool_init_func init_func;
    
    unsigned int pool_size;
} jspool_t;

/*
 creates a pool object.
 @param create_func Object creation function. Should return a void* ptr
 @param init_func Initialization function (set objects' parameters to initial state)
 @param destroy_func Object destruction function
 @param psize Number of objects in pool
*/
extern jspool_t* jspool_create(jspool_create_func create_func, jspool_init_func init_func, jspool_destroy_func destroy_func, unsigned int psize);

/*
 returns a free object from the pool. if no objects are available it'll return NULL
*/
extern void* jspool_get_item(jspool_t* pool);

/*
 puts an object back to the pool and marks it as available
*/
extern void jspool_put_item(jspool_t* pool, void* object);

/*
 frees all malloced data and runs the destroy_func passen in jspool_create
*/
extern void jspool_destroy(jspool_t* pool);

#endif
