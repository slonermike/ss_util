#include "str_util.h"

#include "string.h"
#include "util.h"
#include "math/vector.h"

#include "stdio.h"
#include "ctype.h"

// Removes the numbers at the end of val_in and stores them in buf_out.
//
// returns the number of digits found.
int str_util_remove_numbers_at_end(const char *val_in, char *buf_out, int buf_size)
{
	Assert_return_value(val_in != NULL && buf_out != NULL, 0);
	int len = strlen(val_in);
	Assert_return_value(len < buf_size, 0);
	bool still_finding_numbers = true;
	int digits_found = 0;

	for (int i = len-1; i >= 0; i--) {
		if (still_finding_numbers) {
			if (val_in[i] < '0' || val_in[i] > '9') {
				still_finding_numbers = false;
				buf_out[i+1] = '\0';

				// bring it back around and do this character again.
				i++;
			} else {
				digits_found++;
			}
		} else {
			buf_out[i] = val_in[i];
		}
	}

	// It was ALL NUMBERS???
	if (still_finding_numbers) {
		buf_out[0] = '\0';
	}

	return digits_found;
}

vector2 str_util_parse_vector( const char *vector_string )
{
	vector2 return_val;

	Assert_return_value(vector_string, ZERO_VECTOR);

	int len = strlen(vector_string);

	char buffer[128];
	bool recording_x = true;
	int iter = 0;
	for (int i = 0; i < len; i++) {
		// end of the current string.
		if (vector_string[i] == ',') {
			if (recording_x) {
				return_val.x = (float)atof(buffer);
				recording_x = false;
				iter = 0;
			} else {
				return_val.y = (float)atof(buffer);
				break;
			}
		} else if ((vector_string[i] >= '0' && vector_string[i] <= '9') || vector_string[i] == '.' || vector_string[i] == '-') {
			buffer[iter] = vector_string[i];
			iter++;
			buffer[iter] = '\0';

			if (i == len-1) {
				return_val.y = (float)atof(buffer);
			}
		}
	}
	
	return return_val;
}

// Checks to see if "string" has the prefix "prefix."
//
bool str_has_prefix( const char *string, const char *prefix, bool case_insensitive /*= true*/ )
{
	int str_len = strlen(string);
	int prefix_len = strlen(prefix);

	if ( prefix_len > str_len )
	{
		return false;
	}

	for ( int i = 0; i < prefix_len; i++ )
	{
		if ( case_insensitive )
		{
			if ( tolower(string[i]) != tolower(prefix[i]) )
			{
				return false;
			}
		} else {
			if ( string[i] != prefix[i] )
			{
				return false;
			}
		}
	}

	return true;
}

// Case-insensitive version of strstr.
//
// FIXME: Not as efficient as it could be, since we call strlen 2 times here,
// and 2 more times inside str_has_prefix.
//
const char * strstri( const char *full_string, const char *search_for )
{
	Assert_return_value( full_string != NULL && search_for != NULL, NULL);
	int len1 = strlen(full_string);
	int len2 = strlen(search_for);
	if ( len2 > len1 )
	{
		return NULL;
	} else if ( len2 == 0 )
	{
		return full_string;
	}

	for ( int i = 0; i <= len1-len2 && i < len1; i++ )
	{
		const char *str = &full_string[i];
		if ( str_has_prefix(str, search_for))
		{
			return str;
		}
	}

	return NULL;
}
