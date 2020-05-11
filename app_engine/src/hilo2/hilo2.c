#include "hilo2.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static char GAME_KEY = 'h';
static char STATS_KEY = '%';

static void enter(app_t *app);
static void press_key(app_t *app, char key);
static char *get_display(app_t *app);
static void advance_frame(app_t *app);
static bool is_animating(app_t *app);

static void enter_start_level(hilo2_t *hilo2);

static void setup_app_methods(hilo2_t *hilo2) {
    hilo2->app.enter = enter;
    hilo2->app.press_key = press_key;
    hilo2->app.get_display = get_display;
    hilo2->app.advance_frame = advance_frame;
    hilo2->app.is_animating = is_animating;
}

void init_hilo2(hilo2_t *hilo2, long seed) {
    memset(hilo2, 0, sizeof(hilo2_t));
    setup_app_methods(hilo2);
    hilo2->rng = (int)(seed >= 0 ? seed : -seed);
}

void init_hilo2_from_state(char *state) {
    setup_app_methods((hilo2_t *)state);
}

static int get_random_target(hilo2_t *hilo2) {
    hilo2->rng = (1103515245 * hilo2->rng + 12345) % (1 << 31);
    return (int) (hilo2->rng % 900 + 100);
}

static void push_state(hilo2_t *hilo2) {
    hilo2->pushed_state = hilo2->state;
    hilo2->pushed_frame = hilo2->frame;
    strcpy(hilo2->pushed_display, hilo2->display);
}

static void pop_state(hilo2_t *hilo2) {
    hilo2->state = hilo2->pushed_state;
    hilo2->frame = hilo2->pushed_frame;
    strcpy(hilo2->display, hilo2->pushed_display);
}

static void set_state(hilo2_t *hilo2, hilo2_state_t state, int frame) {
    hilo2->state = state;
    hilo2->frame = frame;
}

static void enter_start_game(hilo2_t *hilo2) {
    hilo2->level = 0;
    hilo2->score = 0;
    set_state(hilo2, HILO2_STATE_START_GAME, 0);
    enter_start_level(hilo2);
}

static void enter_start_level(hilo2_t *hilo2) {
    hilo2->target = get_random_target(hilo2);
    hilo2->guess = 0;
    hilo2->index = 0;
    hilo2->level += 1;
    hilo2->max_guesses = 10;
    int max_times[] = {30, 25, 20, 25, 20, 18, 20, 18, 15, 14,
                       13, 12, 12, 12, 11, 11, 11, 10, 10, 10};
    int max_guesses[] = {12, 12, 12, 11, 11, 11, 10, 10, 10, 10,
                         10, 10, 10, 10, 10, 10, 10, 10, 10, 10};
    hilo2->max_time = max_times[hilo2->level - 1] * 100;
    hilo2->max_guesses = max_guesses[hilo2->level - 1];

    set_state(hilo2, HILO2_STATE_START_LEVEL, 0);
    sprintf(hilo2->display, "LEVEL %02d }", hilo2->level);
}

static void enter_won_level(hilo2_t *hilo2) {
    hilo2->time = hilo2->frame;
    set_state(hilo2, HILO2_STATE_WON_LEVEL, 0);
    hilo2->score += 50 - (hilo2->time + 99) / 100;
    if (hilo2->score > hilo2->hiscore) hilo2->hiscore = hilo2->score;
    sprintf(hilo2->display, "%d NICE }", hilo2->target);
}

static void enter_game_over(hilo2_t *hilo2) {
    set_state(hilo2, HILO2_STATE_GAME_OVER, 0);
    sprintf(hilo2->display, "%d YOU LOST", hilo2->target);
}

static void enter_sub_last_guess(hilo2_t *hilo2) {
    push_state(hilo2);
    set_state(hilo2, HILO2_STATE_SUBROUTINE_LAST_GUESS, 0);
    sprintf(hilo2->display, "LAST GUESS }");
}

static void enter_sub_flash(hilo2_t *hilo2) {
    push_state(hilo2);
    set_state(hilo2, HILO2_STATE_SUBROUTINE_FLASH, 0);
    sprintf(hilo2->display, "");
}

static void enter_sub_stats(hilo2_t *hilo2) {
    push_state(hilo2);
    set_state(hilo2, HILO2_STATE_SUBROUTINE_STATS, 0);
    sprintf(hilo2->display, "SCORE %03d", hilo2->score);
}

/** Implementation of the app interface. */

static void enter(app_t *app) {
    hilo2_t *hilo2 = (hilo2_t *)app;
    sprintf(hilo2->display, "> HI-LO GAME");
    set_state(hilo2, HILO2_STATE_ENTER, 0);
}

