//-----------------------------------------------------------------------------
// File: CPlayer.cpp
//-----------------------------------------------------------------------------

#include "stdafx.h"
#include "Player.h"
#include "Shader.h"
#include "SkyBox.h"
#include "Terrain.h"
#include "Scene.h"
#include "Network.h"
#include "PlayerManager.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CPlayer

CPlayer::CPlayer() : CGameObject(1)
{
	m_pCamera = NULL;

	m_xmf3Position = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_xmf3Right = XMFLOAT3(1.0f, 0.0f, 0.0f);
	m_xmf3Up = XMFLOAT3(0.0f, 1.0f, 0.0f);
	m_xmf3Look = XMFLOAT3(0.0f, 0.0f, 1.0f);

	m_xmf3Velocity = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_xmf3Gravity = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_fMaxVelocityXZ = 0.0f;
	m_fMaxVelocityY = 0.0f;
	m_fFriction = 0.0f;

	//m_pxmf4x4Trans = new XMFLOAT4X4
	//{
	//	1.0f, 0.0f, 0.0f, 0.0f,
	//	0.0f, 1.0f, 0.0f, 0.0f,
	//	0.0f, 0.0f, 1.0f, 0.0f,
	//	0.0f, 0.0f, 0.0f, 1.0f,
	//};

	m_fPitch = 0.0f;
	m_fRoll = 0.0f;
	m_fYaw = 0.0f;

	m_pPlayerUpdatedContext = NULL;
	m_pCameraUpdatedContext = NULL;
}

CPlayer::~CPlayer()
{
	ReleaseShaderVariables();

	if (m_pCamera) delete m_pCamera;
}

void CPlayer::CreateShaderVariables(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList)
{
	if (m_pCamera) m_pCamera->CreateShaderVariables(pd3dDevice, pd3dCommandList);
}

void CPlayer::UpdateShaderVariables(ID3D12GraphicsCommandList* pd3dCommandList)
{
}

void CPlayer::ReleaseShaderVariables()
{
	if (m_pCamera) m_pCamera->ReleaseShaderVariables();
}

void CPlayer::Move(DWORD dwDirection, float fDistance, bool bUpdateVelocity)
{
	if (dwDirection)
	{
		float SetSpeed = 0.3f;

		XMFLOAT3 xmf3Shift = XMFLOAT3(0, 0, 0);
		if (dwDirection & DIR_FORWARD) xmf3Shift = Vector3::Add(xmf3Shift, m_xmf3Look, +fDistance);
		if (dwDirection & DIR_BACKWARD) xmf3Shift = Vector3::Add(xmf3Shift, m_xmf3Look, -fDistance);
#ifdef _WITH_LEFT_HAND_COORDINATES
		if (dwDirection & DIR_RIGHT) xmf3Shift = Vector3::Add(xmf3Shift, m_xmf3Right, +fDistance);
		if (dwDirection & DIR_LEFT) xmf3Shift = Vector3::Add(xmf3Shift, m_xmf3Right, -fDistance);
#else
		if (dwDirection & DIR_RIGHT) xmf3Shift = Vector3::Add(xmf3Shift, m_xmf3Right, -fDistance);
		if (dwDirection & DIR_LEFT) xmf3Shift = Vector3::Add(xmf3Shift, m_xmf3Right, +fDistance);
#endif
		if (dwDirection & DIR_UP) xmf3Shift = Vector3::Add(xmf3Shift, m_xmf3Up, +fDistance);
		if (dwDirection & DIR_DOWN) xmf3Shift = Vector3::Add(xmf3Shift, m_xmf3Up, -fDistance);

		xmf3Shift = Vector3::ScalarProduct(xmf3Shift, SetSpeed, false);

		Move(xmf3Shift, bUpdateVelocity);

		CPlayerManager::Get_Instance()->Set_xmf3Look(m_xmf3Look);
		CPlayerManager::Get_Instance()->Set_xmf3Up(m_xmf3Up);
		CPlayerManager::Get_Instance()->Set_xmf3Right(m_xmf3Right);
	}
}

