//
//  OpenGLView.m
//  GLESEngine
//
//  Created by 彭怀亮 on 5/30/19.
//  Copyright © 2019 彭怀亮. All rights reserved.
//


#import "OpenGLView.h"
#import "common.h"

@interface OpenGLView()
- (void)createFramebuffer;
- (void)deleteFramebuffer;
@end

@implementation OpenGLView

@synthesize framebufferWidth;
@synthesize framebufferHeight;
@dynamic context;


+ (Class)layerClass { return [CAEAGLLayer class]; }

- (void)layoutSubviews
{
    // The framebuffer will be re-created at the beginning of the next setFramebuffer method call.
    [self deleteFramebuffer];
}

-(BOOL)isRetina
{
    return ([[UIScreen mainScreen] respondsToSelector:@selector(displayLinkWithTarget:selector:)] && ([UIScreen mainScreen].scale == 2.0))?1:0;
}

-(id)initWithCoder:(NSCoder *)coder
{
    self = [super initWithCoder:coder];
#ifdef ENABLE_SCREEN_RETINA
    
    // Retina display detected*/
    float scaleFactor = RETINA_SCALE_FACTOR;
    if([self isRetina])
    {
        NSLog(@"EAGLView: [INFO] detected retina display: scaling to %f", scaleFactor);
        // Set contentScale Factor to 2
        self.contentScaleFactor = scaleFactor;
        // Also set our glLayer contentScale Factor to 2
        CAEAGLLayer *eaglLayer = (CAEAGLLayer *)self.layer;
        eaglLayer.contentsScale = scaleFactor; //new line
    }
#endif
    CAEAGLLayer *eaglLayer = (CAEAGLLayer *)self.layer;
    eaglLayer.opaque = TRUE;
    eaglLayer.drawableProperties = [NSDictionary dictionaryWithObjectsAndKeys:
                                    [NSNumber numberWithBool:FALSE], kEAGLDrawablePropertyRetainedBacking,
                                    kEAGLColorFormatRGBA8, kEAGLDrawablePropertyColorFormat,
                                    nil];
    
    return self;
}


- (void)dealloc
{
    [self deleteFramebuffer];
    [context release];
    
    [super dealloc];
}

- (EAGLContext *)context
{
    return context;
}

- (void)setContext:(EAGLContext *)newContext
{
    if (context != newContext)
    {
        [self deleteFramebuffer];
        
        [context release];
        context = [newContext retain];
        
        [EAGLContext setCurrentContext:newContext];
    }
}

- (void)createFramebuffer
{
    if (context && !defaultFramebuffer)
    {
        [EAGLContext setCurrentContext:context];
        [self createResolveBuffer];
    }
}


-(void) createResolveBuffer
{
    
    // Create default framebuffer object.
    glGenFramebuffers(1, &defaultFramebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, defaultFramebuffer);
    
    // Create color render buffer and allocate backing store.
    glGenRenderbuffers(1, &colorRenderbuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, colorRenderbuffer);
    [context renderbufferStorage:GL_RENDERBUFFER fromDrawable:(CAEAGLLayer *)self.layer];
    glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_WIDTH, &framebufferWidth);
    glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_HEIGHT, &framebufferHeight);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, colorRenderbuffer);
    
    glGenRenderbuffers(1, &depthRenderBuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, depthRenderBuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, framebufferWidth, framebufferHeight);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthRenderBuffer);
    
    //re bind color render buffer
    glBindRenderbuffer(GL_RENDERBUFFER, colorRenderbuffer);
    
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        NSLog(@"Failed to make complete framebuffer object %x", glCheckFramebufferStatus(GL_FRAMEBUFFER));
}

-(void)deleteFramebuffer
{
    if (context)
    {
        [EAGLContext setCurrentContext:context];
        
        if (defaultFramebuffer)
        {
            glDeleteFramebuffers(1, &defaultFramebuffer);
            defaultFramebuffer = 0;
        }
        
        if (colorRenderbuffer)
        {
            glDeleteRenderbuffers(1, &colorRenderbuffer);
            colorRenderbuffer = 0;
        }
        
        if (depthRenderBuffer)
        {
            glDeleteRenderbuffers(1, &depthRenderBuffer);
            depthRenderBuffer = 0;
        }
    }
}

-(void)setFramebuffer
{
    if (context)
    {
        [EAGLContext setCurrentContext:context];
        
        if (!defaultFramebuffer)
            [self createFramebuffer];
        glBindFramebuffer(GL_FRAMEBUFFER, defaultFramebuffer);
        glViewport(0, 0, framebufferWidth, framebufferHeight);
    }
}

- (BOOL)presentFramebuffer
{
    BOOL success = FALSE;
    
    if (context)
    {
        [EAGLContext setCurrentContext:context];
        glBindRenderbuffer(GL_RENDERBUFFER, colorRenderbuffer);
        success = [context presentRenderbuffer:GL_RENDERBUFFER];
    }
    
    return success;
}

@end

