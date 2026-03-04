/// \file Spike.cpp
/// \brief Code for the object class CSpike.

#include "Spike.h"
#include "ComponentIncludes.h"
#include "Helpers.h"
#include "Particle.h"
#include "ParticleEngine.h"

CSpike::CSpike(const Vector2& p) : CObject(eSprite::Spike, p)
{
	m_bIsTarget = false;
	m_bStatic = true;
	m_bIsSpike = true;
}