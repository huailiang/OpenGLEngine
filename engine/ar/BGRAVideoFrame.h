#ifndef Example_MarkerBasedAR_BGRAVideoFrame_h
#define Example_MarkerBasedAR_BGRAVideoFrame_h

#include <cstddef>

// A helper struct presenting interleaved BGRA image in memory.
struct BGRAVideoFrame
{
    size_t width;
    size_t height;
    size_t stride;
    
    unsigned char * data;
};


#endif
