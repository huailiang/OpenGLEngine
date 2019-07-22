//
//  AlbumSource.h
//  GLESEngine
//
//  Created by 彭怀亮 on 7/22/19.
//  Copyright © 2019 彭怀亮. All rights reserved.
//

#ifndef AlbumSource_h
#define AlbumSource_h

#import <Foundation/Foundation.h>
#import <CoreMedia/CoreMedia.h>
#import "IARInterface.h"


@interface AlbumSource : NSObject
{
    NSData *imageData; // pick data
    float m_width, m_height;
    UIViewController *viewController;
}

-(void)startPicker: (PickerCallback) callback withArg:(void*) arg;

-(void)setController:(UIViewController*)controller;

-(void*)getImageData:(float*) width withHeight:(float*)height;

-(void)clearImageData;

@end




class iOSAR : public IARInterface
{
    
public:
    
    bool GetAlbumPicker(PickerCallback pick, void* arg);
    
    void OnDestroy() const;
    
    void* GetImageData(float* width, float* height) const;
    
    AlbumSource *source;
    
};


#endif /* AlbumSource_h */
