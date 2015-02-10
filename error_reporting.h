#ifndef __ERROR_REPORTING_H
#define __ERROR_REPORTING_H

#pragma once

enum error_report_type {
	ERT_ERROR,
	ERT_ASSERT,
	ERT_MESSAGE,
	ERT_SCRIPT,

	ERT_NUM_TYPES,
};

void error_reporting_system_initialize();
void error_reporting_system_shutdown();
void error_reporting_render();

void error_report(const char *format, ... );
void assert_report(const char *format, ...);
void message_report(const char *format, ...);
void script_error(const char *format, ...);

#endif // __ERROR_REPORTING_H