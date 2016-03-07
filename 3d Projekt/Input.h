#pragma once
#define DIRECTINPUT_VERSION 0x0800
#include "Linker.h"
#include "Camera.h"
class Input
{
private:

	bool mouseHidden;
	bool* miniMap;
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
	HWND* hwndP;
public:
	Input();
	
	bool initialize(HINSTANCE* hinstance, HWND* hwnd, Camera* camera,bool* miniMap);
	void Shutdown();
	bool frame();
	~Input();
};

