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
//calculating the tangent for the normal map
void ObjHandler::calculateOBJTangents(std::vector<Vertex> &vertices, std::vector<UINT> &indices)
{
	for (int i = 0; i < indices.size(); i += 3)
	{
		//making temporary variables for simpler use
		int i0 = indices.at(i);
		int i1 = indices.at(i + 1);
		int i2 = indices.at(i + 2);

		//Calculating the edges of the triangle
		DirectX::XMFLOAT3 edge1 = 
		{
			(vertices.at(i1).x - vertices.at(i0).x),
			(vertices.at(i1).y - vertices.at(i0).y),
			(vertices.at(i1).z - vertices.at(i0).z) 
		};
		DirectX::XMFLOAT3 edge2 =
		{
			(vertices.at(i2).x - vertices.at(i0).x),
			(vertices.at(i2).y - vertices.at(i0).y),
			(vertices.at(i2).z - vertices.at(i0).z)
		};

		//calculating delta values for the multiplication
		float deltaU1 = vertices.at(i1).u - vertices.at(i0).u;
		float deltaV1 = vertices.at(i1).v - vertices.at(i0).v;
		float deltaU2 = vertices.at(i2).u - vertices.at(i0).u;
		float deltaV2 = vertices.at(i2).v - vertices.at(i0).v;

		//calculating denominator
		float den = 1.0f / (deltaU1*deltaV2 - deltaU2*deltaV1);

		Vertex temp;
		temp.tangent.x = (den * (deltaV2*edge1.x - deltaV1*edge2.x));
		temp.tangent.y = (den * (deltaV2*edge1.y - deltaV1*edge2.y));
		temp.tangent.z = (den * (deltaV2*edge1.z - deltaV1*edge2.z));

		for (int j = i0; j <= i2; j++)
		{
			if (vertices.at(j).sharedTangents != nullptr)
				vertices.at(j).sharedTangents->push_back(temp.tangent);
			else
			{
				vertices.at(j).createTangent();
				vertices.at(j).sharedTangents->push_back(temp.tangent);
			}
		}
	}
	for (int i = 0; i < vertices.size(); i++)
	{
		//interpolating all the tangents that share the same vertex
		vertices.at(i).interpolateTangents();
		DirectX::XMVECTOR temp;
		DirectX::XMVECTOR tempTangent = { (vertices.at(i).tangent.x), (vertices.at(i).tangent.y), (vertices.at(i).tangent.z) };
		DirectX::XMVECTOR tempNormal = { (vertices.at(i).nx), (vertices.at(i).ny), (vertices.at(i).nz) };

		//calculating using the Gram-schmidt thing, to orthonormalize
		temp = (tempTangent - tempNormal * DirectX::XMVector3Dot(tempNormal, tempTangent));
		temp = DirectX::XMVector3Normalize(temp);

		//returning the recalculated tangent to its rightful owner
		vertices.at(i).tangent.x = temp.m128_f32[0];
		vertices.at(i).tangent.y = temp.m128_f32[1];
		vertices.at(i).tangent.z = temp.m128_f32[2];
	}
}
void ObjHandler::MtlHandler(std::string &filePath, std::vector<Material> &objMaterials)
{
	string textureID;
	ifstream loading;
	loading.open(filePath);
	string line2;
	Material tempMat;
	if (!loading)
		std::cout << "\nfailed to load texturefile";
	else
	{
		tempMat.mtlName = "";
		while (!loading.eof())
		{
			loading >> line2;
			if (line2 == "newmtl")
			{
				if (tempMat.mtlName == "")
					loading >> tempMat.mtlName;
				else
				{
					objMaterials.push_back(tempMat);
					loading >> tempMat.mtlName;
				}
			}
			else if (line2 == "Kd")
			{
				loading.ignore(); //skip a space
				loading >> tempMat.values.KD.x;
				loading >> tempMat.values.KD.y;
				loading >> tempMat.values.KD.z;
			}
			else if (line2 == "Ka")
			{
				loading.ignore(); // skip a space
				loading >> tempMat.values.KA.x;
				loading >> tempMat.values.KA.y;
				loading >> tempMat.values.KA.z;
			}
			else if (line2 == "Ks")
			{
				loading.ignore(); //skip a space
				loading >> tempMat.values.KS.x;
				loading >> tempMat.values.KS.y;
				loading >> tempMat.values.KS.z;
			}
			else if (line2 == "Ns")
			{
				loading.ignore();
				loading >> tempMat.values.NS;
			}
			else if (line2 == "map_Kd")
			{
				loading >> tempMat.fileName;
			}
		}
		objMaterials.push_back(tempMat);
	}
	loading.close();
}

