#import <Foundation/Foundation.h>
#import <CoreMedia/CoreMedia.h>
#import <AVFoundation/AVFoundation.h>

#include "BGRAVideoFrame.h"
#include "CameraCalibration.hpp"

@protocol VideoSourceDelegate<NSObject>

-(void)frameReady:(BGRAVideoFrame) frame;

@end

@interface VideoSource : NSObject<AVCaptureVideoDataOutputSampleBufferDelegate>
{
}

@property (nonatomic, retain) AVCaptureSession *captureSession;
@property (nonatomic, retain) AVCaptureDeviceInput *deviceInput;
@property (nonatomic, retain) id<VideoSourceDelegate> delegate;

- (bool) startWithDevicePosition:(AVCaptureDevicePosition)devicePosition;
- (CameraCalibration) getCalibration;
- (CGSize) getFrameSize;

@end
