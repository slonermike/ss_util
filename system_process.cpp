#include "system_process.h"

#include "ss.h"

#define MAX_RUNNING_SYSTEM_PROCESSES		(16)
#define BASE_SYSTEM_PROCESS_NAME			("Unnamed Process")

static system_process *Running_system_processes[MAX_RUNNING_SYSTEM_PROCESSES];
static uint num_running_system_processes = 0;
static bool System_inited = false;

system_process::system_process() : process_timer(0), paused(false)
{
	// Make sure the system process system has been inited.
	Assert_return(System_inited);

	// Make sure we haven't reached our max processes.
	Assert_return(num_running_system_processes < MAX_RUNNING_SYSTEM_PROCESSES);

	// Add to the list of running processes.
	Running_system_processes[num_running_system_processes] = this;
	num_running_system_processes++;
}

system_process::~system_process()
{
	uint process_index = -1;
	for (uint i = 0; i < num_running_system_processes; i++) {
		if (Running_system_processes[i] == this) {
			process_index = i;
			break;
		}
	}

	Assert_return(process_index >= 0);

	// Decrement the number of running processes.
	num_running_system_processes--;

	// Shift the following processes down.
	for (uint i = process_index; i < num_running_system_processes; i++) {
		Running_system_processes[i] = Running_system_processes[i + 1];
	}

	// Null out the last process.
	Running_system_processes[num_running_system_processes] = NULL;
}

void system_process::process()
{
	// Increment the timer.
	process_timer += Frame_time_ms;
}

void system_process::render()
{
}

// Override to make your own name.
const char *system_process::get_name()
{
	return BASE_SYSTEM_PROCESS_NAME;
}

void system_process_initialize()
{
	for (uint i = 0; i < MAX_RUNNING_SYSTEM_PROCESSES; i++) {
		Running_system_processes[i] = NULL;
	}

	num_running_system_processes = 0;
	System_inited = true;
}

// Clear all running system processes.
//
// We assume that any still-running processes have their
// data pointer still stored somewhere, and that cleanup
// will be done by their parent system.
//
void system_process_shutdown()
{
	system_process_initialize();
}

// Per-frame processing for all running processes.
//
void system_process_process()
{
	for (uint i = 0; i < num_running_system_processes; i++) {
		Assert_continue(Running_system_processes[i]);
		if (!Running_system_processes[i]->is_paused()) {
			Running_system_processes[i]->process();
		}
		Running_system_processes[i]->process_always();
	}
}

// Per-frame rendering for all running processes.
//
void system_process_render()
{
	for (uint i = 0; i < num_running_system_processes; i++) {
		Assert_continue(Running_system_processes[i]);
		Running_system_processes[i]->render();
	}	
}
