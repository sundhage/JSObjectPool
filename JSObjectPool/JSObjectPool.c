//
//  JSObjectPool.c
//  JSObjectPool
//
//  Created by Johan Sundhage on 2013-08-12.
//  Copyright (c) 2013 Johan Sundhage. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>

#include "JSObjectPool.h"


jspool_item* _jspool_create_item(jspool_t* obj) {
    jspool_item* item = malloc(sizeof(jspool_item));
    item->busy = false;
    item->object = obj->create_func();
    item->next = NULL;
    item->prev = NULL;
    jsh_set_value_for_key(obj->all_items, item->object, item);
    return item;
}

jspool_t* jspool_create(jspool_create_func create_func, jspool_init_func init_func, jspool_destroy_func destroy_func, unsigned int psize) {

    if (psize == 0) return NULL;
    
    jspool_t* obj = malloc(sizeof(jspool_t));
    obj->busy_items = NULL;
    obj->pool_size = psize;
    obj->create_func = create_func;
    obj->init_func = init_func;
    obj->destroy_func = destroy_func;
    obj->all_items = jsh_create_num_buckets(kJSHASH_POINTER, 1024);
    
    jspool_item* first_item = _jspool_create_item(obj);
    obj->free_items = first_item;
    
    int i;
    for (i=1; i<psize; i++) {
        jspool_item* item = _jspool_create_item(obj);
        item->next = obj->free_items;
        if (item->next) item->next->prev = item;
        obj->free_items = item;
    }
    
    return obj;
}

void* jspool_get_item(jspool_t* pool) {
    // take from free items and put in busy. set busy flag
    if (pool->free_items == NULL) return NULL;
    jspool_item* free_item = pool->free_items;
    if (free_item == NULL) return NULL;
    pool->init_func(free_item->object);
    
    
    pool->free_items = free_item->next;
    if (pool->free_items)
        pool->free_items->prev =  NULL;
    
    // add to busy list.
    free_item->busy = true;
    free_item->next = pool->busy_items;
    if (pool->busy_items)
        pool->busy_items->prev = free_item;
    pool->busy_items = free_item;
    
    return free_item->object;
}

void jspool_put_item(jspool_t* pool, void* object) {
    jspool_item* item = jsh_get_value_for_key(pool->all_items, object);
    item->busy = false;
    if (item == pool->busy_items) {
        pool->busy_items = pool->busy_items->next;
        pool->busy_items->prev = NULL;
    } else {
        // remove
        if (item->next)
            item->next->prev = item->prev;
        item->prev->next = item->next;
    }
    
    item->prev = NULL;
    if (pool->free_items) {
        item->next = pool->free_items;
        item->next->prev = item;
        
    } else {
        item->next = NULL;
    }
    pool->free_items = item;
}

void jspool_destroy(jspool_t* pool) {
    jsh_destroy(pool->all_items, NULL);
    
    jspool_item* item = pool->free_items;
    while (item) {
        pool->destroy_func(item->object);
        jspool_item* temp = item->next;
        free(item);
        item = temp;
    }
    item = pool->busy_items;
    while (item) {
        pool->destroy_func(item->object);
        jspool_item* temp = item->next;
        free(item);
        item = temp;
    }
    
    free((void*)pool);
}
