//
//  OpenGLView.h
//  GLESEngine
//
//  Created by 彭怀亮 on 5/30/19.
//  Copyright © 2019 彭怀亮. All rights reserved.
//

#import <GLKit/GLKit.h>
#import <OpenGLES/ES3/gl.h>
#import <OpenGLES/ES3/glext.h>

#define ENABLE_SCREEN_RETINA

#define RETINA_SCALE_FACTOR 1.8f

@interface OpenGLView : GLKView
{
@private
    EAGLContext *context;
    GLint framebufferWidth;
    GLint framebufferHeight;
    GLuint defaultFramebuffer, colorRenderbuffer, depthRenderBuffer;
}


@property (nonatomic, retain) EAGLContext *context;
@property GLint framebufferWidth;
@property GLint framebufferHeight;

- (void)setFramebuffer;

- (BOOL)presentFramebuffer;

@end


