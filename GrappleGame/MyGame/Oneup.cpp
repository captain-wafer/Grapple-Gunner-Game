/// \file Spike.cpp
/// \brief Code for the object class CSpike.

#include "Oneup.h"
#include "ComponentIncludes.h"
#include "Helpers.h"
#include "Particle.h"
#include "ParticleEngine.h"

COneUp::COneUp(const Vector2& p) : CObject(eSprite::OneUp, p)
{
    m_bIsTarget = false;
    m_bStatic = true;
    m_bIsOneUp = true;
}

void COneUp::CollisionResponse(const Vector2& norm, float d, CObject* pObj) {
    if (m_bDead)return; //already dead, bail out 

    if (pObj && pObj->isPlayer())
    {
        m_bDead = true; //flag for deletion from object list
    }

    CObject::CollisionResponse(norm, d, pObj);
} //CollisionResponse