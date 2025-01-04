#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "map.h"

// Private utility functions
unsigned int _hash_key(const char *key);
int _find_slot(fixed_size_map *map, const char *key);

// Initialize the map
void fixed_size_map_init(fixed_size_map *map) {
    for (int i = 0; i < MAP_SIZE; i++) {
        map->entries[i].is_occupied = false;
    }
}

// Add or update a key-value pair in the map
map_status fixed_size_map_set(fixed_size_map *map, const char *key, MAP_VALUE_TYPE value) {
    if (strlen(key) >= KEY_MAX_LENGTH) {
        return MAP_KEY_TOO_LONG;
    }

    int index = _find_slot(map, key);
    if (index == -1) {
        return MAP_FULL;
    }

    map_entry *entry = &map->entries[index];
    if (entry->is_occupied && strcmp(entry->key, key) != 0) {
        return MAP_KEY_ALREADY_EXISTS;
    }

    // Set the key-value pair in the slot
    strncpy(entry->key, key, KEY_MAX_LENGTH - 1);
    entry->key[KEY_MAX_LENGTH - 1] = '\0';
    entry->value = value;
    entry->is_occupied = true;

    return MAP_SUCCESS;
}

// Retrieve the value associated with a key
map_status fixed_size_map_get(fixed_size_map *map, const char *key, MAP_VALUE_TYPE *out_value) {
    int index = _find_slot(map, key);
    if (index == -1 || !map->entries[index].is_occupied) {
        return MAP_KEY_NOT_FOUND;
    }

    *out_value = map->entries[index].value;
    return MAP_SUCCESS;
}

// Remove a key-value pair from the map
map_status fixed_size_map_remove(fixed_size_map *map, const char *key) {
    int index = _find_slot(map, key);
    if (index == -1 || !map->entries[index].is_occupied) {
        return MAP_KEY_NOT_FOUND;
    }

    map->entries[index].is_occupied = false;
    return MAP_SUCCESS;
}

// Hash a key to an index
unsigned int _hash_key(const char *key) {
    unsigned int hash = 0;
    for (int i = 0; key[i] != '\0'; i++) {
        hash = (hash * 31 + key[i]) % MAP_SIZE;
    }
    return hash;
}

// Find an appropriate slot for a key
int _find_slot(fixed_size_map *map, const char *key) {
    unsigned int hash = _hash_key(key);
    unsigned int start_index = hash;

    // Linear probing to resolve collisions
    while (map->entries[hash].is_occupied) {
        if (strcmp(map->entries[hash].key, key) == 0) {
            return hash; // Found the matching key
        }
        hash = (hash + 1) % MAP_SIZE;
        if (hash == start_index) {
            return -1; // Map is full
        }
    }

    return hash; // Return the first available or matching slot
}