#include "ss.h"

#include <cstring>		// For memset
#include <ctime>
#include <unistd.h>

#include "math/ss_math.h"

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

// Frame time logging.
static uint Frame_time_log[NUM_FRAME_TIMES_TO_LOG];
static uint Render_time_log[NUM_FRAME_TIMES_TO_LOG];
static uint Sim_time_log[NUM_FRAME_TIMES_TO_LOG];
static uint Frame_time_log_iter = 0;


// To be called at the start of execution to set everything up.
//
bool ss_init()
{
	// Set up the time tracking system.
	Frame_start_clocks = clock();
	
	// Initialize all frame logging at zero.
	memset(Frame_time_log, 0, sizeof(uint) * NUM_FRAME_TIMES_TO_LOG);
	memset(Render_time_log, 0, sizeof(uint) * NUM_FRAME_TIMES_TO_LOG);
	memset(Sim_time_log, 0, sizeof(uint) * NUM_FRAME_TIMES_TO_LOG);
	
	return true;
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
	

	/* SS-TODO:
	// Process all the base systems.
	{
		input_process();
		system_process_process();
		gr_process();
		console_process();
	}

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

void ss_set_timescale( float scale_s )
{
	CAP(scale_s, TIMESCALE_MIN, TIMESCALE_MAX);
	Timescale = scale_s;
}

float ss_get_timescale()
{
	return Timescale;
}

