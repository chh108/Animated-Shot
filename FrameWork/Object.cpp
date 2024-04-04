//-----------------------------------------------------------------------------
// File: CGameObject.cpp
//-----------------------------------------------------------------------------

#include "stdafx.h"
#include "Object.h"
#include "Shader.h"
#include "Scene.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
CTexture::CTexture(int nTextures, UINT nTextureType, int nSamplers, int nRootParameters)
{
	m_nTextureType = nTextureType;

	m_nTextures = nTextures;
	if (m_nTextures > 0)
	{
		m_ppd3dTextureUploadBuffers = new ID3D12Resource * [m_nTextures];
		m_ppd3dTextures = new ID3D12Resource * [m_nTextures];
		for (int i = 0; i < m_nTextures; i++) m_ppd3dTextureUploadBuffers[i] = m_ppd3dTextures[i] = NULL;

		m_pd3dSrvGpuDescriptorHandles = new D3D12_GPU_DESCRIPTOR_HANDLE[m_nTextures];

		m_pnResourceTypes = new UINT[m_nTextures];
		m_pdxgiBufferFormats = new DXGI_FORMAT[m_nTextures];
		m_pnBufferElements = new int[m_nTextures];
	}
	m_nRootParameters = nRootParameters;
	if (nRootParameters > 0) m_pnRootParameterIndices = new UINT[nRootParameters];

	m_nSamplers = nSamplers;
	if (m_nSamplers > 0) m_pd3dSamplerGpuDescriptorHandles = new D3D12_GPU_DESCRIPTOR_HANDLE[m_nSamplers];
}

CTexture::~CTexture()
{
	if (m_ppd3dTextures)
	{
		for (int i = 0; i < m_nTextures; i++)
		{
			if (m_ppd3dTextures[i])
				m_ppd3dTextures[i]->Release();
		}
		delete[] m_ppd3dTextures;
	}
	if (m_pnResourceTypes)
		delete[] m_pnResourceTypes;

	if (m_pdxgiBufferFormats)
		delete[] m_pdxgiBufferFormats;

	if (m_pnBufferElements)
		delete[] m_pnBufferElements;

	if (m_pnRootParameterIndices)
		delete[] m_pnRootParameterIndices;

	if (m_pd3dSrvGpuDescriptorHandles)
		delete[] m_pd3dSrvGpuDescriptorHandles;

	if (m_pd3dSamplerGpuDescriptorHandles)
		delete[] m_pd3dSamplerGpuDescriptorHandles;
}

void CTexture::SetSampler(int nIndex, D3D12_GPU_DESCRIPTOR_HANDLE d3dSamplerGpuDescriptorHandle)
{
	m_pd3dSamplerGpuDescriptorHandles[nIndex] = d3dSamplerGpuDescriptorHandle;
}

void CTexture::UpdateShaderVariables(ID3D12GraphicsCommandList* pd3dCommandList)
{
	if (m_nRootParameters == m_nTextures)
	{
		for (int i = 0; i < m_nRootParameters; i++)
		{
			pd3dCommandList->SetGraphicsRootDescriptorTable(m_pnRootParameterIndices[i], m_pd3dSrvGpuDescriptorHandles[i]);
		}
	}
	else
	{
		pd3dCommandList->SetGraphicsRootDescriptorTable(m_pnRootParameterIndices[0], m_pd3dSrvGpuDescriptorHandles[0]);
	}
}

void CTexture::UpdateShaderVariable(ID3D12GraphicsCommandList* pd3dCommandList, int nParameterIndex, int nTextureIndex)
{
	pd3dCommandList->SetGraphicsRootDescriptorTable(m_pnRootParameterIndices[nParameterIndex], m_pd3dSrvGpuDescriptorHandles[nTextureIndex]);
}

void CTexture::ReleaseShaderVariables()
{
}

void CTexture::LoadTextureFromDDSFile(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, wchar_t* pszFileName, UINT nResourceType, UINT nIndex)
{
	if (m_ppd3dTextureUploadBuffers)
	{
		for (int i = 0; i < m_nTextures; i++)
		{
			if (m_ppd3dTextureUploadBuffers[i])
				m_ppd3dTextureUploadBuffers[i]->Release();

			delete[] m_ppd3dTextureUploadBuffers;

			m_ppd3dTextureUploadBuffers = NULL;
		}
	}
}

void CTexture::LoadBuffer(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, void* pData, UINT nElements, UINT nStride, DXGI_FORMAT ndxgiFormat, UINT nIndex)
{
	m_pnResourceTypes[nIndex] = RESOURCE_BUFFER;
	m_pdxgiBufferFormats[nIndex] = ndxgiFormat;
	m_pnBufferElements[nIndex] = nElements;
	m_ppd3dTextures[nIndex] = ::CreateBufferResource(pd3dDevice, pd3dCommandList, pData, nElements * nStride, D3D12_HEAP_TYPE_DEFAULT,
		D3D12_RESOURCE_STATE_GENERIC_READ, &m_ppd3dTextureUploadBuffers[nIndex]);
}

ID3D12Resource* CTexture::CreateTexture(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, UINT nIndex, UINT nResourceType, UINT nWidth, UINT nHeight, UINT nElements, UINT nMipLevels, DXGI_FORMAT dxgiFormat, D3D12_RESOURCE_FLAGS d3dResourceFlags, D3D12_RESOURCE_STATES d3dResourceStates, D3D12_CLEAR_VALUE* pd3dClearValue)
{
	m_pnResourceTypes[nIndex] = nResourceType;
	m_ppd3dTextures[nIndex] = ::CreateTexture2DResource(pd3dDevice, pd3dCommandList, nWidth, nHeight, nElements, nMipLevels, dxgiFormat, d3dResourceFlags, d3dResourceStates, pd3dClearValue);
	return(m_ppd3dTextures[nIndex]);
}

void CTexture::SetRootParameterIndex(int nIndex, UINT nRootParameterIndex)
{
	m_pnRootParameterIndices[nIndex] = nRootParameterIndex;
}

void CTexture::SetGpuDescriptorHandle(int nIndex, D3D12_GPU_DESCRIPTOR_HANDLE d3dSrvGpuDescriptorHandle)
{
	m_pd3dSrvGpuDescriptorHandles[nIndex] = d3dSrvGpuDescriptorHandle;
}

