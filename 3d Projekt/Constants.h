#pragma once
#define WINDOW_WIDTH 800.0f
#define WINDOW_HEIGHT 600.0f
#define WINDOWED true
#define WIREFRAME false
#define VSYNC true



#define PAD 0.0f
#define PI 3.14159265359f
#define SENSITIVITY 0.01f
#define CAMERA_SPEED 1.0f

#define toRadian(degrees) ((degrees)* (PI/180.0f))

enum Primitives{ PLANE , CUBE , PYRAMID, OBJ};
enum lightTypes{ POINTLIGHT }; //add more light types here later
enum moveDirection{ FORWARD,BACKWARD,LEFT,RIGHT};



