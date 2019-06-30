//
//  texmgr_h
//  OpenGLEngine
//
//  Created by 彭怀亮 on 6/5/19.
//  Copyright © 2019 彭怀亮. All rights reserved.
//

#ifndef texmgr_h
#define texmgr_h

#include "texture.h"
#include "asset.h"


using namespace std;


namespace engine
{

    /*
     * texture manager
     * store all texture loaded, to avoid load repeat
     */
    
    class TexMgr
    {
        
        DeclareSington(TexMgr)
        
                
    public:
        
        Texture* FindTexture(GLuint& texid);
    
        bool FindTexture(GLuint& texid, size_t& idx);
        
        bool FindTexture(string& texture, GLuint& texid, size_t& idx);
        
        void LoadTex(const char* path, bool flipY, Texture* tex);
        
        bool RemvTexture(GLuint texid);
        
        bool RemvTexture(std::string& texture);
        
        void UnloadAllTexture();
        
        void SetTextureFormat(GLenum target, GLint filter, GLint wrap, GLint align=1);
        
    private:
        
        GLuint LoadCubemap(std::string cubepath, Texture* tex);
        
        GLuint LoadTexture(const char* path,bool flipY, Texture* tex);
        
        void RealRemove(size_t idx);
        
        void Free(void* data,EXT ext);
        
        void* RealLoad(const char* path,int *width, int *height,EXT ext, GLenum* format, int* level, int* bitsPerPixel);
        
        GLenum GetFormat(int format);
        
    private:
        vector<Texture> _texids;
        vector<size_t> _references;
        size_t _num_tex;
        
    };
    

}
#endif /* texmgr_h */
