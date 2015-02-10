#ifndef __SS_H
#define __SS_H

#pragma once

#include "util.h"

extern float Frame_time_s;
extern float Frame_time_unscaled_s;

extern uint Frame_time_ms;
extern uint Frame_time_unscaled_ms;

extern int System_time_ms;
extern int Game_time_ms;

extern bool Game_paused;
extern bool Frame_limiting;

// Called at application start.
bool ss_init();

// Called every frame.
void ss_do_frame();

void ss_set_timescale( float scale_s );
float ss_get_timescale();

#endif // __SS_H