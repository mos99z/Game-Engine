#define _CRT_SECURE_NO_WARNINGS
#define FLIP_UV_Y false
#define PROGRESSTEXT false
#define MILLAT60FPS 16.6666f

#define KEYFRAMEBYTESIZE(numBones) (numBones * sizeof(Bone) + sizeof(double))

#include "FBXConverterManager.h"

unsigned int m_VertexByteSize = sizeof(Vertex);

FBXLoaderManager::FBXLoaderManager()
{
	mp_FbxManager = nullptr;
	mp_FbxIOSettings = nullptr;
	mp_FbxImporter = nullptr;
	mp_FbxScene = nullptr;
	mp_FbxRootNode = nullptr;

	m_VBufferByteSize = sizeof(VBuffer);
	m_TBufferByteSize = sizeof(TBuffer);
	m_ABufferByteSize = sizeof(ABuffer);
	m_BoneByteSize = sizeof(Bone);

	m_ConsoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	m_MainProgress = { 0,0 };
	m_SecondaryProgress = { 0,1 };
}

FBXLoaderManager::~FBXLoaderManager()
{
	Uninitilize();
}

void FBXLoaderManager::Uninitilize()
{
	mv_Joints.clear();
	m_ControlPoints.clear();
	m_DepthLevel = 0;
	m_FoundMesh = false;
	m_BuiltBaseAnimation = false;
	if (writeStream.is_open())
		writeStream.close();
}

void FBXLoaderManager::Initilize(const wchar_t* _FbxFileName)
{
	Uninitilize();

#if _DEBUG
	_ASSERT_EXPR(_FbxFileName, L"FBXLoader - Passed In File Name Can NOT Be Null!");
#endif

	ms_FbxFileName = ms_AWBXFileName = _FbxFileName;
	ParseOutFileName();

	mp_FbxManager = FbxManager::Create();

#if _DEBUG
	_ASSERT_EXPR(mp_FbxManager, L"FBXLoader - FbxManager failed to create!");
#endif

	mp_FbxGeoConverter = new FbxGeometryConverter(mp_FbxManager);
	mp_FbxIOSettings = FbxIOSettings::Create(mp_FbxManager, IOSROOT);

#if _DEBUG
	_ASSERT_EXPR(mp_FbxIOSettings, L"FBXLoader - FbxIOSettings failed to create!");
#endif

	mp_FbxManager->SetIOSettings(mp_FbxIOSettings);
	mp_FbxImporter = FbxImporter::Create(mp_FbxManager, "");

#if _DEBUG
	_ASSERT_EXPR(mp_FbxImporter, L"FBXLoader - FbxImporter failed to create!");
#endif

	char* deleteMe = new char[ms_FbxFileName.size() + 1];
	deleteMe[ms_FbxFileName.size()] = '\0';
	wcstombs(deleteMe, ms_FbxFileName.c_str(), ms_FbxFileName.size());
	if (!mp_FbxImporter->Initialize(deleteMe, -1, mp_FbxManager->GetIOSettings()))
	{
#if _DEBUG
		std::wstring message = L"Fbx Importer failed to initilize With the following error message: ";
		const char* charErrorMessage = mp_FbxImporter->GetStatus().GetErrorString();
		size_t errorMessageLength = strlen(charErrorMessage) + 1;
		wchar_t* errorMessage = new wchar_t[errorMessageLength];
		mbstowcs(errorMessage, charErrorMessage, errorMessageLength);
		message.append(errorMessage);
		_ASSERT_EXPR(0, message.c_str());
		delete[] errorMessage;
#endif
		delete[] deleteMe;
		return;
	}
	delete[] deleteMe;

	mp_FbxScene = FbxScene::Create(mp_FbxManager, "For_External_Programs");
	FbxAxisSystem changeHandedness(FbxAxisSystem::eDirectX);
	changeHandedness.ConvertScene(mp_FbxScene);

#if _DEBUG
	_ASSERT_EXPR(mp_FbxScene, L"FBXLoader - FbxScene failed to create!");
#endif

	mp_FbxImporter->Import(mp_FbxScene);
	mp_FbxImporter->Destroy();

	mp_FbxRootNode = mp_FbxScene->GetRootNode();
#if PROGRESSTEXT
	m_numNodes = mp_FbxScene->GetNodeCount();
	m_nodesProcessed = 1;
#endif

	if (mp_FbxRootNode)
	{
		if (writeStream.is_open())
			writeStream.close();

		writeStream.open(ms_AWBXFileName.c_str(), std::ios_base::binary | std::ios_base::trunc);
		if (writeStream.is_open())
		{
			writeStream.write("AWBX", 4);

			int numChildren = mp_FbxRootNode->GetChildCount();

			for (int child = 0; child < numChildren; child++)
				HandleSkeleton(mp_FbxRootNode->GetChild(child), 0, -1);

			for (int child = 0; child < numChildren; child++)
				HandleNode(mp_FbxRootNode->GetChild(child));

			writeStream.close();
		}
	}

	mp_FbxManager->Destroy();
}

