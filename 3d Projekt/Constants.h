#pragma once
#define WINDOW_WIDTH 800.0f
#define WINDOW_HEIGHT 600.0f
#define WINDOWED true
#define WIREFRAME false
#define VSYNC 1 //1 == ON, 0 = OFF
#define MAX_TRIANGLES 10000
#define SCREEN_DEPTH 10000.0f


#define PAD 0.0f
#define PI 3.14159265359f
#define SENSITIVITY 1.0f
#define CAMERA_SPEED 0.5f

#define toRadian(degrees) ((degrees)* (PI/180.0f))

enum Primitives{ PLANE , CUBE , PYRAMID, OBJ};
enum lightTypes{ POINTLIGHT }; //add more light types here later
enum moveDirection{ FORWARD,BACKWARD,LEFT,RIGHT};
enum ShaderTypes 
{
	TEXTURESHADER,
	COLORSHADER, 
	CUBEMAPSHADER,
	SKYBOXSHADER,
	TERRAINSHADER,
	OVERLAYSHADER,
	ANIMATIONSHADER};


/*
Shader Resources.

Note: This is subject of change. As we start implementing maps like normal maps, and hight maps 
that needs to be updated with each individual model, we will have to keep those indices close to eachother.

Eventually, all the static resources such as the sky map, will have an index further back.

The index is as of now : (Update this if anything is changed!)

0 = is the index that is being replaces each object with the objects texture map.
1 = is the skymap texture, It is set on initialization and should (for now) not be changed during run time.
2 = This is the NormalMap
3 = the cubemap for the dynamic cube map
4 = Is the render to texture resource(USED AS MINIMAP at the moment)
5 = This is the ui overlay texture. Add a cool ui here! :)

*/

