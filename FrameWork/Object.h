//------------------------------------------------------- ----------------------
// File: Object.h
//-----------------------------------------------------------------------------

#pragma once

#include "Mesh.h"
#include "Camera.h"
#include "FbxSceneContext.h"

#define DIR_FORWARD					0x01
#define DIR_BACKWARD				0x02
#define DIR_LEFT					0x04
#define DIR_RIGHT					0x08
#define DIR_UP						0x10
#define DIR_DOWN					0x20

class CShader;
class CStandardShader;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
#define RESOURCE_TEXTURE2D			0x01
#define RESOURCE_TEXTURE2D_ARRAY	0x02
#define	RESOURCE_TEXTURE2DARRAY		0x03
#define RESOURCE_TEXTURE_CUBE		0x04
#define	RESOURCE_BUFFER				0x05

class CTexture
{
public:
	CTexture(int nTextureResources, UINT nResouceType, int nSamplers, int nRootParameters);
	virtual ~CTexture();

private:
	int								m_nReferences = 0;
	int								m_nTextures = 0;

	UINT							m_nTextureType;

	ID3D12Resource**				m_ppd3dTextures = NULL;
	ID3D12Resource**				m_ppd3dTextureUploadBuffers;

	UINT*							m_pnResourceTypes = NULL;

	DXGI_FORMAT*					m_pdxgiBufferFormats = NULL;
	int*							m_pnBufferElements = NULL;

	int								m_nRootParameters = 0;
	UINT*							m_pnRootParameterIndices = NULL;
	D3D12_GPU_DESCRIPTOR_HANDLE*	m_pd3dSrvGpuDescriptorHandles = NULL;

	int m_nSamplers = 0;
	D3D12_GPU_DESCRIPTOR_HANDLE*	m_pd3dSamplerGpuDescriptorHandles = NULL;

public:
	void AddRef() { m_nReferences++; }
	void Release() { if (--m_nReferences <= 0) delete this; }

	void SetSampler(int nIndex, D3D12_GPU_DESCRIPTOR_HANDLE d3dSamplerGpuDescriptorHandle);

	void UpdateShaderVariables(ID3D12GraphicsCommandList* pd3dCommandList);
	void UpdateShaderVariable(ID3D12GraphicsCommandList* pd3dCommandList, int nParameterIndex, int nTextureIndex);
	void ReleaseShaderVariables();

	//	void LoadTextureFromFile(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, wchar_t* pszFileName, UINT nIndex);
	void LoadTextureFromDDSFile(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, wchar_t* pszFileName, UINT nResourceType, UINT nIndex);
	//	void LoadBufferFromFile(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, wchar_t *pszFileName, UINT nIndex);
	void LoadBuffer(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, void* pData, UINT nElements, UINT nStride, DXGI_FORMAT ndxgiFormat, UINT nIndex);
	ID3D12Resource* CreateTexture(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, UINT nIndex, UINT nResourceType,
		UINT nWidth, UINT nHeight, UINT nElements, UINT nMipLevels, DXGI_FORMAT dxgiFormat, D3D12_RESOURCE_FLAGS d3dResourceFlags, D3D12_RESOURCE_STATES d3dResourceStates, D3D12_CLEAR_VALUE* pd3dClearValue);

	void SetRootParameterIndex(int nIndex, UINT nRootParameterIndex);
	void SetGpuDescriptorHandle(int nIndex, D3D12_GPU_DESCRIPTOR_HANDLE d3dSrvGpuDescriptorHandle);

	int GetRootParameters() { return(m_nRootParameters); }
	int GetTextures() { return(m_nTextures); }
	ID3D12Resource* GetResource(int nIndex) { return (m_ppd3dTextures[nIndex]); }

	UINT GetTextureType() { return(m_nTextureType); }
	UINT GetTextureType(int nIndex) { return(m_pnResourceTypes[nIndex]); }
	DXGI_FORMAT GetBufferFormat(int nIndex) { return(m_pdxgiBufferFormats[nIndex]); }
	int GetBufferElemnets(int nIndex) { return(m_pnBufferElements[nIndex]); }

