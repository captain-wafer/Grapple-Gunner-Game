/// \file Spike.cpp
/// \brief Code for the object class CSpike.

#include "Star.h"
#include "ComponentIncludes.h"
#include "Helpers.h"
#include "Particle.h"
#include "ParticleEngine.h"

CStar::CStar(const Vector2& p) : CObject(eSprite::Star, p)
{
	m_bIsTarget = false;
	m_bStatic = true;
	m_bIsStar = true;
}

void CStar::CollisionResponse(const Vector2& norm, float d, CObject* pObj) {
    if (m_bDead)return; //already dead, bail out 

    if (pObj && pObj->isPlayer())
    {
        m_bDead = true; //flag for deletion from object list
    }

    CObject::CollisionResponse(norm, d, pObj);
} //CollisionResponse