void FBXLoaderManager::HandleNode(FbxNode* _node)
{
	if (m_FoundMesh)
		return;

	int numNodeAttributes = _node->GetNodeAttributeCount();
	for (int attribute = 0; attribute < numNodeAttributes; attribute++)
	{
		FbxNodeAttribute::EType nodeAttributeType = _node->GetNodeAttributeByIndex(attribute)->GetAttributeType();
		switch (nodeAttributeType)
		{
		case FbxNodeAttribute::eUnknown: break;
		case FbxNodeAttribute::eNull: break;
		case FbxNodeAttribute::eMarker: break;
		case FbxNodeAttribute::eSkeleton: break;
		case FbxNodeAttribute::eMesh:
		{
			if (!_node->GetMesh()->IsTriangleMesh())
			{
				mp_FbxGeoConverter->Triangulate(_node->GetNodeAttributeByIndex(attribute), false);
			}
			HandleMesh(_node);
			m_FoundMesh = true;
			break;
		}
		case FbxNodeAttribute::eNurbs: break;
		case FbxNodeAttribute::ePatch: break;
		case FbxNodeAttribute::eCamera: break;
		case FbxNodeAttribute::eCameraStereo: break;
		case FbxNodeAttribute::eCameraSwitcher: break;
		case FbxNodeAttribute::eLight: break;
		case FbxNodeAttribute::eOpticalReference: break;
		case FbxNodeAttribute::eOpticalMarker: break;
		case FbxNodeAttribute::eNurbsCurve: break;
		case FbxNodeAttribute::eTrimNurbsSurface: break;
		case FbxNodeAttribute::eBoundary: break;
		case FbxNodeAttribute::eNurbsSurface: break;
		case FbxNodeAttribute::eShape: break;
		case FbxNodeAttribute::eLODGroup: break;
		case FbxNodeAttribute::eSubDiv: break;
		case FbxNodeAttribute::eCachedEffect: break;
		case FbxNodeAttribute::eLine: break;
		default: break;
		}
	}

	if (m_FoundMesh)
		return;

	int numChildren = _node->GetChildCount();
	m_DepthLevel++;
	for (int child = 0; child < numChildren; child++)
		HandleNode(_node->GetChild(child));

	m_DepthLevel--;
#if PROGRESSTEXT
	SetConsoleCursorPosition(m_ConsoleHandle, m_MainProgress);
	std::wprintf(L"Processing filename %ls  Node %i of %i      ", ms_ActualFileName.c_str(), m_nodesProcessed, m_numNodes);
	m_nodesProcessed++;
#endif
}