	D3D12_SHADER_RESOURCE_VIEW_DESC GetShaderResourceViewDesc(int nIndex);

	void ReleaseUploadBuffers();

};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//

#define MATERIAL_ALBEDO_MAP			0x01
#define MATERIAL_SPECULAR_MAP		0x02
#define MATERIAL_NORMAL_MAP			0x04
#define MATERIAL_METALLIC_MAP		0x08
#define MATERIAL_EMISSION_MAP		0x10
#define MATERIAL_DETAIL_ALBEDO_MAP	0x20
#define MATERIAL_DETAIL_NORMAL_MAP	0x40

class CGameObject;

class CMaterial
{
public:
	CMaterial(int nTextures);
	virtual ~CMaterial();

private:
	int							m_nReferences = 0;

public:
	void AddRef() { m_nReferences++; }
	void Release() { if (--m_nReferences <= 0) delete this; }

public:
	CShader*					m_pShader = NULL;

	XMFLOAT4					m_xmf4AlbedoColor = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	XMFLOAT4					m_xmf4EmissiveColor = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	XMFLOAT4					m_xmf4SpecularColor = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	XMFLOAT4					m_xmf4AmbientColor = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);

	void SetShader(CShader* pShader);
	void SetMaterialType(UINT nType) { m_nType |= nType; }
	void SetTexture(CTexture* pTexture, UINT nTexture = 0);

	virtual void UpdateShaderVariable(ID3D12GraphicsCommandList* pd3dCommandList);

	virtual void ReleaseUploadBuffers();

public:
	UINT						m_nType = 0x00;

	float						m_fGlossiness = 0.0f;
	float						m_fSmoothness = 0.0f;
	float						m_fSpecularHighlight = 0.0f;
	float						m_fMetallic = 0.0f;
	float						m_fGlossyReflection = 0.0f;

public:
	int 						m_nTextures = 0;
	_TCHAR(*m_ppstrTextureNames)[64] = NULL;
	CTexture** m_ppTextures = NULL; //0:Albedo, 1:Specular, 2:Metallic, 3:Normal, 4:Emission, 5:DetailAlbedo, 6:DetailNormal

	void LoadTextureFromFile(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, UINT nType, UINT nRootParameter, _TCHAR* pwstrTextureName, CTexture** ppTexture, CGameObject* pParent, FILE* pInFile, CShader* pShader);

public:
	static CShader*				m_pWireFrameShader;
	static CShader*				m_pSkinnedAnimationWireFrameShader;

	static void CMaterial::PrepareShaders(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, ID3D12RootSignature* pd3dGraphicsRootSignature);

	void SetWireFrameShader() { CMaterial::SetShader(m_pWireFrameShader); }
	void SetSkinnedAnimationWireFrameShader() { CMaterial::SetShader(m_pSkinnedAnimationWireFrameShader); }
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
struct CALLBACKKEY
{
	float  							m_fTime = 0.0f;
	void* m_pCallbackData = NULL;
};

#define _WITH_ANIMATION_INTERPOLATION

class CAnimationCallbackHandler
{
public:
	CAnimationCallbackHandler() { }
	~CAnimationCallbackHandler() { }

public:
	virtual void HandleCallback(void* pCallbackData, float fTrackPosition) { }
};

class CAnimationCurve
{
public:
	CAnimationCurve(int nKeys);
	~CAnimationCurve();

public:
	int								m_nKeys = 0;

	float*							m_pfKeyTimes = NULL;
	float*							m_pfKeyValues = NULL;

public:
	float GetValueByLinearInterpolation(float fPosition);
};

class CAnimationLayer
{
public:
	CAnimationLayer();
	~CAnimationLayer();

public:
	float							m_fWeight = 1.0f;
	int								m_nBlendMode = 1; //0:Additive, 1:Override, 2:Override-Passthrough

