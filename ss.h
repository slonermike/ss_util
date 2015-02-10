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

bool ss_initialize();			// Called at application start.
void ss_shutdown();				// Called at application shutdown.
void ss_do_frame();				// Called every frame--performs frame limiting internally.

void ss_post_quit_message();
bool ss_is_quit_message_posted();

void ss_set_timescale( float scale_pct );
float ss_get_timescale();

#endif // __SS_H