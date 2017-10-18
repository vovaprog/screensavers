#pragma once

#ifdef __unix__
#   include <unistd.h>
#   include <sys/time.h>
#else
#   include <windows.h>
#endif

using namespace std;

class ConstantFps
{
private:
    int sleep;
    int counter;
    int checkCount;
    int checkMillis;
    unsigned int prevMillis;

public:
    ConstantFps(unsigned int fps):
        sleep(0),
        counter(0),
        checkCount(fps / 2),
        checkMillis(1000 / 2),
        prevMillis(0)
    {
    }

private:
    unsigned int getMilliseconds()
    {
#ifdef __unix__
        struct timeval tv;
        gettimeofday(&tv, 0);
        return (unsigned int)((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
#else
        return GetTickCount();
#endif
    }

public:
    void step()
    {
        counter++;

        if (counter >= checkCount)
        {
            counter = 0;

            unsigned int curMillis = getMilliseconds();

            if (prevMillis != 0)
            {
                int millisPassed = curMillis - prevMillis;

                int difMillis = checkMillis - millisPassed;

                int newSleep = sleep + difMillis / checkCount;

                sleep = (sleep + newSleep) / 2;

                if (sleep < 0)
                {
                    sleep = 0;
                }
            }
            else
            {
                sleep = 0;
            }

            prevMillis = curMillis;
        }

        if (sleep > 0)
        {
#ifdef __unix__
            usleep(sleep * 1000);
#else
            Sleep(sleep);
#endif
        }
    }
};
