#include "md5Model.h"
#include "Linker.h"
#include "DirectXToolkit.h"
#include <fstream>
#include <string>

using namespace std;
md5Model::md5Model()
{
	this->worldStruct = new worldConstantBuffer;

	DirectX::XMMATRIX world = DirectX::XMMatrixIdentity();
	
	world = XMMatrixMultiply(world, XMMatrixScaling(0.04f, 0.04f, 0.04f));
	world = XMMatrixMultiply(world, XMMatrixTranslation(-5, 0, -5));
	
	world = XMMatrixTranspose(world);
	DirectX::XMStoreFloat4x4(&this->worldMatrix,world);

}

bool md5Model::Init(ID3D11DeviceContext * context, ID3D11Device * gDevice, ID3D11Buffer* worldbuffer)
{
	this->gDeviceContext = context;
	this->worldBuffer = worldbuffer;
	if (!loadModel(gDevice))
		return false;
	if (!loadAnimation())
		return false;
	return true;
}

bool md5Model::loadModel(ID3D11Device * gDevice)
{
	wstring line;
	wifstream loading;
	loading.open("boy.md5mesh");
	if (!loading)
		return false;
	else
	{
		while (!loading.eof())
		{
			loading >> line;
			//if (line == "MD5Version")
			//{
			//	//checking the version here, if we were to make the
			//	//loader multifunctional with more versions
			//}
			if (line == L"numJoints")
			{
				loading >> this->numJoints;
				//load the numjoints into some variable here
			}
			else if (line == L"commandline")
			{
				getline(loading, line); //skip this whole line
			}
			else if (line == L"numMeshes")
			{
				loading >> numSubsets;
				//store number of meshes
			}
			else if (line == L"joints")
			{
				
				Joint tempJoint;

				loading >> line;
				for (int i = 0; i < numJoints; i++)
				{
					loading >> tempJoint.name; //skip the "{"

											   // Sometimes the names might contain spaces. If that is the case, we need to continue
											   // to read the name until we get to the closing " (quotation marks)
					if (tempJoint.name[tempJoint.name.size() - 1] != '"')
					{
						wchar_t checkChar;
						bool jointNameFound = false;
						while (!jointNameFound)
						{
							checkChar = loading.get();

							if (checkChar == '"')
								jointNameFound = true;

							tempJoint.name += checkChar;
						}
					}
					

					loading >> tempJoint.parentID;

					loading >> line; //skipping the "("

					//store the position of the joint (if RH model
					//swap y and z coords, ours however is LH)
					loading >> tempJoint.pos.x 
						>> tempJoint.pos.z 
						>> tempJoint.pos.y;

					loading >> line >> line; //skipping the ")" and "("

					//store the orientation of the joint
					loading >> tempJoint.orientation.x
						>> tempJoint.orientation.z
						>> tempJoint.orientation.y;

					//remove the Quatation marks from the joints name
					tempJoint.name.erase(0, 1);
					tempJoint.name.erase(tempJoint.name.size() - 1, 1); //remove stupid backslash

					// Compute the w axis of the quaternion (The MD5 model uses a 3D vector to describe the
					// direction the bone is facing. However, we need to turn this into a quaternion, and the way
					// quaternions work, is the xyz values describe the axis of rotation, while the w is a value
					// between 0 and 1 which describes the angle of rotation)
					float t = 1.0f - (tempJoint.orientation.x * tempJoint.orientation.x)
						- (tempJoint.orientation.y * tempJoint.orientation.y)
						- (tempJoint.orientation.z * tempJoint.orientation.z);
					if (t < 0.0f)
						tempJoint.orientation.w = 0.0f;
					else
						tempJoint.orientation.w = -sqrt(t);

					getline(loading, line); //skip the rest of the current line

					//store the joint in the joint vector, which
					//will contain all the joints for the model
					this->joints.push_back(tempJoint);
				}
				loading >> line; //skip the "}"
			}
			else if (line == L"mesh")
			{
				ModelSubset subset;
				int numVerts, numTris, numWeights;

				loading >> line; //skip the "{"
				loading >> line;
				while (line != L"}")
				{
					if (line == L"shader")
					{
						wstring fileName;
						loading >> fileName;
						
						// remove the quatation marks from the filename
						fileName.erase(0, 1);
						fileName.erase(fileName.size() - 1, 1);

						//checking if the texture is already loaded
						bool alreadyLoaded = false;
						for (int i = 0; i < texFileNameArray.size(); ++i)
						{
							if (fileName == texFileNameArray[i])
							{
								alreadyLoaded = true;
								subset.texArrayIndex = i;
							}
						}
						if (!alreadyLoaded)
						{
							HRESULT hr;
							ID3D11ShaderResourceView* tempMeshSRV;
							//martin fixar här
							hr = CreateWICTextureFromFile(gDevice, fileName.c_str(), nullptr, &tempMeshSRV);
							if (SUCCEEDED(hr))
							{
								texFileNameArray.push_back(fileName.c_str());
								subset.texArrayIndex = textures.size();
								textures.push_back(tempMeshSRV);

							}
							else
								MessageBox(0, fileName.c_str(), L"Could not open", MB_OK);
						}
						getline(loading, line); //skip the rest of the line
					}
					else if (line == L"numverts")
					{
						loading >> numVerts; //store the nhumber of vertices

						getline(loading, line); //skip the rest of the line

						for (int i = 0; i < numVerts; i++)
						{
							//this will be the specific vertex struct
							//made for animation
							AnimVertex tempVert;

							loading >> line
								>> line
								>> line; //skipping "vert # ("

							loading >> tempVert.u
								>> tempVert.v; //storing the uv coords

							loading >> line; //skipping ")"

							//specific elements of the animation vertices
							loading >> tempVert.StartWeight; //index of first weight this vertex will be weighted to
							loading >> tempVert.WeightCount; //number of weights for this vertex

							getline(loading, line); //skip the rest iof the line

							//push back this vertex into the subsets vertex vector
							subset.vertices.push_back(tempVert);
						}
					}
					else if (line == L"numtris")
					{
						loading >> numTris;
						subset.numTriangles = numTris;

						getline(loading, line); //skip the rest of this line

						for (int i = 0; i < numTris; i++)
						{
							DWORD tempIndex;
							loading >> line
								>> line; //skip "tri" and tri counter

							for (int j = 0; j < 3; j++)
							{
								loading >> tempIndex;

								//store the index in indices
								subset.indices.push_back(tempIndex);
							}

							getline(loading, line); //skip the rest of the line
						}
					}
					else if (line == L"numweights")
					{
						loading >> numWeights;

						getline(loading, line); //skip the rest of the line

						for (int i = 0; i < numWeights; i++)
						{
							//a weight class we need to make
							Weight tempWeight;
							loading >> line
								>> line; //skipping "weight #"

							loading >> tempWeight.jointID; //storing the weight ID
							loading >> tempWeight.influence; //store weights influence over a vertex

							loading >> line; //skip "("

							//store the weights position in the
							//joints local space
							loading >> tempWeight.pos.x
								>> tempWeight.pos.z
								>> tempWeight.pos.y;

							getline(loading, line); //skipping the rest of the line

							//store the weight into subsets weight array
							subset.weights.push_back(tempWeight);
						}
					}
					else
						getline(loading, line); //skip anything else
					loading >> line; //skip "}"
				}


				//find each vertex position using the joints and weights
				for (int i = 0; i < subset.vertices.size(); ++i)
				{
					//once again the special animation vertex
					AnimVertex tempVert = subset.vertices[i];
					
					//making sure that the postition is empty
					tempVert.pos.x = 0;
					tempVert.pos.y = 0;
					tempVert.pos.z = 0;

					//sum up the joints and weights information to get the vertex position
					for (int j = 0; j < tempVert.WeightCount; ++j)
					{
						Weight tempWeight = subset.weights[tempVert.StartWeight + j];
						Joint tempJoint = this->joints[tempWeight.jointID];

						// Convert joint orientation and weight pos to vectors for easier computation
						// When converting a 3d vector to a quaternion, you should put 0 for "w", and
						// When converting a quaternion to a 3d vector, you can just ignore the "w"
						DirectX::XMVECTOR tempJointOrientation = DirectX::XMVectorSet(tempJoint.orientation.x, tempJoint.orientation.y, tempJoint.orientation.z, tempJoint.orientation.w);
						DirectX::XMVECTOR tempWeightPos = DirectX::XMVectorSet(tempWeight.pos.x, tempWeight.pos.y, tempWeight.pos.z, 0.0f);

						// We will need to use the conjugate of the joint orientation quaternion
						// To get the conjugate of a quaternion, all you have to do is inverse the x, y, and z
						DirectX::XMVECTOR tempConjugate = DirectX::XMVectorSet(-tempJoint.orientation.x, -tempJoint.orientation.y, -tempJoint.orientation.z, tempJoint.orientation.w);

						// Calculate vertex position (in joint space, eg. rotate the point around (0,0,0)) for this weight using the joint orientation quaternion and its conjugate
						// We can rotate a point using a quaternion with the equation "rotatedPoint = quaternion * point * quaternionConjugate"
						DirectX::XMFLOAT3 rotatedPoint;
						DirectX::XMStoreFloat3(&rotatedPoint, DirectX::XMQuaternionMultiply(DirectX::XMQuaternionMultiply(tempJointOrientation, tempWeightPos), tempConjugate));
						
						// Now move the verices position from joint space (0,0,0) to the joints position in world space, taking the weights influence into account
						// The weight influence is used because multiple weights might have an effect on the vertices final position. Each weight is attached to one joint.
						tempVert.pos.x += (tempJoint.pos.x + rotatedPoint.x) * tempWeight.influence;
						tempVert.pos.y += (tempJoint.pos.y + rotatedPoint.y) * tempWeight.influence;
						tempVert.pos.z += (tempJoint.pos.z + rotatedPoint.z) * tempWeight.influence;

						// Basically what has happened above, is we have taken the weights position relative to the joints position
						// we then rotate the weights position (so that the weight is actually being rotated around (0, 0, 0) in world space) using
						// the quaternion describing the joints rotation. We have stored this rotated point in rotatedPoint, which we then add to
						// the joints position (because we rotated the weight's position around (0,0,0) in world space, and now need to translate it
						// so that it appears to have been rotated around the joints position). Finally we multiply the answer with the weights bias,
						// or how much control the weight has over the final vertices position. All weight's bias effecting a single vertex's position
						// must add up to 1.
					}

					
					position tempPos = { tempVert.pos.x,tempVert.pos.y,tempVert.pos.z }; //Convert from "position" struct to XMFLOAT3
					subset.positions.push_back(tempPos);            // Store the vertices position in the position vector instead of straight into the vertex vector
																		 // since we can use the positions vector for certain things like collision detection or picking
																		 // without having to work with the entire vertex structure.
				}

				//put positions into the vertices for this subset
				for (int i = 0; i < subset.vertices.size(); i++)
				{
					subset.vertices[i].pos.x = subset.positions[i].x;
					subset.vertices[i].pos.y = subset.positions[i].y;
					subset.vertices[i].pos.z = subset.positions[i].z;
				}

				//Calculate vertex normals using normal averaging//
				vector<DirectX::XMFLOAT3> tempNormal;

				//normalized and unnormalized normals
				DirectX::XMFLOAT3 unnormalized = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);

				//used to get vector sides from vertiuces
				float vecX, vecY, vecZ;

				//will be used for Calculating edges
				DirectX::XMVECTOR edge1 = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
				DirectX::XMVECTOR edge2 = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);

				//computing face normals
				for (int i = 0; i < subset.numTriangles; ++i)
				{
					//Get the vector describing one edge of our triangle (edge 0,2)
					vecX = subset.vertices[subset.indices[(i * 3)]].pos.x - subset.vertices[subset.indices[(i * 3) + 2]].pos.x;
					vecY = subset.vertices[subset.indices[(i * 3)]].pos.y - subset.vertices[subset.indices[(i * 3) + 2]].pos.y;
					vecZ = subset.vertices[subset.indices[(i * 3)]].pos.z - subset.vertices[subset.indices[(i * 3) + 2]].pos.z;
					edge1 = DirectX::XMVectorSet(vecX, vecY, vecZ, 0.0f);    //Create our first edge

					//Get the vector describing another edge of our triangle (edge 2,1)
					vecX = subset.vertices[subset.indices[(i * 3) + 2]].pos.x - subset.vertices[subset.indices[(i * 3) + 1]].pos.x;
					vecY = subset.vertices[subset.indices[(i * 3) + 2]].pos.y - subset.vertices[subset.indices[(i * 3) + 1]].pos.y;
					vecZ = subset.vertices[subset.indices[(i * 3) + 2]].pos.z - subset.vertices[subset.indices[(i * 3) + 1]].pos.z;
					edge2 = DirectX::XMVectorSet(vecX, vecY, vecZ, 0.0f);    //Create our second edge
					
					//crossproduct to get the normal
					DirectX::XMStoreFloat3(&unnormalized, DirectX::XMVector3Cross(edge1, edge2));
					
					tempNormal.push_back(unnormalized);
				}

				//compute vertex normals
				DirectX::XMVECTOR normalSum = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
				int faces = 0;
				float tX, tY, tZ; //temp axix variables

				//looping trough each vertex
				for (int i = 0; i < subset.vertices.size(); ++i)
				{
					//Check which triangles use this vertex
					for (int j = 0; j < subset.numTriangles; ++j)
					{
						if (subset.indices[j * 3] == i ||
							subset.indices[(j * 3) + 1] == i ||
							subset.indices[(j * 3) + 2] == i)
						{
							tX = DirectX::XMVectorGetX(normalSum) + tempNormal[j].x;
							tY = DirectX::XMVectorGetY(normalSum) + tempNormal[j].y;
							tZ = DirectX::XMVectorGetZ(normalSum) + tempNormal[j].z;

							normalSum = DirectX::XMVectorSet(tX, tY, tZ, 0.0f);    //If a face is using the vertex, add the unormalized face normal to the normalSum

							faces++;
						}
					}

					//Get the actual normal by dividing by
					//the number of faces sharing the vertex
					normalSum.m128_f32[0] = normalSum.m128_f32[0] / faces;
					normalSum.m128_f32[1] = normalSum.m128_f32[1] / faces;
					normalSum.m128_f32[2] = normalSum.m128_f32[2] / faces;
					normalSum.m128_f32[3] = normalSum.m128_f32[3] / faces;

					//normalize the sum
					normalSum = DirectX::XMVector3Normalize(normalSum);

					//store the normal
					subset.vertices[i].normal.x = -DirectX::XMVectorGetX(normalSum);
					subset.vertices[i].normal.y = -DirectX::XMVectorGetY(normalSum);
					subset.vertices[i].normal.z = -DirectX::XMVectorGetZ(normalSum);

					//clear normalSum, faces for the next vertex
					normalSum = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
					faces = 0;
				}
				HRESULT hr;
				// Create index buffer
				D3D11_BUFFER_DESC indexBufferDesc;
				ZeroMemory(&indexBufferDesc, sizeof(indexBufferDesc));

				indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
				indexBufferDesc.ByteWidth = sizeof(DWORD) * subset.numTriangles * 3;
				indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
				indexBufferDesc.CPUAccessFlags = 0;
				indexBufferDesc.MiscFlags = 0;

				D3D11_SUBRESOURCE_DATA iinitData;

				iinitData.pSysMem = &subset.indices[0];
				hr =gDevice->CreateBuffer(&indexBufferDesc, &iinitData, &subset.indexBuff);
				if (FAILED(hr))
					return false;
				//Create Vertex Buffer
				D3D11_BUFFER_DESC vertexBufferDesc;
				ZeroMemory(&vertexBufferDesc, sizeof(vertexBufferDesc));

				vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;                            // We will be updating this buffer, so we must set as dynamic
				vertexBufferDesc.ByteWidth = sizeof(Vertex) * subset.vertices.size();
				vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
				vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;                // Give CPU power to write to buffer
				vertexBufferDesc.MiscFlags = 0;

				D3D11_SUBRESOURCE_DATA vertexBufferData;

				ZeroMemory(&vertexBufferData, sizeof(vertexBufferData));
				vertexBufferData.pSysMem = &subset.vertices[0];
				hr = gDevice->CreateBuffer(&vertexBufferDesc, &vertexBufferData, &subset.vertBuff);
				if (FAILED(hr))
					return false;
				// Push back the temp subset into the models subset vector
				this->subsets.push_back(subset);

			}
		}
		loading.close();
	}
	return true;
}





