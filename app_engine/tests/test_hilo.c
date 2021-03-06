#include "common.h"

#include <stdio.h>

static void test_edit() {
    test("g1", "1__       ");
    test("g12", "12_       ");
    test("g0", "___ GUESS ");
    test("g012", "12_       ");
    test("g1g", "___       ");
    test("g12g", "1__       ");
}

static void test_hi_and_lo() {
    test("g744", "744 TOO LO");
    test("g746", "746 TOO HI");
}

static void test_next_hilo_different_target() {
    test("g745", "745 YOU WON");
    test("gg745", "745 TOO HI");
}

static void test_animation_on_enter() {
    test_t *test = init_test(0);
    press_key(test, 'g');
    advance(test, 10);
    assert_display(test, "> HI-LO GAME");
    advance(test, 500);
    assert_display(test, "> HI-LO GAME");
    advance(test, 2000);
    assert_display(test, "___ GUESS ");
}

static void test_animation_on_reenter() {
    test_t *test = init_test(0);
    press_sequence(test, "gc");
    press_key(test, 'g');
    advance(test, 10);
    assert_display(test, "> HI-LO GAME");
    advance(test, 500);
    assert_display(test, "> HI-LO GAME");
    advance(test, 2000);
    assert_display(test, "___ GUESS ");
}

static void test_animation_on_clear() {
    test_t *test = init_test(0);
    press_sequence(test, "g");
    press_key(test, 'g');
    advance(test, 2000);
    assert_display(test, "___ GUESS ");
}

static void test_animation_on_last_guess() {
    test_t *test = init_test(0);
    press_sequence(test, "g10010010010010010010010010");
    press_key(test, '0');
    advance(test, 500);
    assert_display(test, "1 MORE GUESS");
    advance(test, 1000);
    assert_display(test, "100 TOO LO");
}

static void test_animation_on_win() {
    test_t *test = init_test(0);
    press_sequence(test, "g74");
    press_key(test, '5');
    advance(test, 10);
    assert_display(test, "745       ");
    advance(test, 500);
    assert_display(test, "1 GUESS");
    advance(test, 2000);
    assert_display(test, "745 YOU WON");
}

static void test_animation_on_lose() {
    test_t *test = init_test(0);
    press_sequence(test, "g10010010010010010010010010010");
    press_key(test, '0');
    advance(test, 10);
    assert_display(test, "100       ");
    advance(test, 500);
    assert_display(test, "745 YOU LOST");
}

static void test_show_stats() {
    test_t *test = init_test(0);
    press_sequence(test, "g555");
    assert_display(test, "555 TOO LO");
    press_key(test, '%');
    advance(test, 500);
    assert_display(test, "1 GUESS");
    advance(test, 1000);
    assert_display(test, "555 TOO LO");
}

void test_hilo() {
    test_edit();
    test_hi_and_lo();
    test_next_hilo_different_target();
    test_animation_on_enter();
    test_animation_on_reenter();
    test_animation_on_clear();
    test_animation_on_last_guess();
    test_animation_on_win();
    test_animation_on_lose();
    test_show_stats();
}
