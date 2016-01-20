#include "Input.h"



// ReadKeyboard will read the state of the keyboard into the keyboardState variable.
//The state will show any keys that are currently pressed or not pressed.
bool Input::ReadKeyboard()
{
	HRESULT result;
	//GetAsyncKeyState()
	result = keyboard->GetDeviceState(sizeof(keyboardState), (LPVOID)&keyboardState);
	if (FAILED(result))
	{

		//if keyboard lost focus or was not acquired, try to get control back
		if ((result == DIERR_INPUTLOST) || result == DIERR_NOTACQUIRED || result == E_ACCESSDENIED)
		{
			result = keyboard->Acquire();

		}
		else 
		{
			return false;
		}

	}

	return true;
}

bool Input::ReadMouse()
{
	HRESULT result;

	result = mouse->GetDeviceState(sizeof(DIMOUSESTATE), (LPVOID)&mouseState);
	if (FAILED(result))
	{
		// If the mouse lost focus or was not acquired then try to get control back.
		if ((result == DIERR_INPUTLOST) || (result == DIERR_NOTACQUIRED))
		{
			mouse->Acquire();
		}
		else
		{
			return false;
		}
	}

	return true;



}
Input::Input()
{
	keyboard = 0;
	mouse = 0;
	input = 0;
	mouseX = 0.0f;
	mouseY = 0.0f;

}

bool Input::initialize(HINSTANCE* hinstance, HWND* hwnd,Camera* camera)
{
	this->camera = camera;
	HRESULT hr;
	//Initialize the input interface
	hr = DirectInput8Create(*hinstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&input, NULL);
	if (FAILED(hr))
	{
		return false;
	}

	//Start by intialise the interface for the keyboard
	hr = input->CreateDevice(GUID_SysKeyboard, &this->keyboard, NULL);
	if (FAILED(hr))
	{
		return false;
	}

	keyboard->SetDataFormat(&c_dfDIKeyboard);
	if (FAILED(hr))
	{
		return false;
	}

	hr = keyboard->SetCooperativeLevel(*hwnd, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE);

	if (FAILED(hr))
	{
		return false;
	}

	hr = keyboard->Acquire();
	if (FAILED(hr))
	{
		return false;
	}




	hr = input->CreateDevice(GUID_SysMouse, &this->mouse, NULL);
	if (FAILED(hr))
	{
		return false;
	}
	
	hr= mouse->SetDataFormat(&c_dfDIMouse);
	if (FAILED(hr))
	{
		return false;
	}

	hr = mouse->SetCooperativeLevel(*hwnd, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE);
	if (FAILED(hr))
	{
		return false;
	}

	hr = mouse->Acquire();
	if (FAILED(hr))
	{
		return false;
	}
	return true;

	//DIPROPDWORD dipdw;
	//
	//dipdw.diph.dwSize = sizeof(DIPROPDWORD);
	//dipdw.diph.dwHeaderSize = sizeof(DIPROPHEADER);
	//dipdw.diph.dwObj = 0;
	//dipdw.diph.dwHow = DIPH_DEVICE;
	//dipdw.dwData = 10;
	//hr = keyboard->SetProperty(DIPROP_BUFFERSIZE, &dipdw.diph);


	//hr = mouse->SetProperty(DIPROP_BUFFERSIZE, &dipdw.diph);

	////check https://msdn.microsoft.com/en-us/library/windows/desktop/ee416850(v=vs.85).aspx


}
bool Input::frame()
{
	bool result;
	result = ReadKeyboard();
	if (!result)
	{
		return false;
	}

	result = ReadMouse();
	if (!result)
	{
		return false;
	}
	ProcessInput();
	return true;

	//Might have to change this one later, It feels like it will quit if not BOTH are used at the same time



}
void Input::Shutdown()
{
	if (mouse)
	{
		mouse->Unacquire();
		mouse->Release();
		mouse = 0;
	}
	if (keyboard)
	{
		keyboard->Unacquire();
		keyboard->Release();
		keyboard = 0;
	}

	if (input)
	{
		input->Release();
		input = 0;

	}
	return;

}

void Input::ProcessInput()
{
	// Update the location of the mouse cursor based on the change of the mouse location during the frame.
	mouseX = (mouseState.lX - (WINDOW_WIDTH/2));
	mouseY = (mouseState.lY - (WINDOW_HEIGHT/2));

	
	if (keyboardState[DIK_W])
	{

		camera->move(FORWARD);
	}
	if (keyboardState[DIK_S])
	{

		camera->move(BACKWARD);
	}
	if (keyboardState[DIK_D])
	{

		camera->move(RIGHT);
	}
	if (keyboardState[DIK_A])
	{

		camera->move(LEFT);
	}
	if (keyboardState[DIK_UP])
	{
		camera->rotatePitch(0.000001f);
	}


	//mouseState.lX = WINDOW_WIDTH / 2;
	//mouseState.lY = WINDOW_WIDTH / 2;
	//Makesure it stays inside the screen
	if (mouseX < 0) { mouseX = 0; };
	if (mouseY < 0) { mouseY = 0; };

	if (mouseX > WINDOW_WIDTH) { mouseX = WINDOW_WIDTH; };
	if (mouseY > WINDOW_WIDTH) { mouseY = WINDOW_HEIGHT; };

	DirectX::XMFLOAT3 test = { mouseX,mouseY,0.0f };
	//camera->setViewLookAt(test);
	//camera->rotateYaw(mouseX);
	//camera->rotatePitch(mouseY);
	//return;


}

Input::~Input()
{
}