bool md5Model::loadAnimation()
{
	ModelAnimation tempAnim;

	std::wifstream fileIn;
	
	std::wstring line;

	fileIn.open("boy.md5anim");

	if (fileIn)
	{
		while (fileIn)
		{
			fileIn >> line;

			if (line == L"numFrames")
			{
				fileIn >> tempAnim.numFrames;
			}
			else if (line == L"numJoints")
			{
				fileIn >> tempAnim.numJoints;
			}
			else if (line == L"frameRate")
			{
				fileIn >> tempAnim.frameRate;
			}
			else if (line == L"numAnimatedComponents")
			{
				fileIn >> tempAnim.numAnimatedComponents;
			}
			else if (line == L"hierarchy")
			{
				fileIn >> line; // this is to skip the bracket "{"

				for (int i = 0; i < tempAnim.numJoints; i++) //load each joint
				{
					AnimJointInfo tempJoint;

					fileIn >> tempJoint.name; //get name

											  // Sometimes the names might contain spaces. If that is the case, we need to continue
											  // to read the name until we get to the closing " (quotation marks)
					if (tempJoint.name[tempJoint.name.size() - 1] != '"') //not our code
					{
						wchar_t checkChar;
						bool jointNameFound = false;
						while (!jointNameFound)
						{
							checkChar = fileIn.get();

							if (checkChar == '"')
								jointNameFound = true;

							tempJoint.name += checkChar;
						}
					}
					//remove quotation marks from name
					tempJoint.name.erase(0, 1);
					tempJoint.name.erase(tempJoint.name.size() - 1, 1);


					fileIn >> tempJoint.parentID;
					fileIn >> tempJoint.flags;
					fileIn >> tempJoint.startIndex;

					//make sure the joint exists in the model and has the correct parent ID
					bool jointMatchFound = false;
					for (int j = 0; j < this->numJoints; j++)
					{
						if (this->joints[j].name == tempJoint.name)
						{
							if (this->joints[j].parentID == tempJoint.parentID)
							{
								jointMatchFound = true;
								tempAnim.jointInfo.push_back(tempJoint);
							}

						}

					}
					if (!jointMatchFound)
						return false;

					getline(fileIn, line); //skip the rest of the line





				}

			}
			else if (line == L"bounds") //Load in the AABB for each animation
			{

				fileIn >> line; //Skip bracket

				for (int i = 0; i < tempAnim.numFrames; i++) 
				{
					BoundingBox tempBB;

					fileIn >> line; //skip "("
					fileIn >> tempBB.min.x >> tempBB.min.z >> tempBB.min.y;
					fileIn >> line >> line; //skip "(" ")"
					fileIn >> tempBB.max.x >> tempBB.max.z >> tempBB.max.y;
					fileIn >> line; // "skip ")"

					tempAnim.frameBounds.push_back(tempBB);

				}
			}
			else if (line == L"baseframe") //this is the default position of the animation (but it's not necessarily the same as the TPose)
			{
				fileIn >> line; //Skip "{"

				for (int i = 0; i < tempAnim.numJoints; i++)
				{
					Joint tempBFJ;
					
					fileIn >> line; //skip "("
					fileIn >> tempBFJ.pos.x >> tempBFJ.pos.z >> tempBFJ.pos.y;
					fileIn >> line >> line; // skip ) (
					fileIn >> tempBFJ.orientation.x >> tempBFJ.orientation.z >> tempBFJ.orientation.y;
					fileIn >> line; //skip ")"

					tempAnim.baseFrameJoints.push_back(tempBFJ);

				}
			}

			else if (line == L"frame") //load in each frames skeleton
			{
				FrameData tempFrame;

				fileIn >> tempFrame.frameID;

				fileIn >> line; // skip bracket

				for (int i = 0; i < tempAnim.numAnimatedComponents; i++)
				{
					float tempData;
					fileIn >> tempData;
					tempFrame.frameData.push_back(tempData);

				}
				tempAnim.frameData.push_back(tempFrame);

				//build the frame skeleton

				std::vector<Joint> tempSkeleton;

				for (int i = 0; i < tempAnim.jointInfo.size(); i++)
				{
					int k = 0; //keep track of position in frameData array

					//start the frames joint with the base frames joint
					Joint tempFrameJoint = tempAnim.baseFrameJoints[i];

					tempFrameJoint.parentID = tempAnim.jointInfo[i].parentID;

					if (tempAnim.jointInfo[i].flags & 1) // pos.x    ( 000001 )
						tempFrameJoint.pos.x = tempFrame.frameData[tempAnim.jointInfo[i].startIndex + k++];

					if (tempAnim.jointInfo[i].flags & 2)        // pos.y    ( 000010 )
						tempFrameJoint.pos.z = tempFrame.frameData[tempAnim.jointInfo[i].startIndex + k++];

					if (tempAnim.jointInfo[i].flags & 4)        // pos.z    ( 000100 )
						tempFrameJoint.pos.y = tempFrame.frameData[tempAnim.jointInfo[i].startIndex + k++];

					if (tempAnim.jointInfo[i].flags & 8)        // orientation.x    ( 001000 )
						tempFrameJoint.orientation.x = tempFrame.frameData[tempAnim.jointInfo[i].startIndex + k++];

					if (tempAnim.jointInfo[i].flags & 16)    // orientation.y    ( 010000 )
						tempFrameJoint.orientation.z = tempFrame.frameData[tempAnim.jointInfo[i].startIndex + k++];

					if (tempAnim.jointInfo[i].flags & 32)    // orientation.z    ( 100000 )
						tempFrameJoint.orientation.y = tempFrame.frameData[tempAnim.jointInfo[i].startIndex + k++];

					//compute the quaternions w

					float t = 1.0f - (tempFrameJoint.orientation.x * tempFrameJoint.orientation.x)
						- (tempFrameJoint.orientation.y * tempFrameJoint.orientation.y)
						- (tempFrameJoint.orientation.z * tempFrameJoint.orientation.z);

					if (t < 0.0f)
					{
						tempFrameJoint.orientation.w = 0.0f;
					}
					else
					{
						tempFrameJoint.orientation.w = -sqrtf(t);
					}
					// Now, if the upper arm of your skeleton moves, you need to also move the lower part of your arm, and then the hands, and then finally the fingers (possibly weapon or tool too)
					// This is where joint hierarchy comes in. We start at the top of the hierarchy, and move down to each joints child, rotating and translating them based on their parents rotation
					// and translation. We can assume that by the time we get to the child, the parent has already been rotated and transformed based of it's parent. We can assume this because
					// the child should never come before the parent in the files we loaded in.

					if (tempFrameJoint.parentID >= 0)
					{
						Joint parentJoint = tempSkeleton[tempFrameJoint.parentID];

						XMVECTOR parentJointOrientation = XMVectorSet(parentJoint.orientation.x, parentJoint.orientation.y, parentJoint.orientation.z, parentJoint.orientation.w);
						XMVECTOR tempJointPos = XMVectorSet(tempFrameJoint.pos.x, tempFrameJoint.pos.y, tempFrameJoint.pos.z, 0.0f);
						XMVECTOR parentOrientationConjugate = XMVectorSet(-parentJoint.orientation.x, -parentJoint.orientation.y, -parentJoint.orientation.z, parentJoint.orientation.w);

						//calculate current joints position relative to its parents position
						XMFLOAT3 rotatedPos;

						DirectX::XMStoreFloat3(&rotatedPos, XMQuaternionMultiply( XMQuaternionMultiply(parentJointOrientation, tempJointPos), parentOrientationConjugate));

						//Translate the joint to model space by adding the parent joints pos to it
						tempFrameJoint.pos.x = rotatedPos.x + parentJoint.pos.x;
						tempFrameJoint.pos.y = rotatedPos.y + parentJoint.pos.y;
						tempFrameJoint.pos.z = rotatedPos.z + parentJoint.pos.z;

						// Currently the joint is oriented in its parent joints space, we now need to orient it in
						// model space by multiplying the two orientations together (parentOrientation * childOrientation) <- In that order
						XMVECTOR tempJointOrient = XMVectorSet(tempFrameJoint.orientation.x, tempFrameJoint.orientation.y, tempFrameJoint.orientation.z, tempFrameJoint.orientation.w);
						tempJointOrient = XMQuaternionMultiply(parentJointOrientation, tempJointOrient);

						// Normalize the orienation quaternion
						tempJointOrient = XMQuaternionNormalize(tempJointOrient);

						XMFLOAT4 tempConvert; //need to do this because orientation is not a XMFLOAT4 
						XMStoreFloat4(&tempConvert, tempJointOrient);
						tempFrameJoint.orientation.x = tempConvert.x;
						tempFrameJoint.orientation.y = tempConvert.y;
						tempFrameJoint.orientation.z = tempConvert.z;
						tempFrameJoint.orientation.w = tempConvert.w;
					}

					//store the joint into the temporary frame skeleton
					tempSkeleton.push_back(tempFrameJoint);

				}

				//push back the newly created frame skeleton into the animations frameSkeleton array
				tempAnim.frameSkeleton.push_back(tempSkeleton);
				fileIn >> line; //skip closing bracket
			}

		}
		//calculate and store usefull animation data

		tempAnim.frameTime = 1.0f / tempAnim.frameRate; //set the time per frame;
		tempAnim.totalAnimTime = tempAnim.numFrames * tempAnim.frameTime; //set the total time the animation takes
		tempAnim.currAnimTime = 0.0f;
		this->animations.push_back(tempAnim); //push back the animation into our model


	}
	else
	{
		MessageBox(0, L"Could not open animation file",L"Error", MB_OK);
		return false;
	}




	return true;

}
md5Model::~md5Model()
{
	delete this->worldStruct;
}

