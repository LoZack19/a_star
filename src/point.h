#ifndef POINT_H
#define POINT_H

#define FIELD_ROWS 31
#define FIELD_COLS 28

typedef int field_t[FIELD_ROWS][FIELD_COLS];

typedef struct {
    int column;
    int row;
} Point;

#endif