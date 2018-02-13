// timer.h -- Defines the interface for all PIT-related functions.
//            Written for JamesM's kernel development tutorials.

#ifndef TIMER_H
#define TIMER_H

#include "common.h"

#include <stddef.h>
#include <stdint.h>

void init_timer(uint32_t frequency);

#endif
