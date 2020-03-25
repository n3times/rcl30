#include "penta1000_internal.h"

#include "log.h"

#include <stdlib.h>
#include <string.h>

static app_t *get_current_app(p1_t *p1) {
    app_t *app = NULL;
    if (p1->current_app_type == APP_TYPE_CALC) {
        app = (app_t *)&p1->calc;
    } else if (p1->current_app_type == APP_TYPE_GAME) {
        app = (app_t *)&p1->game;
    }
    return app;
}

/* Implementation of the client interface. */

/* Core */

p1_t *p1_new(long seed) {
    p1_t *p1 = malloc(sizeof(p1_t));
    p1->current_app_type = APP_TYPE_NONE;
    init_calc(&p1->calc);
    init_game(&p1->game, seed);
    return p1;
}

void p1_release(p1_t *p1) {
    free(p1);
}

void p1_press_key(p1_t *p1, char key) {
    app_t *app = get_current_app(p1);
    if (!app) {
        p1->current_app_type = APP_TYPE_CALC;
        app = get_current_app(p1);
        // Bypass enter animation.
        app->enter(app);
        while (app->is_animating(app)) {
            app->advance_frame(app);
        }
    }
    if (key == 'g') {
        if (app == (app_t *)&p1->calc) {
            p1->current_app_type = APP_TYPE_GAME;
        } else {
            p1->current_app_type = APP_TYPE_CALC;
        }
        app = get_current_app(p1);
        app->enter(app);
    } else {
        app->press_key(app, key);
    }
}

char *p1_get_display(p1_t *p1) {
    app_t *app = get_current_app(p1);
    if (!app) return "PENTATRONICS";
    return app->get_display(app);
}

/* Animation */

void p1_advance_frame(p1_t *p1) {
    app_t *app = get_current_app(p1);
    if (app) app->advance_frame(app);
}

bool p1_is_animating(p1_t *p1) {
    app_t *app = get_current_app(p1);
    return app ? app->is_animating(app) : false;
}

/* State */

long p1_get_state_size(p1_t *p1) {
    return sizeof(p1_t);
}

char *p1_get_state(p1_t *p1) {
    long size = sizeof(p1_t);
    void *state = malloc(size);
    memcpy(state, p1, size);
    return (char *)state;
}

void p1_release_state(char *state) {
    free(state);
}

p1_t *p1_new_from_state(char *state) {
    long size = sizeof(p1_t);
    p1_t *p1 = malloc(size);
    memcpy(p1, state, size);
    init_calc_from_state((char *)&p1->calc);
    init_game_from_state((char *)&p1->game);
    return p1;
}

/* Logging */

long p1_log_get_first_available_index(p1_t *p1) {
    log_t *log = &p1->calc.log;
    long first;
    log_get_available_interval(log, &first, NULL);
    return first;
}

long p1_log_get_last_available_index(p1_t *p1) {
    log_t *log = &p1->calc.log;
    long last;
    log_get_available_interval(log, NULL, &last);
    return last;
}

char *p1_log_get_entry(p1_t *p1, long index) {
    log_t *log = &p1->calc.log;
    return log_get_entry(log, index);
}

void p1_log_clear(p1_t *p1) {
    log_t *log = &p1->calc.log;
    log_clear(log);
}