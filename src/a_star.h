#ifndef A_STAR_H
#define A_STAR_H

#include "extra.h"

#define MAX_PATH_LENGTH 1024
#define MOVE_COST 1.0

// Error handling enum
typedef enum {
    ASTAR_SUCCESS = 0,
    ASTAR_NO_PATH_FOUND,
    ASTAR_PATH_TOO_LONG,
    ASTAR_INVALID_START,
    ASTAR_INVALID_GOAL,
    ASTAR_OUT_OF_MEMORY,
    ASTAR_MAP_ERROR
} a_star_err;

// Structure to hold the path
struct path {
    position_t points[MAX_PATH_LENGTH];
    int length;
};

typedef struct path path_t;

a_star_err a_star(
    position_t start, position_t goal, const field_t grid, path_t *out_path
);

#endif