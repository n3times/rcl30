# Python interface to the Pentatronics 1000 C engine.
#
# See also penta1000.h.

import ctypes

def make_lib():
    global libp1
    libp1 = ctypes.cdll.LoadLibrary('libpenta1000.so')
    # core
    libp1.p1_new.argtypes = [ctypes.c_long]
    libp1.p1_new.restype = ctypes.c_void_p
    libp1.p1_release.argtypes = [ctypes.c_void_p]
    libp1.p1_press_key.argtypes = [ctypes.c_void_p, ctypes.c_char]
    libp1.p1_get_display.argtypes = [ctypes.c_void_p]
    libp1.p1_get_display.restype = ctypes.c_char_p
    # animations
    libp1.p1_advance_frame.argtypes = [ctypes.c_void_p]
    libp1.p1_is_animating.argtypes = [ctypes.c_void_p]
    libp1.p1_is_animating.restype = ctypes.c_bool
    # serialization
    libp1.p1_get_state_buffer_size.argtypes = [ctypes.c_void_p]
    libp1.p1_get_state_buffer_size.restype = ctypes.c_long
    libp1.p1_get_state_buffer.argtypes = [ctypes.c_void_p]
    libp1.p1_get_state_buffer.restype = ctypes.c_void_p
    libp1.p1_release_state_buffer.argtypes = [ctypes.c_void_p]
    libp1.p1_new_from_state_buffer.argtypes = [ctypes.c_void_p]
    libp1.p1_new_from_state_buffer.restype = ctypes.c_void_p
    # logging
    libp1.p1_log_get_first_available_index.argtypes = [ctypes.c_void_p]
    libp1.p1_log_get_first_available_index.restype = ctypes.c_long
    libp1.p1_log_get_last_available_index.argtypes = [ctypes.c_void_p]
    libp1.p1_log_get_last_available_index.restype = ctypes.c_long
    libp1.p1_log_get_entry.argtypes = [ctypes.c_void_p, ctypes.c_long]
    libp1.p1_log_get_entry.restype = ctypes.c_char_p
    libp1.p1_log_clear.argtypes = [ctypes.c_void_p]

# Core

def p1_new(random_seed):
    make_lib()
    return libp1.p1_new(random_seed)

def p1_release(p1):
    return libp1.p1_release(p1)

def p1_press_key(p1, key):
    return libp1.p1_press_key(p1, ord(key))

def p1_get_display(p1):
    return libp1.p1_get_display(p1).decode("ascii")

# Animation

def p1_advance_frame(p1):
    return libp1.p1_advance_frame(p1)

def p1_is_animating(p1):
    return libp1.p1_is_animating(p1)

# State

def p1_get_state_buffer_size(p1):
    return libp1.p1_get_state_buffer_size(p1)

def p1_get_state_buffer(p1):
    return libp1.p1_get_state_buffer(p1)

def p1_release_state_buffer(p1):
    return libp1.p1_release_state_buffer(p1)

def p1_new_from_state_buffer(p1):
    make_lib()
    return libp1.p1_new_from_state_buffer(p1)

# Log

def p1_log_get_first_available_index(p1):
    return libp1.p1_log_get_first_available_index(p1)

def p1_log_get_last_available_index(p1):
    return libp1.p1_log_get_last_available_index(p1)

def p1_log_get_entry(p1, index):
    return libp1.p1_log_get_entry(p1, index).decode('ascii')

def p1_log_clear(p1):
    return libp1.p1_log_clear(p1)
