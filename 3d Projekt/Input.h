#pragma once
#define DIRECTINPUT_VERSION 0x0800
#include "Linker.h"
#include "Camera.h"
class Input
{
private:

	Camera* camera;
	LPDIRECTINPUT8 input;
	LPDIRECTINPUTDEVICE8 keyboard;
	LPDIRECTINPUTDEVICE8 mouse;
	unsigned char keyboardState[256];
	DIMOUSESTATE mouseState;
	float mouseX, mouseY;
	float lastMouseX, lastMouseY;
	void ProcessInput();
	bool ReadKeyboard();
	bool ReadMouse();
public:
	Input();
	
	bool initialize(HINSTANCE* hinstance, HWND* hwnd, Camera* camera);
	void Shutdown();
	bool frame();
	~Input();
};

