#include <stdint.h>
#include "extra.h"

struct position position_next(struct position *size, struct position *start, enum direction direction) {
    struct position end;
    uint8_t drow = 0;
    uint8_t dcol = 0;

    switch (direction) {
        case UP: drow = size->row-1; break;
        case DOWN: drow = +1; break;
        case LEFT: dcol = size->column-1; break;
        case RIGHT: dcol = +1; break;
        case NONE: break;
    }

    end.row = (start->row + drow) % size->row;
    end.column = (start->column + dcol) % size->column;

    return end;
}