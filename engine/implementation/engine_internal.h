#include "engine.h"

typedef struct aos_s {
    int stack_depth;
    double number_1;
    key_t op_1;
    double number_2;
    key_t op_2;
    double number_3;
} aos_t;

typedef struct game_s {
    int target;
    int index;
    int guess;
    int is_number_editing;
    char number_editing[4];
} game_t;

struct calc_s {
    int is_new;
    int is_game;
    int is_data_error;
    int is_overflow;
    int is_number_editing;
    char number_editing[25];
    aos_t aos;
    game_t game;
};

void handle_op(calc_t *calc, key_t op);
void edit_number(calc_t *calc, key_t key);
void resolve_edit_number(calc_t *calc);
