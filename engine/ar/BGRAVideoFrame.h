#ifndef BGRAVideoFrame_h
#define BGRAVideoFrame_h

#include <cstddef>


struct BGRAVideoFrame
{
    size_t width;
    size_t height;
    size_t stride;
    
    unsigned char * data;
};


#endif