void FBXLoaderManager::ParseOutFileName()
{
	int letterCount = 0;
	int startindex = 0;
	int deleteAt = 0;
	size_t stringSize = ms_AWBXFileName.size();
	while (stringSize > letterCount)
	{
		if (ms_AWBXFileName.at(letterCount) == L'.')
			deleteAt = letterCount;
		if (ms_AWBXFileName.at(letterCount) == L'\\')
			startindex = letterCount;
		letterCount++;
	}

	if (deleteAt > 0)
	{
		ms_AWBXFileName.erase(deleteAt);
		ms_ActualFileName = (ms_AWBXFileName.c_str() + startindex + 1);
		ms_AWBXFileName.append(L".AWBX");
	}
}

void FBXLoaderManager::HandleMesh(fbxsdk::FbxNode* _node)
{
	HandleControlPoints(_node);
	HandleDeformers(_node);
	m_Animation;
	FbxMesh* nodeMesh = _node->GetMesh();

	// Write Out Mesh Header Data
	DataType meshDataType = MeshData;
	writeStream.write((char*)&meshDataType, sizeof(DataType));

	// TODO: Implement parent index tracker for mesh hierarchy
	//unsigned int parentIndex;

	fbxsdk::FbxAMatrix globalMatrix = _node->EvaluateGlobalTransform();
	writeStream.write((char*)&globalMatrix, sizeof(fbxsdk::FbxDouble4) * 4);

	std::vector<Vertex> uniqueVerticies;
	unsigned int m_numUniqueVerts = 0;

	unsigned int m_numIndexes = nodeMesh->GetPolygonVertexCount();
	unsigned int* m_IndexBuffer = new unsigned int[m_numIndexes];

	auto versBuffer = nodeMesh->GetControlPoints();

	// For each polygon in the input mesh
	int nodeMeshPolyCount = nodeMesh->GetPolygonCount();
	int currVert = 0;
	for (int iPolygon = 0; iPolygon < nodeMeshPolyCount; iPolygon++)
	{
		_ASSERT_EXPR(3 == nodeMesh->GetPolygonSize(iPolygon), L"Polygon does not have exactly 3 verts!");
		// For each vertex in the polygon
		for (unsigned iPolygonVertex = 0; iPolygonVertex < 3; iPolygonVertex++)
		{

#if PROGRESSTEXT
			SetConsoleCursorPosition(m_ConsoleHandle, m_SecondaryProgress);
			std::wprintf(L"Processing Poly %i of %i Polygons          ", iPolygon, nodeMeshPolyCount);
#endif

			int fbxCornerIndex = nodeMesh->GetPolygonVertex(iPolygon, iPolygonVertex);
			Vertex newVertex;

#pragma region Get Verticies

			fbxsdk::FbxVector4 fbxVertex = versBuffer[fbxCornerIndex];
			newVertex.m_vertexData.m_Position[0] = (float)fbxVertex[0];
			newVertex.m_vertexData.m_Position[1] = (float)fbxVertex[1];
			newVertex.m_vertexData.m_Position[2] = (float)fbxVertex[2];

#pragma endregion

#pragma region Get Normals

			fbxsdk::FbxVector4 fbxNormal;
			nodeMesh->GetPolygonVertexNormal(iPolygon, iPolygonVertex, fbxNormal);
			fbxNormal.Normalize();
			newVertex.m_vertexData.m_Normal[0] = (float)fbxNormal[0];
			newVertex.m_vertexData.m_Normal[1] = (float)fbxNormal[1];
			newVertex.m_vertexData.m_Normal[2] = (float)fbxNormal[2];

#pragma endregion

#pragma region Get UVs

			fbxsdk::FbxVector2 fbxUV = fbxsdk::FbxVector2(0.0, 0.0);
			fbxsdk::FbxLayerElementUV* elementUV = nodeMesh->GetLayer(0)->GetUVs();
			if (elementUV)
			{
				int iUVIndex = fbxCornerIndex;
				switch (elementUV->GetMappingMode())
				{
				case fbxsdk::FbxLayerElement::eByControlPoint:
				{
					switch (elementUV->GetReferenceMode())
					{
					case FbxGeometryElement::eIndexToDirect:
						iUVIndex = elementUV->GetIndexArray().GetAt(fbxCornerIndex);
						break;
					default: break;
					}
					break;
				}
				case fbxsdk::FbxLayerElement::eByPolygonVertex:
				{
					switch (elementUV->GetReferenceMode())
					{
					case FbxGeometryElement::eDirect:
					case FbxGeometryElement::eIndexToDirect:
						iUVIndex = nodeMesh->GetTextureUVIndex(iPolygon, iPolygonVertex);
						break;
					default: break;
					}
					break;
				}
				default: break;
				}
				fbxUV = elementUV->GetDirectArray().GetAt(iUVIndex);
				newVertex.m_textureData.m_UV[0] = (float)fbxUV[0];
				newVertex.m_textureData.m_UV[1] = (float)fbxUV[1];
				if (FLIP_UV_Y)
					newVertex.m_textureData.m_UV[1] = 1.0f - (float)fbxUV[1];
			}

#pragma endregion

#pragma region Get Tangents

			FbxVector4 Tangent;
			FbxGeometryElementTangent* elementTangent = nodeMesh->GetElementTangent(0);
			if (elementTangent)
			{
				int tangentIndex = fbxCornerIndex;
				FbxLayerElement::EMappingMode tangentMapMode = elementTangent->GetMappingMode();
				if (tangentMapMode == FbxGeometryElement::eByPolygonVertex)
				{
					switch (elementTangent->GetReferenceMode())
					{
					case FbxGeometryElement::eDirect:
						tangentIndex = currVert;
						break;
					case FbxGeometryElement::eIndexToDirect:
						tangentIndex = elementTangent->GetIndexArray().GetAt(currVert);
						break;
					default: break;
					}
				}
				else if (tangentMapMode == FbxGeometryElement::eByControlPoint)
				{
					switch (elementTangent->GetReferenceMode())
					{
					case FbxGeometryElement::eIndexToDirect:
						tangentIndex = elementTangent->GetIndexArray().GetAt(fbxCornerIndex);
						break;
					default: break;
					}
				}
				Tangent = elementTangent->GetDirectArray().GetAt(tangentIndex);
				newVertex.m_textureData.m_Tangent[0] = (float)Tangent[0];
				newVertex.m_textureData.m_Tangent[1] = (float)Tangent[1];
				newVertex.m_textureData.m_Tangent[2] = (float)Tangent[2];
				newVertex.m_textureData.m_Tangent[3] = (float)Tangent[3];
			}

#pragma endregion

#pragma region GetAnimation Data

			newVertex.m_animationData.m_JointIndex[0] = m_ControlPoints[fbxCornerIndex].m_BoneWeights[0].m_JointIndex;
			newVertex.m_animationData.m_JointIndex[1] = m_ControlPoints[fbxCornerIndex].m_BoneWeights[1].m_JointIndex;
			newVertex.m_animationData.m_JointIndex[2] = m_ControlPoints[fbxCornerIndex].m_BoneWeights[2].m_JointIndex;
			newVertex.m_animationData.m_JointIndex[3] = m_ControlPoints[fbxCornerIndex].m_BoneWeights[3].m_JointIndex;

			newVertex.m_animationData.m_JointWeight[0] = m_ControlPoints[fbxCornerIndex].m_BoneWeights[0].m_JointWeight;
			newVertex.m_animationData.m_JointWeight[1] = m_ControlPoints[fbxCornerIndex].m_BoneWeights[1].m_JointWeight;
			newVertex.m_animationData.m_JointWeight[2] = m_ControlPoints[fbxCornerIndex].m_BoneWeights[2].m_JointWeight;
			newVertex.m_animationData.m_JointWeight[3] = m_ControlPoints[fbxCornerIndex].m_BoneWeights[3].m_JointWeight;

#pragma endregion

			bool unique = true;
			for (unsigned int uniqueVert = 0; uniqueVert < m_numUniqueVerts; uniqueVert++)
			{
				if (uniqueVerticies[uniqueVert] == newVertex)
				{
					m_IndexBuffer[currVert] = uniqueVert;
					unique = false;
					break;
				}
			}

			if (unique)
			{
				uniqueVerticies.push_back(newVertex);
				m_IndexBuffer[currVert] = m_numUniqueVerts;
				m_numUniqueVerts++;
			}

			currVert++;
		}//End each Vert
	}//End Each Poly

	VBuffer* m_VertBuffer = new VBuffer[m_numUniqueVerts];
	TBuffer* m_TextureBuffer = new TBuffer[m_numUniqueVerts];
	ABuffer* m_AnimationBuffer = new ABuffer[m_numUniqueVerts];

	for (unsigned int currVert = 0; currVert < m_numUniqueVerts; currVert++)
	{
		memcpy(&m_VertBuffer[currVert], &uniqueVerticies[currVert].m_vertexData, m_VBufferByteSize);
		memcpy(&m_TextureBuffer[currVert], &uniqueVerticies[currVert].m_textureData, m_TBufferByteSize);
		memcpy(&m_AnimationBuffer[currVert], &uniqueVerticies[currVert].m_animationData, m_ABufferByteSize);
	}

#pragma region Write out Data Buffers

#pragma region Vertex Data

	VertexHeader newVertHeader;
	newVertHeader.m_DataByteSize = m_numUniqueVerts * m_VBufferByteSize;
	writeStream.write((char*)&newVertHeader.m_DataType, sizeof(DataType));
	writeStream.write((char*)&newVertHeader.m_DataByteSize, sizeof(unsigned int));
	writeStream.write((char*)m_VertBuffer, newVertHeader.m_DataByteSize);

	delete[] m_VertBuffer;
	m_VertBuffer = nullptr;

#pragma endregion

#pragma region Index Data

	IndexHeader newIndexHeader;
	newIndexHeader.m_DataByteSize = m_numIndexes * sizeof(unsigned int);
	writeStream.write((char*)&newIndexHeader.m_DataType, sizeof(DataType));
	writeStream.write((char*)&newIndexHeader.m_DataByteSize, sizeof(unsigned int));
	writeStream.write((char*)m_IndexBuffer, newIndexHeader.m_DataByteSize);

	delete[] m_IndexBuffer;
	m_IndexBuffer = nullptr;

#pragma endregion

#pragma region Texture Data

	TextureHeader newTextureHeader;
	newTextureHeader.m_DataByteSize = m_numUniqueVerts * m_TBufferByteSize;
	writeStream.write((char*)&newTextureHeader.m_DataType, sizeof(DataType));
	writeStream.write((char*)&newTextureHeader.m_DataByteSize, sizeof(unsigned int));
	writeStream.write((char*)m_TextureBuffer, newTextureHeader.m_DataByteSize);

	delete[] m_TextureBuffer;
	m_TextureBuffer = nullptr;

#pragma endregion

#pragma region Weight Data

	WeightsHeader newWeightHeader;
	newWeightHeader.m_DataByteSize = m_numUniqueVerts * m_ABufferByteSize;
	writeStream.write((char*)&newWeightHeader.m_DataType, sizeof(DataType));
	writeStream.write((char*)&newWeightHeader.m_DataByteSize, sizeof(unsigned int));
	writeStream.write((char*)m_AnimationBuffer, newWeightHeader.m_DataByteSize);

	delete[] m_AnimationBuffer;
	m_AnimationBuffer = nullptr;

#pragma endregion

#pragma region Animation Data

	AnimationHeader newAnimationHeader;
	writeStream.write((char*)&newAnimationHeader.m_DataType, sizeof(DataType));

	AnimationBuffer AnimationDataBuffer;
	AnimationDataBuffer.m_NumKeyFrames = (int)m_Animation.mv_KeyFrames.size();
	writeStream.write((char*)&AnimationDataBuffer.m_NumKeyFrames, sizeof(int));

	AnimationDataBuffer.m_numBones = (int)mv_Joints.size();
	writeStream.write((char*)&AnimationDataBuffer.m_numBones, sizeof(int));
	unsigned int sizeOfFloat = sizeof(float);
	for (int currKeyFrame = 0; currKeyFrame < AnimationDataBuffer.m_NumKeyFrames; currKeyFrame++)
	{
		writeStream.write((char*)&m_Animation.mv_KeyFrames[currKeyFrame].m_TimeStamp, sizeOfFloat);
	}

	unsigned int sizeOfBoneBuffers = sizeof(Bone) * AnimationDataBuffer.m_numBones;
	for (int currKeyFrame = 0; currKeyFrame < AnimationDataBuffer.m_NumKeyFrames; currKeyFrame++)
	{
		writeStream.write((char*)&m_Animation.mv_KeyFrames[currKeyFrame].mv_bones[0], sizeOfBoneBuffers);
	}

#pragma endregion

#pragma endregion

}

