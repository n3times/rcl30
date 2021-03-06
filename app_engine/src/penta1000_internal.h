#ifndef P1000_INTERNAL_H
#define P1000_INTERNAL_H

#include "penta1000.h"

#include "calc.h"
#include "hilo.h"
#include "hilo2.h"

#include <stdbool.h>

// Increment version whenever 'struct p1_s' changes.
#define PENTA_1000_VERSION 6

typedef enum apps_type_e {
    APP_TYPE_NONE = 0,
    APP_TYPE_CALC = 1,
    APP_TYPE_HILO = 2,
    APP_TYPE_HILO2 = 3,
} app_type_t;

struct p1_s {
    long version;
    app_type_t current_app_type;
    calc_t calc;
    hilo_t hilo;
    hilo2_t hilo2;
};

#endif /* P1000_INTERNAL_H */
