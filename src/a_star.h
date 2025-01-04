#ifndef A_STAR_H
#define A_STAR_H

#include "point.h"

#define MAX_PATH_LENGTH 1024
#define INFINITY_COST 99999.0
#define MOVE_COST 1.0

#define MAX_PATH_LENGTH 1024
#define INFINITY_COST 99999.0
#define MOVE_COST 1.0

// Grid cell types
#define CELL_PATH 0
#define CELL_WALL 1

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
typedef struct {
    Point points[MAX_PATH_LENGTH];
    int length;
} Path;

a_star_err a_star(Point start, Point goal, int rows, int cols,
                  const int grid[rows][cols], Path *out_path);

#endif