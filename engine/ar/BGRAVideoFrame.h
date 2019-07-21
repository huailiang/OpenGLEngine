//
//  BGRAVideoFrame.h
//  GLESEngine
//
//  Created by 彭怀亮 on 7/12/19.
//  Copyright © 2019 彭怀亮. All rights reserved.
//

#ifndef BGRAVideoFrame_h
#define BGRAVideoFrame_h


struct BGRAVideoFrame
{
    size_t width;
    size_t height;
    size_t stride;
    
    unsigned char * data;
};


#endif