void ObjHandler::create(std::vector<Model*>** childrenArray, std::vector<Vertex>* modelVerts,
	std::string &textureName, ID3D11Device* gDevice, ID3D11DeviceContext * gDeviceContext,
	ID3D11Buffer * worldBuffer, worldConstantBuffer * worldStruct, int &count, std::vector<DirectX::XMFLOAT3> *uvCoord,
	std::vector<DirectX::XMFLOAT3> *vCoord, std::vector<DirectX::XMINT3> *testIn, int &offset, bool &father,
	std::vector<UINT> &indices, std::vector<DirectX::XMFLOAT3> *vNCoord)
{
	Vertex Coordinates;

	if (father)
	{
		bool existWithinVerts = false;
		UINT indexCounter = 0;
		for (UINT i = 0; i < testIn->size(); i++)
		{
			Coordinates.x = vCoord->at((testIn->at(i).x - 1)).x;
			Coordinates.y = vCoord->at((testIn->at(i).x - 1)).y;
			Coordinates.z = vCoord->at((testIn->at(i).x - 1)).z;

			Coordinates.u = uvCoord->at((testIn->at(i).y - 1)).x;
			Coordinates.v = uvCoord->at((testIn->at(i).y - 1)).y;

			Coordinates.nx = vNCoord->at((testIn->at(i).z - 1)).x;
			Coordinates.ny = vNCoord->at((testIn->at(i).z - 1)).y;
			Coordinates.nz = vNCoord->at((testIn->at(i).z - 1)).z;

			if (modelVerts != nullptr)
			{
				for (UINT j = 0; j < modelVerts->size(); j++)
				{
					if (modelVerts->at(j).x == Coordinates.x &&
						modelVerts->at(j).y == Coordinates.y &&
						modelVerts->at(j).z == Coordinates.z &&
						modelVerts->at(j).u == Coordinates.u &&
						modelVerts->at(j).v == Coordinates.v &&
						modelVerts->at(j).nx == Coordinates.nx &&
						modelVerts->at(j).ny == Coordinates.ny &&
						modelVerts->at(j).nz == Coordinates.nz) //add the rest of the variables
					{
						existWithinVerts = true;
						indices.push_back(j);
						break;
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
				indices.push_back(indexCounter); 
				indexCounter++;
			}
			existWithinVerts = false;
			offset++;
		}
		calculateOBJTangents(*modelVerts, indices);
		father = false;
		count = 0;
	}
	else
	{
		if (childrenArray[0] == nullptr)
			childrenArray[0] = new vector<Model*>;
		std::vector<Vertex> sendCoordinates;
		bool existWithinVerts = false;
		UINT indexCounter = 0;
		std::vector<UINT> childIndices;
		for (int i = 0; i < count; i++)
		{

			Coordinates.x = vCoord->at((testIn->at(offset).x - 1)).x;
			Coordinates.y = vCoord->at((testIn->at(offset).x - 1)).y;
			Coordinates.z = vCoord->at((testIn->at(offset).x - 1)).z;

			Coordinates.u = uvCoord->at((testIn->at(offset).y - 1)).x;
			Coordinates.v = uvCoord->at((testIn->at(offset).y - 1)).y;

			Coordinates.nx = vNCoord->at((testIn->at(offset).z - 1)).x;
			Coordinates.ny = vNCoord->at((testIn->at(offset).z - 1)).y;
			Coordinates.nz = vNCoord->at((testIn->at(offset).z - 1)).z;

			if (sendCoordinates.size() != 0)
			{
				for (UINT j = 0; j < sendCoordinates.size(); j++)
				{
					if (sendCoordinates.at(j).x == Coordinates.x &&
						sendCoordinates.at(j).y == Coordinates.y &&
						sendCoordinates.at(j).z == Coordinates.z &&
						sendCoordinates.at(j).u == Coordinates.u &&
						sendCoordinates.at(j).v == Coordinates.v &&
						sendCoordinates.at(j).nx == Coordinates.nx &&
						sendCoordinates.at(j).ny == Coordinates.ny &&
						sendCoordinates.at(j).nz == Coordinates.nz) //add the rest of the variables
					{
						existWithinVerts = true;
						childIndices.push_back(j);
						break;
					}
				}
				if (!existWithinVerts)
				{
					sendCoordinates.push_back(Coordinates);
					childIndices.push_back(indexCounter);
					indexCounter++;
				}
			}
			else
			{
				sendCoordinates.push_back(Coordinates);
				childIndices.push_back(indexCounter);
				indexCounter++;
			}
			existWithinVerts = false;

			offset++;
		}
		
		childrenArray[0]->push_back(new Model(&sendCoordinates, &textureName, gDevice, gDeviceContext, worldBuffer, worldStruct, childIndices));
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
	vector<Material> objMaterial;

	//std::string mtlLib = "";

	vector<DirectX::XMFLOAT3> uvCoord, vCoord, vNCoord;
	DirectX::XMFLOAT3 uvIn, vecIn, normIn;
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
					std::string tempString;
					loading >> tempString;
					MtlHandler(tempString, objMaterial);
				}
				if (line2 == "usemtl")
				{
					std::string tempString;
					loading >> tempString;
					for (int i = 0; i < objMaterial.size(); i++)
					{
						if (objMaterial.at(i).mtlName == tempString)
						{
							textureName = objMaterial.at(i).fileName;
							break;
						}
					}
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
				if (line2 == "vn")
				{
					loading >> normIn.x;
					loading >> normIn.y;
					loading >> normIn.z;
					vNCoord.push_back(normIn);
				}
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
								if (loading.peek() == 'u')
								{
									create(childrenArray, modelVerts, textureName, gDevice, gDeviceContext, worldBuffer,
										worldStruct, count, &uvCoord, &vCoord, &testIn, offset, father, indices, &vNCoord);
								}
								if (loading.peek() == 'g' || loading.eof())
								{
									create(childrenArray, modelVerts, textureName, gDevice, gDeviceContext, worldBuffer,
										worldStruct, count, &uvCoord, &vCoord, &testIn, offset, father, indices, &vNCoord);
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
