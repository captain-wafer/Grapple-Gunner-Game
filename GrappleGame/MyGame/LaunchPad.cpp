/// \file LaunchPad.cpp
/// \brief Code for the object class CLaunchPad.

#include "LaunchPad.h"
#include "ComponentIncludes.h"
#include "Helpers.h"
#include "Particle.h"
#include "ParticleEngine.h"

CLaunchPad::CLaunchPad(const Vector2& p) : CObject(eSprite::LaunchPad, p)
{
	m_bIsTarget = false;
	m_bStatic = true;
	m_bIsLaunchPad = true;
}