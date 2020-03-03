#include "engine_internal.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_DIGITS_NUM 10

static void x_to_d(char *formatted, double x, int len);
static char get_op_char(key_t key);

static void get_calc_display(calc_t *calc, char *display) {
    comp_t *comp = &calc->comp;
    char extended_display[100];
    memset(extended_display, 0, 100);
    aos_t *aos = &comp->aos;

    if (calc->is_new) {
        strcpy(extended_display, "PENTATRONICS");
    } else if (comp->error == ERROR_ILLEGAL_OP) {
        strcpy(extended_display, "ILLEGAL OP");
    } else if (comp->error == ERROR_OVERFLOW) {
        strcpy(extended_display, "OVERFLOW");
    } else {
        if (aos->stack_depth >= 1) {
            char number[30];
            x_to_d(number, aos->number_1, MAX_DIGITS_NUM);
            strcat(extended_display, number);
        }
        if (aos->stack_depth >= 2) {
            char op = get_op_char(aos->op_1);
            char op_string[2];
            op_string[0] = op;
            op_string[1] = 0;
            strcat(extended_display, op_string);
        }
        if (aos->stack_depth >= 3) {
            printf("===>\n");
            char number[30];
            x_to_d(number, aos->number_2, MAX_DIGITS_NUM);
            strcat(extended_display, number);
        }
        if (aos->stack_depth >= 4) {
            char op = get_op_char(aos->op_2);
            char op_string[2];
            op_string[0] = op;
            op_string[1] = 0;
            strcat(extended_display, op_string);
        }
        if (comp->is_number_editing) {
            strcat(extended_display, comp->number_editing);
        }
    }
    if (extended_display[0] == '\0') {
        strcpy(extended_display, "RDY");
    }
    int offset = strlen(extended_display) - 12;
    if (offset < 0) {
        offset = 0;
    }
    strcpy(display, extended_display + offset);
}

char *get_display(calc_t *calc) {
    if (!calc->is_game) {
        get_calc_display(calc, calc->display);
    }
    return calc->display;
}

static void x_to_d(char *formatted, double x, int len) {
    char s[200];
    char f[5];
    char *result = s;
    int is_negative = x < 0;
    if (is_negative) x = -x;
    if (x == 0) {
        result = "0";
    } else {
        int is_big = 0;
        sprintf(s, "%.0f", x);
        is_big = (strlen(s) > len);

        int is_small = 0;
        sprintf(f, "%%.%df", len - 1);
        sprintf(s, f, x);
        is_small = !strcmp(s, "0.000000000");

        if (is_big || is_small) {
            sprintf(s, "%.6e", x);
        } else {
            sprintf(s, "%.0f", x);
            sprintf(f, "%%.%luf", len - strlen(s));
            sprintf(s, f, x);
            if (strchr(s, '.')) {
                int i = strlen(s);
                while (s[i - 1] == '0') {
                    s[i - 1] = 0;
                    i--;
                }
                if (s[i - 1] == '.') s[i - 1] = 0;
            }
        }
        if (is_negative) {
            memmove(s + 1, s, strlen(s) + 1);
            s[0] = '-';
        }
    }
    strcpy(formatted, result);
}

static char get_op_char(key_t key) {
    char *ops = "+-*/";
    return ops[key - KEY_PLUS];
}
