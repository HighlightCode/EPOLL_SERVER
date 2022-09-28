#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <assert.h>
#include <iostream>
#include <unistd.h>
#include <sys/time.h>

#include "types.h"


inline void SleepMillisecond(uint32 ms)
{
    ::usleep(1000*ms);
}

inline unsigned long long GetTimeMicroSecond()
{
    struct timeval tm;
    ::gettimeofday(&tm, NULL);
    return ((unsigned long long) tm.tv_sec * 1000 *1000 + (tm.tv_usec));

}

inline unsigned long long GetTimeMillisecond()
{
	return GetTimeMicroSecond()/1000;
}

inline bool TimeToTm(time_t t, tm* tt)
{
    if(localtime_r(&t, tt) != NULL)
    {
        return true;
    }
    return false;
}

inline time_t TmToTime(tm * tt)
{
	return mktime(tt);
}

inline std::string TimeToString(time_t t)
{
	char m[128] = {0};
	tm tt;
	if (TimeToTm(t, &tt))
		{
		sprintf(m, "%d-%02d-%02d %02d:%02d:%02d", tt.tm_year+1900, tt.tm_mon+1, tt.tm_mday, tt.tm_hour, tt.tm_min, tt.tm_sec);
	}
	else
	{
		sprintf(m, "0000-00-00 00:00:00");
	}
	return m;
}
