#pragma once

#if defined ( WIN32 )
#include <windows.h>
#else
#include <sys/time.h>
#endif

class Timer
{
public:
    Timer();
    ~Timer();

    void Start();
    void Stop();
    void Reset();
    double Time();
private:
    // Private Timer Data
    double m_time0, m_elapsed;
    bool m_running;
    double GetTime();

    #if defined( WIN32 )
    // Private Windows Timer Data
        LARGE_INTEGER m_performance_counter, m_performance_frequency;
        double m_oneoverfrequency;
    #else
    // Private UNIX Timer Data
        struct timeval m_timeofday;
    #endif
};
