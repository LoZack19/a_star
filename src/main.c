#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "priority_queue.h"
#include "map.h"
#include "a_star.h"

field_t field = {
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
    Point p = {.column = x, .row = y};
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
            top_node.data.column,
            top_node.data.row
        );
    } else {
        printf("Error: Could not peek element (queue may be empty).\n");
    }

    while (!priority_queue_is_empty(&pq)) {
        if (priority_queue_pop(&pq, &top_node) == PQ_SUCCESS) {
            printf(
                "Pop: Priority = %d, Data = (%d,%d)\n",
                top_node.priority,
                top_node.data.column,
                top_node.data.row
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

void print_field_with_path(const field_t grid, const Path* path) {
    // Create a copy of the grid to mark the path
    field_t display_grid;
    memcpy(display_grid, grid, FIELD_ROWS * FIELD_COLS * sizeof(int));
    
    // Mark path on the grid copy with '2'
    if (path->length > 0) {
        for (int i = 0; i < path->length; i++) {
            Point p = path->points[i];
            display_grid[p.row][p.column] = 2;
        }
    }
    
    // Print the grid with appropriate characters
    for (int y = 0; y < FIELD_ROWS; y++) {
        for (int x = 0; x < FIELD_COLS; x++) {
            switch (display_grid[y][x]) {
                case CELL_PATH:  // Empty path
                    printf("  ");
                    break;
                case CELL_WALL:  // Wall
                    printf("██");
                    break;
                case 2:          // Path found
                    printf("••");
                    break;
                default:
                    printf("??");
            }
        }
        printf("\n");
    }
}

int test_a_star() {
    Point start = {1, 1};
    Point goal = {26, 29};
    Path path;
    
    // First print the original maze
    printf("\nOriginal Maze:\n");
    print_field_with_path(field, &(Path){.length = 0});
    
    // Find the path
    a_star_err result = a_star(start, goal, field, &path);
    
    // Print results
    printf("\nA* Result: ");
    switch (result) {
        case ASTAR_SUCCESS:
            printf("Path found! Length: %d\n\n", path.length);
            print_field_with_path(field, &path);
            
            printf("\nPath coordinates: ");
            for (int i = 0; i < path.length; i++) {
                printf("(%d,%d) ", path.points[i].column, path.points[i].row);
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
    puts(" --- TEST QUEUE --- ");
    test_queue();
    puts(" --- TEST MAP --- ");
    test_map();
    puts(" --- TEST A STAR --- ");
    test_a_star();
}