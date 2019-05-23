### A effeciant render engine for rendering

Running on the macos

## Library:
1. openGL
2. glad
3. grewf
4. freetype
5. glm


## Feature

1. support include library in glsl

```c
#include "lib/brdf.glsl"
```

2. support define glsl macro in cpp

if you used macro like this in your glsl:

```c
#ifdef _CALLBACK__ 
for(int i=0;i<LIGHT_NUM;i++)
{
	[...]
}
#else
[...]
#endif
```

you can generate kinds of varances before compile in c++:

```cpp
Shader("model.vs","outline.fs",nullptr, Macro("_CALLBACK__","","LIGHT_NUM","4")
```


You can review synthetic shader in /temp/ folder.

## Light 

#define _SpotLight_ in main.cpp & at main.fs 
- spotlight

<br><img src='image/spot.jpg'> <br>

#define _PointLight_ in main.cpp & at main.fs   
- pointlight

<br><img src='image/point.jpg'> <br>

none to define 
-directligt

<br><img src='image/direct.jpg'> <br>

penghuailiang
2019-05-10


