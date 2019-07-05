#version 330 core


/*
 * harmonics for realtime rendering:
 *  https://huailiang.github.io/blog/2019/harmonics/
 */

#include "lib/appdata.glsl"
#include "lib/camera.glsl"


uniform mat4 view_proj;
uniform mat4 model;


out VS_OUT{
    vec3 normal;
}vs;

void main(void)
{
    gl_Position = Engine_MVP(model) * vec4(aPos,1);
    vec3 world_normal = mat3(transpose(inverse(model))) * aNormal;
    vs.normal = normalize(world_normal);
}