void FBXLoaderManager::HandleSkeleton(fbxsdk::FbxNode* _node, int _currIndex, int _parentIndex)
{
	if (_node->GetNodeAttribute() && _node->GetNodeAttribute()->GetAttributeType() && _node->GetNodeAttribute()->GetAttributeType() == FbxNodeAttribute::eSkeleton)
	{
		Joint newJoint;
		newJoint.ms_Name = _node->GetName();
		newJoint.m_ParentIdex = _parentIndex;
		mv_Joints.push_back(newJoint);
	}

	int numChildren = _node->GetChildCount();
	for (int child = 0; child < numChildren; child++)
		HandleSkeleton(_node->GetChild(child), (int)mv_Joints.size(), _currIndex);
}

void FBXLoaderManager::HandleControlPoints(fbxsdk::FbxNode* _node)
{
	FbxMesh* currMesh = _node->GetMesh();
	unsigned int numControlPoints = currMesh->GetControlPointsCount();
	FbxDouble4 currPosition;
	for (unsigned int controlPoint = 0; controlPoint < numControlPoints; controlPoint++)
	{
		ControlPoint newControlPoint;
		m_ControlPoints.push_back(newControlPoint);
		currPosition = currMesh->GetControlPointAt(controlPoint);
		newControlPoint.m_Position[0] = (float)currPosition.mData[0];
		newControlPoint.m_Position[1] = (float)currPosition.mData[1];
		newControlPoint.m_Position[2] = (float)currPosition.mData[2];
	}
}

