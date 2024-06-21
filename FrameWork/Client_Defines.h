#pragma once

static const unsigned int g_iWinSizeX = 1280;
static const unsigned int g_iWinSizeY = 720;
static const float	g_fWinSizeX = 1280.f;
static const float g_fWinSizeY = 720.f;

enum LEVEL {LEVEL_STATIC,LEVEL_LOADING,LEVEL_LOGO,LEVEL_GAMEPLAY,LEVEL_END};
enum PLAYERSTATE {PS_IDLE, PS_WALK, PS_ATTACK, PS_DAMAGED, PS_DIE, PS_END};
enum MONSTERSTATE {MS_IDLE, MS_MOVE, MS_ATTACK, MS_DAMAGE, MS_DIE};
enum FILE_TYPE {PNG, DDS};
enum class SHADER_TYPE {  UI, Terrain, SkyBox, WireFrame, SkinnedAnimationWireFrame, Texture};


template<typename T>
unsigned long Safe_AddRef(T& Temp)
{
	unsigned long dwRefCnt = 0;

	if (nullptr != Temp)
	{
		dwRefCnt = Temp->AddRef();
	}

	return dwRefCnt;
}

template<typename T>
unsigned long Safe_Release(T& Temp)
{
	unsigned long dwRefCnt = 0;

	if (Temp)
	{
		dwRefCnt = Temp->Release();
		Temp = nullptr;
	}

	return dwRefCnt;
}