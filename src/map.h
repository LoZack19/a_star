#ifndef MAP_H
#define MAP_H

#include <stdbool.h>

#include "extra.h"
#define MAP_VALUE_TYPE int

#define MAP_SIZE 1024
#define KEY_MAX_LENGTH 64

typedef struct {
    char key[KEY_MAX_LENGTH];
    MAP_VALUE_TYPE value;
    bool is_occupied;
} map_entry;

typedef struct {
    map_entry entries[MAP_SIZE];
} fixed_size_map;

// Enum for error handling
typedef enum {
    MAP_SUCCESS = 0,
    MAP_KEY_NOT_FOUND,
    MAP_FULL,
    MAP_KEY_ALREADY_EXISTS,
    MAP_KEY_TOO_LONG
} map_status;

// Function prototypes
void fixed_size_map_init(fixed_size_map *map);
map_status fixed_size_map_set(fixed_size_map *map, const char *key, MAP_VALUE_TYPE value);
map_status fixed_size_map_get(fixed_size_map *map, const char *key, MAP_VALUE_TYPE *out_value);
map_status fixed_size_map_remove(fixed_size_map *map, const char *key);


#endif