void CPlayer::Move(const XMFLOAT3& xmf3Shift, bool bUpdateVelocity)
{
	// 일정한 속도로 이동하도록 수정
	XMFLOAT3 constantVelocity = xmf3Shift; // 여기서 일정한 속도로 설정

	if (bUpdateVelocity)
	{
		m_xmf3Velocity = { constantVelocity.x * 10.f, constantVelocity.y * 10.f, constantVelocity.z * 10.f }; // 일정한 속도로 고정
	}
	else
	{
		m_xmf3Position = Vector3::Add(m_xmf3Position, constantVelocity);
		m_pCamera->Move(constantVelocity);
	}
}

void CPlayer::Rotate(float x, float y, float z)
{
#ifndef _WITH_LEFT_HAND_COORDINATES
	x = -x; y = -y; z = -z;
#endif
	DWORD nCurrentCameraMode = m_pCamera->GetMode();
	if ((nCurrentCameraMode == FIRST_PERSON_CAMERA) || (nCurrentCameraMode == THIRD_PERSON_CAMERA))
	{
		if (x != 0.0f)
		{
			m_fPitch += x;
			if (m_fPitch > +89.0f) { x -= (m_fPitch - 89.0f); m_fPitch = +89.0f; }
			if (m_fPitch < -89.0f) { x -= (m_fPitch + 89.0f); m_fPitch = -89.0f; }
		}
		if (y != 0.0f)
		{
			m_fYaw += y;
			if (m_fYaw > 360.0f) m_fYaw -= 360.0f;
			if (m_fYaw < 0.0f) m_fYaw += 360.0f;
		}
		if (z != 0.0f)
		{
			m_fRoll += z;
			if (m_fRoll > +20.0f) { z -= (m_fRoll - 20.0f); m_fRoll = +20.0f; }
			if (m_fRoll < -20.0f) { z -= (m_fRoll + 20.0f); m_fRoll = -20.0f; }
		}
		m_pCamera->Rotate(x, y, z);
		if (y != 0.0f)
		{
			XMMATRIX xmmtxRotate = XMMatrixRotationAxis(XMLoadFloat3(&m_xmf3Up), XMConvertToRadians(y));
			m_xmf3Look = Vector3::TransformNormal(m_xmf3Look, xmmtxRotate);
			m_xmf3Right = Vector3::TransformNormal(m_xmf3Right, xmmtxRotate);
		}
	}
	else if (nCurrentCameraMode == SPACESHIP_CAMERA)
	{
		m_pCamera->Rotate(x, y, z);
		if (x != 0.0f)
		{
			XMMATRIX xmmtxRotate = XMMatrixRotationAxis(XMLoadFloat3(&m_xmf3Right), XMConvertToRadians(x));
			m_xmf3Look = Vector3::TransformNormal(m_xmf3Look, xmmtxRotate);
			m_xmf3Up = Vector3::TransformNormal(m_xmf3Up, xmmtxRotate);
		}
		if (y != 0.0f)
		{
			XMMATRIX xmmtxRotate = XMMatrixRotationAxis(XMLoadFloat3(&m_xmf3Up), XMConvertToRadians(y));
			m_xmf3Look = Vector3::TransformNormal(m_xmf3Look, xmmtxRotate);
			m_xmf3Right = Vector3::TransformNormal(m_xmf3Right, xmmtxRotate);
		}
		if (z != 0.0f)
		{
			XMMATRIX xmmtxRotate = XMMatrixRotationAxis(XMLoadFloat3(&m_xmf3Look), XMConvertToRadians(z));
			m_xmf3Up = Vector3::TransformNormal(m_xmf3Up, xmmtxRotate);
			m_xmf3Right = Vector3::TransformNormal(m_xmf3Right, xmmtxRotate);
		}
	}

	m_xmf3Look = Vector3::Normalize(m_xmf3Look);
	m_xmf3Right = Vector3::CrossProduct(m_xmf3Up, m_xmf3Look, true);
	m_xmf3Up = Vector3::CrossProduct(m_xmf3Look, m_xmf3Right, true);
}

