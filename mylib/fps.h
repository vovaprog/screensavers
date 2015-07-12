#ifndef FPS_H_INCLUDED
#define FPS_H_INCLUDED

#ifdef __linux__
#   include <sys/time.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

struct fps_counter{
	double fps;
	unsigned int frame_count;
#ifdef __linux__
	struct timeval tick_count;
#else
	unsigned int tick_count;
#endif
};

void start_fps_counter(struct fps_counter *fpsc);

void stop_fps_counter(struct fps_counter *fpsc);

double get_fps(struct fps_counter *fpsc);

#ifdef __cplusplus
}
#endif

#endif