void FBXLoaderManager::HandleDeformers(fbxsdk::FbxNode* _node)
{
	FbxMesh* _mesh = _node->GetMesh();
	unsigned int numDeformers = _mesh->GetDeformerCount();
	FbxAMatrix meshTransform = FbxAMatrix(
		_node->GetGeometricTranslation(FbxNode::eSourcePivot),
		_node->GetGeometricRotation(FbxNode::eSourcePivot),
		_node->GetGeometricScaling(FbxNode::eSourcePivot));

	

	for (unsigned int deformer = 0; deformer < numDeformers; deformer++)
	{
		FbxSkin* currSkin = reinterpret_cast<FbxSkin*>(_mesh->GetDeformer(deformer, FbxDeformer::eSkin));
		if (!currSkin)
			continue;

		unsigned int numClusters = currSkin->GetClusterCount();

		for (unsigned int cluster = 0; cluster < numClusters; cluster++)
		{
			FbxCluster* currCluster = currSkin->GetCluster(cluster);
			std::string jointName = currCluster->GetLink()->GetName();
			unsigned int jointIndex = GetJointIndexByName(jointName);
			if (jointIndex == -1)
			{
#if _DEBUG
				_ASSERT_EXPR(0, L"Joint name does not exsist in vector!");
#endif
				continue;
			}

			FbxAMatrix transform;
			FbxAMatrix transformLink;
			FbxAMatrix globalBindPoseInverse;

			currCluster->GetTransformMatrix(transform);
			currCluster->GetTransformLinkMatrix(transformLink);
			globalBindPoseInverse = transformLink.Inverse() * transform * meshTransform;
			mv_Joints[jointIndex].m_Node = currCluster->GetLink();
			mv_Joints[jointIndex].m_GlobalBindPoseInverse = globalBindPoseInverse;
			unsigned int numCPIndicies = currCluster->GetControlPointIndicesCount();
			for (unsigned int cpIndex = 0; cpIndex < numCPIndicies; cpIndex++)
			{
				VertexWeight newBoneWeights;
				newBoneWeights.m_JointIndex = jointIndex;
				newBoneWeights.m_JointWeight = (float)currCluster->GetControlPointWeights()[cpIndex];
				m_ControlPoints[currCluster->GetControlPointIndices()[cpIndex]].m_BoneWeights.push_back(newBoneWeights);
			}

			FbxAnimStack* currAniStack = mp_FbxScene->GetSrcObject<fbxsdk::FbxAnimStack>();
			FbxString currAniName = currAniStack->GetName();
			FbxTakeInfo* currTakeInfo = mp_FbxScene->GetTakeInfo(currAniName);
			FbxTime aniStartTime = currTakeInfo->mLocalTimeSpan.GetStart();
			FbxTime aniEndTime = currTakeInfo->mLocalTimeSpan.GetStop();
			long long numAniFrames = aniEndTime.GetFrameCount(FbxTime::eFrames24) - aniStartTime.GetFrameCount(FbxTime::eFrames24) + 1;
			FbxLongLong aniLength = aniEndTime.GetMilliSeconds();

#pragma region Build Animation Shell If not already done

			if (!m_BuiltBaseAnimation)
			{
				m_BuiltBaseAnimation = true;
				for (float timeStamp = 0; timeStamp < aniLength; timeStamp += MILLAT60FPS)
				{
					KeyFrame newKeyFrame;
					newKeyFrame.m_TimeStamp = timeStamp;
					newKeyFrame.mv_bones.resize(mv_Joints.size());
					m_Animation.mv_KeyFrames.push_back(newKeyFrame);

				}
				KeyFrame newKeyFrame;
				newKeyFrame.m_TimeStamp = (float)aniLength;
				newKeyFrame.mv_bones.resize(mv_Joints.size());
				m_Animation.mv_KeyFrames.push_back(newKeyFrame);
			}

#pragma endregion

			int currKeyFrame = 0;
			for (float timeStamp = 0; true; timeStamp += MILLAT60FPS)
			{
				if (timeStamp > aniLength)
					timeStamp =(float)aniLength;
				Bone newBone;
				FbxAMatrix transformOffset = _node->EvaluateGlobalTransform((FbxLongLong)timeStamp) * meshTransform;
				FbxAMatrix globalTransform = transformOffset.Inverse() * currCluster->GetLink()->EvaluateGlobalTransform((FbxLongLong)timeStamp);

				FbxDouble* currVec = globalTransform.GetQ().Buffer();
				newBone.m_QuaternionRot[0] = (float)currVec[0];
				newBone.m_QuaternionRot[1] = (float)currVec[1];
				newBone.m_QuaternionRot[2] = (float)currVec[2];
				newBone.m_QuaternionRot[3] = (float)currVec[3];

				currVec = globalTransform.GetS().Buffer();
				newBone.m_Scaling[0] = (float)currVec[0];
				newBone.m_Scaling[1] = (float)currVec[1];
				newBone.m_Scaling[2] = (float)currVec[2];

				currVec = globalTransform.GetT().Buffer();
				newBone.m_Translation[0] = (float)currVec[0];
				newBone.m_Translation[1] = (float)currVec[1];
				newBone.m_Translation[2] = (float)currVec[2];

				m_Animation.mv_KeyFrames[currKeyFrame].mv_bones[jointIndex] = newBone;
				currKeyFrame++;

				if (timeStamp == aniLength)
					break;
			}
		}

#pragma region Fill Out Dummy Weights

		int numControlPoints = (int)m_ControlPoints.size();
		for (int ctrPt = 0; ctrPt < numControlPoints; ctrPt++)
		{
			int numWeights = (int)m_ControlPoints[ctrPt].m_BoneWeights.size();
			for (int blank = numWeights; blank < 4; blank++)
			{
				VertexWeight blankInfo;
				m_ControlPoints[ctrPt].m_BoneWeights.push_back(blankInfo);
			}
		}
#pragma endregion

	}
}

