#include <string>
#include "Timer.h"

// Timer Method Definitions
Timer::Timer()
{
    #if defined( WIN32 )
        // Windows Timer Initialization
        QueryPerformanceFrequency( &m_performance_frequency );
        m_oneoverfrequency = 1.0/((double)m_performance_frequency.QuadPart);
    #endif

        m_time0 = m_elapsed = 0;
        m_running = 0;
}

double Timer::GetTime()
{

#if defined( WIN32 )
        // Windows GetTime
        QueryPerformanceCounter( &m_performance_counter );
        return (double) m_performance_counter.QuadPart * m_oneoverfrequency;
#else
        // UNIX GetTime
        gettimeofday( &m_timeofday, NULL );
        return m_timeofday.tv_sec + m_timeofday.tv_usec / 1000000.0;
#endif
}

Timer::~Timer()
{

}

void Timer::Start()
{
    m_running = 1;
    m_time0 = GetTime();
}

void Timer::Stop()
{
    m_running = 0;
    m_elapsed += GetTime() - m_time0;
}

void Timer::Reset()
{
    m_running = 0;
    m_elapsed = 0;
}

double Timer::Time()
{
    if (m_running)
    {
        Stop();
        Start();
    }
    return m_elapsed;
}
