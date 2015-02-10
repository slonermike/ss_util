#pragma once

#ifndef __TIMESTAMP_H
#define __TIMESTAMP_H

class timestamp {
public:
	timestamp();
	void set(int time_ms);
	void invalidate();
	bool valid() const;
	bool elapsed() const;
	int time_until() const;
	int time_since() const;
private:
	int stamp_time;
};

#endif //__TIMESTAMP_H