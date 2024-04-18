#pragma once

static const unsigned int g_iWinSizeX = 1280;
static const unsigned int g_iWinSizeY = 720;
static const float	g_fWinSizeX = 1280.f;
static const float g_fWinSizeY = 720.f;

enum LEVEL {LEVEL_STATIC,LEVEL_LOADING,LEVEL_LOGO,LEVEL_GAMEPLAY,LEVEL_END};
enum PLAYERSTATE {PS_IDLE,PS_WALK,PS_RUN,PS_ATTACK,PS_HIT,PS_DIE,PS_END};
enum class SHADER_TYPE {Terrain, SkyBox, WireFrame, SkinnedAnimationWireFrame};