void md5Model::update(float deltaTime, int animation)
{

	this->animations[animation].currAnimTime += deltaTime; //update the current animation time

	if (this->animations[animation].currAnimTime > this->animations[animation].totalAnimTime)
		this->animations[animation].currAnimTime = 0.0f;

	//which frame are we on
	float currentFrame = this->animations[animation].currAnimTime * this->animations[animation].frameRate;
	int frame0 = floorf(currentFrame);
	int frame1 = frame0 + 1;

	//make sure w don't go over the number of frames
	if (frame0 == this->animations[animation].numFrames - 1)
		frame1 = 0;

	float interpolation = currentFrame - frame0; //get the remainder time between frame 0 and frame 1 to use as interpolation factor

	std::vector<Joint> interpolatedSkeleton; // create a frame skeleton to store the intepolated skeleton in

		//compute the interpolated skeleton

		for (int i = 0; i < this->animations[animation].numJoints; i++)
		{

			Joint tempJoint;
			Joint joint0 = this->animations[animation].frameSkeleton[frame0][i]; // get the i'th joint of frame 0's skeleton
			Joint joint1 = this->animations[animation].frameSkeleton[frame1][i];

			tempJoint.parentID = joint0.parentID; //set the tempJoints parent id

			//Turn the two quaternions into XMVectors

			XMVECTOR joint0Orient = XMVectorSet(joint0.orientation.x, joint0.orientation.y, joint0.orientation.z, joint0.orientation.w);
			XMVECTOR joint1Orient = XMVectorSet(joint1.orientation.x, joint1.orientation.y, joint1.orientation.z, joint1.orientation.w);

			// Interpolate positions
			tempJoint.pos.x = joint0.pos.x + (interpolation * (joint1.pos.x - joint0.pos.x));
			tempJoint.pos.y = joint0.pos.y + (interpolation * (joint1.pos.y - joint0.pos.y));
			tempJoint.pos.z = joint0.pos.z + (interpolation * (joint1.pos.z - joint0.pos.z));

			// interpolate orientations using spherical interpolation (Slerp)
			XMFLOAT4 tempConvert; //needed because orient is not a xmfloat4
			XMStoreFloat4(&tempConvert, XMQuaternionSlerp(joint0Orient,joint1Orient, interpolation));

			tempJoint.orientation.x = tempConvert.x;
			tempJoint.orientation.y = tempConvert.y;
			tempJoint.orientation.z = tempConvert.z;
			tempJoint.orientation.w = tempConvert.w;

			interpolatedSkeleton.push_back(tempJoint); //push back the joint into our interpolated skeleton
		}

		for (int k = 0; k < this->numSubsets; k++)
		{
			for (int i = 0; i < this->subsets[k].vertices.size(); ++i)
			{
				AnimVertex tempVert = this->subsets[k].vertices[i];
				tempVert.pos = { 0,0,0 }; // make sure pos is cleared
				tempVert.normal = { 0,0,0 }; //clear normal

				//sum up the joints and weights information to get vertex's position and normal
				for (int j = 0; j < tempVert.WeightCount; ++j)
				{

					Weight tempWeight = this->subsets[k].weights[tempVert.StartWeight + j];
					Joint tempJoint = interpolatedSkeleton[tempWeight.jointID];

					//Convert joint orientation to vectors 
					XMVECTOR tempJointOrientation = XMVectorSet(tempJoint.orientation.x, tempJoint.orientation.y, tempJoint.orientation.z, tempJoint.orientation.w);
					XMVECTOR tempWeightPos = XMVectorSet(tempWeight.pos.x, tempWeight.pos.y, tempWeight.pos.z, 0.0f);

					// We will need to use the conjugate of the joint orientation quaternion
					XMVECTOR tempJointOrientationConjugate = XMQuaternionInverse(tempJointOrientation);

					// Calculate vertex position (in joint space, eg. rotate the point around (0,0,0)) for this weight using the joint orientation quaternion and its conjugate
					// We can rotate a point using a quaternion with the equation "rotatedPoint = quaternion * point * quaternionConjugate"
					XMFLOAT3 rotatedPoint;
					XMStoreFloat3(&rotatedPoint, XMQuaternionMultiply(XMQuaternionMultiply(tempJointOrientation, tempWeightPos), tempJointOrientationConjugate));


					// Now move the verices position from joint space (0,0,0) to the joints position in world space, taking the weights bias into account
					tempVert.pos.x += (tempJoint.pos.x + rotatedPoint.x) * tempWeight.influence;
					tempVert.pos.y += (tempJoint.pos.y + rotatedPoint.y) * tempWeight.influence;
					tempVert.pos.z += (tempJoint.pos.z + rotatedPoint.z) * tempWeight.influence;

					// Compute the normals for this frames skeleton using the weight normals from before
					// We can comput the normals the same way we compute the vertices position, only we don't have to translate them (just rotate)
					XMVECTOR tempWeightNormal = XMVectorSet(tempWeight.normal.x, tempWeight.normal.y, tempWeight.normal.z, 0.0f);

					// Rotate the normal
					XMStoreFloat3(&rotatedPoint, XMQuaternionMultiply(XMQuaternionMultiply(tempJointOrientation, tempWeightNormal), tempJointOrientationConjugate));

					// Add to vertices normal and ake weight bias into account
					tempVert.normal.x -= rotatedPoint.x * tempWeight.influence;
					tempVert.normal.y -= rotatedPoint.y * tempWeight.influence;
					tempVert.normal.z -= rotatedPoint.z * tempWeight.influence;

				}

				this->subsets[k].positions[i] = tempVert.pos;                // Store the vertices position in the position vector instead of straight into the vertex vector
				this->subsets[k].vertices[i].normal = tempVert.normal;        // Store the vertices normal

				XMFLOAT3 tempConvert;
				XMStoreFloat3(&tempConvert, XMVector3Normalize(XMLoadFloat3(&XMFLOAT3(this->subsets[k].vertices[i].normal))));

				this->subsets[k].vertices[i].normal.x = tempConvert.x;
				this->subsets[k].vertices[i].normal.y = tempConvert.y;
				this->subsets[k].vertices[i].normal.z = tempConvert.z;

			}

			//put the positions into the vertices for this subset
			for (int i = 0; i < this->subsets[k].vertices.size(); i++)
			{

				this->subsets[k].vertices[i].pos = this->subsets[k].positions[i];

				//Update the subsets vertex buffer
				//first lock the buffer
				D3D11_MAPPED_SUBRESOURCE mappedVertBuff;
				HRESULT	hr;
				hr = gDeviceContext->Map(this->subsets[k].vertBuff, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedVertBuff);
					
				memcpy(mappedVertBuff.pData,&this->subsets[k].vertices[0], sizeof(AnimVertex)* this->subsets[k].vertices.size());

				gDeviceContext->Unmap(this->subsets[k].vertBuff, 0);
			}
		}


		//now make the whole mesh move (so that he does not run in place

		

}

