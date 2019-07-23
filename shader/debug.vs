#version 330 core

#include "lib/appdata.glsl"

/*
 * engine depth draw seq:
 *   skybox(1)-> ar background(0.996)-> 3d items->debug(-0.97)-> button background(-0.98)-> font(-0.99)
 */


#ifdef _AR_
uniform mat4 view;
uniform mat4 proj;
#endif

out vec2 TexCoords;

void main()
{
    TexCoords = aTexCoords;
    
#ifdef _AR_ //这里画虚拟物品
    vec3 pos = vec3(aPos.x * 0.4, aPos.y * 0.4, aPos.z * 0.4);
    gl_Position = proj * view *  vec4(pos, 1.0);
#else
#ifdef _AR_BG_
    gl_Position = vec4(aPos, 0.9996, 1.0);  //画AR背景， 0.996只比skybox（z=1）往前一点
#else
    gl_Position = vec4(aPos, -0.97, 1.0);  //用来debug
#endif
#endif
}
