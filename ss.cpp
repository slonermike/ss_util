#include "ss.h"

#include <cstring>		// For memset
#include <ctime>
#include <unistd.h>

#include "math/ss_math.h"
#include "system_process.h"

// System-level parameters.
//
#define NUM_FRAME_TIMES_TO_LOG		(32)
#define TARGET_FRAME_TIME_MS		(8)
#define TARGET_FRAME_TIME_S			(i2fl(TARGET_FRAME_TIME_MS) / MS_PER_SEC)
#define MAX_FRAME_TIME				(0.1f)
#define MIN_FRAME_TIME				(0.0009f)

#define TIMESCALE_MIN				(0.2f)
#define TIMESCALE_MAX				(5.0f)

#define CLOCKS_PER_MS				(CLOCKS_PER_SEC / MS_PER_SEC)


// Global variables.
float Frame_time_s = 0.0f;
float Frame_time_unscaled_s = 0.0f;

uint Frame_time_ms = 0;
uint Frame_time_unscaled_ms;

int System_time_ms = 0;
int Game_time_ms = 0;

bool Game_paused = false;
bool Frame_limiting = true;

// Local variables.
static clock_t Frame_start_clocks = 0;
static float Timescale = 1.0f;
static bool Quit_message_posted = false;

// Frame time logging.
static uint Frame_time_log[NUM_FRAME_TIMES_TO_LOG];
static uint Render_time_log[NUM_FRAME_TIMES_TO_LOG];
static uint Sim_time_log[NUM_FRAME_TIMES_TO_LOG];
static uint Frame_time_log_iter = 0;


// To be called at the start of execution to set everything up.
//
bool ss_initialize()
{
	system_process_initialize();
	// Set up the time tracking system.
	Frame_start_clocks = clock();
	
	// Initialize all frame logging at zero.
	memset(Frame_time_log, 0, sizeof(uint) * NUM_FRAME_TIMES_TO_LOG);
	memset(Render_time_log, 0, sizeof(uint) * NUM_FRAME_TIMES_TO_LOG);
	memset(Sim_time_log, 0, sizeof(uint) * NUM_FRAME_TIMES_TO_LOG);
	
	return true;
}

void ss_shutdown()
{
	system_process_shutdown();
}

// Called every frame to process...well...everything.
//
void ss_do_frame()
{
	// Update the timer.
	{
		clock_t prev_frame_clock_time = Frame_start_clocks;
		clock_t cur_clock_time = clock();

		Frame_time_unscaled_ms = (uint)(cur_clock_time - prev_frame_clock_time) / CLOCKS_PER_MS;
		Frame_time_unscaled_s = i2fl(Frame_time_unscaled_ms) / i2fl(MS_PER_SEC);
		
		CAP(Frame_time_unscaled_ms, MIN_FRAME_TIME * MS_PER_SEC, MAX_FRAME_TIME * MS_PER_SEC);
		CAP(Frame_time_unscaled_s, MIN_FRAME_TIME, MAX_FRAME_TIME);

		// Limit the frame time.
		if (Frame_time_unscaled_ms < TARGET_FRAME_TIME_MS && Frame_limiting) {

			// Sleep for X microseconds.
			uint sleep_ms = (TARGET_FRAME_TIME_MS - Frame_time_unscaled_ms);
			usleep(sleep_ms * US_PER_MS);
			
			Frame_time_unscaled_ms += sleep_ms;
			CAP(Frame_time_unscaled_ms, MIN_FRAME_TIME * MS_PER_SEC, MAX_FRAME_TIME * MS_PER_SEC);
			Frame_time_unscaled_s = i2fl(Frame_time_unscaled_ms) / i2fl(MS_PER_SEC);
		}

		Frame_time_log[Frame_time_log_iter] = Frame_time_unscaled_ms;

		System_time_ms += Frame_time_unscaled_ms;
		if (!Game_paused) {
			Game_time_ms += Frame_time_ms;
		}
		
		Frame_start_clocks = cur_clock_time;

		Frame_time_ms = (uint)(i2fl(Frame_time_unscaled_ms) * Timescale);
		Frame_time_s = i2fl(Frame_time_ms);
	}

	clock_t process_start = clock();
	

	// Process all the base systems.
	{
// SS-TODO:		input_process();
		system_process_process();
// SS-TODO:		gr_process();
// SS-TODO:		console_process();
	}

/* SS-TODO:
	{
		if (Game_paused_step || input_is_key_just_pressed(IKT_NUMPAD1) || input_is_key_just_pressed(IKT_PAUSE)) {
			Game_paused = !Game_paused;
			Game_paused_step = false;
		}

		if (Game_paused && (input_is_key_just_pressed(IKT_NUMPAD0) || input_is_key_just_pressed(IKT_INSERT))) {
			Game_paused_step = true;
			Game_paused = false;
		}
	}

	input_post_process();
*/

	Sim_time_log[Frame_time_log_iter] = (uint)( i2fl(clock() - process_start) / i2fl(CLOCKS_PER_MS) );
}

// Tell the application to quit.
//
void ss_post_quit_message()
{
	Quit_message_posted = true;
}

// Ask if the application is ready to quit.
//
bool ss_is_quit_message_posted()
{
	return Quit_message_posted;
}

// Scale time by the specified value.
//
// scale_pct: 0.1 = 10% speed, 10 = 1000% speed.
//
void ss_set_timescale( float scale_pct )
{
	CAP(scale_pct, TIMESCALE_MIN, TIMESCALE_MAX);
	Timescale = scale_pct;
}

// Get timescale multiplier value.
//
float ss_get_timescale()
{
	return Timescale;
}

