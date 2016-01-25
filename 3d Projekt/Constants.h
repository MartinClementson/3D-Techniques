#pragma once
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define WINDOWED true
#define PAD 0.0f
#define PI 3.14159265359f
#define SENSITIVITY 0.001f
#define CAMERA_SPEED 0.001f

#define toRadian(degrees) ((degrees)* (PI/180.0f))

enum Primitives{ PLANE , CUBE , PYRAMID, OBJ};
enum lightTypes{ POINTLIGHT }; //add more light types here later
enum moveDirection{ FORWARD,BACKWARD,LEFT,RIGHT};



