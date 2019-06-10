#ifndef _APP_DATA_
#define _APP_DATA_

/*
* 这里的定义和mesh.h对应
* appdata的叫法是和unity里保持对应，好记吧
 */


#define Vt_Pos3    0x0001
#define Vt_Pos2    0x0002
#define Vt_UV      0x0010
#define Vt_TAN     0x0020
#define Vt_BIT     0x0030
#define Vt_Normal  0x0100
#define Vt_Color   0x1000


#define _Pos3   layout (location = 0) in vec3 aPos;
#define _Pos2   layout (location = 0) in vec2 aPos;
#define _Coord  layout (location = 1) in vec2 aTexcoord;
#define _Normal layout (location = 2) in vec3 aNormal;
#define _Color  layout (location = 2) in vec3 aColor;


#define LayoutBaseVert2() _Pos2 _Coord

#define LayoutBaseVert3() _Pos3 _Coord

#define LayoutVertex() _Pos3 _Coord _Normal

#define LayoutColorVertex() _Pos3 _Coord _Color

#define LayoutCompxVertex() _Pos3 _Coord _Normal _Color

#endif // _APP_DATA_
