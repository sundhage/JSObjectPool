//
//  main.c
//  JSObjectPool
//
//  Created by Johan Sundhage on 2013-08-12.
//  Copyright (c) 2013 Johan Sundhage. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>

#include "JSObjectPool.h"

typedef struct thedata {
    void* chunk;
    int size;
} thedata;

void* o_create() {
    thedata* data = malloc(sizeof(thedata));
    data->chunk = malloc(1024*1000);
    return data;
}

void* o_init(void* stuff) {
    ((thedata*)stuff)->size = 0;
    return stuff;
}

void o_destroy(void* stuff) {
    printf("destroying user obj\n");
    free(((thedata*)stuff)->chunk);
    free(stuff);
}

int main(int argc, const char * argv[])
{
    jspool_t* pool = jspool_create(o_create, o_init, o_destroy, 10);
    thedata* item1 = jspool_get_item(pool);
    thedata* item2 = jspool_get_item(pool);
    thedata* item3 = jspool_get_item(pool);
    thedata* item4 = jspool_get_item(pool);
    thedata* item5 = jspool_get_item(pool);
    thedata* item6 = jspool_get_item(pool);
    thedata* item7 = jspool_get_item(pool);
    printf("START GETTING\n");
    jspool_put_item(pool, item4);
    jspool_put_item(pool, item2);
    jspool_put_item(pool, item5);
    jspool_put_item(pool, item6);
    thedata* item8 = jspool_get_item(pool);
    thedata* item9 = jspool_get_item(pool);
    thedata* item10 = jspool_get_item(pool);
    thedata* item11 = jspool_get_item(pool);
    thedata* item12 = jspool_get_item(pool);
    thedata* item13 = jspool_get_item(pool);
    thedata* item14 = jspool_get_item(pool);
    
    thedata* item15 = jspool_get_item(pool);
    if (item15 == NULL) {
        jspool_put_item(pool, item1);
        item15 = jspool_get_item(pool);
    }
    thedata* item16 = jspool_get_item(pool);
    
    
    jspool_destroy(pool);
    
    // insert code here...
    printf("Hello, World!\n");
    return 0;
}