	int								m_nAnimatedBoneFrames = 0;
	CAnimationCurve					*(*m_ppAnimationCurves)[9] = NULL;
	CGameObject						**m_ppAnimatedBoneFrameCaches = NULL; //[m_nAnimatedBoneFrames]

//	bool							*m_pbBlendedBones = NULL;

public:
	void LoadAnimationKeyValues(int nBoneFrame, int nCurve, FILE* pInFile);

	void GetSRT(int nBoneFrame, float fPosition, XMFLOAT3* pxmf3Scales, XMFLOAT3* pxmf3Rotations, XMFLOAT3* pxmf3Translations);
};

class CAnimationSet
{
public:
	CAnimationSet(float fStartTime, float fEndTime, char* pstrName);
	~CAnimationSet();

public:
	char							m_pstrAnimationSetName[64];

	int								m_nAnimationLayers = 0;
	CAnimationLayer* m_pAnimationLayers = NULL;

	int								m_nCommonBoneFramesInLayers = 0;
	CGameObject** m_ppCommonBoneFrameCachesInLayers = NULL; //[m_nAnimationLayers][m_nCommonBoneFramesInLayers]

	XMFLOAT3** m_ppxmf3Scales = NULL;
	XMFLOAT3** m_ppxmf3Rotations = NULL;
	XMFLOAT3** m_ppxmf3Translations = NULL;

	float							m_fStartTime = 0.0f;
	float							m_fEndTime = 0.0f;
	float							m_fLength = 0.0f;

	float 							m_fPosition = 0.0f;
	int 							m_nType = ANIMATION_TYPE_LOOP; //Once, Loop, PingPong

	int 							m_nCallbackKeys = 0;
	CALLBACKKEY* m_pCallbackKeys = NULL;

	CAnimationCallbackHandler* m_pAnimationCallbackHandler = NULL;

public:
	float UpdatePosition(float fElapsedTime, float fStartTime, float fEndTime);
	void SetPosition(float fPosition) { m_fPosition = fPosition; }

	void Animate(float fTrackPosition, float fTrackWeight, float fStartTime, float fEndTime, bool bOverride);

	void SetAnimationType(int nType) { m_nType = nType; }

	void SetCallbackKeys(int nCallbackKeys);
	void SetCallbackKey(int nKeyIndex, float fTime, void* pData);
	void SetAnimationCallbackHandler(CAnimationCallbackHandler* pCallbackHandler);

	void HandleCallback();
};

class CAnimationSets
{
private:
	int								m_nReferences = 0;

public:
	void AddRef() { m_nReferences++; }
	void Release() { if (--m_nReferences <= 0) delete this; }

public:
	CAnimationSets(int nAnimationSets);
	~CAnimationSets();

public:
	int								m_nAnimationSets = 0;
	CAnimationSet** m_ppAnimationSets = NULL;

	int 							m_nSkinnedMeshes = 0;
	CSkinnedMesh** m_ppSkinnedMeshes = NULL; //[SkinnedMeshes], Skinned Mesh Cache

	int								m_nBoneFrames = 0;
	CGameObject** m_ppBoneFrameCaches = NULL; //[m_nBoneFrames]

public:
	void SetCallbackKeys(int nAnimationSet, int nCallbackKeys);
	void SetCallbackKey(int nAnimationSet, int nKeyIndex, float fTime, void* pData);
	void SetAnimationCallbackHandler(int nAnimationSet, CAnimationCallbackHandler* pCallbackHandler);
};

class CAnimationTrack
{
public:
	CAnimationTrack() { }
	~CAnimationTrack() { }

public:
	BOOL 							m_bEnable = true;
	float 							m_fSpeed = 1.0f;
	float 							m_fPosition = 0.0f;
	float 							m_fWeight = 1.0f;

	int 							m_nAnimationSet = 0;

