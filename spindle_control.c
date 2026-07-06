#include "grbl.h"

#ifndef SPINDLE_STATE_DISABLE
  #define SPINDLE_STATE_DISABLE SPINDLE_DISABLE
#endif

void spindle_init() {}

void spindle_run(uint8_t direction, float rpm)
{
  spindle_set_state(direction, rpm);
}

void spindle_set_state(uint8_t state, float rpm)
{
  (void)state;
  (void)rpm;
}

void spindle_stop() {}

uint8_t spindle_get_state()
{
  return SPINDLE_STATE_DISABLE;
}