#ifndef EXTRA_H
#define EXTRA_H

#include <stddef.h>

#define FIELD_ROWS 31
#define FIELD_COLS 28

typedef int field_t[FIELD_ROWS][FIELD_COLS];

struct position {
    size_t row;
    size_t column;
};

typedef struct position position_t;

enum direction {
    NONE,
    UP,
    DOWN,
    RIGHT,
    LEFT,
};

struct position position_next(
    struct position *size, struct position *start, enum direction direction
);

#endif