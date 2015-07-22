#ifdef __linux__
#   include <unistd.h>
#   include <sys/time.h>
#else
#   include <windows.h>
#endif

unsigned int getMilliseconds()
{
#ifdef __linux__
    struct timeval tv;
    gettimeofday(&tv, 0);
    return (unsigned int)((tv.tv_sec * 1000) + (tv.tv_usec / 1000));		
#else
    return GetTickCount();
#endif
}
