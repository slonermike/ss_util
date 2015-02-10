#include "timestamp.h"

#include "ss.h"

timestamp::timestamp()
{
	invalidate();
}

void timestamp::set(int time_ms)
{
	stamp_time = (Game_time_ms + time_ms);
}

void timestamp::invalidate()
{
	stamp_time = -1;
}

bool timestamp::valid() const
{
	return stamp_time >= 0;
}

bool timestamp::elapsed() const
{
	return (stamp_time <= Game_time_ms);
}

int timestamp::time_until() const
{
	return (stamp_time - Game_time_ms);
}

int timestamp::time_since() const
{
	return (Game_time_ms - stamp_time);
}