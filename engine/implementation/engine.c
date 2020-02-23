#include "engine_internal.h"

#include <stdlib.h>
#include <string.h>

void new_state(state_t **state) {
    *state = malloc(sizeof(state_t));
    memset(*state, 0, sizeof(state_t));
    (*state)->is_new = 1;
}

void release_state(state_t *state) {
    free(state);
}
