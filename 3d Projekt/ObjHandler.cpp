#include "ObjHandler.h"
#include <fstream>
#include <vector>
#include <iostream>
#include <string>

using namespace std;
ObjHandler::ObjHandler()
{
}

ObjHandler::ObjHandler(std::string filePath, std::vector<Vertex>* modelVerts)
{
	//Recieve a string to the file path,
	//Recieve a pointer to the model class vertices array

	vector<DirectX::XMINT3> testIn;
	DirectX::XMINT3 index;

	//big variable that everything goes into
	Vertex Coordinates;


	vector<Vertex> vNCoord;
	vector<DirectX::XMFLOAT3> uvCoord, vCoord;
	Vertex normIn;
	DirectX::XMFLOAT3 uvIn, vecIn;

	int count = 0;
	//string fileName = "test.obj";
	string line2;
	ifstream loading;
	loading.open(filePath);
	std::istringstream inputString();
	if (!loading)
		std::cout << "\nFailed to load file";
	else
	{
		//int tempIndex[3];
		int tempIndex;
		try
		{
			while (!loading.eof())
			{
				loading >> line2;
				if (line2 == "v")
				{
					loading >> vecIn.x;
					loading >> vecIn.y;
					loading >> vecIn.z;
					//vecIn.pad = PAD;
					
					//temp <-----------------------------
					/*vecIn.r = 0.0f;
					vecIn.g = 1.0f;
					vecIn.b = 0.0f;
					vecIn.ColorPad = PAD;*/

					vCoord.push_back(vecIn);
					count++;
				}
				if (line2 == "vt")
				{
					loading >> uvIn.x;
					loading >> uvIn.y;
					uvIn.z = 1.0f;
					uvCoord.push_back(uvIn);
					count++;
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
					//{
						if (loading.peek() != ' ' && loading.peek() != '/')
						{
							for (int x = 0; x < 3; x++)
							{
								/*loading >> tempIndex[x];
								testIn.push_back(tempIndex[x]);
								loading.ignore();*/
								loading >> index.x;
								loading.ignore();
								loading >> index.y;
								loading.ignore();
								loading >> index.z;
								testIn.push_back(index);
								loading.ignore();
								count++;
							}
						}
						else
							loading.ignore();
					//}
				}
			}
			/*for (int i = 0; i < testIn.size(); i++)
			{
			}*/
		}
		catch (...)
		{
			//cout << "failed";
			throw;
		}
		//cout << "\n\n" << count;
	}
	loading.close();
	for (int i = 0; i < testIn.size(); i++)
	{
		Coordinates.x = vCoord[(testIn[i].x - 1)].x;
		Coordinates.y = vCoord[(testIn[i].x - 1)].y;
		Coordinates.z = vCoord[(testIn[i].x - 1)].z;
		

		Coordinates.r = PAD;
		Coordinates.g = PAD;
		Coordinates.b = PAD;
		

		Coordinates.u = uvCoord[(testIn[i].x - 1)].x;
		Coordinates.v = uvCoord[(testIn[i].x - 1)].y;
	
	

		//modelVerts->push_back(vCoord[(testIn[i].x - 1)]); //<---------------------
		modelVerts->push_back(Coordinates);
	}

	
}




ObjHandler::~ObjHandler()
{
}
