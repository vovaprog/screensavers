#ifdef __linux__
#   include <sys/time.h>
#else
#   include <windows.h>
#endif

#include <fps.h>

#ifdef __linux__

static int tdif(struct timeval *x,struct timeval *y,struct timeval *r)
{
	struct timeval *b,*s;
	int ret;

	x->tv_sec+=x->tv_usec/1000000;
	x->tv_usec%=1000000;

	y->tv_sec+=y->tv_usec/1000000;
	y->tv_usec%=1000000;
		
	if(x->tv_sec > y->tv_sec)
	{
		b=x;s=y;ret=1;
	}
	else if(x->tv_sec < y->tv_sec)
	{
		b=y;s=x;ret=-1;
	}
	else
	{
		if(x->tv_usec > y->tv_usec)
		{
			b=x;s=y;ret=1;
		}
		else if(x->tv_usec < y->tv_usec)
		{
			b=y;s=x;ret=-1;
		}
		else
		{
			b=x;s=y;ret=0;
		}
	}
	
	r->tv_sec=b->tv_sec-s->tv_sec;
	r->tv_usec=b->tv_usec-s->tv_usec;
	
	return ret;
}

static unsigned int timeval_to_msec(struct timeval *tv)
{
	return tv->tv_sec * 1000 + tv->tv_usec / 1000;
}

#endif

void start_fps_counter(struct fps_counter *fpsc)
{
#ifdef __linux__
	gettimeofday(&fpsc->tick_count,0);
#else
	fpsc->tick_count=GetTickCount();
#endif	
	fpsc->frame_count=0;
}

void stop_fps_counter(struct fps_counter *fpsc)
{
#ifdef __linux__
	struct timeval tv,dif;
	
	gettimeofday(&tv,0);
	
	tdif(&fpsc->tick_count,&tv,&dif);
	
	fpsc->fps=(double)fpsc->frame_count/((double)timeval_to_msec(&dif)/1000.0);
#else
	fpsc->fps=(double)fpsc->frame_count/((double)(GetTickCount()-fpsc->tick_count)/1000.0);
#endif	
}

double get_fps(struct fps_counter *fpsc)
{
	stop_fps_counter(fpsc);
	return fpsc->fps;
}


