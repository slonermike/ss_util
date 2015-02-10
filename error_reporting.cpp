#include "error_reporting.h"

#include <vector>
#include <cstdio>
#include "stdarg.h"

#include "util.h"
#include "math/ss_math.h"

#define MAX_ERROR_SCREEN_PCT				(0.33f)
#define ERROR_DISPLAY_MS					(10000)
#define ERROR_MAX_LEN						(256)
#define MAX_NUM_ERRORS						(128)

#define ERROR_REPORTING_ENABLED

static uint Error_report_colors[] = {
	0xFF660099,
	0xFF000099,
	0x99999999,
	0x00006699,
};
Assert_compile_time(sizeof(Error_report_colors) == sizeof(uint) * ERT_NUM_TYPES);

static bool Error_reporting_enabled = false;

struct reported_error {
	char msg[ERROR_MAX_LEN];
	int display_time_remaining;
	error_report_type type;
};

std::vector<reported_error> Reported_error_list;

void error_reporting_system_initialize()
{
	Reported_error_list.clear();
#ifdef ERROR_REPORTING_ENABLED
	Error_reporting_enabled = false;
#else
	Error_reporting_enabled = true;
#endif
}

void error_reporting_system_shutdown()
{
	if (!Error_reporting_enabled) {
		return;
	}
}

void error_report_internal(error_report_type type, const char *format, va_list arg_list)
{
	// too many errors.
	if (Reported_error_list.size() >= MAX_NUM_ERRORS) {
		return;
	}

	CAP(type, (error_report_type)0, (error_report_type)(ERT_NUM_TYPES-1));

	reported_error err;
	snprintf(err.msg, ERROR_MAX_LEN, format, arg_list);
	err.display_time_remaining = ERROR_DISPLAY_MS;
	err.type = type;

	Reported_error_list.push_back(err);
}

void error_report( const char *format, ... )
{
	va_list arg_list;
	va_start(arg_list, format);
	error_report_internal(ERT_ERROR, format, arg_list);
}

void assert_report( const char *format, ... )
{
	va_list arg_list;
	va_start(arg_list, format);
	error_report_internal(ERT_ASSERT, format, arg_list);
}

void message_report( const char *format, ... )
{
	va_list arg_list;
	va_start(arg_list, format);
	error_report_internal(ERT_MESSAGE, format, arg_list);
}

void script_error( const char *format, ... )
{
	va_list arg_list;
	va_start(arg_list, format);
	error_report_internal(ERT_SCRIPT, format, arg_list);
}
