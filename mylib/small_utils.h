#pragma once

unsigned int getMilliseconds();

#define RED_MASK   0x0000ff
#define GREEN_MASK 0x00ff00
#define BLUE_MASK  0xff0000

#define RED_SHIFT 0
#define GREEN_SHIFT 8
#define BLUE_SHIFT 16

inline unsigned int GetRed(unsigned int x)
{
    return ((x & RED_MASK) >> RED_SHIFT);
}

inline unsigned int GetGreen(unsigned int x)
{
    return ((x & GREEN_MASK) >> GREEN_SHIFT);
}

inline unsigned int GetBlue(unsigned int x)
{
    return ((x & BLUE_MASK) >> BLUE_SHIFT);
}

inline unsigned int CreateColor(unsigned int r,unsigned int g,unsigned int b)
{
    return 0xff000000 | (r << RED_SHIFT) | (g<<GREEN_SHIFT) | (b<<BLUE_SHIFT);    
}