void CPlayer::Update(float fTimeElapsed)
{
	if (CPlayerManager::Get_Instance()->Get_Coll())
	{
		SetPosition(CPlayerManager::Get_Instance()->Get_Pos());
		CPlayerManager::Get_Instance()->Set_Coll(true);
	}
	m_xmf3Velocity = Vector3::Add(m_xmf3Velocity, m_xmf3Gravity);
	float fLength = sqrtf(m_xmf3Velocity.x * m_xmf3Velocity.x + m_xmf3Velocity.z * m_xmf3Velocity.z);
	float fMaxVelocityXZ = m_fMaxVelocityXZ;
	if (fLength > m_fMaxVelocityXZ)
	{
		m_xmf3Velocity.x *= (fMaxVelocityXZ / fLength);
		m_xmf3Velocity.z *= (fMaxVelocityXZ / fLength);
	}
	float fMaxVelocityY = m_fMaxVelocityY;
	fLength = sqrtf(m_xmf3Velocity.y * m_xmf3Velocity.y);
	if (fLength > m_fMaxVelocityY) m_xmf3Velocity.y *= (fMaxVelocityY / fLength);

	XMFLOAT3 xmf3Velocity = Vector3::ScalarProduct(m_xmf3Velocity, fTimeElapsed, false);
	Move(xmf3Velocity, false);
	if (m_pPlayerUpdatedContext) OnPlayerUpdateCallback(fTimeElapsed);

	DWORD nCurrentCameraMode = m_pCamera->GetMode();
	if (nCurrentCameraMode == THIRD_PERSON_CAMERA) m_pCamera->Update(m_xmf3Position, fTimeElapsed);
	if (m_pCameraUpdatedContext) OnCameraUpdateCallback(fTimeElapsed);
	if (nCurrentCameraMode == THIRD_PERSON_CAMERA) m_pCamera->SetLookAt(m_xmf3Position);
	m_pCamera->RegenerateViewMatrix();

	fLength = Vector3::Length(m_xmf3Velocity);
	float fDeceleration = (m_fFriction * fTimeElapsed);	
	if (fDeceleration > fLength) fDeceleration = fLength;
	m_xmf3Velocity = Vector3::Add(m_xmf3Velocity, Vector3::ScalarProduct(m_xmf3Velocity, -fDeceleration, true));		// false

	if (GetPosition().x != CPlayerManager::Get_Instance()->Get_Pos().x||
		GetPosition().y != CPlayerManager::Get_Instance()->Get_Pos().y || 
		GetPosition().z != CPlayerManager::Get_Instance()->Get_Pos().z)
	{
		CPlayerManager::Get_Instance()->Set_Pos(GetPosition());        //유월이일
		CNetwork::Get_Instance()->SetSendPacket(CS_MOVE);					//HY
	}

	if (GetLookVector().x != CPlayerManager::Get_Instance()->Get_xmf3Look().x ||
		GetLookVector().y != CPlayerManager::Get_Instance()->Get_xmf3Look().y ||
		GetLookVector().z != CPlayerManager::Get_Instance()->Get_xmf3Look().z ||
		GetUpVector().x != CPlayerManager::Get_Instance()->Get_xmf3Up().x ||
		GetUpVector().y != CPlayerManager::Get_Instance()->Get_xmf3Up().y ||
		GetUpVector().z != CPlayerManager::Get_Instance()->Get_xmf3Up().z || 
		GetRightVector().x != CPlayerManager::Get_Instance()->Get_xmf3Right().x ||
		GetRightVector().y != CPlayerManager::Get_Instance()->Get_xmf3Right().y ||
		GetRightVector().z != CPlayerManager::Get_Instance()->Get_xmf3Right().z )
	{
		CPlayerManager::Get_Instance()->Set_xmf3Look(GetLookVector());        //유월이일
		CPlayerManager::Get_Instance()->Set_xmf3Up(GetUpVector());        //유월이일
		CPlayerManager::Get_Instance()->Set_xmf3Right(GetRightVector());        //유월이일
		CNetwork::Get_Instance()->SetSendPacket(CS_CAMERA);					//HY
	}


}

