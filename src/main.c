#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "priority_queue.h"
#include "map.h"

#define FIELD_ROWS 31
#define FIELD_COLS 28

int field[FIELD_ROWS][FIELD_COLS] = {
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,1,1,1,1,0,1,1,1,1,1,0,1,1,0,1,1,1,1,1,0,1,1,1,1,0,1},
    {1,0,1,1,1,1,0,1,1,1,1,1,0,1,1,0,1,1,1,1,1,0,1,1,1,1,0,1},
    {1,0,1,1,1,1,0,1,1,1,1,1,0,1,1,0,1,1,1,1,1,0,1,1,1,1,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,1,1,1,1,0,1,1,0,1,1,1,1,1,1,1,1,0,1,1,0,1,1,1,1,0,1},
    {1,0,1,1,1,1,0,1,1,0,1,1,1,1,1,1,1,1,0,1,1,0,1,1,1,1,0,1},
    {1,0,0,0,0,0,0,1,1,0,0,0,0,1,1,0,0,0,0,1,1,0,0,0,0,0,0,1},
    {1,1,1,1,1,1,0,1,1,1,1,1,0,1,1,0,1,1,1,1,1,0,1,1,1,1,1,1},
    {1,1,1,1,1,1,0,1,1,1,1,1,0,1,1,0,1,1,1,1,1,0,1,1,1,1,1,1},
    {1,1,1,1,1,1,0,1,1,0,0,0,0,0,0,0,0,0,0,1,1,0,1,1,1,1,1,1},
    {1,1,1,1,1,1,0,1,1,0,1,1,1,1,1,1,1,1,0,1,1,0,1,1,1,1,1,1},
    {1,1,1,1,1,1,0,1,1,0,1,1,1,1,1,1,1,1,0,1,1,0,1,1,1,1,1,1},
    {0,0,0,0,0,0,2,0,0,0,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0},
    {1,1,1,1,1,1,0,1,1,0,1,1,1,1,1,1,1,1,0,1,1,0,1,1,1,1,1,1},
    {1,1,1,1,1,1,0,1,1,0,1,1,1,1,1,1,1,1,0,1,1,0,1,1,1,1,1,1},
    {1,1,1,1,1,1,0,1,1,0,0,0,0,0,0,0,0,0,0,1,1,0,1,1,1,1,1,1},
    {1,1,1,1,1,1,0,1,1,0,1,1,1,1,1,1,1,1,0,1,1,0,1,1,1,1,1,1},
    {1,1,1,1,1,1,0,1,1,0,1,1,1,1,1,1,1,1,0,1,1,0,1,1,1,1,1,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,1,1,1,1,0,1,1,1,1,1,0,1,1,0,1,1,1,1,1,0,1,1,1,1,0,1},
    {1,0,1,1,1,1,0,1,1,1,1,1,0,1,1,0,1,1,1,1,1,0,1,1,1,1,0,1},
    {1,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,1},
    {1,1,1,0,1,1,0,1,1,0,1,1,1,1,1,1,1,1,0,1,1,0,1,1,0,1,1,1},
    {1,1,1,0,1,1,0,1,1,0,1,1,1,1,1,1,1,1,0,1,1,0,1,1,0,1,1,1},
    {1,0,0,0,0,0,0,1,1,0,0,0,0,1,1,0,0,0,0,1,1,0,0,0,0,0,0,1},
    {1,0,1,1,1,1,1,1,1,1,1,1,0,1,1,0,1,1,1,1,1,1,1,1,1,1,0,1},
    {1,0,1,1,1,1,1,1,1,1,1,1,0,1,1,0,1,1,1,1,1,1,1,1,1,1,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3,1},
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
};

// Test the priority queue
int test_queue() {
    priority_queue pq;
    priority_queue_init(&pq);

    if (priority_queue_push(&pq, 5, 100.5) != PQ_SUCCESS) {
        printf("Error: Could not push element (queue may be full).\n");
    }
    if (priority_queue_push(&pq, 3, 200.25) != PQ_SUCCESS) {
        printf("Error: Could not push element (queue may be full).\n");
    }
    if (priority_queue_push(&pq, 8, 300.75) != PQ_SUCCESS) {
        printf("Error: Could not push element (queue may be full).\n");
    }
    if (priority_queue_push(&pq, 1, 400.125) != PQ_SUCCESS) {
        printf("Error: Could not push element (queue may be full).\n");
    }

    heap_node top_node;
    if (priority_queue_peek(&pq, &top_node) == PQ_SUCCESS) {
        printf(
            "Peek: Priority = %d, Data = %.3f\n",
            top_node.priority,
            top_node.data
        );
    } else {
        printf("Error: Could not peek element (queue may be empty).\n");
    }

    while (!priority_queue_is_empty(&pq)) {
        if (priority_queue_pop(&pq, &top_node) == PQ_SUCCESS) {
            printf(
                "Pop: Priority = %d, Data = %.3f\n",
                top_node.priority,
                top_node.data
            );
        } else {
            printf("Error: Could not pop element (queue may be empty).\n");
        }
    }

    return 0;
}

// Test the fixed-size map
int test_map() {
    fixed_size_map map;
    fixed_size_map_init(&map);

    // Add some key-value pairs
    if (fixed_size_map_set(&map, "key1", 10.5) == MAP_SUCCESS) {
        printf("Set key1 to 10.5\n");
    }
    if (fixed_size_map_set(&map, "key2", 20.25) == MAP_SUCCESS) {
        printf("Set key2 to 20.25\n");
    }

    // Attempt to retrieve values
    MAP_VALUE_TYPE value;
    if (fixed_size_map_get(&map, "key1", &value) == MAP_SUCCESS) {
        printf("Get key1: %.2f\n", value);
    } else {
        printf("Error: key1 not found\n");
    }

    if (fixed_size_map_get(&map, "key3", &value) == MAP_KEY_NOT_FOUND) {
        printf("Error: key3 not found\n");
    }

    // Attempt to remove a key
    if (fixed_size_map_remove(&map, "key1") == MAP_SUCCESS) {
        printf("Removed key1\n");
    }

    if (fixed_size_map_get(&map, "key1", &value) == MAP_KEY_NOT_FOUND) {
        printf("Error: key1 not found after removal\n");
    }

    return 0;
}

int main() {
    test_queue();
    test_map();
}