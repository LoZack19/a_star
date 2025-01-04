#ifndef PRIORITY_QUEUE_H
#define PRIORITY_QUEUE_H

#include <stdbool.h>

#include "extra.h"
#define PRIORITY_QUEUE_DATA_TYPE position_t
#define HEAP_SIZE 1024

enum priority_queue_err {
    PQ_SUCCESS,
    PQ_FULL,
    PQ_EMPTY
};

typedef struct {
    int priority; // smaller is higher
    PRIORITY_QUEUE_DATA_TYPE data;
} heap_node;

typedef struct {
    heap_node heap[HEAP_SIZE];
    int size;
} priority_queue;

// Function prototypes
void priority_queue_init(priority_queue *pq);
bool priority_queue_is_empty(priority_queue *pq);
bool priority_queue_is_full(priority_queue *pq);

enum priority_queue_err priority_queue_push(
    priority_queue *pq,
    int priority,
    PRIORITY_QUEUE_DATA_TYPE data
);
enum priority_queue_err priority_queue_pop(
    priority_queue *pq,
    heap_node *out_node
);
enum priority_queue_err priority_queue_peek(
    priority_queue *pq,
    heap_node *out_node
);

#endif