CCamera* CPlayer::OnChangeCamera(DWORD nNewCameraMode, DWORD nCurrentCameraMode)
{
	CCamera* pNewCamera = NULL;
	switch (nNewCameraMode)
	{
	case FIRST_PERSON_CAMERA:
		pNewCamera = new CFirstPersonCamera(m_pCamera);
		break;
	case THIRD_PERSON_CAMERA:
		pNewCamera = new CThirdPersonCamera(m_pCamera);
		break;
	case SPACESHIP_CAMERA:
		pNewCamera = new CSpaceShipCamera(m_pCamera);
		break;
	}
	if (nCurrentCameraMode == SPACESHIP_CAMERA)
	{
		m_xmf3Right = Vector3::Normalize(XMFLOAT3(m_xmf3Right.x, 0.0f, m_xmf3Right.z));
		m_xmf3Up = Vector3::Normalize(XMFLOAT3(0.0f, 1.0f, 0.0f));
		m_xmf3Look = Vector3::Normalize(XMFLOAT3(m_xmf3Look.x, 0.0f, m_xmf3Look.z));

		m_fPitch = 0.0f;
		m_fRoll = 0.0f;
		m_fYaw = Vector3::Angle(XMFLOAT3(0.0f, 0.0f, 1.0f), m_xmf3Look);
		if (m_xmf3Look.x < 0.0f) m_fYaw = -m_fYaw;
	}
	else if ((nNewCameraMode == SPACESHIP_CAMERA) && m_pCamera)
	{
		m_xmf3Right = m_pCamera->GetRightVector();
		m_xmf3Up = m_pCamera->GetUpVector();
		m_xmf3Look = m_pCamera->GetLookVector();
	}

	if (pNewCamera)
	{
		pNewCamera->SetMode(nNewCameraMode);
		pNewCamera->SetPlayer(this);
	}

	if (m_pCamera) delete m_pCamera;

	return(pNewCamera);
}

void CPlayer::OnPrepareRender()
{
	m_xmf4x4ToParent._11 = m_xmf3Right.x; m_xmf4x4ToParent._12 = m_xmf3Right.y; m_xmf4x4ToParent._13 = m_xmf3Right.z;
	m_xmf4x4ToParent._21 = m_xmf3Up.x; m_xmf4x4ToParent._22 = m_xmf3Up.y; m_xmf4x4ToParent._23 = m_xmf3Up.z;
	m_xmf4x4ToParent._31 = m_xmf3Look.x; m_xmf4x4ToParent._32 = m_xmf3Look.y; m_xmf4x4ToParent._33 = m_xmf3Look.z;
	m_xmf4x4ToParent._41 = m_xmf3Position.x; m_xmf4x4ToParent._42 = m_xmf3Position.y; m_xmf4x4ToParent._43 = m_xmf3Position.z;
}

void CPlayer::Render(ID3D12GraphicsCommandList* pd3dCommandList, CCamera* pCamera)
{
	DWORD nCameraMode = (pCamera) ? pCamera->GetMode() : 0x00;
	if (nCameraMode == THIRD_PERSON_CAMERA) CGameObject::Render(pd3dCommandList, pCamera);
}

