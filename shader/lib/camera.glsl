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
	mat4 CAMERA_PROJ;
	mat4 CAMERA_VIEW;
    vec3 CAMERA_POS;
};


mat4 Engine_VP()
{
	return CAMERA_PROJ * CAMERA_VIEW;
}

mat4 INV_VIEW()
{
    return inverse(CAMERA_VIEW);
}


mat4 INV_PROJ()
{
    return inverse(CAMERA_PROJ);
}


mat4 Engine_MVP(mat4 model)
{
	return Engine_VP() * model;
}


vec3 Camera_WorldPos()
{
    return CAMERA_POS;
}


vec4 Camera_ViewPos()
{
    return CAMERA_VIEW * vec4(CAMERA_POS,1);
}


vec4 Camera_ProjPos()
{
    return Engine_VP() * vec4(CAMERA_POS,1);
}

#endif