	float							m_fStartTime = 0.0f;
	float							m_fEndTime = 0.0f;
	float							m_fLength = 0.0f;

public:
	void SetAnimationSet(int nAnimationSet) { m_nAnimationSet = nAnimationSet; }

	void SetEnable(bool bEnable) { m_bEnable = bEnable; }
	void SetSpeed(float fSpeed) { m_fSpeed = fSpeed; }
	void SetWeight(float fWeight) { m_fWeight = fWeight; }
	void SetPosition(float fPosition) { m_fPosition = fPosition; }

	void SetStartEndTime(float fTrackStartTime, float fTrackEndTime) { m_fStartTime = fTrackStartTime; m_fEndTime = fTrackEndTime; m_fLength = m_fEndTime - m_fStartTime; }
};

class CLoadedModelInfo
{
public:
	CLoadedModelInfo() { }
	~CLoadedModelInfo();

public:
	CGameObject* m_pModelRootObject = NULL;

	CAnimationSets* m_pAnimationSets = NULL;

public:
	void PrepareSkinning(int nSkinnedMeshes);
};

class CAnimationController 
{
public:
	CAnimationController(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, int nAnimationTracks, CLoadedModelInfo* pModel, bool bApplyRootMotion = false);
	~CAnimationController();

public:
    float 							m_fTime = 0.0f;
	bool							m_bApplyRootMotion = false;

	int 							m_nAnimationTracks = 0;
	CAnimationTrack 				*m_pAnimationTracks = NULL;

	CAnimationSets					*m_pAnimationSets = NULL;

	ID3D12Resource  				**m_ppd3dcbSkinningBoneTransforms = NULL; //[SkinnedMeshes]
	XMFLOAT4X4						**m_ppcbxmf4x4MappedSkinningBoneTransforms = NULL;
public:

	void SetTrackAnimationSet(int nAnimationTrack, int nAnimationSet);
	void SetTrackEnable(int nAnimationTrack, bool bEnable);
	void SetTrackPosition(int nAnimationTrack, float fPosition);
	void SetTrackSpeed(int nAnimationTrack, float fSpeed);
	void SetTrackWeight(int nAnimationTrack, float fWeight);
	void SetTrackStartEndTime(int nAnimationTrack, float fStartTime, float fEndTime);

	void SetAnimationType(int nAnimationSet, int nType);

	void SetCallbackKeys(int nAnimationSet, int nCallbackKeys);
	void SetCallbackKey(int nAnimationSet, int nKeyIndex, float fTime, void* pData);
	void SetAnimationCallbackHandler(int nAnimationSet, CAnimationCallbackHandler* pCallbackHandler);

	void AdvanceTime(float fElapsedTime, CGameObject* pRootGameObject);

	void UpdateShaderVariables(ID3D12GraphicsCommandList* pd3dCommandList);
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
class CGameObject
{
private:
	int								m_nReferences = 0;

public:
	void AddRef();
	void Release();

public:
	CGameObject();
	CGameObject(int nMaterials);
    virtual ~CGameObject();

public:
	bool							m_bActive = true;

	char							m_pstrFrameName[64];

	CMesh							*m_pMesh = NULL;

	int								m_nMaterials = 0;
	CMaterial						**m_ppMaterials = NULL;
	
	XMFLOAT4X4						m_xmf4x4ToParent;
	XMFLOAT4X4  					m_xmf4x4World;

	XMFLOAT3						m_xmf3Scale;
	XMFLOAT3						m_xmf3Rotation;
	XMFLOAT3						m_xmf3Translation;

	XMFLOAT3						m_xmf3ScaleLayerBlending;
	XMFLOAT3						m_xmf3RotationLayerBlending;
	XMFLOAT3						m_xmf3TranslationLayerBlending;

	CGameObject*					m_pParent = NULL;
	CGameObject*					m_pChild = NULL;
	CGameObject*					m_pSibling = NULL;


	CAnimationController 			*m_pAnimationController = NULL;