void md5Model::render()
{

	this->sendToConstantBuffer();
	this->gDeviceContext->GSSetConstantBuffers(0, 1, &worldBuffer);

	UINT32 vertexSize = sizeof(AnimVertex);
	UINT32 offset = 0;

	this->gDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);



	for (int i = 0; i < this->numSubsets; i++)
	{

	this->gDeviceContext->IASetIndexBuffer(this->subsets.at(i).indexBuff, DXGI_FORMAT_R32_UINT, 0);
	this->gDeviceContext->IASetVertexBuffers(0, 1, &this->subsets.at(i).vertBuff, &vertexSize, &offset);

	
	
	// NO textures are loaded yet!
	this->gDeviceContext->PSSetShaderResources(0, 1, &textures[this->subsets.at(i).texArrayIndex]);
	
	this->gDeviceContext->DrawIndexed(this->subsets.at(i).indices.size(), 0, 0);
	}


	
}

void md5Model::sendToConstantBuffer()
{
	this->worldStruct->world = this->worldMatrix;
	//this->worldStruct->normalWorld = this->normalWorld;
	D3D11_MAPPED_SUBRESOURCE mappedResourceWorld;
	ZeroMemory(&mappedResourceWorld, sizeof(mappedResourceWorld));

	//mapping to the matrixbuffer
	this->gDeviceContext->Map(worldBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResourceWorld);

	worldConstantBuffer* temporaryWorld = (worldConstantBuffer*)mappedResourceWorld.pData;

	*temporaryWorld = *worldStruct;

	this->gDeviceContext->Unmap(worldBuffer, 0);
}
void md5Model::Release()
{
	for (int i = 0; i < subsets.size(); i++)
	{
		this->subsets.at(i).Release();

	}

	for (int i = 0; i < textures.size(); i++)
	{
		this->textures.at(i)->Release();

	}


}
