//
//  AppDelegate.h
//  GLESEngine
//
//  Created by 彭怀亮 on 5/30/19.
//  Copyright © 2019 彭怀亮. All rights reserved.
//

#import <UIKit/UIKit.h>


@class ViewController;

@interface AppDelegate : UIResponder <UIApplicationDelegate>

@property (strong, nonatomic) UIWindow *window;

@property (nonatomic, retain) IBOutlet ViewController *viewController;

@end

