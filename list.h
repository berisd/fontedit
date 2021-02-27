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
    VALUE_TYPE *value;                       \
    uint32_t index;                                         \
    LIST_NAME##Entry *next; \
}; \
typedef struct _##LIST_NAME LIST_NAME; \
struct _##LIST_NAME { \
    LIST_NAME##Entry *firstEntry; \
    LIST_NAME##Entry *lastEntry;             \
    size_t size;                              \
};                                           \
typedef void (*LIST_NAME##_ValueConsumer)(VALUE_TYPE *, void *context); \
LIST_NAME *LIST_NAME##_create();             \
void LIST_NAME##_destroy(LIST_NAME *list); \
LIST_NAME##Entry *LIST_NAME##Entry_create(VALUE_TYPE *value); \
LIST_NAME *LIST_NAME##_push(VALUE_TYPE *val, LIST_NAME *list); \
void LIST_NAME##_iterate(LIST_NAME *list, LIST_NAME##_ValueConsumer consumer, void *context); \

#define BRS_LIST_DEFN(LIST_NAME, VALUE_TYPE) \
LIST_NAME *LIST_NAME##_create() { \
    LIST_NAME *list = malloc(sizeof(LIST_NAME));\
    list->firstEntry = NULL;\
    list->lastEntry = NULL;                  \
    list->size = 0;                          \
    return list;\
}                                            \
                                             \
void LIST_NAME##_destroy(LIST_NAME *list) { \
    if (list == NULL)                        \
        return;                              \
        \
    LIST_NAME##Entry *entry = list->firstEntry; \
    while (entry != NULL) { \
        LIST_NAME##Entry *tmp = entry; \
        entry = entry->next; \
        free(tmp); \
    } \
    free(list);                                            \
}                                            \
                                             \
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
    newEntry->index = list->size;                                         \
    list->size++;                                             \
}  \
                                             \
void LIST_NAME##_iterate(LIST_NAME *list, LIST_NAME##_ValueConsumer consumer, void *context) { \
    if (list == NULL)                        \
        return;                              \
        \
    LIST_NAME##Entry *entry = list->firstEntry; \
    while (entry != NULL) { \
        VALUE_TYPE *value = entry->value;    \
        consumer(value, context);                                     \
        entry = entry->next; \
    } \
}                                            \

#endif //BRS_LIST_H