void CPlayer::SetTextureByType(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, int nType, void* pArg)
{
	CTexture* pTexture = new CTexture(1, RESOURCE_TEXTURE2D_ARRAY, 0, 1);

	switch (nType) {
	case 1:
		pTexture->LoadTextureFromFile(pd3dDevice, pd3dCommandList, L"Model/Textures/T_Rabby_01.png", RESOURCE_TEXTURE2D_ARRAY, 15, 0, PNG);
		break;
	case 2:
		pTexture->LoadTextureFromFile(pd3dDevice, pd3dCommandList, L"Model/Textures/T_Rabby_02.png", RESOURCE_TEXTURE2D_ARRAY, 15, 0, PNG);
		break;
	case 3:
		pTexture->LoadTextureFromFile(pd3dDevice, pd3dCommandList, L"Model/Textures/T_Rabby_03.png", RESOURCE_TEXTURE2D_ARRAY, 15, 0, PNG);
		break;
	default:
		pTexture->LoadTextureFromFile(pd3dDevice, pd3dCommandList, L"Model/Textures/T_Rabby_01.png", RESOURCE_TEXTURE2D_ARRAY, 15, 0, PNG);
		break;
	}

	CScene::CreateShaderResourceViews(pd3dDevice, pTexture, 0, 15);

	CMaterial* pNewMaterial = new CMaterial(1);
	pNewMaterial->SetTexture(pTexture);

	SetMaterial(0, pNewMaterial);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 
//#define _WITH_DEBUG_CALLBACK_DATA

void CSoundCallbackHandler::HandleCallback(void* pCallbackData, float fTrackPosition)
{
	_TCHAR* pWavName = (_TCHAR*)pCallbackData;
#ifdef _WITH_DEBUG_CALLBACK_DATA
	TCHAR pstrDebug[256] = { 0 };
	_stprintf_s(pstrDebug, 256, _T("%s\n"), pWavName);
	OutputDebugString(pstrDebug);
#endif
#ifdef _WITH_SOUND_RESOURCE
	PlaySound(pWavName, ::ghAppInstance, SND_RESOURCE | SND_ASYNC);
#else
	PlaySound(pWavName, NULL, SND_FILENAME | SND_ASYNC);
#endif
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CAngryBotPlayer

CAngrybotPlayer::CAngrybotPlayer(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, ID3D12RootSignature* pd3dGraphicsRootSignature, void* pContext)
{
	m_pCamera = ChangeCamera(THIRD_PERSON_CAMERA, 0.0f);

	// Animation이 없이 모델만 있는 파일을 읽어올 때 AnimationSet과 같은 변수들을 따로 만들어서 관리해주는 방식이 필요 0419 fin
	// Texture, Material 읽어온 DATA 저장해야함. 0424

	/*CLoadedModelInfo* pRatoModel = CGameObject::LoadGeometryAndAnimationFromFile(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature,
		"Monster/Rato.bin", NULL);
	SetChild(pRatoModel->m_pModelRootObject, true);*/

	CLoadedModelInfo* pPlayerModel = CGameObject::LoadGeometryAndAnimationFromFile(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature,
		"Monster/Rabby_Queen.bin", NULL);
	SetChild(pPlayerModel->m_pModelRootObject, true);

	/*CTexture* pTexture = new CTexture(1, RESOURCE_TEXTURE2D_ARRAY, 0, 1);
	CMaterial* pMaterial = CMaterial::v_Materials[16];
	CTextureProperty TextureProperty = pMaterial->GetTextureProperty(0);
	pTexture = TextureProperty.GetTextureFromVec(0);

	CScene::CreateShaderResourceViews(pd3dDevice, pTexture, 0, 15);

	CMaterial* pNewMaterial = new CMaterial(1);
	pNewMaterial->SetTexture(pTexture);

	SetMaterial(0, pNewMaterial);*/

	m_pSkinnedAnimationController = new CAnimationController(pd3dDevice, pd3dCommandList, 5, pPlayerModel);
	m_pSkinnedAnimationController->SetTrackAnimationSet(PS_IDLE, PS_IDLE);
	m_pSkinnedAnimationController->SetTrackSpeed(PS_IDLE, 1.0f);
	m_pSkinnedAnimationController->SetTrackStartEndTime(PS_IDLE, 0.0f, 1.0f);

	m_pSkinnedAnimationController->SetTrackAnimationSet(PS_WALK, PS_WALK);
	m_pSkinnedAnimationController->SetTrackSpeed(PS_WALK, 0.7f);
	m_pSkinnedAnimationController->SetTrackStartEndTime(PS_WALK, 2.125f, 2.7f);

	m_pSkinnedAnimationController->SetTrackAnimationSet(PS_ATTACK, PS_ATTACK);
	m_pSkinnedAnimationController->SetTrackSpeed(PS_ATTACK, 1.0f);
	m_pSkinnedAnimationController->SetTrackStartEndTime(PS_ATTACK, 1.29f, 1.875f);

	m_pSkinnedAnimationController->SetTrackAnimationSet(PS_DAMAGED, PS_DAMAGED);
	m_pSkinnedAnimationController->SetTrackSpeed(PS_DAMAGED, 1.0f);
	m_pSkinnedAnimationController->SetTrackStartEndTime(PS_DAMAGED, 2.95f, 3.4f);

	m_pSkinnedAnimationController->SetTrackAnimationSet(PS_DIE, PS_DIE);
	m_pSkinnedAnimationController->SetTrackSpeed(PS_DIE, 1.0f);
	m_pSkinnedAnimationController->SetTrackStartEndTime(PS_DIE, 3.8f, 4.6f);

	m_pSkinnedAnimationController->SetTrackEnable(PS_IDLE, true);
	m_pSkinnedAnimationController->SetTrackEnable(PS_WALK, false);
	m_pSkinnedAnimationController->SetTrackEnable(PS_ATTACK, false);
	m_pSkinnedAnimationController->SetTrackEnable(PS_DAMAGED, false);
	m_pSkinnedAnimationController->SetTrackEnable(PS_DIE, false);
	//
	//#ifdef _WITH_SOUND_CALLBACK
	//	m_pSkinnedAnimationController->SetCallbackKeys(PS_WALK, 2);   // SOUND CALLBACK
	//	m_pSkinnedAnimationController->SetCallbackKey(PS_WALK, 0, 1.3f, _T("Sound/SmallFootstep01.wav"));
	//	m_pSkinnedAnimationController->SetCallbackKey(PS_WALK, 1, 1.57f, _T("Sound/SmallFootstep01.wav"));

		/*m_pSkinnedAnimationController = new CAnimationController(pd3dDevice, pd3dCommandList, 5, pRatoModel);
		m_pSkinnedAnimationController->SetTrackAnimationSet(PS_IDLE, PS_IDLE);
		m_pSkinnedAnimationController->SetTrackSpeed(PS_IDLE, 1.0f);
		m_pSkinnedAnimationController->SetTrackStartEndTime(PS_IDLE, 1.3f, 2.25f);

		m_pSkinnedAnimationController->SetTrackAnimationSet(PS_WALK, PS_WALK);
		m_pSkinnedAnimationController->SetTrackSpeed(PS_WALK, 1.0f);
		m_pSkinnedAnimationController->SetTrackStartEndTime(PS_WALK, 0.04f, 1.0f);

		m_pSkinnedAnimationController->SetTrackAnimationSet(PS_ATTACK, PS_ATTACK);
		m_pSkinnedAnimationController->SetTrackSpeed(PS_ATTACK, 1.0f);
		m_pSkinnedAnimationController->SetTrackStartEndTime(PS_ATTACK, 2.5f, 3.01f);

		m_pSkinnedAnimationController->SetTrackAnimationSet(PS_DAMAGED, PS_DAMAGED);
		m_pSkinnedAnimationController->SetTrackSpeed(PS_DAMAGED, 1.0f);
		m_pSkinnedAnimationController->SetTrackStartEndTime(PS_DAMAGED, 5.04f, 5.8f);

		m_pSkinnedAnimationController->SetTrackAnimationSet(PS_DIE, PS_DIE);
		m_pSkinnedAnimationController->SetTrackSpeed(PS_DIE, 1.0f);
		m_pSkinnedAnimationController->SetTrackStartEndTime(PS_DIE, 5.875, 6.875f);

		m_pSkinnedAnimationController->SetTrackEnable(PS_IDLE, false);
		m_pSkinnedAnimationController->SetTrackEnable(PS_WALK, false);
		m_pSkinnedAnimationController->SetTrackEnable(PS_ATTACK, false);
		m_pSkinnedAnimationController->SetTrackEnable(PS_DAMAGED, false);
		m_pSkinnedAnimationController->SetTrackEnable(PS_DIE, false);*/

#ifdef _WITH_SOUND_CALLBACK
	m_pSkinnedAnimationController->SetCallbackKeys(PS_WALK, 2);   // SOUND CALLBACK
	m_pSkinnedAnimationController->SetCallbackKey(PS_WALK, 0, 1.3f, _T("Sound/SmallFootstep01.wav"));
	m_pSkinnedAnimationController->SetCallbackKey(PS_WALK, 1, 1.57f, _T("Sound/SmallFootstep01.wav"));

	CAnimationCallbackHandler* pAnimationCallbackHandler = new CSoundCallbackHandler();
	m_pSkinnedAnimationController->SetAnimationCallbackHandler(PS_WALK, pAnimationCallbackHandler);
#endif

	SetPlayerUpdatedContext(pContext);
	SetCameraUpdatedContext(pContext);

	//if (pRatoModel) delete pRatoModel;
	if (pPlayerModel) delete pPlayerModel;

	CreateShaderVariables(pd3dDevice, pd3dCommandList);

	CHeightMapTerrain* pTerrain = (CHeightMapTerrain*)pContext;
	//SetPosition(XMFLOAT3(310.0f, pTerrain->GetHeight(310.0f, 595.0f) + 10.0f, 595.0f));
	//SetScale(XMFLOAT3(0.3f, 0.3f, 0.3f));
	m_iTpye = CPlayerManager::Get_Instance()->Get_Type();
	SetPosition(CPlayerManager::Get_Instance()->Get_Pos());
	SetScale(XMFLOAT3(0.3f, 0.3f, 0.3f));
	CPlayerManager::Get_Instance()->Set_Pos(GetPosition());
	CPlayerManager::Get_Instance()->Set_Scale(GetScale());
}

CAngrybotPlayer::~CAngrybotPlayer()
{
}

void CAngrybotPlayer::OnPrepareRender()
{
	CPlayer::OnPrepareRender();

	m_xmf4x4ToParent = Matrix4x4::Multiply(XMMatrixScaling(m_xmf3Scale.x, m_xmf3Scale.y, m_xmf3Scale.z), m_xmf4x4ToParent);
	//m_xmf4x4ToParent = Matrix4x4::Multiply(XMMatrixRotationX(-90.0f), m_xmf4x4ToParent);
}

CCamera* CAngrybotPlayer::ChangeCamera(DWORD nNewCameraMode, float fTimeElapsed)
{
	DWORD nCurrentCameraMode = (m_pCamera) ? m_pCamera->GetMode() : 0x00;
	if (nCurrentCameraMode == nNewCameraMode) return(m_pCamera);
	switch (nNewCameraMode)
	{
	case FIRST_PERSON_CAMERA:
		SetFriction(250.0f);
		SetGravity(XMFLOAT3(0.0f, -400.0f, 0.0f));
		SetMaxVelocityXZ(300.0f);
		SetMaxVelocityY(400.0f);
		m_pCamera = OnChangeCamera(FIRST_PERSON_CAMERA, nCurrentCameraMode);
		m_pCamera->SetTimeLag(0.0f);
		m_pCamera->SetOffset(XMFLOAT3(0.0f, 20.0f, 0.0f));
		m_pCamera->GenerateProjectionMatrix(1.01f, 5000.0f, ASPECT_RATIO, 60.0f);
		m_pCamera->SetViewport(0, 0, FRAME_BUFFER_WIDTH, FRAME_BUFFER_HEIGHT, 0.0f, 1.0f);
		m_pCamera->SetScissorRect(0, 0, FRAME_BUFFER_WIDTH, FRAME_BUFFER_HEIGHT);
		break;
	case SPACESHIP_CAMERA:
		SetFriction(125.0f);
		SetGravity(XMFLOAT3(0.0f, 0.0f, 0.0f));
		SetMaxVelocityXZ(300.0f);
		SetMaxVelocityY(400.0f);
		m_pCamera = OnChangeCamera(SPACESHIP_CAMERA, nCurrentCameraMode);
		m_pCamera->SetTimeLag(0.0f);
		m_pCamera->SetOffset(XMFLOAT3(0.0f, 0.0f, 0.0f));
		m_pCamera->GenerateProjectionMatrix(1.01f, 5000.0f, ASPECT_RATIO, 60.0f);
		m_pCamera->SetViewport(0, 0, FRAME_BUFFER_WIDTH, FRAME_BUFFER_HEIGHT, 0.0f, 1.0f);
		m_pCamera->SetScissorRect(0, 0, FRAME_BUFFER_WIDTH, FRAME_BUFFER_HEIGHT);
		break;
	case THIRD_PERSON_CAMERA:
		SetFriction(250.0f);
		SetGravity(XMFLOAT3(0.0f, -250.0f, 0.0f));
		SetMaxVelocityXZ(300.0f);
		SetMaxVelocityY(400.0f);
		m_pCamera = OnChangeCamera(THIRD_PERSON_CAMERA, nCurrentCameraMode);
		m_pCamera->SetTimeLag(0.25f);
		m_pCamera->SetOffset(XMFLOAT3(0.0f, 50.0f, -70.0f));
		m_pCamera->GenerateProjectionMatrix(1.01f, 5000.0f, ASPECT_RATIO, 60.0f);
		m_pCamera->SetViewport(0, 0, FRAME_BUFFER_WIDTH, FRAME_BUFFER_HEIGHT, 0.0f, 1.0f);
		m_pCamera->SetScissorRect(0, 0, FRAME_BUFFER_WIDTH, FRAME_BUFFER_HEIGHT);
		break;
	default:
		break;
	}
	m_pCamera->SetPosition(Vector3::Add(m_xmf3Position, m_pCamera->GetOffset()));
	Update(fTimeElapsed);

	return(m_pCamera);
}

void CAngrybotPlayer::OnPlayerUpdateCallback(float fTimeElapsed)
{
	CHeightMapTerrain* pTerrain = (CHeightMapTerrain*)m_pPlayerUpdatedContext;
	XMFLOAT3 xmf3Scale = pTerrain->GetScale();
	XMFLOAT3 xmf3PlayerPosition = GetPosition();
	int z = (int)(xmf3PlayerPosition.z / xmf3Scale.z);
	bool bReverseQuad = ((z % 2) != 0);
	float fHeight = pTerrain->GetHeight(xmf3PlayerPosition.x, xmf3PlayerPosition.z, bReverseQuad) + 10.0f;
	if (xmf3PlayerPosition.y < fHeight)
	{
		XMFLOAT3 xmf3PlayerVelocity = GetVelocity();
		xmf3PlayerVelocity.y = 0.0f;
		SetVelocity(xmf3PlayerVelocity);
		xmf3PlayerPosition.y = fHeight;
		SetPosition(xmf3PlayerPosition);
	}
}

void CAngrybotPlayer::OnCameraUpdateCallback(float fTimeElapsed)
{
	CHeightMapTerrain* pTerrain = (CHeightMapTerrain*)m_pCameraUpdatedContext;
	XMFLOAT3 xmf3Scale = pTerrain->GetScale();
	XMFLOAT3 xmf3CameraPosition = m_pCamera->GetPosition();
	int z = (int)(xmf3CameraPosition.z / xmf3Scale.z);
	bool bReverseQuad = ((z % 2) != 0);
	float fHeight = pTerrain->GetHeight(xmf3CameraPosition.x, xmf3CameraPosition.z, bReverseQuad) + 15.0f;
	if (xmf3CameraPosition.y <= fHeight)
	{
		xmf3CameraPosition.y = fHeight;
		m_pCamera->SetPosition(xmf3CameraPosition);
		if (m_pCamera->GetMode() == THIRD_PERSON_CAMERA)
		{
			CThirdPersonCamera* p3rdPersonCamera = (CThirdPersonCamera*)m_pCamera;
			p3rdPersonCamera->SetLookAt(GetPosition());
		}
	}
}

#ifdef _WITH_SOUND_CALLBACK
void CAngrybotPlayer::Move(ULONG dwDirection, float fDistance, bool bUpdateVelocity)
{
	m_pSkinnedAnimationController->SetTrackEnable(PS_IDLE, (dwDirection) ? true : false);

	CPlayer::Move(dwDirection, fDistance, bUpdateVelocity);
}

void CAngrybotPlayer::Update(float fTimeElapsed)
{
	CPlayer::Update(fTimeElapsed);

	if (m_pSkinnedAnimationController)
	{
		bool IsAnimationDone = false;

		float fLength = sqrtf(m_xmf3Velocity.x * m_xmf3Velocity.x + m_xmf3Velocity.z * m_xmf3Velocity.z);

		float fWALKWweight = fLength;
		float fIDLEWeight = 1.0f - fWALKWweight;

		m_pSkinnedAnimationController->SetTrackWeight(PS_IDLE, fIDLEWeight);
		m_pSkinnedAnimationController->SetTrackWeight(PS_WALK, fWALKWweight);

		switch (m_pSkinnedAnimationController->m_nCurrentTrack)
		{
		case PS_IDLE:
			if (!::IsZero(fLength)) {
				m_pSkinnedAnimationController->SetTrackEnable(PS_IDLE, false);
				m_pSkinnedAnimationController->SetTrackEnable(PS_WALK, true);
			}
			break;
		case PS_WALK:
			if (::IsZero(fLength))
			{
				float fCurrent = m_pSkinnedAnimationController->m_fTime * 0.5f;
				float fDuration = m_pSkinnedAnimationController->m_pAnimationTracks[PS_WALK].m_fLength;

				if (fCurrent >= fDuration) {
					m_pSkinnedAnimationController->SetTrackEnable(PS_IDLE, true);
					m_pSkinnedAnimationController->SetTrackEnable(PS_WALK, false);
				}
			}
			break;
		}

		if (CPlayerManager::Get_Instance()->Get_Animation() != m_pSkinnedAnimationController->m_nCurrentTrack)
		{
			CPlayerManager::Get_Instance()->Set_Animation(m_pSkinnedAnimationController->m_nCurrentTrack);
			CNetwork::Get_Instance()->SetSendPacket(CS_ANIMATION);
		}
		//HY
	}
}
#endif