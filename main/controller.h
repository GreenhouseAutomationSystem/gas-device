#ifndef CONTROLLER_H
#define CONTROLLER_H
#include "state.h"
#include "temperature_sensor.h"
#include <parson.h>

void ctrl_init();
void ctrl_schedules_set(JSON_Array *schedules);

#endif