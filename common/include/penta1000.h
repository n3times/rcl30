/**
 * API for clients that want to implement a Pentatronics 1000 emulator.
 *
 * Clients should use 2 threads: one thread, possibly the main thread, to handle
 * user input (key presses) and another thread to run the display animations.
 *
 * The animation thread should call 'p1_advance_frame' every 10 ms as long as
 * 'p1_is_animating' is true.
 */

#include <stdbool.h>

typedef struct p1_s p1_t;

// Allocates a new Pentatronics 1000 object and inits it.
//
// After the allocation, the client should update the display and start
// animations if necessary.
p1_t *p1_new(long seed);

// Deallocates a Pentatronics 1000 object.
void p1_release(p1_t *p1);

// Reports a key press by the user.
//
// After calling this method, clients should update the display and perform
// animations as necessary.
//
// 'key' is in '0123456789.~+-*/=%cg' which covers the 10 digits, the 4
// arithmetic operations, percent, equal, dot and:
// ~ for +/-
// c for clear
// g for game (switches between game and calc mode)
void p1_press_key(p1_t *p1, char key);

// Gets the display as a null terminated string with at most 24 effective
// characters.
//
// The text should be right justified on the display. A dot ('.') modifies the
// character just before it.
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
 * Serialization.
 */

// Returns a serialized version of a p1_t object.
//
// The size of the object is stored in 'size_out'.
//
// The serialized object can then be stored in the file system and deserialized
// later with 'p1_deserialize'.
//
// This object can later be freed with 'free'.
void *p1_serialize(p1_t *p1, long *size_out);

// Returns a new p1_t object from its serialized version.
//
// This object can later be freed with 'p1_release'.
p1_t *p1_deserialize(void *serialized_object);

/*
 * Logging.
 */

// Returns the index of the first and last available log entries.
//
// By convention returns 0 and 0 if no entry is available, for example after log
// entries have been cleared.
//
// There are up to 100 available log entries but there may be less if individual
// log entries are larger than average.
void p1_get_log_available_interval(p1_t *p1,
                                   long *first_index_out,
                                   long *last_index_out);

// Returns the entry of given index.
//
// The index must be such that:
// first_available_index <= index <= last_available_index).
char *p1_get_log_entry(p1_t *p1, long index);

// Clears all log entries.
void p1_clear_log(p1_t *p1);
