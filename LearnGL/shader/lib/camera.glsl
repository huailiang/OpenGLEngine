#ifndef _CAMERA_
#define _CAMERA_


/*
ubo is designed for performance
*/


/*
There is main camera's data
*/

layout (std140) uniform Camera
{
	mat4 cam_proj;
	mat4 cam_view;
	vec3 cam_pos;
};


vec3 Engine_ViewPos
{
	return cam_pos;
}


mat4 Engine_VP()
{
	return cam_proj * cam_view;
}

mat4 Engine_MVP(mat4 model)
{
	return Engine_VP() * model;
}




#endif
