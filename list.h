//
// Created by bernd on 03.01.21.
//

#ifndef BRS_LIST_H
#define BRS_LIST_H

#include <stddef.h>
#include <stdlib.h>

#define BRS_LIST_DECL(LIST_NAME, VALUE_TYPE) \
typedef struct _##LIST_NAME##Entry LIST_NAME##Entry; \
struct _##LIST_NAME##Entry { \
    VALUE_TYPE *value; \
    LIST_NAME##Entry *next; \
}; \
typedef struct _##LIST_NAME LIST_NAME; \
struct _##LIST_NAME { \
    LIST_NAME##Entry *firstEntry; \
    LIST_NAME##Entry *lastEntry; \
};                                           \
LIST_NAME *LIST_NAME##_create();              \
LIST_NAME##Entry *LIST_NAME##Entry_create(VALUE_TYPE *value); \
LIST_NAME *LIST_NAME##_push(VALUE_TYPE *val, LIST_NAME *list); \

#define BRS_LIST_DEFN(LIST_NAME, VALUE_TYPE) \
LIST_NAME *LIST_NAME##_create() { \
    LIST_NAME *list = malloc(sizeof(LIST_NAME));\
    list->firstEntry = NULL;\
    list->lastEntry = NULL;\
    return list;\
}                                            \
LIST_NAME##Entry *LIST_NAME##Entry_create(VALUE_TYPE *value) { \
    LIST_NAME##Entry *entry = malloc(sizeof(LIST_NAME##Entry)); \
    entry->value = value; \
    entry->next = NULL; \
    return entry; \
}               \
                                             \
LIST_NAME *LIST_NAME##_push(VALUE_TYPE *val, LIST_NAME *list) {     \
    LIST_NAME##Entry *newEntry = LIST_NAME##Entry_create(val); \
    if (list->lastEntry == NULL) { \
    list->firstEntry = newEntry; \
    } else { \
    list->lastEntry->next = newEntry; \
    }                                        \
    list->lastEntry = newEntry;                                       \
}  \

#endif //BRS_LIST_H
