#include "Healthpack.h"
#include "ComponentIncludes.h"
#include "Helpers.h"
#include "Particle.h"
#include "ParticleEngine.h"

CHealthPack::CHealthPack(const Vector2& p) : CObject(eSprite::HealthPack, p)
{
	m_bIsTarget = false;
	m_bStatic = true;
	m_bIsHealthpack = true;
}

void CHealthPack::CollisionResponse(const Vector2& norm, float d, CObject* pObj)
{
	if (m_bDead)return; //already dead, bail out 

	if (pObj && pObj->isPlayer())
	{
		m_bDead = true; //flag for deletion from object list
	}

	CObject::CollisionResponse(norm, d, pObj);
}