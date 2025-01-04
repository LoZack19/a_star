# A* Algorithm Implementation for Embedded Systems

This repository provides an optimized A* pathfinding implementation designed specifically for embedded systems. The implementation avoids dynamic memory allocation and uses macros to configure data structures, making it efficient for special-purpose usage.

## Features

- **No Dynamic Memory Allocation**: The A* algorithm implementation avoids dynamic memory allocation.
- **Macros for Customization**: The code uses macros to configure the key parameters, such as maximum path length, grid size, and cost values, allowing users to easily adjust the implementation to fit their needs.
- **Optimized for Embedded Systems**: The code is optimized for systems with limited processing power and memory, ensuring fast and efficient execution.

## File Overview

### 1. `a_star.h`
Contains the main A* algorithm function, along with the required data structures for holding paths and error codes.

Key Definitions:
- `MAX_PATH_LENGTH`: The maximum length of a path (set to 1024).
- `CELL_PATH`, `CELL_WALL`: Defines the types of grid cells (path and wall).
- `a_star_err`: Enum for error handling during the pathfinding process.

Main Function:
- `a_star()`: The A* algorithm function that calculates the shortest path between a start and goal point on a grid.

### 2. `map.h`
Provides the definition for a fixed-size map that allows storing key-value pairs with a limited number of entries. This is used for storing additional data that may be required by A* or other algorithms. It is implemented as an open addressing hash table.

Key Definitions:
- `MAP_SIZE`: The maximum number of entries in the map (set to 1024).
- `MAP_VALUE_TYPE`: The data type used for map values (set to `int`).
- `map_entry`: Structure for each map entry consisting of a key, value, and an occupancy flag.
- `fixed_size_map`: The map structure that holds the entries.

Functions:
- `fixed_size_map_init()`: Initializes a fixed-size map.
- `fixed_size_map_set()`, `fixed_size_map_get()`, `fixed_size_map_remove()`: Functions for setting, getting, and removing key-value pairs.

### 3. `point.h`
Defines the structure for points on the grid and the grid itself.

Key Definitions:
- `FIELD_ROWS` and `FIELD_COLS`: The dimensions of the grid (set to 31 rows and 28 columns).
- `field_t`: A two-dimensional array representing the grid.
- `Point`: A structure representing a point on the grid, consisting of a row and column.

### 4. `priority_queue.h`
Implements a priority queue (min-heap) to support the A* algorithm's open list, which stores the nodes to be processed.

Key Definitions:
- `HEAP_SIZE`: The maximum number of elements the priority queue can hold (set to 1024).
- `heap_node`: A node structure that contains a priority (cost) and a data element (a `Point`).
- `priority_queue`: The priority queue structure that holds the heap.

Functions:
- `priority_queue_init()`: Initializes the priority queue.
- `priority_queue_is_empty()`, `priority_queue_is_full()`: Functions to check if the queue is empty or full.
- `priority_queue_push()`, `priority_queue_pop()`, `priority_queue_peek()`: Functions to manage the priority queue (push, pop, peek operations).

## How to Use

### Initialization

Before using the A* algorithm, make sure to initialize the required data structures:
- The grid (`field_t`) representing the environment should be set up with `CELL_PATH` and `CELL_WALL` values.
- A `Path` structure to store the final path.
- A `priority_queue` to handle the open list during the A* execution.

### Running the A* Algorithm

To run the A* algorithm:

```c
#include "a_star.h"

// Example usage
Point start = {0, 0};  // Set the starting point
Point goal = {10, 10};  // Set the goal point
field_t grid;  // Your grid initialized with appropriate values
Path out_path;  // Path to store the result

a_star_err result = a_star(start, goal, grid, &out_path);
if (result == ASTAR_SUCCESS) {
    // Process the found path
} else {
    // Handle errors
}
```

### Error Handling

The `a_star_err` enum provides the following error codes to handle different situations:
- `ASTAR_SUCCESS`: Path found successfully.
- `ASTAR_NO_PATH_FOUND`: No path was found.
- `ASTAR_PATH_TOO_LONG`: The path exceeds the maximum allowed length.
- `ASTAR_INVALID_START`: The start point is invalid.
- `ASTAR_INVALID_GOAL`: The goal point is invalid.
- `ASTAR_OUT_OF_MEMORY`: Memory allocation failed (shouldn't happen with this implementation).
- `ASTAR_MAP_ERROR`: An error occurred with the map.

### Configuring the Grid

The grid is defined as a 2D array of `FIELD_ROWS` by `FIELD_COLS`, and each cell can either be a path (`CELL_PATH = 0`) or a wall (`CELL_WALL = 1`). Set up the grid with the appropriate values before calling the A* algorithm.

### Customizing Parameters

You can customize the following parameters by modifying the macros:
- `MAX_PATH_LENGTH`: The maximum allowed path length.
- `INFINITY_COST`: The cost representing an inaccessible point.
- `MOVE_COST`: The cost to move between adjacent cells.

## Dependencies

- The implementation uses basic C constructs and requires no external libraries.