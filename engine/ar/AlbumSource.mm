//
//  AlbumSource.m
//  GLESEngine
//
//  Created by 彭怀亮 on 7/22/19.
//  Copyright © 2019 彭怀亮. All rights reserved.
//

#import "AlbumSource.h"
#import <Photos/Photos.h>
#import <AVFoundation/AVFoundation.h>

// _OBJ_C_
@interface AlbumSource() <UINavigationControllerDelegate, UIImagePickerControllerDelegate>
{
    PickerCallback m_callback;
    void* cb_arg;
}
-(void)imagePickerControllerDidCancel:(UIImagePickerController *)picker;

-(void)imagePickerController:(UIImagePickerController *)picker didFinishPickingMediaWithInfo:(NSDictionary<NSString *,id> *)info;

@end

@implementation AlbumSource

-(void)setController:(UIViewController*)controller
{
    viewController = controller;
}

-(void)startPicker : (PickerCallback) callback withArg:(void*) arg;
{
    if (![UIImagePickerController isSourceTypeAvailable:UIImagePickerControllerSourceTypePhotoLibrary]) return;
    m_callback = callback;
    cb_arg = arg;
    UIImagePickerController *picker = [[UIImagePickerController alloc] init];
    picker.delegate = self;
    picker.allowsEditing = YES;
    picker.sourceType = UIImagePickerControllerSourceTypePhotoLibrary;
    
    [viewController presentViewController:picker animated:YES completion:nil];
}

-(void)imagePickerControllerDidCancel:(UIImagePickerController *)picker
{
    NSLog(@"pick canceled");
    [picker dismissViewControllerAnimated:YES completion:nil];
}

-(void)imagePickerController:(UIImagePickerController *)picker didFinishPickingMediaWithInfo:(NSDictionary<NSString *,id> *)info
{
    UIImage *image = [info objectForKey:UIImagePickerControllerEditedImage];
    NSLog(@"pick start width %f, height: %f", image.size.width, image.size.height);
    m_width = image.size.width;
    m_height = image.size.height;
    imageData = UIImageJPEGRepresentation(image, 0.5f);
    [picker dismissViewControllerAnimated:YES completion:nil];
    if(m_callback) m_callback(cb_arg);
}

-(void*)getImageData:(float*) width withHeight:(float*)height
{
    *width = m_width;
    *height = m_height;
    return (void*)[imageData bytes];
}

-(void)clearImageData
{
    [imageData dealloc];
    imageData =  nil;
}


// __cplusplus__
bool iOSAR::GetAlbumPicker(PickerCallback pick, void* arg)
{
    if(source != nil)
    {
        [source startPicker: pick withArg:arg];
    }
    return true;
}

void iOSAR::OnDestroy() const
{
    if(source != nil)
    {
        [source clearImageData];
    }
}

void* iOSAR::GetImageData(float* width, float* height) const
{
    if(source != nil)
    {
        return [source getImageData: width withHeight:height];
    }
    return 0;
}


@end
