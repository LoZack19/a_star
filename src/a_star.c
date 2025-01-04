#include "a_star.h"
#include "map.h"
#include "priority_queue.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Convert point to string for map key
static void point_to_key(Point p, char *key) {
    snprintf(key, KEY_MAX_LENGTH, "%d,%d", p.x, p.y);
}

// Manhattan distance heuristic
static double heuristic(Point a, Point b) {
    return abs(a.x - b.x) + abs(a.y - b.y);
}

// Check if a point is within grid bounds and walkable
static bool is_valid_point(Point p, int rows, int cols,
                           const int grid[rows][cols]) {
    return p.x >= 0 && p.x < cols && p.y >= 0 && p.y < rows &&
           grid[p.y][p.x] == CELL_PATH; // Explicitly check for path cell
}

// Get valid neighbors of a point
static a_star_err get_neighbors(Point p, int rows, int cols,
                                const int grid[rows][cols], Point neighbors[4],
                                int *num_neighbors) {
    Point possible_moves[] = {
        {p.x + 1, p.y}, {p.x - 1, p.y}, {p.x, p.y + 1}, {p.x, p.y - 1}};

    *num_neighbors = 0;
    for (int i = 0; i < 4; i++) {
        if (is_valid_point(possible_moves[i], rows, cols, grid)) {
            neighbors[*num_neighbors] = possible_moves[i];
            (*num_neighbors)++;
        }
    }

    return ASTAR_SUCCESS;
}

// Store point data in priority queue
static MAP_VALUE_TYPE pack_point_data(Point p, int rows) {
    return (MAP_VALUE_TYPE)(p.x * rows + p.y);
}

// Extract point from priority queue data
static Point unpack_point_data(MAP_VALUE_TYPE data, int rows) {
    Point p;
    p.x = (MAP_VALUE_TYPE)(data / rows);
    p.y = (MAP_VALUE_TYPE)(data % rows);
    return p;
}

// Reconstruct path from the pathToNode map
static a_star_err reconstruct_path(fixed_size_map *path_map, Point current,
                                   Path *out_path, int rows) {
    char current_key[KEY_MAX_LENGTH];
    int next_point_data;

    out_path->length = 0;

    // Add end point
    out_path->points[out_path->length++] = current;

    // Keep adding points until we can't find any more in the map
    while (true) {
        point_to_key(current, current_key);
        if (fixed_size_map_get(path_map, current_key, &next_point_data) !=
            MAP_SUCCESS) {
            break;
        }

        if (out_path->length >= MAX_PATH_LENGTH) {
            return ASTAR_PATH_TOO_LONG;
        }

        current = unpack_point_data(next_point_data, rows);
        out_path->points[out_path->length++] = current;
    }

    // Reverse the path
    for (int i = 0; i < out_path->length / 2; i++) {
        Point temp = out_path->points[i];
        out_path->points[i] = out_path->points[out_path->length - 1 - i];
        out_path->points[out_path->length - 1 - i] = temp;
    }

    return ASTAR_SUCCESS;
}

// A* implementation
a_star_err a_star(Point start, Point goal, int rows, int cols,
                  const int grid[rows][cols], Path *out_path) {
    // Validate input parameters
    if (!is_valid_point(start, rows, cols, grid)) {
        return ASTAR_INVALID_START;
    }
    if (!is_valid_point(goal, rows, cols, grid)) {
        return ASTAR_INVALID_GOAL;
    }

    priority_queue open_set;
    fixed_size_map cost_map;
    fixed_size_map came_from_map;

    priority_queue_init(&open_set);
    fixed_size_map_init(&cost_map);
    fixed_size_map_init(&came_from_map);

    // Add start node
    char start_key[KEY_MAX_LENGTH];
    point_to_key(start, start_key);

    if (fixed_size_map_set(&cost_map, start_key, 0.0) != MAP_SUCCESS) {
        return ASTAR_MAP_ERROR;
    }

    if (priority_queue_push(&open_set, 0, start) != PQ_SUCCESS) {
        return ASTAR_OUT_OF_MEMORY;
    }

    while (!priority_queue_is_empty(&open_set)) {
        heap_node current_node;
        if (priority_queue_pop(&open_set, &current_node) != PQ_SUCCESS) {
            return ASTAR_MAP_ERROR;
        }

        Point current = current_node.data;
        char current_key[KEY_MAX_LENGTH];
        point_to_key(current, current_key);

        // Check if we reached the goal
        if (current.x == goal.x && current.y == goal.y) {
            return reconstruct_path(&came_from_map, current, out_path, rows);
        }

        // Get current cost
        int current_cost;
        if (fixed_size_map_get(&cost_map, current_key, &current_cost) !=
            MAP_SUCCESS) {
            return ASTAR_MAP_ERROR;
        }

        // Get neighbors
        Point neighbors[4];
        int num_neighbors;
        a_star_err err =
            get_neighbors(current, rows, cols, grid, neighbors, &num_neighbors);
        if (err != ASTAR_SUCCESS) {
            return err;
        }

        // Process each neighbor
        for (int i = 0; i < num_neighbors; i++) {
            char neighbor_key[KEY_MAX_LENGTH];
            point_to_key(neighbors[i], neighbor_key);

            double tentative_cost = current_cost + MOVE_COST;
            int neighbor_cost;
            bool update_neighbor = false;

            if (fixed_size_map_get(&cost_map, neighbor_key, &neighbor_cost) !=
                MAP_SUCCESS) {
                update_neighbor = true;
            } else if (tentative_cost < neighbor_cost) {
                update_neighbor = true;
            }

            if (update_neighbor) {
                // Update path and costs
                double neighbor_data = pack_point_data(current, rows);
                if (fixed_size_map_set(&came_from_map, neighbor_key,
                                       neighbor_data) != MAP_SUCCESS) {
                    return ASTAR_MAP_ERROR;
                }
                if (fixed_size_map_set(&cost_map, neighbor_key,
                                       tentative_cost) != MAP_SUCCESS) {
                    return ASTAR_MAP_ERROR;
                }

                double estimated_total =
                    tentative_cost + heuristic(neighbors[i], goal);

                if (priority_queue_push(&open_set, (int)(estimated_total * 100),
                                        neighbors[i]) != PQ_SUCCESS) {
                    return ASTAR_OUT_OF_MEMORY;
                }
            }
        }
    }

    return ASTAR_NO_PATH_FOUND;
}