unsigned int FBXLoaderManager::GetJointIndexByName(std::string _name)
{
	int numJoints = (int)mv_Joints.size();
	for (int joint = 0; joint < numJoints; joint++)
	{
		if (mv_Joints[joint].ms_Name == _name)
			return joint;
	}
	return -1;
}

//void FBXLoaderManager::GatherAnimationData()
//{
//	int numAnimationStacks = mp_FbxScene->GetSrcObjectCount<FbxAnimStack>();
//	int numAnimationLayers = 0;
//	FbxAnimStack* currAnimStack = nullptr;
//	FbxAnimLayer* currAnimLayer = nullptr;
//	FbxAnimCurve* currCurve = nullptr;
//	FbxAnimCurveKey currKey;
//	for (int stack = 0; stack < numAnimationStacks; stack++)
//	{
//		currAnimStack = mp_FbxScene->GetSrcObject<FbxAnimStack>(stack);
//		if (currAnimStack)
//		{
//			numAnimationLayers = currAnimStack->GetMemberCount<FbxAnimLayer>();
//			for (int layer = 0; layer < numAnimationLayers; layer++)
//			{
//				currAnimLayer = currAnimStack->GetMember<FbxAnimLayer>(layer);
//				if (currAnimLayer)
//				{
//					int numCurves = currAnimLayer->GetMemberCount<FbxAnimCurve>();
//					for (int curve = 0; curve < numCurves; curve++)
//					{
//						currCurve = currAnimLayer->GetMember<FbxAnimCurve>(curve);
//						if (currCurve)
//						{
//							int numKeys = currCurve->KeyGetCount();
//							for (int key = 0; key < numKeys; key++)
//							{
//								currKey = currCurve->KeyGet(key);
//								currKey.SetTime(0);
//							}
//						}
//					}
//				}
//			}
//		}
//	}
//}

