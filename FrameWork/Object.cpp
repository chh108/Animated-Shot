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
		for (int i = 0; i < m_nTextures; i++) if (m_ppd3dTextures[i]) m_ppd3dTextures[i]->Release();
		delete[] m_ppd3dTextures;
	}
	if (m_pnResourceTypes) delete[] m_pnResourceTypes;
	if (m_pdxgiBufferFormats) delete[] m_pdxgiBufferFormats;
	if (m_pnBufferElements) delete[] m_pnBufferElements;

	if (m_pnRootParameterIndices) delete[] m_pnRootParameterIndices;
	if (m_pd3dSrvGpuDescriptorHandles) delete[] m_pd3dSrvGpuDescriptorHandles;

	if (m_pd3dSamplerGpuDescriptorHandles) delete[] m_pd3dSamplerGpuDescriptorHandles;
}

void CTexture::SetRootParameterIndex(int nIndex, UINT nRootParameterIndex)
{
	m_pnRootParameterIndices[nIndex] = nRootParameterIndex;
}

void CTexture::SetGpuDescriptorHandle(int nIndex, D3D12_GPU_DESCRIPTOR_HANDLE d3dSrvGpuDescriptorHandle)
{
	m_pd3dSrvGpuDescriptorHandles[nIndex] = d3dSrvGpuDescriptorHandle;
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

void CTexture::ReleaseUploadBuffers()
{
	if (m_ppd3dTextureUploadBuffers)
	{
		for (int i = 0; i < m_nTextures; i++) if (m_ppd3dTextureUploadBuffers[i]) m_ppd3dTextureUploadBuffers[i]->Release();
		delete[] m_ppd3dTextureUploadBuffers;
		m_ppd3dTextureUploadBuffers = NULL;
	}
}

//void CTexture::LoadTextureFromFile(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, wchar_t* pszFileName, UINT nIndex)
//{
//	m_ppd3dTextures[nIndex] = ::CreateTextureResourceFromDDSFile(pd3dDevice, pd3dCommandList, pszFileName, &m_ppd3dTextureUploadBuffers[nIndex], D3D12_RESOURCE_STATE_GENERIC_READ);
//}

void CTexture::LoadTextureFromDDSFile(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, wchar_t* pszFileName, UINT nResourceType, UINT nIndex)
{
	m_pnResourceTypes[nIndex] = nResourceType;
	m_ppd3dTextures[nIndex] = ::CreateTextureResourceFromDDSFile(pd3dDevice, pd3dCommandList, pszFileName, &m_ppd3dTextureUploadBuffers[nIndex], D3D12_RESOURCE_STATE_GENERIC_READ/*D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE*/);
}

void CTexture::LoadBuffer(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, void* pData, UINT nElements, UINT nStride, DXGI_FORMAT ndxgiFormat, UINT nIndex)
{
	m_pnResourceTypes[nIndex] = RESOURCE_BUFFER;
	m_pdxgiBufferFormats[nIndex] = ndxgiFormat;
	m_pnBufferElements[nIndex] = nElements;
	m_ppd3dTextures[nIndex] = ::CreateBufferResource(pd3dDevice, pd3dCommandList, pData, nElements * nStride, D3D12_HEAP_TYPE_DEFAULT, D3D12_RESOURCE_STATE_GENERIC_READ, &m_ppd3dTextureUploadBuffers[nIndex]);
}

ID3D12Resource* CTexture::CreateTexture(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, UINT nIndex, UINT nResourceType, UINT nWidth, UINT nHeight, UINT nElements, UINT nMipLevels, DXGI_FORMAT dxgiFormat, D3D12_RESOURCE_FLAGS d3dResourceFlags, D3D12_RESOURCE_STATES d3dResourceStates, D3D12_CLEAR_VALUE* pd3dClearValue)
{
	m_pnResourceTypes[nIndex] = nResourceType;
	m_ppd3dTextures[nIndex] = ::CreateTexture2DResource(pd3dDevice, pd3dCommandList, nWidth, nHeight, nElements, nMipLevels, dxgiFormat, d3dResourceFlags, d3dResourceStates, pd3dClearValue);
	return(m_ppd3dTextures[nIndex]);
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
	case RESOURCE_TEXTURE2D: //(d3dResourceDesc.Dimension == D3D12_RESOURCE_DIMENSION_TEXTURE2D)(d3dResourceDesc.DepthOrArraySize == 1)
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
	if (m_pShader) m_pShader->Release();	
	m_pShader = pShader;

	if (m_pShader) m_pShader->AddRef();
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
	m_pWireFrameShader->CreateShader(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature, SHADER_TYPE::WireFrame);
	m_pWireFrameShader->CreateShaderVariables(pd3dDevice, pd3dCommandList);

	m_pSkinnedAnimationWireFrameShader = new CSkinnedAnimationWireFrameShader();
	m_pSkinnedAnimationWireFrameShader->CreateShader(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature, SHADER_TYPE::SkinnedAnimationWireFrame);
	m_pSkinnedAnimationWireFrameShader->CreateShaderVariables(pd3dDevice, pd3dCommandList);
}

void CMaterial::UpdateShaderVariable(ID3D12GraphicsCommandList* pd3dCommandList)
{
	for (int i = 0; i < m_nTextures; i++)
	{
		if (m_ppTextures[i]) m_ppTextures[i]->UpdateShaderVariables(pd3dCommandList);
		//		if (m_ppTextures[i]) m_ppTextures[i]->UpdateShaderVariable(pd3dCommandList, 0, 0);
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
CAnimationCurve::CAnimationCurve(int nKeys) // AnimationCurve
{
	m_nKeys = nKeys;
	m_pfKeyTimes = new float[nKeys];
	m_pfKeyValues = new float[nKeys];
}

CAnimationCurve::~CAnimationCurve() // Delete AnimationCurve
{
	if (m_pfKeyTimes) delete[] m_pfKeyTimes;
	if (m_pfKeyValues) delete[] m_pfKeyValues;
}

float CAnimationCurve::GetValueByLinearInterpolation(float fPosition) // 선형 보간. 
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

void CAnimationSets::AddAnimationSet(int nIndex, CAnimationSet *pAnimationSet)
{
	if (nIndex >= 0 && nIndex < m_nAnimationSets)
		m_ppAnimationSets[nIndex] = new CAnimationSet(*pAnimationSet);
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
CAnimationController::CAnimationController(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, int nAnimationTracks, CLoadedModelInfo* pModel, bool bApplyRootMotion)
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

void CAnimationController::AdvanceTime(float fTimeElapsed, CGameObject* pRootGameObject)
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

CGameObject::CGameObject(int nMaterials) : CGameObject()
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
	if (m_pMesh) m_pMesh->Release();

	if (m_nMaterials > 0)
	{
		for (int i = 0; i < m_nMaterials; i++)
		{
			if (m_ppMaterials[i]) m_ppMaterials[i]->Release();
		}
	}
	if (m_ppMaterials) delete[] m_ppMaterials;

	if (m_pSkinnedAnimationController) delete m_pSkinnedAnimationController;
}

void CGameObject::AddRef() 
{ 
	m_nReferences++; 

	if (m_pSibling) m_pSibling->AddRef();
	if (m_pChild) m_pChild->AddRef();
}

void CGameObject::Release() 
{ 
	if (m_pChild) m_pChild->Release();
	if (m_pSibling) m_pSibling->Release();

	if (--m_nReferences <= 0) delete this; 
}

void CGameObject::SetMesh(CMesh* pMesh)
{
	if (m_pMesh) m_pMesh->Release();
	m_pMesh = pMesh;
	if (m_pMesh) m_pMesh->AddRef();
}

void CGameObject::SetShader(CShader* pShader)
{
	m_nMaterials = 1;
	m_ppMaterials = new CMaterial * [m_nMaterials];
	m_ppMaterials[0] = new CMaterial(0);
	m_ppMaterials[0]->SetShader(pShader);
}

void CGameObject::SetShader(int nMaterial, CShader* pShader)
{
	if (m_ppMaterials[nMaterial]) m_ppMaterials[nMaterial]->SetShader(pShader);
}

void CGameObject::SetWireFrameShader()
{
	m_nMaterials = 1;
	m_ppMaterials = new CMaterial * [m_nMaterials];
	m_ppMaterials[0] = NULL;
	CMaterial *pMaterial = new CMaterial(0);
	pMaterial->SetWireFrameShader();
	SetMaterial(0, pMaterial);
}

void CGameObject::SetSkinnedAnimationWireFrameShader()
{
	m_nMaterials = 1;
	m_ppMaterials = new CMaterial * [m_nMaterials];
	m_ppMaterials[0] = NULL;
	CMaterial* pMaterial = new CMaterial(0);
	pMaterial->SetSkinnedAnimationWireFrameShader();
	SetMaterial(0, pMaterial);
}

void CGameObject::SetMaterial(int nMaterial, CMaterial* pMaterial)
{
	if (m_ppMaterials[nMaterial]) m_ppMaterials[nMaterial]->Release();
	m_ppMaterials[nMaterial] = pMaterial;
	if (m_ppMaterials[nMaterial]) m_ppMaterials[nMaterial]->AddRef();
}

void CGameObject::SetChild(CGameObject* pChild, bool bReferenceUpdate)
{
	if (pChild)
	{
		pChild->m_pParent = this;
		if (bReferenceUpdate) pChild->AddRef();
	}
	if (m_pChild)
	{
		if (pChild) pChild->m_pSibling = m_pChild->m_pSibling;
		m_pChild->m_pSibling = pChild;
	}
	else
	{
		m_pChild = pChild;
	}
}

void CGameObject::Animate(float fTimeElapsed)
{
	OnPrepareRender();

	if (m_pSkinnedAnimationController)
	{
		ResetForAnimationBlending();
		m_pSkinnedAnimationController->AdvanceTime(fTimeElapsed, this);
	}

	if (m_pSibling) m_pSibling->Animate(fTimeElapsed);
	if (m_pChild) m_pChild->Animate(fTimeElapsed);
}

void CGameObject::Render(ID3D12GraphicsCommandList *pd3dCommandList, CCamera *pCamera)
{
	if (m_bActive)
	{
		if (m_pSkinnedAnimationController) m_pSkinnedAnimationController->UpdateShaderVariables(pd3dCommandList);

		if (m_pMesh)
		{
			UpdateShaderVariable(pd3dCommandList, &m_xmf4x4World);

			if (m_nMaterials > 0)
			{
				for (int i = 0; i < m_nMaterials; i++)
				{
					if (m_ppMaterials[i])
					{
						if (m_ppMaterials[i]->m_pShader) m_ppMaterials[i]->m_pShader->Render(pd3dCommandList, pCamera);
						m_ppMaterials[i]->UpdateShaderVariable(pd3dCommandList);
					}
					m_pMesh->Render(pd3dCommandList, i);
				}
			}
		}
	}

	if (m_pSibling) m_pSibling->Render(pd3dCommandList, pCamera);
	if (m_pChild) m_pChild->Render(pd3dCommandList, pCamera);
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
	if (m_pSkinnedAnimationController) m_pSkinnedAnimationController->SetTrackAnimationSet(nAnimationTrack, nAnimationSet);
}

void CGameObject::SetTrackAnimationPosition(int nAnimationTrack, float fPosition)
{
	if (m_pSkinnedAnimationController) m_pSkinnedAnimationController->SetTrackPosition(nAnimationTrack, fPosition);
}

void CGameObject::LoadAnimationFromFile(FILE* pInFile, CLoadedModelInfo* pLoadedModel)
{
	char pstrToken[64] = { '\0' };
	UINT nReads = 0;

	int nAnimationSets = 0;

	for (; ; )
	{
		::ReadStringFromFile(pInFile, pstrToken);
		if (!strcmp(pstrToken, "<AnimationSets>:"))
		{
			nAnimationSets = ::ReadIntegerFromFile(pInFile);
			pLoadedModel->m_pAnimationSets = new CAnimationSets(nAnimationSets);
		}
		else if (!strcmp(pstrToken, "<AnimationSet>:"))
		{
			int nAnimationSet = ::ReadIntegerFromFile(pInFile);

			::ReadStringFromFile(pInFile, pstrToken); //Animation Set Name

			float fStartTime = ::ReadFloatFromFile(pInFile);
			float fEndTime = ::ReadFloatFromFile(pInFile);

			pLoadedModel->m_pAnimationSets->m_ppAnimationSets[nAnimationSet] = new CAnimationSet(fStartTime, fEndTime, pstrToken);
			CAnimationSet* pAnimationSet = pLoadedModel->m_pAnimationSets->m_ppAnimationSets[nAnimationSet];

			::ReadStringFromFile(pInFile, pstrToken);
			if (!strcmp(pstrToken, "<AnimationLayers>:"))
			{
				pAnimationSet->m_nAnimationLayers = ::ReadIntegerFromFile(pInFile);
				pAnimationSet->m_pAnimationLayers = new CAnimationLayer[pAnimationSet->m_nAnimationLayers];

				pAnimationSet->m_ppxmf3Scales = new XMFLOAT3 * [pAnimationSet->m_nAnimationLayers]; //for Animation Blending of Layers
				pAnimationSet->m_ppxmf3Rotations = new XMFLOAT3 * [pAnimationSet->m_nAnimationLayers];
				pAnimationSet->m_ppxmf3Translations = new XMFLOAT3 * [pAnimationSet->m_nAnimationLayers];

				for (int i = 0; i < pAnimationSet->m_nAnimationLayers; i++)
				{
					::ReadStringFromFile(pInFile, pstrToken);
					if (!strcmp(pstrToken, "<AnimationLayer>:"))
					{
						int nAnimationLayer = ::ReadIntegerFromFile(pInFile);
						CAnimationLayer* pAnimationLayer = &pAnimationSet->m_pAnimationLayers[nAnimationLayer];

						pAnimationLayer->m_nAnimatedBoneFrames = ::ReadIntegerFromFile(pInFile);
						pAnimationLayer->m_ppAnimatedBoneFrameCaches = new CGameObject * [pAnimationLayer->m_nAnimatedBoneFrames];
						pAnimationLayer->m_ppAnimationCurves = new CAnimationCurve * [pAnimationLayer->m_nAnimatedBoneFrames][9];

						pAnimationSet->m_ppxmf3Scales[i] = new XMFLOAT3[pAnimationLayer->m_nAnimatedBoneFrames];
						pAnimationSet->m_ppxmf3Rotations[i] = new XMFLOAT3[pAnimationLayer->m_nAnimatedBoneFrames];
						pAnimationSet->m_ppxmf3Translations[i] = new XMFLOAT3[pAnimationLayer->m_nAnimatedBoneFrames];

						//						pAnimationLayer->m_nBlendMode = ::ReadIntegerFromFile(pInFile);
						//						if (pAnimationLayer->m_nBlendMode != 1) pAnimationLayer->m_pbBlendedBones = new bool [pAnimationLayer->m_nAnimatedBoneFrames];
						pAnimationLayer->m_fWeight = ::ReadFloatFromFile(pInFile); //m_nBlendMode:Override-Passthrough

						for (int j = 0; j < pAnimationLayer->m_nAnimatedBoneFrames; j++)
						{
							::ReadStringFromFile(pInFile, pstrToken);
							if (!strcmp(pstrToken, "<AnimationCurve>:"))
							{
								int nCurveNode = ::ReadIntegerFromFile(pInFile); //j

								for (int k = 0; k < 9; k++) pAnimationLayer->m_ppAnimationCurves[j][k] = NULL;

								::ReadStringFromFile(pInFile, pstrToken); //Frame Name
								pAnimationLayer->m_ppAnimatedBoneFrameCaches[j] = pLoadedModel->m_pModelRootObject->FindFrame(pstrToken);
								//								if (pAnimationLayer->m_nBlendMode != 1) pAnimationLayer->m_pbBlendedBones[j] = false;

								//								if ((i != 0) && (pAnimationLayer->m_nBlendMode != 1)) pAnimationLayer->m_pbBlendedBones[j] = ::FindBoneFrameFromPreviousAnimationLayers(j, pAnimationSet, pAnimationLayer->m_ppAnimatedBoneFrameCaches[j]);

								for (; ; )
								{
									::ReadStringFromFile(pInFile, pstrToken);
									if (!strcmp(pstrToken, "<TX>:")) pAnimationLayer->LoadAnimationKeyValues(j, 0, pInFile);
									else if (!strcmp(pstrToken, "<TY>:")) pAnimationLayer->LoadAnimationKeyValues(j, 1, pInFile);
									else if (!strcmp(pstrToken, "<TZ>:")) pAnimationLayer->LoadAnimationKeyValues(j, 2, pInFile);
									else if (!strcmp(pstrToken, "<RX>:")) pAnimationLayer->LoadAnimationKeyValues(j, 3, pInFile);
									else if (!strcmp(pstrToken, "<RY>:")) pAnimationLayer->LoadAnimationKeyValues(j, 4, pInFile);
									else if (!strcmp(pstrToken, "<RZ>:")) pAnimationLayer->LoadAnimationKeyValues(j, 5, pInFile);
									else if (!strcmp(pstrToken, "<SX>:")) pAnimationLayer->LoadAnimationKeyValues(j, 6, pInFile);
									else if (!strcmp(pstrToken, "<SY>:")) pAnimationLayer->LoadAnimationKeyValues(j, 7, pInFile);
									else if (!strcmp(pstrToken, "<SZ>:")) pAnimationLayer->LoadAnimationKeyValues(j, 8, pInFile);
									else if (!strcmp(pstrToken, "</AnimationCurve>"))
									{
										break;
									}
								}
							}
						}
						::ReadStringFromFile(pInFile, pstrToken); //</AnimationLayer>
					}
				}
				::ReadStringFromFile(pInFile, pstrToken); //</AnimationLayers>

				int nCommonBoneFrames = 0;
				for (int i = 0; i < pAnimationSet->m_nAnimationLayers; i++)
				{
					CAnimationLayer* piAnimationLayer = &pAnimationSet->m_pAnimationLayers[i];
					for (int j = i + 1; j < pAnimationSet->m_nAnimationLayers; j++)
					{
						CAnimationLayer* pjAnimationLayer = &pAnimationSet->m_pAnimationLayers[j];
						//						if ((i != j) && ())
						for (int k = 0; k < piAnimationLayer->m_nAnimatedBoneFrames; k++)
						{
							for (int h = 0; h < pjAnimationLayer->m_nAnimatedBoneFrames; h++)
							{
								if (piAnimationLayer->m_ppAnimatedBoneFrameCaches[k] == pjAnimationLayer->m_ppAnimatedBoneFrameCaches[h]) nCommonBoneFrames++;
							}
						}
					}
				}
				if (nCommonBoneFrames > 0)
				{
					pAnimationSet->m_nCommonBoneFramesInLayers = nCommonBoneFrames;
					pAnimationSet->m_ppCommonBoneFrameCachesInLayers = new CGameObject * [pAnimationSet->m_nCommonBoneFramesInLayers];
				}
				//				for (int j = 0; j < pAnimationLayer->m_nAnimatedBoneFrames; j++)
//					pAnimationSet->m_ppxmf3Scales[i] = new XMFLOAT3[pAnimationLayer->m_nAnimatedBoneFrames];
//				pAnimationSet->m_ppxmf3Rotations[i] = new XMFLOAT3[pAnimationLayer->m_nAnimatedBoneFrames];
//				pAnimationSet->m_ppxmf3Translations[i] = new XMFLOAT3[pAnimationLayer->m_nAnimatedBoneFrames];
			}
			::ReadStringFromFile(pInFile, pstrToken); //</AnimationSet>
		}
		else if (!strcmp(pstrToken, "</AnimationSets>"))
		{

			break;
		}
	}
}

CGameObject* CGameObject::LoadFrameHierarchyFromFile(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, ID3D12RootSignature* pd3dGraphicsRootSignature, CGameObject* pParent, FILE* pInFile, CShader* pShader, int* pnSkinnedMeshes, int* pnFrames)
{
	char pstrToken[64] = { '\0' };
	UINT nReads = 0;

	(*pnFrames)++;
	int nFrame = ::ReadIntegerFromFile(pInFile);

	CGameObject* pGameObject = new CGameObject();
	::ReadStringFromFile(pInFile, pGameObject->m_pstrFrameName);

	for (; ; )
	{
		::ReadStringFromFile(pInFile, pstrToken);
		if (!strcmp(pstrToken, "<Transform>:")) // Transform
		{
			nReads = (UINT)::fread(&pGameObject->m_xmf4x4ToParent, sizeof(XMFLOAT4X4), 1, pInFile);

			nReads = (UINT)::fread(&pGameObject->m_xmf3Scale, sizeof(XMFLOAT3), 1, pInFile);
			nReads = (UINT)::fread(&pGameObject->m_xmf3Rotation, sizeof(XMFLOAT3), 1, pInFile);
			nReads = (UINT)::fread(&pGameObject->m_xmf3Translation, sizeof(XMFLOAT3), 1, pInFile);

		}
		else if (!strcmp(pstrToken, "<Mesh>:")) // Mesh
		{
			CMesh* pMesh = new CMesh(pd3dDevice, pd3dCommandList);
			pMesh->LoadMeshFromFile(pd3dDevice, pd3dCommandList, pInFile);
			pGameObject->SetMesh(pMesh);

			/**/pGameObject->SetWireFrameShader();
		}
		else if (!strcmp(pstrToken, "<SkinDeformations>:")) // SkinDeformation
		{
			if (pnSkinnedMeshes) (*pnSkinnedMeshes)++;

			CSkinnedMesh* pSkinnedMesh = new CSkinnedMesh(pd3dDevice, pd3dCommandList);
			pSkinnedMesh->LoadSkinDeformationsFromFile(pd3dDevice, pd3dCommandList, pInFile);
			pSkinnedMesh->CreateShaderVariables(pd3dDevice, pd3dCommandList);

			::ReadStringFromFile(pInFile, pstrToken); //<Mesh>:
			if (!strcmp(pstrToken, "<Mesh>:")) pSkinnedMesh->LoadMeshFromFile(pd3dDevice, pd3dCommandList, pInFile);

			pGameObject->SetMesh(pSkinnedMesh);

			/**/pGameObject->SetSkinnedAnimationWireFrameShader();
		}
		else if (!strcmp(pstrToken, "<Children>:")) // Children
		{
			int nChilds = ::ReadIntegerFromFile(pInFile);
			if (nChilds > 0)
			{
				for (int i = 0; i < nChilds; i++)
				{
					::ReadStringFromFile(pInFile, pstrToken);
					if (!strcmp(pstrToken, "<Frame>:"))
					{
						CGameObject* pChild = CGameObject::LoadFrameHierarchyFromFile(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature, pGameObject, pInFile, pShader, pnSkinnedMeshes, pnFrames);
						if (pChild) pGameObject->SetChild(pChild);
#ifdef _WITH_DEBUG_FRAME_HIERARCHY
						TCHAR pstrDebug[256] = { 0 };
						_stprintf_s(pstrDebug, 256, _T("(Frame: %p) (Parent: %p)\n"), pChild, pGameObject);
						OutputDebugString(pstrDebug);
#endif
					}
				}
			}
		}
		else if (!strcmp(pstrToken, "</Frame>"))
		{
			break;
		}
	}
	return(pGameObject);
} 

CLoadedModelInfo* CGameObject::LoadGeometryAndAnimationFromFile(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, ID3D12RootSignature* pd3dGraphicsRootSignature, char* pstrFileName, CShader* pShader)
{
	FILE* pInFile = NULL;
	::fopen_s(&pInFile, pstrFileName, "rb");
	::rewind(pInFile);

	CLoadedModelInfo* pLoadedModel = new CLoadedModelInfo();
	pLoadedModel->m_pModelRootObject = new CGameObject();
	strcpy_s(pLoadedModel->m_pModelRootObject->m_pstrFrameName, "RootNode");

	char pstrToken[64] = { '\0' };
	int nFrames = 0, nSkinnedMeshes = 0;

	for (; ; )
	{
		if (::ReadStringFromFile(pInFile, pstrToken))
		{
			if (!strcmp(pstrToken, "<Hierarchy>"))
			{
				for (; ; )
				{
					::ReadStringFromFile(pInFile, pstrToken);
					if (!strcmp(pstrToken, "<Frame>:"))
					{
						CGameObject* pRootFrame = CGameObject::LoadFrameHierarchyFromFile(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature, NULL, pInFile, pShader, &nSkinnedMeshes, &nFrames);
						if (pRootFrame) pLoadedModel->m_pModelRootObject->SetChild(pRootFrame);
					}
					else if (!strcmp(pstrToken, "</Hierarchy>"))
					{
						break;
					}
				}
			}
			else if (!strcmp(pstrToken, "<Animation>"))
			{
				CGameObject::LoadAnimationFromFile(pInFile, pLoadedModel);

				pLoadedModel->m_pAnimationSets->m_nBoneFrames = nFrames;
				pLoadedModel->m_pAnimationSets->m_ppBoneFrameCaches = new CGameObject * [nFrames];
				int nFrame = 0;
				pLoadedModel->m_pModelRootObject->m_pChild->CacheFrameHierarchies(pLoadedModel->m_pAnimationSets->m_ppBoneFrameCaches, &nFrame);

				pLoadedModel->PrepareSkinning(nSkinnedMeshes);
			}
			else if (!strcmp(pstrToken, "</Animation>"))
			{
				break;
			}
		}
		else
		{
			break;
		}
	}

#ifdef _WITH_DEBUG_FRAME_HIERARCHY
	TCHAR pstrDebug[256] = { 0 };
	_stprintf_s(pstrDebug, 256, _T("Frame Hierarchy\n"));
	OutputDebugString(pstrDebug);

	CGameObject::PrintFrameInfo(pLoadedModel->m_pModelRootObject, NULL);
#endif

	::fclose(pInFile);

	return(pLoadedModel);
}

void CGameObject::PrintFrameInfo(CGameObject* pGameObject, CGameObject* pParent)
{
	TCHAR pstrDebug[256] = { 0 };

	_stprintf_s(pstrDebug, 256, _T("(Frame: %p) (Parent: %p)\n"), pGameObject, pParent);
	OutputDebugString(pstrDebug);

	if (pGameObject->m_pSibling) CGameObject::PrintFrameInfo(pGameObject->m_pSibling, pParent);
	if (pGameObject->m_pChild) CGameObject::PrintFrameInfo(pGameObject->m_pChild, pGameObject);
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
	CGameObject* pFrameObject = FindFrame(pstrFrameName);
	if (pFrameObject) pFrameObject->m_bActive = bActive;
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

	/*
	XMFLOAT4 xmf4Color = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
	if (!strcmp(m_pstrFrameName, "L_shoulder")) xmf4Color = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);
	pd3dCommandList->SetGraphicsRoot32BitConstants(1, 4, &xmf4Color, 16);
*/
}

void CGameObject::UpdateShaderVariable(ID3D12GraphicsCommandList *pd3dCommandList, CMaterial *pMaterial)
{
}

void CGameObject::ReleaseShaderVariables()
{
}

void CGameObject::ReleaseUploadBuffers()
{
	if (m_pMesh) m_pMesh->ReleaseUploadBuffers();

	for (int i = 0; i < m_nMaterials; i++)
	{
		if (m_ppMaterials[i]) m_ppMaterials[i]->ReleaseUploadBuffers();
	}

	if (m_pSibling) m_pSibling->ReleaseUploadBuffers();
	if (m_pChild) m_pChild->ReleaseUploadBuffers();
}

void CGameObject::SetPosition(float x, float y, float z)
{
	m_xmf4x4ToParent._41 = x;
	m_xmf4x4ToParent._42 = y;
	m_xmf4x4ToParent._43 = z;

	UpdateTransform(NULL);
}

void CGameObject::SetPosition(XMFLOAT3 xmf3Position)
{
	SetPosition(xmf3Position.x, xmf3Position.y, xmf3Position.z);
}

void CGameObject::SetScale(float x, float y, float z)
{
	XMMATRIX mtxScale = XMMatrixScaling(x, y, z);
	m_xmf4x4ToParent = Matrix4x4::Multiply(mtxScale, m_xmf4x4ToParent);

	UpdateTransform(NULL);
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
	m_xmf4x4ToParent = Matrix4x4::Multiply(mtxRotate, m_xmf4x4ToParent);

	UpdateTransform(NULL);
}

void CGameObject::Rotate(XMFLOAT3 *pxmf3Axis, float fAngle)
{
	XMMATRIX mtxRotate = XMMatrixRotationAxis(XMLoadFloat3(pxmf3Axis), XMConvertToRadians(fAngle));
	m_xmf4x4ToParent = Matrix4x4::Multiply(mtxRotate, m_xmf4x4ToParent);

	UpdateTransform(NULL);
}

void CGameObject::Rotate(XMFLOAT4* pxmf4Quaternion)
{
	XMMATRIX mtxRotate = XMMatrixRotationQuaternion(XMLoadFloat4(pxmf4Quaternion));
	m_xmf4x4ToParent = Matrix4x4::Multiply(mtxRotate, m_xmf4x4ToParent);

	UpdateTransform(NULL);
}

void CGameObject::UpdateTransform(XMFLOAT4X4* pxmf4x4Parent)
{
	m_xmf4x4World = (pxmf4x4Parent) ? Matrix4x4::Multiply(m_xmf4x4ToParent, *pxmf4x4Parent) : m_xmf4x4ToParent;

	if (m_pSibling) m_pSibling->UpdateTransform(pxmf4x4Parent);
	if (m_pChild) m_pChild->UpdateTransform(&m_xmf4x4World);
}

void CGameObject::ResetForAnimationBlending()
{
	m_xmf3ScaleLayerBlending = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_xmf3RotationLayerBlending = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_xmf3TranslationLayerBlending = XMFLOAT3(0.0f, 0.0f, 0.0f);

	if (m_pSibling) m_pSibling->ResetForAnimationBlending();
	if (m_pChild) m_pChild->ResetForAnimationBlending();
}

void CGameObject::CacheFrameHierarchies(CGameObject** ppBoneFrameCaches, int* pnFrame)
{
	ppBoneFrameCaches[(*pnFrame)++] = this;

	if (m_pSibling) m_pSibling->CacheFrameHierarchies(ppBoneFrameCaches, pnFrame);
	if (m_pChild) m_pChild->CacheFrameHierarchies(ppBoneFrameCaches, pnFrame);
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

	// 문자열 끝에 NULL 종료 문자 추가
	pstrToken[nStrLength] = '\0';

	return(nStrLength);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//
CAngrybotObject::CAngrybotObject(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, ID3D12RootSignature *pd3dGraphicsRootSignature, CLoadedModelInfo* pModel, int nAnimationTracks)
{
	CLoadedModelInfo* pAngrybotModel = pModel;
	if (!pAngrybotModel) pAngrybotModel = CGameObject::LoadGeometryAndAnimationFromFile(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature, "Model/Angrybot.bin", NULL);

	SetChild(pAngrybotModel->m_pModelRootObject, true);
	m_pSkinnedAnimationController = new CAnimationController(pd3dDevice, pd3dCommandList, nAnimationTracks, pAngrybotModel);

	strcpy_s(m_pstrFrameName, "Angrybot");

	Rotate(-90.0f, 0.0f, 0.0f);
	SetScale(0.2f, 0.2f, 0.2f);
}

CAngrybotObject::~CAngrybotObject()
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//
CElvenWitchObject::CElvenWitchObject(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, ID3D12RootSignature* pd3dGraphicsRootSignature, CLoadedModelInfo* pModel, int nAnimationTracks)
{
	CLoadedModelInfo* pElvenWitchModel = pModel;
	if (!pElvenWitchModel) pElvenWitchModel = CGameObject::LoadGeometryAndAnimationFromFile(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature, "Model/Elven_Witch.bin", NULL);

	SetChild(pElvenWitchModel->m_pModelRootObject, true);
	m_pSkinnedAnimationController = new CAnimationController(pd3dDevice, pd3dCommandList, nAnimationTracks, pElvenWitchModel);

	strcpy_s(m_pstrFrameName, "ElvenWitch");

	Rotate(-90.0f, 0.0f, 0.0f);
	SetScale(0.15f, 0.15f, 0.15f);

	SetActive("elven_staff", false);
	SetActive("elven_staff01", false);
}

CElvenWitchObject::~CElvenWitchObject()
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//
CMonsterWeaponObject::CMonsterWeaponObject(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, ID3D12RootSignature* pd3dGraphicsRootSignature, CLoadedModelInfo* pModel, int nAnimationTracks)
{
	CLoadedModelInfo* pMonsterWeaponModel = pModel;
	if (!pMonsterWeaponModel) pMonsterWeaponModel = CGameObject::LoadGeometryAndAnimationFromFile(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature, "Model/MonsterWeapon.bin", NULL);

	SetChild(pMonsterWeaponModel->m_pModelRootObject, true);
	m_pSkinnedAnimationController = new CAnimationController(pd3dDevice, pd3dCommandList, nAnimationTracks, pMonsterWeaponModel);

	strcpy_s(m_pstrFrameName, "MonsterWeapon");

	SetScale(0.35f, 0.35f, 0.35f);
}

CMonsterWeaponObject::~CMonsterWeaponObject()
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//
CLionObject::CLionObject(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, ID3D12RootSignature* pd3dGraphicsRootSignature, CLoadedModelInfo* pModel, int nAnimationTracks)
{
	CLoadedModelInfo* pLionModel = pModel;
	if (!pLionModel) pLionModel = CGameObject::LoadGeometryAndAnimationFromFile(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature, "Model/Lion.bin", NULL);

	SetChild(pLionModel->m_pModelRootObject, true);
	m_pSkinnedAnimationController = new CAnimationController(pd3dDevice, pd3dCommandList, nAnimationTracks, pLionModel);

	strcpy_s(m_pstrFrameName, "Lion");

	Rotate(-90.0f, 0.0f, 0.0f);
	SetScale(0.2f, 0.2f, 0.2f);

	SetActive("SK_Lion_LOD1", false);
	SetActive("SK_Lion_LOD2", false);
	SetActive("SK_Lion_LOD3", false);
}

CLionObject::~CLionObject()
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//
CEagleObject::CEagleObject(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, ID3D12RootSignature* pd3dGraphicsRootSignature, CLoadedModelInfo* pModel, int nAnimationTracks)
{
	CLoadedModelInfo* pEagleModel = pModel;
	if (!pEagleModel) pEagleModel = CGameObject::LoadGeometryAndAnimationFromFile(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature, "Model/Eagle.bin", NULL);

	SetChild(pEagleModel->m_pModelRootObject, true);
	m_pSkinnedAnimationController = new CAnimationController(pd3dDevice, pd3dCommandList, nAnimationTracks, pEagleModel);

	strcpy_s(m_pstrFrameName, "Eagle");

	Rotate(-90.0f, 0.0f, 0.0f);
	SetScale(0.2f, 0.2f, 0.2f);
}

CEagleObject::~CEagleObject()
{
}