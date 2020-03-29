/**
 * API for clients that want to implement a Pentatronics 1000 emulator.
 *
 * Clients should use 2 threads: one thread, typically the main thread, to
 * handle user input (key presses) and another thread to run the display
 * animations.
 *
 * The animation thread should call 'p1_advance_frame' every 10 ms as long as
 * 'p1_is_animating' is true.
 */

#include <stdbool.h>

typedef struct p1_s p1_t;

// Creates a new Pentatronics 1000 object and initializes it.
//
// After the creation, the client should update the display and start
// animations if necessary.
p1_t *p1_new(long seed);

// Releases a Pentatronics 1000 object and the resources associated to it.
void p1_release(p1_t *p1);

// Reports a key press by the user.
//
// After calling this method, clients should update the display and perform
// animations as necessary.
//
// 'key' is in '0123456789.~+-*/=%cg' which covers the 10 digits, the decimal
// point, +/- ('~'), the 4 arithmetic operations, equal, percent, clear and 'g'
// (switches between game and calc mode).
void p1_press_key(p1_t *p1, char key);

// Gets the display as a null terminated string with at most 24 characters.
//
// The text should be right justified on the display. A dot ('.') modifies the
// character just before it. There are at most 12 non-dot characters.
char *p1_get_display(p1_t *p1);

/*
 * Animation.
 */

// Advances the animation by 1 frame.
//
// The client should call this method  every 10 ms if 'p1_is_animating' is true.
// Afterward, the client should update the display.
void p1_advance_frame(p1_t *p1);

// Returns true if 'p1_advance_frame' should be called.
bool p1_is_animating(p1_t *p1);

/*
 * State.
 */

// Returns a p1_t object initialized from a given state buffer.
//
// The p1_t object should be released with 'p1_release'.
p1_t *p1_new_from_state_buffer(const char *state_buffer);

// Releases the resources associated to 'state_buffer'.
void p1_release_state_buffer(char *state_buffer);

// Returns the number of bytes used to represent the state of a p1_t object.
//
// This can be used together with 'p1_get_state_buffer' to save a state buffer
// into the file system.
long p1_get_state_buffer_size(p1_t *p1);

// Returns the state buffer representing a p1_t object.
//
// The state buffer is an array of bytes that can be stored in the file system
// and used to reconstruct a p1_t object with 'p1_new_from_state_buffer'.
//
// The state buffer should be released with 'p1_release_state_buffer'.
char *p1_get_state_buffer(p1_t *p1);

/*
 * Logging.
 */

// Returns the index of the first available log entry.
//
// By convention returns 0 if no entry is available, for example after log
// entries have been cleared. Otherwise the index is at least 1.
//
// Note that only the latest log entries are available. There are up to 100
// entries but there may be less if individual log entries are larger than
// average.
long p1_log_get_first_available_index(p1_t *p1);

// Returns the index of the last available log entry.
//
// By convention returns 0 if no entry is available, for example after log
// entries have been cleared. Otherwise the index is at least 1.
//
// Note that only the latest log entries are available. There are up to 100
// entries but there may be less if individual log entries are larger than
// average.
long p1_log_get_last_available_index(p1_t *p1);

// Returns the entry of given index.
//
// The index must be such that:
// first_available_index <= index <= last_available_index).
char *p1_log_get_entry(p1_t *p1, long index);

// Clears all log entries.
void p1_log_clear(p1_t *p1);
