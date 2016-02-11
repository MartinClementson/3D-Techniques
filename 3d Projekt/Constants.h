#pragma once
#define WINDOW_WIDTH 800.0f
#define WINDOW_HEIGHT 600.0f
#define WINDOWED true
#define WIREFRAME false
#define VSYNC 1 //1 == ON, 0 = OFF



#define PAD 0.0f
#define PI 3.14159265359f
#define SENSITIVITY 1.0f
#define CAMERA_SPEED 0.5f

#define toRadian(degrees) ((degrees)* (PI/180.0f))

enum Primitives{ PLANE , CUBE , PYRAMID, OBJ};
enum lightTypes{ POINTLIGHT }; //add more light types here later
enum moveDirection{ FORWARD,BACKWARD,LEFT,RIGHT};
enum ShaderTypes { TEXTURESHADER, COLORSHADER, REFLECTIONSHADER,SKYBOXSHADER,PARTICLESHADER };


