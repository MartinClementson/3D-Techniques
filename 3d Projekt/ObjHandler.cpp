#include "ObjHandler.h"
#include <fstream>
#include <iostream>
#include <string>

#include "Linker.h"
#include "DataTypes.h"

using namespace std;
ObjHandler::ObjHandler()
{
	
	
}

std::string ObjHandler::MtlHandler(std::string &filePath, std::string &material)
{
	string textureID;
	ifstream loading;
	loading.open(filePath);
	string line2, line3;
	if (!loading)
		std::cout << "\nfailed to load texturefile";
	else
	{
		while (!loading.eof())
		{
			if (line2 == material)
			{
				//input materials here later
				loading >> line3;
				if (line3 == "map_Kd")
				{
					loading >> textureID;
					line2 = "";
				}
			}
			else
				loading >> line2;
		}
	}
	loading.close();
	return textureID;
}

void ObjHandler::create(std::vector<Model*>** childrenArray, std::vector<Vertex>* modelVerts,
	std::string &textureName, ID3D11Device* gDevice, ID3D11DeviceContext * gDeviceContext,
	ID3D11Buffer * worldBuffer, worldConstantBuffer * worldStruct, int &count, std::vector<DirectX::XMFLOAT3> *uvCoord,
	std::vector<DirectX::XMFLOAT3> *vCoord, std::vector<DirectX::XMINT3> *testIn, int &offset, bool &father,  std::vector<UINT> &indices)
{
	Vertex Coordinates;

	if (father)
	{
		bool existWithinVerts = false;
		UINT indexCounter = 0;
		//for (int i = 0; i < vCoord->size(); i++)
		//{
		//	Coordinates.x = vCoord->at(i).x;
		//	Coordinates.y = vCoord->at(i).y;
		//	Coordinates.z = vCoord->at(i).z;

		//	Coordinates.u = 1.0f;
		//	Coordinates.v = 1.0f;

		//	modelVerts->push_back(Coordinates);
		//}
		//for (int i = 0; i < testIn->size(); i++)
		//{
		//	/*Coordinates.x = vCoord->at((testIn->at(i).x - 1)).x;
		//	Coordinates.y = vCoord->at((testIn->at(i).x - 1)).y;
		//	Coordinates.z = vCoord->at((testIn->at(i).x - 1)).z;


		//	Coordinates.r = PAD;
		//	Coordinates.g = PAD;
		//	Coordinates.b = PAD;


		//	Coordinates.u = uvCoord->at((testIn->at(i).y - 1)).x;
		//	Coordinates.v = uvCoord->at((testIn->at(i).y - 1)).y;*/


		//	indices.push_back(testIn->at(i).x-1);
		//	//modelVerts->push_back(Coordinates);
		//	offset++;
		//}
		for (UINT i = 0; i < testIn->size(); i++)
		{
			//testing a theory
			Coordinates.x = vCoord->at((testIn->at(i).x - 1)).x;
			Coordinates.y = vCoord->at((testIn->at(i).x - 1)).y;
			Coordinates.z = vCoord->at((testIn->at(i).x - 1)).z;


			Coordinates.r = PAD;
			Coordinates.g = PAD;
			Coordinates.b = PAD;


			Coordinates.u = uvCoord->at((testIn->at(i).y - 1)).x;
			Coordinates.v = uvCoord->at((testIn->at(i).y - 1)).y;


			if (modelVerts != nullptr)
			{
				for (UINT j = 0; j < modelVerts->size(); j++)
				{
					if (modelVerts->at(j).x == Coordinates.x &&
						modelVerts->at(j).y == Coordinates.y &&
						modelVerts->at(j).z == Coordinates.z &&
						modelVerts->at(j).u == Coordinates.u &&
						modelVerts->at(j).v == Coordinates.v) //add the rest of the variables
					{
						existWithinVerts = true;
						indices.push_back(j);
					}
				}
				if (!existWithinVerts)
				{
					modelVerts->push_back(Coordinates);
					indices.push_back(indexCounter);
					indexCounter++;
				}
			}
			else
			{
				modelVerts->push_back(Coordinates);
				indices.push_back(indexCounter); //have to fix custom index
				indexCounter++;
			}
			existWithinVerts = false;
			offset++;
		}
		father = false;
		count = 0;
	}
	else
	{
		if (childrenArray[0] == nullptr)
			childrenArray[0] = new vector<Model*>;
		std::vector<Vertex> sendCoordinates;
		for (int i = 0; i < count; i++)
		{

			Coordinates.x = vCoord->at((testIn->at(offset).x - 1)).x;
			Coordinates.y = vCoord->at((testIn->at(offset).x - 1)).y;
			Coordinates.z = vCoord->at((testIn->at(offset).x - 1)).z;

			Coordinates.u = uvCoord->at((testIn->at(offset).y - 1)).x;
			Coordinates.v = uvCoord->at((testIn->at(offset).y - 1)).y;


			sendCoordinates.push_back(Coordinates);

			offset++;
		}

		childrenArray[0]->push_back(new Model(&sendCoordinates, &textureName, gDevice, gDeviceContext, worldBuffer, worldStruct, indices));
		count = 0;
	}
}

