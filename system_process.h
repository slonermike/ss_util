#ifndef __SYSTEM_PROCESS_H
#define __SYSTEM_PROCESS_H

#pragma once

#include "util.h"

class system_process {
public:

	system_process();										// Initialize and register the process.
	~system_process();									// Uninitialize and unregister the process.

	virtual void process();							// Per-frame processing.
	virtual void process_always() {};		// Process regardless of whether the process is paused.
	virtual void render();							// Per-frame rendering.
	virtual const char *get_name();
	void pause(bool set_paused) {paused = set_paused;}
	virtual bool is_paused() {return paused;}
	
protected:
	uint process_timer;
	bool paused;
};

void system_process_initialize();
void system_process_shutdown();
void system_process_process();
void system_process_render();

#endif