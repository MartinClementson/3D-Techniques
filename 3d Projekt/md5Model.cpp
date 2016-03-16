#include "md5Model.h"
#include "Linker.h"
#include <fstream>
#include <string>

using namespace std;
md5Model::md5Model()
{
	this->worldStruct = new worldConstantBuffer;
	DirectX::XMStoreFloat4x4(&this->worldMatrix,DirectX::XMMatrixIdentity());
}

bool md5Model::Init(ID3D11DeviceContext * context, ID3D11Device * gDevice, ID3D11Buffer* worldbuffer)
{
	this->gDeviceContext = context;
	this->worldBuffer = worldbuffer;
	if (!loadModel(gDevice))
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

					//an if controller if the jointname has spaces
					while (tempJoint.name[tempJoint.name.size() - 1] != '"')
					{
						wstring temp;
						loading >> temp;
						tempJoint.name = tempJoint.name + temp;
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
							ID3D11ShaderResourceView* tempMeshSRV;
							//martin fixar här
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

					
					DirectX::XMFLOAT3 tempPos = { tempVert.pos.x,tempVert.pos.y,tempVert.pos.z }; //Convert from "position" struct to XMFLOAT3
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


md5Model::~md5Model()
{
	delete this->worldStruct;
}

void md5Model::update()
{
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
	//this->gDeviceContext->PSSetShaderResources(0, 1, &modelTextures[this->subsets.at(i).texArrayIndex]);
	
	this->gDeviceContext->DrawIndexed(this->subsets.at(i).indices.size(), 0, 0);
	}


	//IF there is a texture. apply it to the pixel shader

	//if (texture != nullptr)

	//if (normalMap != nullptr)
	//	this->gDeviceContext->PSSetShaderResources(2, 1, &this->normalMap);
	//fix draw indexed, first place should be the number of indices
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


}
