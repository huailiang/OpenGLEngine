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
#define _Coord  layout (location = 1) in vec2 aTexCoords;
#define _Normal layout (location = 2) in vec3 aNormal;
#define _Color  layout (location = 2) in vec3 aColor;
#define _Tan    layout (location = 3) in vec3 aTangent;
#define _Bitan  layout (location = 4) in vec3 aBitangent;


/*
 * 可以直接使用定义好的数据结构
 */
#define LayoutBaseVert2() _Pos2 _Coord

#define LayoutBaseVert3() _Pos3 _Coord

#define LayoutVertex() _Pos3 _Coord _Normal

#define LayoutColorVertex() _Pos3 _Coord _Color

#define LayoutCompxVertex() _Pos3 _Coord _Normal _Color


/*
 * 也可以直接使用组合type 定义顶点数据
 * 建议使用此方式， 可读性比较高
 */
#ifndef VERT_TYPE
    #define VERT_TYPE 0x0000
#endif

#if VERT_TYPE & Vt_Pos3
    _Pos3
#endif

#if VERT_TYPE & Vt_Pos2
    _Pos2
#endif

#if VERT_TYPE & Vt_UV
    _Coord
#endif

#if VERT_TYPE & Vt_Normal
    _Normal
#endif

#if VERT_TYPE & Vt_Color
    _Color
#endif

#if VERT_TYPE & Vt_TAN
    _Tan
#endif

#if VERT_TYPE & Vt_BIT
    _Bitan
#endif



#undef VERT_TYPE


#endif // _APP_DATA_