ObjHandler::ObjHandler(std::vector<Model*>** childrenArray,std::string filePath, std::vector<Vertex>* modelVerts, std::string &textureName,
	ID3D11Device* gDevice, ID3D11DeviceContext * gDeviceContext,
	ID3D11Buffer * worldBuffer, worldConstantBuffer * worldStruct,
	 std::vector<UINT> &indices)
{

#pragma region Description
	///////////////////////////////////////////////////////
	
	
	/*This function gets everything it needs to create children to the parent
	
	
	suggestion:
	check if there is more than one object in the obj. If there is only one, there is no need for the children,
	(perhaps check after the first object has been read from the file? if it was alone. peek() or sumthing)
	
	If there is more than one object. the childrenArray must be initialized like this
	*childrenArray =  new std::vector<Model*>;  (The deletion is already made in the model destructor)
	then used with pushback
	
	Something like this. The model constructor for the children is below

	*childrenArray->push_back(new Model(
		std::vector<Vertex>* vertArray,		<--- a pointer to the array of vertices, gathered from the obj.
		std::string * texturePath,			<--- a pointer to the string, containing the texture file name.
		ID3D11Device* gDevice,					|
		ID3D11DeviceContext * gDeviceContext,	|<--- send the rest of these parameters in, you have them from the objHandler constructor (that we are in)
		ID3D11Buffer * worldBuffer,				|
		worldConstantBuffer * worldStruct))		| worldConstantBuffer  is the name of the struct  data type




	Furthermore David_coolbro. if there is only one object in the file, try to do it like before.

	if it's to hard to do, create it as a child. and let the parent be abstract

	IF the parent is abstract we might need to add more checks in the model class, for example not to release buffers when they don't exist
	and not to create any buffers or load textures when this function we're in now returns back to the parent object.


	How it all works when rendering

	This is how it looks like after import. all the children are parts of the obj, but separate meshes (except for child1_1)
	(all are model objects) 
	Parent
		|-child1
		|	|
		|	|-child1_1 <-this will not occur with obj import, but might in the future if "parenting different objects" will be implemented
		|
		|-child2
		|-child3
		|-child4

Engine->render()
    |
    |  
	|-Parent->render()							   //call update function .
		|
		|- Parent->update()						   //update rotation/animation, etc , if there are any children, Render them.
			|
			|-Parent->renderChildren()			   //Give the children the matrix of the parent and call their render function
				|
				|- child1 ->render()			   //Do the same as the parent has done up until now.
				|	|
				|	|-child1->update()
				|		|
				|		|-child1->renderChildren() //This is a special case. not gonna happen with this obj importer
				|			|
				|			|- child1_1->()render		
				|
				|
				|- child2 ->render()
				|
				|- child3->render()
				|
				|- child4->render()
	

	After the child4 has rendered. it will go back up to the parent and render it last
	
	
	
	
	*/

#pragma endregion Explaination of function





	
	vector<DirectX::XMINT3> testIn;
	DirectX::XMINT3 index;


	std::string mtlLib = "";

	vector<Vertex> vNCoord;
	vector<DirectX::XMFLOAT3> uvCoord, vCoord;
	Vertex normIn;
	DirectX::XMFLOAT3 uvIn, vecIn;
	bool father = true;

	int count = 0, offset = 0;
	string line2;
	ifstream loading;
	loading.open(filePath);
	if (!loading)
		std::cout << "\nFailed to load file";
	else
	{
		try
		{
			while (!loading.eof())
			{
				loading >> line2;
				if (line2 == "mtllib")
				{
					loading >> mtlLib;
				}
				if (line2 == "usemtl")
				{
					std::string tempString;
					loading >> tempString;
					textureName = MtlHandler(mtlLib, tempString);
				}
				if (line2 == "v")
				{
					loading >> vecIn.x;
					loading >> vecIn.y;
					loading >> vecIn.z;

					vCoord.push_back(vecIn);
				}
				if (line2 == "vt")
				{
					loading >> uvIn.x;
					loading >> uvIn.y;
					uvIn.z = 1.0f;
					uvCoord.push_back(uvIn);
				}
				/*if (line2 == "vn")
				{
					loading >> normIn.x;
					loading >> normIn.y;
					loading >> normIn.z;
					vNCoord.push_back(normIn);
					count++;
				}*/
				if (line2 == "f")
				{
					loading.ignore();
					//for (int i = 0; i < 3; i++) //some .obj files have 4 faces, make a while loops that peeks the next character and bases the loop on that
						if (loading.peek() != ' ' && loading.peek() != '/')
						{
							for (int x = 0; x < 3; x++)
							{
								loading >> index.x;
								loading.ignore();
								loading >> index.y;
								loading.ignore();
								loading >> index.z;
								testIn.push_back(index);
								loading.ignore();
								count++;
								if (loading.peek() == 'g' || loading.eof())
								{
									create(childrenArray, modelVerts, textureName, gDevice, gDeviceContext, worldBuffer,
										worldStruct, count, &uvCoord, &vCoord, &testIn, offset, father, indices);
								}
							}
						}
						else
							loading.ignore();
				}
			}
		}
		catch (...)
		{
			throw;
		}
	}
	loading.close();
}




ObjHandler::~ObjHandler()
{
}
