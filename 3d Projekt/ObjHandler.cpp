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

	vector<Vertex> vCoord, vNCoord;
	//vector<UV> uvCoord;
	Vertex vecIn, normIn;
	//UV uvIn;

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
					
					//temp
					vecIn.r = 0.0f;
					vecIn.g = 1.0f;
					vecIn.b = 0.0f;
					vecIn.ColorPad = PAD;

					vecIn.pad = PAD;
					vCoord.push_back(vecIn);
					count++;
				}
				/*if (line2 == "vt")
				{
					loading >> uvIn.u;
					loading >> uvIn.v;
					uvCoord.push_back(uvIn);
					count++;
				}*/
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
		modelVerts->push_back(vCoord[(testIn[i].x - 1)]);
	}

	
}




ObjHandler::~ObjHandler()
{
}