bool Vertex::operator==(const Vertex& _rhs)
{
	//return !memcmp(this, &_rhs, m_VertexByteSize);

	if (this->m_vertexData.m_Position[0] != _rhs.m_vertexData.m_Position[0] ||
		this->m_vertexData.m_Position[1] != _rhs.m_vertexData.m_Position[1] ||
		this->m_vertexData.m_Position[2] != _rhs.m_vertexData.m_Position[2] ||
		this->m_textureData.m_UV[0] != _rhs.m_textureData.m_UV[0] ||
		this->m_textureData.m_UV[1] != _rhs.m_textureData.m_UV[1] ||
		this->m_vertexData.m_Normal[0] != _rhs.m_vertexData.m_Normal[0] ||
		this->m_vertexData.m_Normal[1] != _rhs.m_vertexData.m_Normal[1] ||
		this->m_vertexData.m_Normal[2] != _rhs.m_vertexData.m_Normal[2] ||
		this->m_textureData.m_Tangent[0] != _rhs.m_textureData.m_Tangent[0] ||
		this->m_textureData.m_Tangent[1] != _rhs.m_textureData.m_Tangent[1] ||
		this->m_textureData.m_Tangent[2] != _rhs.m_textureData.m_Tangent[2] ||
		this->m_textureData.m_Tangent[3] != _rhs.m_textureData.m_Tangent[3] ||
		this->m_vertexData.m_Diffuse[0] != _rhs.m_vertexData.m_Diffuse[0] ||
		this->m_vertexData.m_Diffuse[1] != _rhs.m_vertexData.m_Diffuse[1] ||
		this->m_vertexData.m_Diffuse[2] != _rhs.m_vertexData.m_Diffuse[2] ||
		this->m_vertexData.m_Diffuse[3] != _rhs.m_vertexData.m_Diffuse[3])
		return false;
	return true;
}