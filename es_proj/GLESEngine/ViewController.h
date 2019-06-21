//
//  ViewController.h
//  GLESEngine
//
//  Created by 彭怀亮 on 5/30/19.
//  Copyright © 2019 彭怀亮. All rights reserved.
//

#import <GLKit/GLKit.h>


@interface ViewController : GLKViewController
{
    NSInteger _animationFrameInterval;
}

-(void)OnApplicationPause:(bool) pause;
-(void)OnApplicationQuit;

@end

