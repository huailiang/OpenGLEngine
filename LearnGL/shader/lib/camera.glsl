#ifndef _CAMERA_
#define _CAMERA_


/*
*
* 使用ubo 也是为了性能优化
* 这里主要存储了主摄像机的数据
*
*/


layout (std140) uniform Block
{
	mat4 cam_proj;
	mat4 cam_view;
    vec3 cam_pos;
};


mat4 Engine_VP()
{
	return cam_proj * cam_view;
}

mat4 INV_VIEW()
{
    return inverse(cam_view);
}


mat4 INV_PROJ()
{
    return inverse(cam_proj);
}


mat4 Engine_MVP(mat4 model)
{
	return Engine_VP() * model;
}


vec3 Camera_WorldPos()
{
    return cam_pos;
}


vec4 Camera_ViewPos()
{
    return cam_view * vec4(cam_pos,1);
}


vec4 Camera_ProjPos()
{
    return Engine_VP() * vec4(cam_pos,1);
}

#endif
