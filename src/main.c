#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "priority_queue.h"
#include "map.h"
#include "a_star.h"

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
    {0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0},
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
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
};

Point point_from(int x, int y) {
    Point p = {.x = x, .y = y};
    return p;
}

// Test the priority queue
int test_queue() {
    priority_queue pq;
    priority_queue_init(&pq);

    if (priority_queue_push(&pq, 5, point_from(5, 6)) != PQ_SUCCESS) {
        printf("Error: Could not push element (queue may be full).\n");
    }
    if (priority_queue_push(&pq, 3, point_from(6, 6)) != PQ_SUCCESS) {
        printf("Error: Could not push element (queue may be full).\n");
    }
    if (priority_queue_push(&pq, 8, point_from(8, 8)) != PQ_SUCCESS) {
        printf("Error: Could not push element (queue may be full).\n");
    }
    if (priority_queue_push(&pq, 1, point_from(8, 9)) != PQ_SUCCESS) {
        printf("Error: Could not push element (queue may be full).\n");
    }

    heap_node top_node;
    if (priority_queue_peek(&pq, &top_node) == PQ_SUCCESS) {
        printf(
            "Peek: Priority = %d, Data = (%d,%d)\n",
            top_node.priority,
            top_node.data.x,
            top_node.data.y
        );
    } else {
        printf("Error: Could not peek element (queue may be empty).\n");
    }

    while (!priority_queue_is_empty(&pq)) {
        if (priority_queue_pop(&pq, &top_node) == PQ_SUCCESS) {
            printf(
                "Pop: Priority = %d, Data = (%d,%d)\n",
                top_node.priority,
                top_node.data.x,
                top_node.data.y
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
    if (fixed_size_map_set(&map, "key1", 10) == MAP_SUCCESS) {
        printf("Set key1 to 10\n");
    }
    if (fixed_size_map_set(&map, "key2", 20) == MAP_SUCCESS) {
        printf("Set key2 to 20\n");
    }

    // Attempt to retrieve values
    MAP_VALUE_TYPE value;
    if (fixed_size_map_get(&map, "key1", &value) == MAP_SUCCESS) {
        printf("Get key1: %d\n", value);
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

// Example usage
int test_a_star() {
    Point start = {1, 1};
    Point goal = {29, 26};
    Path path;
    
    a_star_err result = a_star(start, goal, FIELD_ROWS, FIELD_COLS, field, &path);
    
    switch (result) {
        case ASTAR_SUCCESS:
            printf("Path found! Length: %d\n", path.length);
            for (int i = 0; i < path.length; i++) {
                printf("(%d,%d) ", path.points[i].x, path.points[i].y);
            }
            printf("\n");
            break;
        case ASTAR_NO_PATH_FOUND:
            printf("No path found\n");
            break;
        case ASTAR_PATH_TOO_LONG:
            printf("Path exceeded maximum length\n");
            break;
        case ASTAR_INVALID_START:
            printf("Invalid start position\n");
            break;
        case ASTAR_INVALID_GOAL:
            printf("Invalid goal position\n");
            break;
        case ASTAR_OUT_OF_MEMORY:
            printf("Out of memory error\n");
            break;
        case ASTAR_MAP_ERROR:
            printf("Map error occurred\n");
            break;
    }
    
    return 0;
}

int main() {
    test_queue();
    test_map();
    test_a_star();
}