static void press_key(app_t *app, char key) {
    hilo2_t *hilo2 = (hilo2_t *)app;

    if (is_animating(app) && hilo2->state != HILO2_STATE_PLAY_LEVEL) return;

    int is_digit = '0' <= key && key <= '9';

    if (key == STATS_KEY) {
        enter_sub_stats(hilo2);
        return;
    }

    if (hilo2->state == HILO2_STATE_WON_LEVEL) {
        enter_start_level(hilo2);
        return;
    }

    if (key == GAME_KEY) {
        enter_start_game(hilo2);
        return;
    }

    if (hilo2->state == HILO2_STATE_GAME_OVER) {
        enter_sub_flash(hilo2);
        return;
    }

    if (hilo2->state == HILO2_STATE_START_LEVEL) {
        set_state(hilo2, HILO2_STATE_PLAY_LEVEL, 0);
    }

    // HILO2_STATE_PLAY_LEVEL:

    bool ignore_key = !is_digit;
    if (key == '0' && hilo2->guess_textfield[0] == '_') {
        ignore_key = true;
    }
    if (ignore_key) {
        enter_sub_flash(hilo2);
        return;
    }

    int i = 0;
    for (i = 0; i < 3; i++) {
        if (hilo2->guess_textfield[i] == '_') break;
    }
    if (i > 0 || key != '0') {
        hilo2->guess_textfield[i] = key;
        if (i == 2) {
            hilo2->guess = atoi(hilo2->guess_textfield);
            hilo2->index++;
        }
    }
    if (i == 2) {
        if (hilo2->guess == hilo2->target) {
            enter_won_level(hilo2);
            return;
        } else if (hilo2->index >= hilo2->max_guesses) {
            enter_game_over(hilo2);
            return;
        } else if (hilo2->index == hilo2->max_guesses - 1) {
            enter_sub_last_guess(hilo2);
            return;
        }
        bool high = (hilo2->guess - hilo2->target) > 0;
        sprintf(hilo2->display, "%03d %s  ",
                hilo2->guess, high ? "TOO HI" : "TOO LO");
        strcpy(hilo2->guess_textfield, "___");
    } else {
        sprintf(hilo2->display, "%s         ", hilo2->guess_textfield);
    }
    int time_left = hilo2->max_time - hilo2->frame;
    if (time_left <= 999) {
        hilo2->display[strlen(hilo2->display) - 1] = '0' + time_left / 100;
    }
}

static char *get_display(app_t *app) {
    hilo2_t *hilo2 = (hilo2_t *)app;
    return hilo2->display;
}

static void advance_frame(app_t *app) {
    hilo2_t *hilo2 = (hilo2_t *)app;

    hilo2->frame++;

    switch (hilo2->state) {
    case HILO2_STATE_ENTER:
        if (hilo2->frame == 100) enter_start_game(hilo2);
        break;
    case HILO2_STATE_START_GAME:
        break;
    case HILO2_STATE_START_LEVEL:
        if (hilo2->frame == 150) {
            sprintf(hilo2->display, "%02d GUESSES }", hilo2->max_guesses);
        }
        if (hilo2->frame == 300) {
            sprintf(hilo2->display, "%02d SECONDS }", hilo2->max_time / 100);
        }
        if (hilo2->frame == 450) {
            strcpy(hilo2->guess_textfield, "___");
            sprintf(hilo2->display, "___ GUESS %02d", hilo2->max_time / 100);
            hilo2->frame = -1;
        }
        break;
    case HILO2_STATE_PLAY_LEVEL:
        {
            int time_left = hilo2->max_time - hilo2->frame;
            if (time_left == 0) {
                enter_game_over(hilo2);
            } else if (time_left <= 999 && time_left % 100 == 99) {
                hilo2->display[strlen(hilo2->display) - 1] = '0' + time_left / 100;
            }
            break;
        }
    case HILO2_STATE_WON_LEVEL:
        if (hilo2->frame == 200) {
            sprintf(hilo2->display, "%d.%d SECS }", hilo2->time / 100, hilo2->time % 10);
        } else if (hilo2->frame == 400) {
            sprintf(hilo2->display, " SCORE %03d }", hilo2->score);
        } else if (hilo2->frame == 600) {
            hilo2->frame = -1;
            sprintf(hilo2->display, "ANY KEY");
        }
        break;
    case HILO2_STATE_GAME_OVER:
        if (hilo2->frame == 200) {
            sprintf(hilo2->display, "GAME OVER }");
        } else if (hilo2->frame == 400) {
            if (hilo2->score >= hilo2->hiscore) {
                hilo2->hiscore = hilo2->score;
                sprintf(hilo2->display, "HI-SCORE %03d", hilo2->score);
            } else {
                sprintf(hilo2->display, "SCORE %03d", hilo2->score);
            }
            hilo2->frame = -1;
        }
        break;
    case HILO2_STATE_SUBROUTINE_LAST_GUESS:
        if (hilo2->frame == 100) {
            pop_state(hilo2);
            bool high = (hilo2->guess - hilo2->target) > 0;
            sprintf(hilo2->display, "%03d %s  ",
                    hilo2->guess, high ? "TOO HI" : "TOO LO");
            strcpy(hilo2->guess_textfield, "___");
        }
        break;
    case HILO2_STATE_SUBROUTINE_FLASH:
        if (hilo2->frame == 5) pop_state(hilo2);
        break;
    case HILO2_STATE_SUBROUTINE_STATS:
        if (hilo2->frame == 120) {
            sprintf(hilo2->display, "HI-SCORE %03d", hilo2->hiscore);
        } else if (hilo2->frame == 240) {
            pop_state(hilo2);
        }
        break;
    }
}

static bool is_animating(app_t *app) {
    hilo2_t *hilo2 = (hilo2_t *)app;
    return hilo2->frame >= 0;
}