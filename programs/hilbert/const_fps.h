#ifndef CONST_FPS_H_INCLUDED
#define CONST_FPS_H_INCLUDED

#ifdef __linux__
#   include <unistd.h>
#else
#   include <windows.h>
#endif

#include <fps.h>

class const_fps{
private:	
	fps_counter fpsc;
	double needed_fps;
	int sleep_millisec;
public:
	const_fps(double needed_fps):needed_fps(needed_fps),sleep_millisec(0)
	{
		start_fps_counter(&fpsc);
	}

	void sleep()
	{		
		fpsc.frame_count++;
		
		double fps=get_fps(&fpsc);
		double dif=fabs(fps-needed_fps);
		
		if(dif>2.0)
		{
			int step;
			
			if(dif<10.0) step=1;
			else if(dif<30.0) step=5;
			else step=10;
			
			if(fps>needed_fps)
			{
				sleep_millisec+=step;
				start_fps_counter(&fpsc);
			}
			else
			{
				sleep_millisec-=step;
				start_fps_counter(&fpsc);
			}
		}
		
		if(sleep_millisec<=0) sleep_millisec=0;
		else
		{
#ifdef __linux__
			usleep(sleep_millisec*1000);
#else
			Sleep(sleep_millisec);
#endif			
		}
	}
};

#endif

