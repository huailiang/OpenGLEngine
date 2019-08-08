//
//  ViewController.m
//  GLESEngine
//
//  Created by 彭怀亮 on 5/30/19.
//  Copyright © 2019 彭怀亮. All rights reserved.
//

#import "ViewController.h"
#import "VideoSource.h"
#import "AlbumSource.h"
#import "IARInterface.h"
#import "MarkerDetector.hpp"
#include "Context.h"
#include "scene/scenewrap.h"


@interface ViewController () <VideoSourceDelegate>
{
    ESContext _esContext;
    BOOL initial;
}

@property (strong, nonatomic)EAGLContext *context;
@property (strong, nonatomic) GLKBaseEffect *effect;
@property (strong, nonatomic) VideoSource *videoSource;
@property (strong, nonatomic) AlbumSource *albumSource;
@property (nonatomic) MarkerDetector * markerDetector;

- (void)setupGL:(int)w with:(int)h;
- (void)tearDownGL;
@end

@implementation ViewController

- (void)viewDidLoad
{
    [super viewDidLoad];
    initial = FALSE;
    self.context = [[EAGLContext alloc]initWithAPI:kEAGLRenderingAPIOpenGLES3];
    if(!self.context) NSLog(@"Failed to create ES context");
    
    GLKView *view = (GLKView *)self.view;
    view.context = self.context;
    view.drawableDepthFormat = GLKViewDrawableDepthFormat24;
    view.drawableStencilFormat = GLKViewDrawableStencilFormat8;
    view.drawableColorFormat = GLKViewDrawableColorFormatRGBA8888;
}

-(void)viewDidAppear:(BOOL)animated
{
    if(initial) return;
    GLKView *view = (GLKView *)self.view;
    [self setupGL:(int)view.drawableWidth  with:(int)view.drawableHeight];
    if(_esContext.width != (GLint)view.drawableWidth || _esContext.height != (GLint)view.drawableHeight)
    {
        _esContext.width = (GLint)view.drawableWidth;
        _esContext.height = (GLint)view.drawableHeight;
        if (_esContext.updateWindow) _esContext.updateWindow(&_esContext);
    }
    
    if(self.albumSource == nil)
    {
        self.albumSource = [[AlbumSource alloc] init];
        [self.albumSource setController:self];
        iOSAR* ar = new iOSAR();
        ar->source = self.albumSource;
        if(_esContext.arProcess) {
            _esContext.arProcess(&_esContext, (IARInterface*)ar);
        }
    }
    initial = TRUE;
    [super viewDidAppear:animated];
}

- (void)dealloc
{
    [self tearDownGL];
    if ([EAGLContext currentContext] == self.context)
        [EAGLContext setCurrentContext:nil];
    delete self.markerDetector;
    [super dealloc];
}

- (void) checkCamera
{
    if(OpenCamera() )
    {
        if(self.videoSource == nil)
        {
            self.videoSource = [[VideoSource alloc] init];
            self.markerDetector = new MarkerDetector([self.videoSource getCalibration]);
            [self.videoSource startWithDevicePosition:AVCaptureDevicePositionBack]; //默认启动后置摄像头
            
            CGSize frameSize = [self.videoSource getFrameSize];
            CameraCalibration camCalib = [self.videoSource getCalibration];
            glm::mat3 intrinsic = camCalib.getIntrinsic();
            if(_esContext.frameInitFunc)
            {
                _esContext.frameInitFunc(&_esContext, (float)frameSize.width, (float)frameSize.height, intrinsic);
            }
        }
        self.videoSource.delegate = self;
    }
    else
    {
        if(self.videoSource != nil)
        {
            self.videoSource.delegate = nil;
        }
    }
}

- (void)update
{
    [self checkCamera];
    if (_esContext.updateFunc)
    {
        _esContext.updateFunc(&_esContext, self.timeSinceLastUpdate);
    }
}

- (void)glkView:(GLKView *)view drawInRect:(CGRect)rect
{
    if(_esContext.width != (GLint)view.drawableWidth || _esContext.height != (GLint)view.drawableHeight)
    {
        _esContext.width = (GLint)view.drawableWidth;
        _esContext.height = (GLint)view.drawableHeight;
        if (_esContext.updateWindow) {
            _esContext.updateWindow(&_esContext);
        }
    }
    if (_esContext.drawFunc)
    {
        _esContext.drawFunc(&_esContext);
    }
}


-(void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event
{
    CGSize size = self.view.bounds.size;
    NSSet *allTouches = [event allTouches];
    UITouch *touch = [allTouches anyObject];  
    CGPoint point = [touch locationInView:[touch view]];
    int x = point.x;
    int y = point.y;
    if(_esContext.tapFunc)
    {
        _esContext.tapFunc(&_esContext, (float)(x / size.width), (float)(y /size.height));
    }
}

- (void)setupGL:(int)w with:(int)h
{
    if(![EAGLContext setCurrentContext:self.context])
    {
        NSLog(@" setCurrentContext error");
    }
    memset(&_esContext, 0, sizeof(_esContext));
    InitScene(&_esContext,w,h);
}

- (void)tearDownGL
{
    [EAGLContext setCurrentContext:self.context];
    if(_esContext.shutdownFunc)
    {
        _esContext.shutdownFunc(&_esContext);
    }
}

-(void)OnApplicationPause:(bool) pause
{
    if(_esContext.pauseFunc)
    {
        _esContext.pauseFunc(&_esContext, pause);
    }
}

-(void)OnApplicationQuit
{
    [self tearDownGL];
}

//在接收到内存警告时会调用，且系统会自动处理内存释放 拟器simulator-菜单栏-hardware-simulate memory warning
- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    if ([self isViewLoaded] && ([[self view] window] == nil)) {
        self.view = nil;
        [self tearDownGL];
        if ([EAGLContext currentContext] == self.context)
            [EAGLContext setCurrentContext:nil];
        self.context = nil;
    }
}

- (BOOL)shouldAutorotate
{
    return YES;
}


#pragma mark - VideoSourceDelegate
-(void)frameReady:(BGRAVideoFrame) frame
{
    if(OpenCamera())
    {
        dispatch_sync( dispatch_get_main_queue(), ^{
            if (_esContext.frameReadyFunc) {
                _esContext.frameReadyFunc(&_esContext, frame);
            }
        });
        
        self.markerDetector->processFrame(frame);
        
        dispatch_async( dispatch_get_main_queue(), ^{
            if(_esContext.frameDetectFunc)
            {
                _esContext.frameDetectFunc(&_esContext, self.markerDetector->getTransformations());
            }
        });
    }
}


@end
