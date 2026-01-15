//
// Created by ikryxxdev on 1/14/26.
//

#ifndef DYNARR_H
#define DYNARR_H
#include <stdlib.h>

#ifndef DYNARR_INIT_CAP
#define DYNARR_INIT_CAP 256
#endif

#define dynarr_reserve(da, cap)                                                     \
    do {                                                                            \
        ((da)->capacity = cap);                                                     \
        (da)->items = realloc((da)->items, (da)->capacity*sizeof(*(da)->items));    \
        memset((da)->items, 0, (da)->capacity);                                     \
    } while(0)
#define dynarr_append(da, item)                                                             \
    do {                                                                                    \
        if ((da)->count >= (da)->capacity) {                                                \
            (da)->capacity = (da)->capacity == 0 ? DYNARR_INIT_CAP : (da)->capacity*2;      \
            (da)->items = realloc((da)->items, (da)->capacity*sizeof(*(da)->items));        \
        }                                                                                   \
                                                                                            \
        (da)->items[(da)->count++] = (item);                                                \
    } while (0)

#define dynarr_free(da) free((da)->items)

// Append several items to a dynamic array
#define dynarr_append_many(da, new_items, new_items_count)                                  \
    do {                                                                                    \
        if ((da)->count + (new_items_count) > (da)->capacity) {                             \
            if ((da)->capacity == 0) {                                                      \
                (da)->capacity = DYNARR_INIT_CAP;                                           \
            }                                                                               \
            while ((da)->count + (new_items_count) > (da)->capacity) {                      \
                (da)->capacity *= 2;                                                        \
            }                                                                               \
            (da)->items = realloc((da)->items, (da)->capacity*sizeof(*(da)->items));        \
        }                                                                                   \
        memcpy((da)->items + (da)->count, (new_items), (new_items_count)*sizeof(*(da)->items)); \
        (da)->count += (new_items_count);                                                   \
    } while (0)

#define dynarr_resize(da, new_size)                                                        \
    do {                                                                                   \
        if ((new_size) > (da)->capacity) {                                                 \
            (da)->capacity = (new_size);                                                   \
            (da)->items = realloc((da)->items, (da)->capacity * sizeof(*(da)->items));     \
        }                                                                                  \
        (da)->count = (new_size);                                                          \
    } while (0)


#endif //DYNARR_H