	void SetMesh(CMesh* pMesh);
	void SetShader(CShader* pShader);
	void SetShader(int nMaterial, CShader* pShader);
	void SetWireFrameShader();
	void SetSkinnedAnimationWireFrameShader();
	void SetMaterial(int nMaterial, CMaterial* pMaterial);

	void SetChild(CGameObject* pChild, bool bReferenceUpdate = false);

	virtual void BuildMaterials(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList) { }

	virtual void OnPrepareAnimate() { }
	virtual void Animate(float fTimeElapsed);

	virtual void OnPrepareRender() { }
	virtual void Render(ID3D12GraphicsCommandList *pd3dCommandList, CCamera *pCamera = NULL);

	virtual void CreateShaderVariables(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList);
	virtual void UpdateShaderVariables(ID3D12GraphicsCommandList *pd3dCommandList);
	virtual void ReleaseShaderVariables();

	static void UpdateShaderVariable(ID3D12GraphicsCommandList *pd3dCommandList, XMFLOAT4X4 *pxmf4x4World);
	static void UpdateShaderVariable(ID3D12GraphicsCommandList *pd3dCommandList, FbxAMatrix *pfbxf4x4World);

	virtual void ReleaseUploadBuffers();

	XMFLOAT3 GetPosition();
	XMFLOAT3 GetLook();
	XMFLOAT3 GetUp();
	XMFLOAT3 GetRight();

	void SetPosition(float x, float y, float z);
	void SetPosition(XMFLOAT3 xmf3Position);
	void SetScale(float x, float y, float z);

	void MoveStrafe(float fDistance = 1.0f);
	void MoveUp(float fDistance = 1.0f);
	void MoveForward(float fDistance = 1.0f);

	void Rotate(float fPitch = 10.0f, float fYaw = 10.0f, float fRoll = 10.0f);
	void Rotate(XMFLOAT3 *pxmf3Axis, float fAngle);
	void Rotate(XMFLOAT4 *pxmf4Quaternion);

	CGameObject* GetParent() { return (m_pParent); }
	void UpdateTransform(XMFLOAT4X4* pxmf4x4Parent = NULL);

	void ResetForAnimationBlending();

	void CacheFrameHierachies(CGameObject** ppBoneFrameCaches, int* pnFrame);

	CGameObject* FindFrame(char* pstrFrameName);
	void SetActive(char* pstrFrameName, bool bActive);

	UINT GetMeshType() { return((m_pMesh) ? m_pMesh->GetType() : 0x00); }

public:
	CAnimationController* m_pSkinnedAnimationController = NULL;

	CSkinnedMesh* FindSkinnedMesh(char* pstrSkinnedMeshName);
	void FindAndSetSkinnedMesh(CSkinnedMesh** ppSkinnedMeshes, int* pnSkinnedMesh);

	void SetTrackAnimationSet(int nAnimationTrack, int nAnimationSet);
	void SetTrackAnimationPosition(int nAnimationTrack, float fPosition);

	static void LoadAnimationFromFile(FILE* pInFile, CLoadedModelInfo* pLoadedModel);
	static CGameObject* LoadFrameHierarchyFromFile(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, ID3D12RootSignature* pd3dGraphicsRootSignature, CGameObject* pParent, FILE* pInFile, CShader* pShader, int* pnSkinnedMeshes, int* pnFrames);

	static CLoadedModelInfo* LoadGeometryAndAnimationFromFile(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, ID3D12RootSignature* pd3dGraphicsRootSignature, char* pstrFileName, CShader* pShader);

	static void PrintFrameInfo(CGameObject* pGameObject, CGameObject* pParent);
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
class CAngrybotObject : public CGameObject
{
public:
	CAngrybotObject(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, ID3D12RootSignature *pd3dGraphicsRootSignature, FbxManager *pfbxSdkManager, FbxScene *pfbxScene);
	virtual ~CAngrybotObject();
};