D3D12_SHADER_RESOURCE_VIEW_DESC CTexture::GetShaderResourceViewDesc(int nIndex)
{
	ID3D12Resource* pShaderResource = GetResource(nIndex);
	D3D12_RESOURCE_DESC d3dResourceDesc = pShaderResource->GetDesc();

	D3D12_SHADER_RESOURCE_VIEW_DESC d3dShaderResourceViewDesc;
	d3dShaderResourceViewDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;

	int nTextureType = GetTextureType(nIndex);
	switch (nTextureType)
	{
	case RESOURCE_TEXTURE2D: 
		//(d3dResourceDesc.Dimension == D3D12_RESOURCE_DIMENSION_TEXTURE2D)(d3dResourceDesc.DepthOrArraySize == 1)
	case RESOURCE_TEXTURE2D_ARRAY: //[]
		d3dShaderResourceViewDesc.Format = d3dResourceDesc.Format;
		d3dShaderResourceViewDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
		d3dShaderResourceViewDesc.Texture2D.MipLevels = -1;
		d3dShaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
		d3dShaderResourceViewDesc.Texture2D.PlaneSlice = 0;
		d3dShaderResourceViewDesc.Texture2D.ResourceMinLODClamp = 0.0f;
		break;

	case RESOURCE_TEXTURE2DARRAY: //(d3dResourceDesc.Dimension == D3D12_RESOURCE_DIMENSION_TEXTURE2D)(d3dResourceDesc.DepthOrArraySize != 1)
		d3dShaderResourceViewDesc.Format = d3dResourceDesc.Format;
		d3dShaderResourceViewDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2DARRAY;
		d3dShaderResourceViewDesc.Texture2DArray.MipLevels = -1;
		d3dShaderResourceViewDesc.Texture2DArray.MostDetailedMip = 0;
		d3dShaderResourceViewDesc.Texture2DArray.PlaneSlice = 0;
		d3dShaderResourceViewDesc.Texture2DArray.ResourceMinLODClamp = 0.0f;
		d3dShaderResourceViewDesc.Texture2DArray.FirstArraySlice = 0;
		d3dShaderResourceViewDesc.Texture2DArray.ArraySize = d3dResourceDesc.DepthOrArraySize;
		break;

	case RESOURCE_TEXTURE_CUBE: //(d3dResourceDesc.Dimension == D3D12_RESOURCE_DIMENSION_TEXTURE2D)(d3dResourceDesc.DepthOrArraySize == 6)
		d3dShaderResourceViewDesc.Format = d3dResourceDesc.Format;
		d3dShaderResourceViewDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURECUBE;
		d3dShaderResourceViewDesc.TextureCube.MipLevels = 1;
		d3dShaderResourceViewDesc.TextureCube.MostDetailedMip = 0;
		d3dShaderResourceViewDesc.TextureCube.ResourceMinLODClamp = 0.0f;
		break;

	case RESOURCE_BUFFER: //(d3dResourceDesc.Dimension == D3D12_RESOURCE_DIMENSION_BUFFER)
		d3dShaderResourceViewDesc.Format = m_pdxgiBufferFormats[nIndex];
		d3dShaderResourceViewDesc.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;
		d3dShaderResourceViewDesc.Buffer.FirstElement = 0;
		d3dShaderResourceViewDesc.Buffer.NumElements = m_pnBufferElements[nIndex];
		d3dShaderResourceViewDesc.Buffer.StructureByteStride = 0;
		d3dShaderResourceViewDesc.Buffer.Flags = D3D12_BUFFER_SRV_FLAG_NONE;
		break;
	}
	return(d3dShaderResourceViewDesc);
}

