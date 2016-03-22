#pragma once
#include "Linker.h"
//http://www.braynzarsoft.net/viewtutorial/q16390-27-loading-an-md5-model
//http://www.braynzarsoft.net/viewtutorial/q16390-28-skeletal-animation-based-on-the-md5-format
class md5Model
{


private:
	struct ModelSubset {
		/*
		Each subset of our model will get their own one of these structures.
		It will have it's own vertex and index buffer. This is because when we animate 
		we don't have to update a buffer for the whole mesh. only the subsets that have changed.
		*/

	int texArrayIndex;
	int numTriangles;

	std::vector<AnimVertex> vertices;
	std::vector<DWORD> indices; // 32-bit unsigned int.
	std::vector<Weight> weights;

	std::vector<position> positions;

	ID3D11Buffer* vertBuff = nullptr;
	ID3D11Buffer* indexBuff = nullptr;

	void Release() { 
		if(vertBuff !=nullptr)
			this->vertBuff->Release(); 
		if (indexBuff != nullptr)
			this->indexBuff->Release(); }
	};

	struct BoundingBox
	{
		DirectX::XMFLOAT3 min;
		DirectX::XMFLOAT3 max;
	};
	
	struct AnimJointInfo
	{
		std::wstring name;
		int parentID;
		int flags;
		int startIndex;

	};

	struct FrameData
	{
		int frameID;
		std::vector<float> frameData;
	};
	struct ModelAnimation
	{
		int numFrames;
		int numJoints;
		int frameRate;
		int numAnimatedComponents;

		float frameTime;
		float totalAnimTime;
		float currAnimTime;

		std::vector<AnimJointInfo> jointInfo;
		std::vector<BoundingBox> frameBounds;
		std::vector<Joint> baseFrameJoints;
		std::vector<FrameData> frameData;
		std::vector<std::vector<Joint>> frameSkeleton;
	
	};
	int numSubsets;
	int numJoints;

	std::vector<Joint> joints;
	std::vector<ModelSubset> subsets;
	std::vector<ModelAnimation> animations;

	
	std::vector<std::wstring> texFileNameArray;
	
	DirectX::XMFLOAT4X4 worldMatrix;

	std::vector<ID3D11ShaderResourceView*> textures;
	
	//This is a pointer to a deviceContext, we will store the adress to the main device here.
	//since we use it alot, we shall avoid putting it in all the functions.
	ID3D11DeviceContext* gDeviceContext;



	worldConstantBuffer* worldStruct = nullptr;
	ID3D11Buffer* worldBuffer = nullptr; //this is a pointer to the constant buffer, sent from the engine
	

	void sendToConstantBuffer();
public:
	md5Model();
	bool Init(ID3D11DeviceContext* context, ID3D11Device* gDevice,ID3D11Buffer* worldbuffer);
	bool loadModel(ID3D11Device* gDevice);
	bool loadAnimation();
	~md5Model();

	void update(float deltaTime,int animation);
	void render();

	void Release();
};

