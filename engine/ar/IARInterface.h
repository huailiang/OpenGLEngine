//
//  IARInterface.h
//  GLESEngine
//
//  Created by 彭怀亮 on 7/22/19.
//  Copyright © 2019 彭怀亮. All rights reserved.
//

#ifndef IARInterface_h
#define IARInterface_h


struct IARInterface
{
    
    virtual bool GetAlbumPicker() const = 0;
    
    virtual void OnDestroy() const = 0;
    
    virtual void* GetImageData(float* width, float* height) const = 0;
    
};



#endif /* IARInterface_h */
