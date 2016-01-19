#pragma once
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define WINDOWED true
#define PAD 0.0f
#define PI 3.14159265359f

#define toRadian(degrees) ((degrees)* (PI/180.0f))

enum Primitives{ PLANE , CUBE , PYRAMID};
enum lightTypes{ POINTLIGHT }; //add more light types here later