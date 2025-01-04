#include "a_star.h"
#include "map.h"
#include "extra.h"
#include "priority_queue.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Convert point to string for map key
static void point_to_key(position_t p, char *key) {
    snprintf(key, KEY_MAX_LENGTH, "%ld,%ld", p.row, p.column);
}

// Manhattan distance heuristic
static int32_t heuristic(position_t a, position_t b) {
    return abs((int)a.column - (int)b.column) + abs((int)a.row - (int)b.row);
}

// Check if a point is within grid bounds and walkable
static bool is_valid_point(position_t p, const field_t grid) {
    return p.column < FIELD_COLS
        && p.row < FIELD_ROWS
        && CAN_TRAVERSE(grid[p.row][p.column]);
}

// Get valid neighbors of a point
static a_star_err get_neighbors(
    position_t p,
    const field_t grid,
    position_t neighbors[4], int *num_neighbors
) {
    position_t size = {.row = FIELD_ROWS, .column = FIELD_COLS};
    
    position_t possible_moves[] = {
        position_next(&size, &p, UP),
        position_next(&size, &p, DOWN),
        position_next(&size, &p, LEFT),
        position_next(&size, &p, RIGHT),
    };

    *num_neighbors = 0;
    for (int i = 0; i < 4; i++) {
        if (is_valid_point(possible_moves[i], grid)) {
            neighbors[*num_neighbors] = possible_moves[i];
            *num_neighbors += 1;
        }
    }

    return ASTAR_SUCCESS;
}

// Store point data in priority queue
static MAP_VALUE_TYPE pack_point_data(position_t p) {
    return (MAP_VALUE_TYPE)(p.column * FIELD_ROWS + p.row);
}

// Extract point from priority queue data
static position_t unpack_point_data(MAP_VALUE_TYPE data) {
    position_t p;
    p.column = (MAP_VALUE_TYPE)(data / FIELD_ROWS);
    p.row = (MAP_VALUE_TYPE)(data % FIELD_ROWS);
    return p;
}

// Reconstruct path from the pathToNode map
static a_star_err reconstruct_path(
    fixed_size_map *path_map, position_t current, path_t *out_path
) {
    union {
        map_status map;
        a_star_err a_star;
    } err;

    char current_key[KEY_MAX_LENGTH];
    int next_point_data;

    out_path->length = 0;

    // Add end point
    out_path->points[out_path->length] = current;
    out_path->length += 1;

    // Keep adding points until we can't find any more in the map
    for (;;) {
        point_to_key(current, current_key);
        err.map = fixed_size_map_get(path_map, current_key, &next_point_data);
        if (err.map != MAP_SUCCESS) break;

        if (out_path->length >= MAX_PATH_LENGTH) {
            return ASTAR_PATH_TOO_LONG;
        }

        current = unpack_point_data(next_point_data);
        out_path->points[out_path->length] = current;
        out_path->length += 1;
    }

    // Reverse the path
    int half_length = out_path->length >> 1;
    for (int i = 0; i < half_length; i++) {
        position_t temp = out_path->points[i];
        out_path->points[i] = out_path->points[(out_path->length-1) - i];
        out_path->points[(out_path->length-1) - i] = temp;
    }

    return ASTAR_SUCCESS;
}

// A* implementation
a_star_err a_star(position_t start, position_t goal, const field_t grid, path_t *out_path) {
    // Validate input parameters
    if (!is_valid_point(start, grid)) {
        return ASTAR_INVALID_START;
    }
    if (!is_valid_point(goal, grid)) {
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

        position_t current = current_node.data;
        char current_key[KEY_MAX_LENGTH];
        point_to_key(current, current_key);

        // Check if we reached the goal
        if (current.column == goal.column && current.row == goal.row) {
            return reconstruct_path(&came_from_map, current, out_path);
        }

        // Get current cost
        int current_cost;
        if (fixed_size_map_get(&cost_map, current_key, &current_cost) !=
            MAP_SUCCESS) {
            return ASTAR_MAP_ERROR;
        }

        // Get neighbors
        position_t neighbors[4];
        int num_neighbors;
        a_star_err err =
            get_neighbors(current, grid, neighbors, &num_neighbors);
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
                double neighbor_data = pack_point_data(current);
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