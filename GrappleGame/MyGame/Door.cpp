/// \file Door.cpp
/// \brief Code for the object class CDoor.

#include "Door.h"
#include "ComponentIncludes.h"
#include "Helpers.h"
#include "Particle.h"
#include "ParticleEngine.h"

CDoor::CDoor(eSprite doorSprite, const Vector2& p) : CObject(eSprite::Door, p)
{
	m_bIsTarget = false;
	m_bStatic = true;
	m_bIsDoor = true;
	m_bIsDoorLocked = true;
}