#ifndef __STR_UTIL_H
#define __STR_UTIL_H

#pragma once

#include "math/vector.h"

int str_util_remove_numbers_at_end(const char *val_in, char *buf_out, int buf_size);
vector2 str_util_parse_vector(const char *vector_string);
bool str_has_prefix(const char *string, const char *prefix, bool case_insensitive = true);
const char *strstri(const char *full_string, const char *search_for);

#endif //__STR_UTIL_H