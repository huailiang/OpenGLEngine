//
//  ViewController.m
//  GLESEngine
//
//  Created by 彭怀亮 on 5/30/19.
//  Copyright © 2019 彭怀亮. All rights reserved.
//

#import "ViewController.h"
#include "Context.h"
#include "scene/scenewrap.h"

@interface ViewController ()
{
    ESContext _esContext;
}

@property  (strong,nonatomic)EAGLContext *context;
@property (strong, nonatomic) GLKBaseEffect *effect;

- (void)setupGL;
- (void)tearDownGL;
@end

@implementation ViewController

- (void)viewDidLoad
{
    [super viewDidLoad];
    self.context = [[EAGLContext alloc]initWithAPI:kEAGLRenderingAPIOpenGLES3];
    if(!self.context)
    {
        NSLog(@"Failed to create ES context");
    }
    
    GLKView *view = (GLKView *)self.view;
    view.context = self.context;
    view.drawableDepthFormat = GLKViewDrawableDepthFormat24;
    view.drawableStencilFormat = GLKViewDrawableStencilFormat8;
    view.drawableColorFormat = GLKViewDrawableColorFormatRGBA8888;
    [self setupGL];
    if(_esContext.width != (GLint)view.drawableWidth || _esContext.height != (GLint)view.drawableHeight)
    {
        _esContext.width = (GLint)view.drawableWidth;
        _esContext.height = (GLint)view.drawableHeight;
        if (_esContext.updateWindow) {
            _esContext.updateWindow(&_esContext);
        }
    }
}

- (void)dealloc
{
    [self tearDownGL];
    if ([EAGLContext currentContext] == self.context)
        [EAGLContext setCurrentContext:nil];
    [super dealloc];
}

- (void)update
{
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
    NSLog(@"%f, %f", self.view.center.x, self.view.center.y);
    NSLog(@"%f, %f", self.view.bounds.size.width, self.view.bounds.size.height);
    NSSet *allTouches = [event allTouches];    //返回与当前接收者有关的所有的触摸对象
    UITouch *touch = [allTouches anyObject];   //视图中的所有对象
    CGPoint point = [touch locationInView:[touch view]]; //返回触摸点在视图中的当前坐标
    int x = point.x;
    int y = point.y;
    CGFloat scale = [UIScreen mainScreen].scale;
    CGRect screenRect = [[UIScreen mainScreen] bounds];
    CGSize screenSize = screenRect.size;
    double xsc = screenSize.width * scale / self.view.bounds.size.width;
    double ysc = screenSize.height* scale/ self.view.bounds.size.height;
    NSLog(@"touch pos: (%d, %d)", (int)(x * xsc), (int)(y * ysc));
}

- (void)setupGL
{
    if(![EAGLContext setCurrentContext:self.context])
    {
        NSLog(@"  setCurrentContext error");
    }
    memset(&_esContext, 0, sizeof(_esContext));
    InitScene(&_esContext);
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


@end