void CTexture::ReleaseUploadBuffers()
{
	if (m_ppd3dTextureUploadBuffers)
	{
		for (int i = 0; i < m_nTextures; i++) if (m_ppd3dTextureUploadBuffers[i]) m_ppd3dTextureUploadBuffers[i]->Release();
		delete[] m_ppd3dTextureUploadBuffers;
		m_ppd3dTextureUploadBuffers = NULL;
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
CMaterial::CMaterial(int nTextures)
{
	m_nTextures = nTextures;

	m_ppTextures = new CTexture * [m_nTextures];
	m_ppstrTextureNames = new _TCHAR[m_nTextures][64];
	for (int i = 0; i < m_nTextures; i++)
		m_ppTextures[i] = NULL;
	for (int i = 0; i < m_nTextures; i++)
		m_ppstrTextureNames[i][0] = '\0';
}

CMaterial::~CMaterial()
{
	if (m_pShader) m_pShader->Release();

	if (m_nTextures > 0)
	{
		for (int i = 0; i < m_nTextures; i++) if (m_ppTextures[i]) m_ppTextures[i]->Release();
		delete[] m_ppTextures;

		if (m_ppstrTextureNames) delete[] m_ppstrTextureNames;
	}
}

void CMaterial::SetShader(CShader* pShader)
{
	if (m_pShader) 
		m_pShader->Release();
	
	m_pShader = pShader;

	if (m_pShader) 
		m_pShader->AddRef();
}

void CMaterial::SetTexture(CTexture* pTexture, UINT nTexture)
{
	if (m_ppTextures[nTexture]) 
		m_ppTextures[nTexture]->Release();
	
	m_ppTextures[nTexture] = pTexture;
	
	if (m_ppTextures[nTexture]) 
		m_ppTextures[nTexture]->AddRef();
}

void CMaterial::ReleaseUploadBuffers()
{
	for (int i = 0; i < m_nTextures; i++)
	{
		if (m_ppTextures[i]) m_ppTextures[i]->ReleaseUploadBuffers();
	}
}

CShader* CMaterial::m_pWireFrameShader = NULL;
CShader* CMaterial::m_pSkinnedAnimationWireFrameShader = NULL;

void CMaterial::PrepareShaders(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, ID3D12RootSignature* pd3dGraphicsRootSignature)
{
	m_pWireFrameShader = new CWireFrameShader();
	m_pWireFrameShader->CreateShader(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature);
	m_pWireFrameShader->CreateShaderVariables(pd3dDevice, pd3dCommandList);

	m_pSkinnedAnimationWireFrameShader = new CSkinnedAnimationWireFrameShader();
	m_pSkinnedAnimationWireFrameShader->CreateShader(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature);
	m_pSkinnedAnimationWireFrameShader->CreateShaderVariables(pd3dDevice, pd3dCommandList);
}

void CMaterial::UpdateShaderVariable(ID3D12GraphicsCommandList* pd3dCommandList)
{
	for (int i = 0; i < m_nTextures; i++)
	{
		if (m_ppTextures[i]) 
			m_ppTextures[i]->UpdateShaderVariables(pd3dCommandList);
		//		if (m_ppTextures[i]) m_ppTextures[i]->UpdateShaderVariable(pd3dCommandList, 0, 0);
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
CAnimationCurve::CAnimationCurve(int nKeys)
{
	m_nKeys = nKeys;
	m_pfKeyTimes = new float[nKeys];
	m_pfKeyValues = new float[nKeys];
}

CAnimationCurve::~CAnimationCurve()
{
	if (m_pfKeyTimes) delete[] m_pfKeyTimes;
	if (m_pfKeyValues) delete[] m_pfKeyValues;
}

float CAnimationCurve::GetValueByLinearInterpolation(float fPosition)
{
	for (int k = 0; k < (m_nKeys - 1); k++)
	{
		if ((m_pfKeyTimes[k] <= fPosition) && (fPosition < m_pfKeyTimes[k + 1]))
		{
			float t = (fPosition - m_pfKeyTimes[k]) / (m_pfKeyTimes[k + 1] - m_pfKeyTimes[k]);
			return(m_pfKeyValues[k] * (1.0f - t) + m_pfKeyValues[k + 1] * t);
		}
	}
	return(m_pfKeyValues[m_nKeys - 1]);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
CAnimationLayer::CAnimationLayer()
{
}

CAnimationLayer::~CAnimationLayer()
{
	for (int i = 0; i < m_nAnimatedBoneFrames; i++)
	{
		for (int j = 0; j < 9; j++)
		{
			if (m_ppAnimationCurves[i][j]) delete m_ppAnimationCurves[i][j];
		}
	}
	if (m_ppAnimationCurves) delete[] m_ppAnimationCurves;

	if (m_ppAnimatedBoneFrameCaches) delete[] m_ppAnimatedBoneFrameCaches;
}

void CAnimationLayer::LoadAnimationKeyValues(int nBoneFrame, int nCurve, FILE* pInFile)
{
	int nAnimationKeys = ::ReadIntegerFromFile(pInFile);

	m_ppAnimationCurves[nBoneFrame][nCurve] = new CAnimationCurve(nAnimationKeys);

	::fread(m_ppAnimationCurves[nBoneFrame][nCurve]->m_pfKeyTimes, sizeof(float), nAnimationKeys, pInFile);
	::fread(m_ppAnimationCurves[nBoneFrame][nCurve]->m_pfKeyValues, sizeof(float), nAnimationKeys, pInFile);
}

void CAnimationLayer::GetSRT(int j, float fPosition, XMFLOAT3* pxmf3Scales, XMFLOAT3* pxmf3Rotations, XMFLOAT3* pxmf3Translations)
{
	if (m_ppAnimationCurves[j][0]) pxmf3Translations[j].x = m_ppAnimationCurves[j][0]->GetValueByLinearInterpolation(fPosition);
	if (m_ppAnimationCurves[j][1]) pxmf3Translations[j].y = m_ppAnimationCurves[j][1]->GetValueByLinearInterpolation(fPosition);
	if (m_ppAnimationCurves[j][2]) pxmf3Translations[j].z = m_ppAnimationCurves[j][2]->GetValueByLinearInterpolation(fPosition);
	if (m_ppAnimationCurves[j][3]) pxmf3Rotations[j].x = m_ppAnimationCurves[j][3]->GetValueByLinearInterpolation(fPosition);
	if (m_ppAnimationCurves[j][4]) pxmf3Rotations[j].y = m_ppAnimationCurves[j][4]->GetValueByLinearInterpolation(fPosition);
	if (m_ppAnimationCurves[j][5]) pxmf3Rotations[j].z = m_ppAnimationCurves[j][5]->GetValueByLinearInterpolation(fPosition);
	if (m_ppAnimationCurves[j][6]) pxmf3Scales[j].x = m_ppAnimationCurves[j][6]->GetValueByLinearInterpolation(fPosition);
	if (m_ppAnimationCurves[j][7]) pxmf3Scales[j].y = m_ppAnimationCurves[j][7]->GetValueByLinearInterpolation(fPosition);
	if (m_ppAnimationCurves[j][8]) pxmf3Scales[j].z = m_ppAnimationCurves[j][8]->GetValueByLinearInterpolation(fPosition);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
CAnimationSet::CAnimationSet(float fStartTime, float fEndTime, char* pstrName)
{
	m_fStartTime = fStartTime;
	m_fEndTime = fEndTime;
	m_fLength = fEndTime - fStartTime;

	strcpy_s(m_pstrAnimationSetName, 64, pstrName);
}

CAnimationSet::~CAnimationSet()
{
	if (m_pAnimationLayers) delete[] m_pAnimationLayers;

	if (m_pCallbackKeys) delete[] m_pCallbackKeys;
	if (m_pAnimationCallbackHandler) delete m_pAnimationCallbackHandler;
}

void CAnimationSet::HandleCallback()
{
	if (m_pAnimationCallbackHandler)
	{
		for (int i = 0; i < m_nCallbackKeys; i++)
		{
			if (::IsEqual(m_pCallbackKeys[i].m_fTime, m_fPosition, ANIMATION_CALLBACK_EPSILON))
			{
				if (m_pCallbackKeys[i].m_pCallbackData) m_pAnimationCallbackHandler->HandleCallback(m_pCallbackKeys[i].m_pCallbackData, m_fPosition);
				break;
			}
		}
	}
}

float CAnimationSet::UpdatePosition(float fElapsedTime, float fTrackStartTime, float fTrackEndTime)
{
	switch (m_nType)
	{
	case ANIMATION_TYPE_LOOP:
	{
		m_fPosition += fElapsedTime;
		if (m_fPosition < fTrackStartTime) m_fPosition = fTrackStartTime;
		if (m_fPosition > fTrackEndTime) m_fPosition = fTrackStartTime;
		//			m_fPosition = fmod(fTrackPosition, m_pfKeyFrameTimes[m_nKeyFrames-1]); // m_fPosition = fTrackPosition - int(fTrackPosition / m_pfKeyFrameTimes[m_nKeyFrames-1]) * m_pfKeyFrameTimes[m_nKeyFrames-1];
		//			m_fPosition = fmod(fTrackPosition, m_fLength); //if (m_fPosition < 0) m_fPosition += m_fLength;
		//			m_fPosition = fTrackPosition - int(fTrackPosition / m_fLength) * m_fLength;
		break;
	}
	case ANIMATION_TYPE_ONCE:
		m_fPosition += fElapsedTime;
		if (m_fPosition < fTrackStartTime) m_fPosition = fTrackStartTime;
		if (m_fPosition > fTrackEndTime) m_fPosition = fTrackEndTime;
		break;
	case ANIMATION_TYPE_PINGPONG:
		break;
	}

	return(m_fPosition);
}

void CAnimationSet::Animate(float fElapsedTime, float fTrackWeight, float fTrackStartTime, float fTrackEndTime, bool bOverride)
{
	float fPosition = UpdatePosition(fElapsedTime, fTrackStartTime, fTrackEndTime);

	for (int i = 0; i < m_nAnimationLayers; i++)
	{
		for (int j = 0; j < m_pAnimationLayers[i].m_nAnimatedBoneFrames; j++)
		{
			CGameObject* pBoneFrameCache = m_pAnimationLayers[i].m_ppAnimatedBoneFrameCaches[j];
			m_ppxmf3Scales[i][j] = pBoneFrameCache->m_xmf3Scale;
			m_ppxmf3Rotations[i][j] = pBoneFrameCache->m_xmf3Rotation;
			m_ppxmf3Translations[i][j] = pBoneFrameCache->m_xmf3Translation;

			m_pAnimationLayers[i].GetSRT(j, fPosition, m_ppxmf3Scales[i], m_ppxmf3Rotations[i], m_ppxmf3Translations[i]);
		}
	}

	for (int i = 0; i < m_nAnimationLayers; i++) //Blending of Animation Layers
	{
		for (int j = 0; j < m_pAnimationLayers[i].m_nAnimatedBoneFrames; j++)
		{
			CGameObject* pBoneFrameCache = m_pAnimationLayers[i].m_ppAnimatedBoneFrameCaches[j];
			switch (m_pAnimationLayers[i].m_nBlendMode)
			{
			case 0: //Additive
				pBoneFrameCache->m_xmf3ScaleLayerBlending = Vector3::Add(pBoneFrameCache->m_xmf3ScaleLayerBlending, m_ppxmf3Scales[i][j]);
				pBoneFrameCache->m_xmf3RotationLayerBlending = Vector3::Add(pBoneFrameCache->m_xmf3RotationLayerBlending, m_ppxmf3Rotations[i][j]);
				pBoneFrameCache->m_xmf3TranslationLayerBlending = Vector3::Add(pBoneFrameCache->m_xmf3TranslationLayerBlending, m_ppxmf3Translations[i][j]);
				break;
			case 1: //Override
				pBoneFrameCache->m_xmf3ScaleLayerBlending = m_ppxmf3Scales[i][j];
				pBoneFrameCache->m_xmf3RotationLayerBlending = m_ppxmf3Rotations[i][j];
				pBoneFrameCache->m_xmf3TranslationLayerBlending = m_ppxmf3Translations[i][j];
				break;
			case 2: //Override-Passthrough
				pBoneFrameCache->m_xmf3ScaleLayerBlending = Vector3::Add(pBoneFrameCache->m_xmf3ScaleLayerBlending, m_ppxmf3Scales[i][j], m_pAnimationLayers[i].m_fWeight);
				pBoneFrameCache->m_xmf3RotationLayerBlending = Vector3::Add(pBoneFrameCache->m_xmf3RotationLayerBlending, m_ppxmf3Rotations[i][j], m_pAnimationLayers[i].m_fWeight);
				pBoneFrameCache->m_xmf3TranslationLayerBlending = Vector3::Add(pBoneFrameCache->m_xmf3TranslationLayerBlending, m_ppxmf3Translations[i][j], m_pAnimationLayers[i].m_fWeight);
				break;
			}
		}
	}
}

void CAnimationSet::SetCallbackKeys(int nCallbackKeys)
{
	m_nCallbackKeys = nCallbackKeys;
	m_pCallbackKeys = new CALLBACKKEY[nCallbackKeys];
}

void CAnimationSet::SetCallbackKey(int nKeyIndex, float fKeyTime, void* pData)
{
	m_pCallbackKeys[nKeyIndex].m_fTime = fKeyTime;
	m_pCallbackKeys[nKeyIndex].m_pCallbackData = pData;
}

void CAnimationSet::SetAnimationCallbackHandler(CAnimationCallbackHandler* pCallbackHandler)
{
	m_pAnimationCallbackHandler = pCallbackHandler;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
CAnimationSets::CAnimationSets(int nAnimationSets)
{
	m_nAnimationSets = nAnimationSets;
	m_ppAnimationSets = new CAnimationSet * [nAnimationSets];
	for (int i = 0; i < m_nAnimationSets; i++) m_ppAnimationSets[i] = NULL;
}

CAnimationSets::~CAnimationSets()
{
	for (int i = 0; i < m_nAnimationSets; i++) if (m_ppAnimationSets[i]) delete m_ppAnimationSets[i];
	if (m_ppAnimationSets) delete[] m_ppAnimationSets;

	if (m_ppSkinnedMeshes) delete[] m_ppSkinnedMeshes;
}

void CAnimationSets::SetCallbackKeys(int nAnimationSet, int nCallbackKeys)
{
	m_ppAnimationSets[nAnimationSet]->m_nCallbackKeys = nCallbackKeys;
	m_ppAnimationSets[nAnimationSet]->m_pCallbackKeys = new CALLBACKKEY[nCallbackKeys];
}

void CAnimationSets::SetCallbackKey(int nAnimationSet, int nKeyIndex, float fKeyTime, void* pData)
{
	m_ppAnimationSets[nAnimationSet]->m_pCallbackKeys[nKeyIndex].m_fTime = fKeyTime;
	m_ppAnimationSets[nAnimationSet]->m_pCallbackKeys[nKeyIndex].m_pCallbackData = pData;
}

void CAnimationSets::SetAnimationCallbackHandler(int nAnimationSet, CAnimationCallbackHandler* pCallbackHandler)
{
	m_ppAnimationSets[nAnimationSet]->SetAnimationCallbackHandler(pCallbackHandler);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
CLoadedModelInfo::~CLoadedModelInfo()
{
}

void CLoadedModelInfo::PrepareSkinning(int nSkinnedMeshes)
{
	m_pAnimationSets->m_nSkinnedMeshes = nSkinnedMeshes;
	m_pAnimationSets->m_ppSkinnedMeshes = new CSkinnedMesh * [m_pAnimationSets->m_nSkinnedMeshes];

	int nSkinnedMesh = 0;
	m_pModelRootObject->FindAndSetSkinnedMesh(m_pAnimationSets->m_ppSkinnedMeshes, &nSkinnedMesh);

	for (int i = 0; i < m_pAnimationSets->m_nSkinnedMeshes; i++) m_pAnimationSets->m_ppSkinnedMeshes[i]->PrepareSkinning(m_pModelRootObject);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
CAnimationController::CAnimationController(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, int nAnimationTracks, CLoadedModelInfo* pModel, bool bApplyRootMotion = false)
{
	m_nAnimationTracks = nAnimationTracks;
	m_pAnimationTracks = new CAnimationTrack[nAnimationTracks];

	m_bApplyRootMotion = bApplyRootMotion;

	if (m_pAnimationSets) m_pAnimationSets->Release();
	m_pAnimationSets = pModel->m_pAnimationSets;
	if (m_pAnimationSets) m_pAnimationSets->AddRef();

	m_ppd3dcbSkinningBoneTransforms = new ID3D12Resource * [m_pAnimationSets->m_nSkinnedMeshes];
	m_ppcbxmf4x4MappedSkinningBoneTransforms = new XMFLOAT4X4 * [m_pAnimationSets->m_nSkinnedMeshes];

	UINT ncbElementBytes = (((sizeof(XMFLOAT4X4) * SKINNED_ANIMATION_BONES) + 255) & ~255); // 256의 배수로 고정
	for (int i = 0; i < m_pAnimationSets->m_nSkinnedMeshes; i++)
	{
		m_ppd3dcbSkinningBoneTransforms[i] = ::CreateBufferResource(pd3dDevice, pd3dCommandList, NULL, ncbElementBytes, D3D12_HEAP_TYPE_UPLOAD, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER, NULL);
		m_ppd3dcbSkinningBoneTransforms[i]->Map(0, NULL, (void**)&m_ppcbxmf4x4MappedSkinningBoneTransforms[i]);
	}
}

CAnimationController::~CAnimationController()
{
	if (m_pAnimationTracks) delete[] m_pAnimationTracks;

	for (int i = 0; i < m_pAnimationSets->m_nSkinnedMeshes; i++)
	{
		m_ppd3dcbSkinningBoneTransforms[i]->Unmap(0, NULL);
		m_ppd3dcbSkinningBoneTransforms[i]->Release();
	}
	if (m_ppd3dcbSkinningBoneTransforms) delete[] m_ppd3dcbSkinningBoneTransforms;
	if (m_ppcbxmf4x4MappedSkinningBoneTransforms) delete[] m_ppcbxmf4x4MappedSkinningBoneTransforms;

	if (m_pAnimationSets) m_pAnimationSets->Release();
}

void CAnimationController::SetTrackAnimationSet(int nAnimationTrack, int nAnimationSet)
{
	if (m_pAnimationTracks)
	{
		m_pAnimationTracks[nAnimationTrack].SetAnimationSet(nAnimationSet);
		m_pAnimationTracks[nAnimationTrack].SetStartEndTime(m_pAnimationSets->m_ppAnimationSets[nAnimationSet]->m_fStartTime, m_pAnimationSets->m_ppAnimationSets[nAnimationSet]->m_fEndTime);
	}
}

void CAnimationController::SetTrackEnable(int nAnimationTrack, bool bEnable)
{
	if (m_pAnimationTracks) m_pAnimationTracks[nAnimationTrack].SetEnable(bEnable);
}

void CAnimationController::SetTrackPosition(int nAnimationTrack, float fPosition)
{
	if (m_pAnimationTracks)
	{
		m_pAnimationTracks[nAnimationTrack].SetPosition(fPosition);
		if (m_pAnimationSets->m_ppAnimationSets) m_pAnimationSets->m_ppAnimationSets[m_pAnimationTracks[nAnimationTrack].m_nAnimationSet]->SetPosition(fPosition);
	}
}

void CAnimationController::SetTrackSpeed(int nAnimationTrack, float fSpeed)
{
	if (m_pAnimationTracks) m_pAnimationTracks[nAnimationTrack].SetSpeed(fSpeed);
}

void CAnimationController::SetTrackWeight(int nAnimationTrack, float fWeight)
{
	if (m_pAnimationTracks) m_pAnimationTracks[nAnimationTrack].SetWeight(fWeight);
}

void CAnimationController::SetTrackStartEndTime(int nAnimationTrack, float fStartTime, float fEndTime)
{
	if (m_pAnimationTracks) m_pAnimationTracks[nAnimationTrack].SetStartEndTime(fStartTime, fEndTime);
}

void CAnimationController::SetAnimationType(int nAnimationSet, int nType)
{
	if (m_pAnimationSets->m_ppAnimationSets) m_pAnimationSets->m_ppAnimationSets[nAnimationSet]->SetAnimationType(nType);
}

void CAnimationController::SetCallbackKeys(int nAnimationSet, int nCallbackKeys)
{
	if (m_pAnimationSets)
	{
		m_pAnimationSets->m_ppAnimationSets[nAnimationSet]->m_nCallbackKeys = nCallbackKeys;
		m_pAnimationSets->m_ppAnimationSets[nAnimationSet]->m_pCallbackKeys = new CALLBACKKEY[nCallbackKeys];
	}
}

void CAnimationController::SetCallbackKey(int nAnimationSet, int nKeyIndex, float fTime, void* pData)
{
	if (m_pAnimationSets)
	{
		m_pAnimationSets->m_ppAnimationSets[nAnimationSet]->m_pCallbackKeys[nKeyIndex].m_fTime = fTime;
		m_pAnimationSets->m_ppAnimationSets[nAnimationSet]->m_pCallbackKeys[nKeyIndex].m_pCallbackData = pData;
	}
}

void CAnimationController::SetAnimationCallbackHandler(int nAnimationSet, CAnimationCallbackHandler* pCallbackHandler)
{
	if (m_pAnimationSets) m_pAnimationSets->m_ppAnimationSets[nAnimationSet]->SetAnimationCallbackHandler(pCallbackHandler);
}

void CAnimationController::AdvanceTime(float fElapsedTime, CGameObject* pRootGameObject)
{
	m_fTime += fTimeElapsed;
	int nEnabledAnimationTracks = 0;

	if (m_pAnimationTracks)
	{
		for (int i = 0; i < m_nAnimationTracks; i++)
		{
			if (m_pAnimationTracks[i].m_bEnable)
			{
				nEnabledAnimationTracks++;
				CAnimationSet* pAnimationSet = m_pAnimationSets->m_ppAnimationSets[m_pAnimationTracks[i].m_nAnimationSet];
				pAnimationSet->Animate(fTimeElapsed * m_pAnimationTracks[i].m_fSpeed, m_pAnimationTracks[i].m_fWeight, m_pAnimationTracks[i].m_fStartTime, m_pAnimationTracks[i].m_fEndTime, (i == 0));
			}
		}
	}
	//*
	if (nEnabledAnimationTracks == 1)
	{
		for (int i = 0; i < m_nAnimationTracks; i++)
		{
			if (m_pAnimationTracks[i].m_bEnable)
			{
				CAnimationSet* pAnimationSet = m_pAnimationSets->m_ppAnimationSets[m_pAnimationTracks[i].m_nAnimationSet];
				for (int i = 0; i < pAnimationSet->m_nAnimationLayers; i++)
				{
					for (int j = 0; j < pAnimationSet->m_pAnimationLayers[i].m_nAnimatedBoneFrames; j++)
					{
						CGameObject* pBoneFrameCache = pAnimationSet->m_pAnimationLayers[i].m_ppAnimatedBoneFrameCaches[j];
						XMMATRIX S = XMMatrixScaling(pBoneFrameCache->m_xmf3ScaleLayerBlending.x, pBoneFrameCache->m_xmf3ScaleLayerBlending.y, pBoneFrameCache->m_xmf3ScaleLayerBlending.z);
						XMMATRIX R = XMMatrixMultiply(XMMatrixMultiply(XMMatrixRotationX(pBoneFrameCache->m_xmf3RotationLayerBlending.x), XMMatrixRotationY(pBoneFrameCache->m_xmf3RotationLayerBlending.y)), XMMatrixRotationZ(pBoneFrameCache->m_xmf3RotationLayerBlending.z));
						XMMATRIX T = XMMatrixTranslation(pBoneFrameCache->m_xmf3TranslationLayerBlending.x, pBoneFrameCache->m_xmf3TranslationLayerBlending.y, pBoneFrameCache->m_xmf3TranslationLayerBlending.z);
						XMStoreFloat4x4(&pBoneFrameCache->m_xmf4x4ToParent, XMMatrixMultiply(XMMatrixMultiply(S, R), T));
					}
				}
			}
		}
	}
	else
	{
		for (int i = 0; i < m_nAnimationTracks; i++)
		{
			if (m_pAnimationTracks[i].m_bEnable)
			{
				CAnimationSet* pAnimationSet = m_pAnimationSets->m_ppAnimationSets[m_pAnimationTracks[i].m_nAnimationSet];
				for (int i = 0; i < pAnimationSet->m_nAnimationLayers; i++)
				{
					for (int j = 0; j < pAnimationSet->m_pAnimationLayers[i].m_nAnimatedBoneFrames; j++)
					{
						CGameObject* pBoneFrameCache = pAnimationSet->m_pAnimationLayers[i].m_ppAnimatedBoneFrameCaches[j];
						XMMATRIX S = XMMatrixScaling(pBoneFrameCache->m_xmf3ScaleLayerBlending.x, pBoneFrameCache->m_xmf3ScaleLayerBlending.y, pBoneFrameCache->m_xmf3ScaleLayerBlending.z);
						XMMATRIX R = XMMatrixMultiply(XMMatrixMultiply(XMMatrixRotationX(pBoneFrameCache->m_xmf3RotationLayerBlending.x), XMMatrixRotationY(pBoneFrameCache->m_xmf3RotationLayerBlending.y)), XMMatrixRotationZ(pBoneFrameCache->m_xmf3RotationLayerBlending.z));
						XMMATRIX T = XMMatrixTranslation(pBoneFrameCache->m_xmf3TranslationLayerBlending.x, pBoneFrameCache->m_xmf3TranslationLayerBlending.y, pBoneFrameCache->m_xmf3TranslationLayerBlending.z);
						XMStoreFloat4x4(&pBoneFrameCache->m_xmf4x4ToParent, XMMatrixMultiply(XMMatrixMultiply(S, R), T));
					}
				}
			}
		}
	}
	//*/

	pRootGameObject->UpdateTransform(NULL);

	for (int k = 0; k < m_nAnimationTracks; k++)
	{
		if (m_pAnimationTracks[k].m_bEnable && m_pAnimationSets->m_ppAnimationSets) m_pAnimationSets->m_ppAnimationSets[m_pAnimationTracks[k].m_nAnimationSet]->HandleCallback();
	}
}

void CAnimationController::UpdateShaderVariables(ID3D12GraphicsCommandList* pd3dCommandList)
{
	for (int i = 0; i < m_pAnimationSets->m_nSkinnedMeshes; i++)
	{
		m_pAnimationSets->m_ppSkinnedMeshes[i]->m_pd3dcbSkinningBoneTransforms = m_ppd3dcbSkinningBoneTransforms[i];
		m_pAnimationSets->m_ppSkinnedMeshes[i]->m_pcbxmf4x4MappedSkinningBoneTransforms = m_ppcbxmf4x4MappedSkinningBoneTransforms[i];
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
CGameObject::CGameObject()
{
	m_xmf4x4ToParent = Matrix4x4::Identity();
	m_xmf4x4World = Matrix4x4::Identity();
}

CGameObject::CGameObject(int nMaterials)
{
	m_nMaterials = nMaterials;
	if (m_nMaterials > 0)
	{
		m_ppMaterials = new CMaterial * [m_nMaterials];
		for (int i = 0; i < m_nMaterials; i++) m_ppMaterials[i] = NULL;
	}
}

CGameObject::~CGameObject()
{
#ifndef _WITH_FBX_SCENE_INSTANCING
	if (m_pfbxScene) ::ReleaseMeshFromFbxNodeHierarchy(m_pfbxScene->GetRootNode());
	if (m_pfbxScene) m_pfbxScene->Destroy();
#endif
	if (m_pAnimationController) delete m_pAnimationController;
}

void CGameObject::AddRef() 
{ 
	m_nReferences++; 
}

void CGameObject::Release() 
{ 
	if (--m_nReferences <= 0) delete this; 
}

void CGameObject::SetMesh(CMesh* pMesh)
{
}

void CGameObject::SetShader(CShader* pShader)
{
}

void CGameObject::SetShader(int nMaterial, CShader* pShader)
{
}

void CGameObject::SetWireFrameShader()
{
}

void CGameObject::SetSkinnedAnimationWireFrameShader()
{
}

void CGameObject::SetMaterial(int nMaterial, CMaterial* pMaterial)
{
}

void CGameObject::SetChild(CGameObject* pChild, bool bReferenceUpdate)
{
}

void CGameObject::Animate(float fTimeElapsed)
{
	if (m_pAnimationController)
	{
		m_pAnimationController->AdvanceTime(fTimeElapsed);	
		FbxTime fbxCurrentTime = m_pAnimationController->GetCurrentTime();
		::AnimateFbxNodeHierarchy(m_pfbxScene->GetRootNode(), fbxCurrentTime);
	}
}

void CGameObject::Render(ID3D12GraphicsCommandList *pd3dCommandList, CCamera *pCamera)
{
	OnPrepareRender();

	FbxAMatrix fbxf4x4World = ::XmFloat4x4MatrixToFbxMatrix(m_xmf4x4World);
	if (m_pfbxScene) ::RenderFbxNodeHierarchy(pd3dCommandList, m_pfbxScene->GetRootNode(), m_pAnimationController->GetCurrentTime(), fbxf4x4World);
}

CSkinnedMesh* CGameObject::FindSkinnedMesh(char* pstrSkinnedMeshName)
{
	CSkinnedMesh* pSkinnedMesh = NULL;
	if (m_pMesh && (m_pMesh->GetType() & VERTEXT_BONE_INDEX_WEIGHT))
	{
		pSkinnedMesh = (CSkinnedMesh*)m_pMesh;
		if (!strcmp(pSkinnedMesh->m_pstrMeshName, pstrSkinnedMeshName)) return(pSkinnedMesh);
	}

	if (m_pSibling) if (pSkinnedMesh = m_pSibling->FindSkinnedMesh(pstrSkinnedMeshName)) return(pSkinnedMesh);
	if (m_pChild) if (pSkinnedMesh = m_pChild->FindSkinnedMesh(pstrSkinnedMeshName)) return(pSkinnedMesh);

	return(NULL);
}

void CGameObject::FindAndSetSkinnedMesh(CSkinnedMesh** ppSkinnedMeshes, int* pnSkinnedMesh)
{
	if (m_pMesh && (m_pMesh->GetType() & VERTEXT_BONE_INDEX_WEIGHT)) ppSkinnedMeshes[(*pnSkinnedMesh)++] = (CSkinnedMesh*)m_pMesh;

	if (m_pSibling) m_pSibling->FindAndSetSkinnedMesh(ppSkinnedMeshes, pnSkinnedMesh);
	if (m_pChild) m_pChild->FindAndSetSkinnedMesh(ppSkinnedMeshes, pnSkinnedMesh);
}

void CGameObject::SetTrackAnimationSet(int nAnimationTrack, int nAnimationSet)
{
}

void CGameObject::SetTrackAnimationPosition(int nAnimationTrack, float fPosition)
{
}

void CGameObject::LoadAnimationFromFile(FILE* pInFile, CLoadedModelInfo* pLoadedModel)
{
}

CGameObject* CGameObject::LoadFrameHierarchyFromFile(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, ID3D12RootSignature* pd3dGraphicsRootSignature, CGameObject* pParent, FILE* pInFile, CShader* pShader, int* pnSkinnedMeshes, int* pnFrames)
{
	return nullptr;
}

CLoadedModelInfo* CGameObject::LoadGeometryAndAnimationFromFile(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, ID3D12RootSignature* pd3dGraphicsRootSignature, char* pstrFileName, CShader* pShader)
{
	return nullptr;
}

void CGameObject::PrintFrameInfo(CGameObject* pGameObject, CGameObject* pParent)
{
}

CGameObject* CGameObject::FindFrame(char* pstrFrameName)
{
	CGameObject* pFrameObject = NULL;

	if (!strcmp(m_pstrFrameName, pstrFrameName)) return(this);

	if (m_pSibling) if (pFrameObject = m_pSibling->FindFrame(pstrFrameName)) return(pFrameObject);
	if (m_pChild) if (pFrameObject = m_pChild->FindFrame(pstrFrameName)) return(pFrameObject);

	return(NULL);
}

void CGameObject::SetActive(char* pstrFrameName, bool bActive)
{
}

void CGameObject::CreateShaderVariables(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList)
{
}

void CGameObject::UpdateShaderVariables(ID3D12GraphicsCommandList *pd3dCommandList)
{
}

void CGameObject::UpdateShaderVariable(ID3D12GraphicsCommandList *pd3dCommandList, XMFLOAT4X4 *pxmf4x4World)
{
	XMFLOAT4X4 xmf4x4World;
	XMStoreFloat4x4(&xmf4x4World, XMMatrixTranspose(XMLoadFloat4x4(pxmf4x4World)));
	pd3dCommandList->SetGraphicsRoot32BitConstants(1, 16, &xmf4x4World, 0);
}

void CGameObject::UpdateShaderVariable(ID3D12GraphicsCommandList *pd3dCommandList, FbxAMatrix *pfbxf4x4World)
{
	XMFLOAT4X4 xmf4x4World = ::FbxMatrixToXmFloat4x4Matrix(pfbxf4x4World);
	XMStoreFloat4x4(&xmf4x4World, XMMatrixTranspose(XMLoadFloat4x4(&xmf4x4World)));
	pd3dCommandList->SetGraphicsRoot32BitConstants(1, 16, &xmf4x4World, 0);
}

void CGameObject::ReleaseShaderVariables()
{
}

void CGameObject::ReleaseUploadBuffers()
{
#ifndef _WITH_FBX_SCENE_INSTANCING
	if (m_pfbxScene) ::ReleaseUploadBufferFromFbxNodeHierarchy(m_pfbxScene->GetRootNode());
#endif
}

void CGameObject::SetPosition(float x, float y, float z)
{
	m_xmf4x4World._41 = x;
	m_xmf4x4World._42 = y;
	m_xmf4x4World._43 = z;
}

void CGameObject::SetPosition(XMFLOAT3 xmf3Position)
{
	SetPosition(xmf3Position.x, xmf3Position.y, xmf3Position.z);
}

void CGameObject::SetScale(float x, float y, float z)
{
	XMMATRIX mtxScale = XMMatrixScaling(x, y, z);
	m_xmf4x4World = Matrix4x4::Multiply(mtxScale, m_xmf4x4World);
}

XMFLOAT3 CGameObject::GetPosition()
{
	return(XMFLOAT3(m_xmf4x4World._41, m_xmf4x4World._42, m_xmf4x4World._43));
}

XMFLOAT3 CGameObject::GetLook()
{
	return(Vector3::Normalize(XMFLOAT3(m_xmf4x4World._31, m_xmf4x4World._32, m_xmf4x4World._33)));
}

XMFLOAT3 CGameObject::GetUp()
{
	return(Vector3::Normalize(XMFLOAT3(m_xmf4x4World._21, m_xmf4x4World._22, m_xmf4x4World._23)));
}

XMFLOAT3 CGameObject::GetRight()
{
	return(Vector3::Normalize(XMFLOAT3(m_xmf4x4World._11, m_xmf4x4World._12, m_xmf4x4World._13)));
}

void CGameObject::MoveStrafe(float fDistance)
{
	XMFLOAT3 xmf3Position = GetPosition();
	XMFLOAT3 xmf3Right = GetRight();
	xmf3Position = Vector3::Add(xmf3Position, xmf3Right, fDistance);
	CGameObject::SetPosition(xmf3Position);
}

void CGameObject::MoveUp(float fDistance)
{
	XMFLOAT3 xmf3Position = GetPosition();
	XMFLOAT3 xmf3Up = GetUp();
	xmf3Position = Vector3::Add(xmf3Position, xmf3Up, fDistance);
	CGameObject::SetPosition(xmf3Position);
}

void CGameObject::MoveForward(float fDistance)
{
	XMFLOAT3 xmf3Position = GetPosition();
	XMFLOAT3 xmf3Look = GetLook();
	xmf3Position = Vector3::Add(xmf3Position, xmf3Look, fDistance);
	CGameObject::SetPosition(xmf3Position);
}

void CGameObject::Rotate(float fPitch, float fYaw, float fRoll)
{
	XMMATRIX mtxRotate = XMMatrixRotationRollPitchYaw(XMConvertToRadians(fPitch), XMConvertToRadians(fYaw), XMConvertToRadians(fRoll));
	m_xmf4x4World = Matrix4x4::Multiply(mtxRotate, m_xmf4x4World);
}

void CGameObject::Rotate(XMFLOAT3 *pxmf3Axis, float fAngle)
{
	XMMATRIX mtxRotate = XMMatrixRotationAxis(XMLoadFloat3(pxmf3Axis), XMConvertToRadians(fAngle));
	m_xmf4x4World = Matrix4x4::Multiply(mtxRotate, m_xmf4x4World);
}

void CGameObject::Rotate(XMFLOAT4 *pxmf4Quaternion)
{
	XMMATRIX mtxRotate = XMMatrixRotationQuaternion(XMLoadFloat4(pxmf4Quaternion));
	m_xmf4x4World = Matrix4x4::Multiply(mtxRotate, m_xmf4x4World);
}

void CGameObject::UpdateTransform(XMFLOAT4X4* pxmf4x4Parent)
{
}

void CGameObject::ResetForAnimationBlending()
{
}

void CGameObject::CacheFrameHierachies(CGameObject** ppBoneFrameCaches, int* pnFrame)
{
}

//#define _WITH_DEBUG_FRAME_HIERARCHY

UINT ReadUnsignedIntegerFromFile(FILE* pInFile)
{
	UINT nValue = 0;
	UINT nReads = (UINT)::fread(&nValue, sizeof(UINT), 1, pInFile);
	return(nValue);
}

int ReadIntegerFromFile(FILE* pInFile)
{
	int nValue = 0;
	UINT nReads = (UINT)::fread(&nValue, sizeof(int), 1, pInFile);
	return(nValue);
}

float ReadFloatFromFile(FILE* pInFile)
{
	float fValue = 0;
	UINT nReads = (UINT)::fread(&fValue, sizeof(float), 1, pInFile);
	return(fValue);
}

int ReadStringFromFile(FILE* pInFile, char* pstrToken)
{
	int nStrLength = 0;
	UINT nReads = 0;
	nReads = (UINT)::fread(&nStrLength, sizeof(int), 1, pInFile);
	nReads = (UINT)::fread(pstrToken, sizeof(char), nStrLength, pInFile);
	pstrToken[nStrLength] = '\0';

	return(nStrLength);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//
CAngrybotObject::CAngrybotObject(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, ID3D12RootSignature *pd3dGraphicsRootSignature, FbxManager *pfbxSdkManager, FbxScene *pfbxScene)
{
	m_pfbxScene = pfbxScene;
	if (!m_pfbxScene)
	{
		m_pfbxScene = ::LoadFbxSceneFromFile(pd3dDevice, pd3dCommandList, pfbxSdkManager, "Model/Monster.fbx");
		::CreateMeshFromFbxNodeHierarchy(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature, m_pfbxScene->GetRootNode());
	}
	m_pAnimationController = new CAnimationController(m_pfbxScene);
}

CAngrybotObject::~CAngrybotObject()
{
}