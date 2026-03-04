/// \file Turret.cpp
/// \brief Code for the turret object class CTurret.

#include "Turret.h"
#include "ComponentIncludes.h"
#include "ObjectManager.h"
#include "TileManager.h"
#include "Player.h"
#include "Helpers.h"
#include "Particle.h"
#include "ParticleEngine.h"

/// Create and initialize a turret object given its position.
/// \param p Position of turret.

CTurret::CTurret(const Vector2& p): CObject(eSprite::Turret, p){
  m_bStatic = false; //turrets are static
  t = m_pTimer->GetTime();
  tAir = m_pTimer->GetTime();
} //constructor

/// Rotate the turret and fire the gun at at the closest available target if
/// there is one, and rotate the turret at a constant speed otherwise.

void CTurret::move(){
    Vector2 view = GetViewVector(); //view vector
    Vector2 norm = VectorNormalCC(view); //normal to view vector
    if (m_pTimer->GetTime() - tAir > 0.1f)
    {
        inAir = true;
        tAir = m_pTimer->GetTime();
    }
    if(flip)
    {
        m_vVelocity.x = moveRight;
        m_fRoll = 0.0f;
    }
    else
    {
        m_vVelocity.x = moveLeft;
        m_fRoll = M_PI;
    }
    if (inAir)
    {
        m_vVelocity.y = gravity;
    }
    else
    {
        m_vVelocity.y = 0.0;
    }
    
    if(m_pPlayer){ //safety
    const float r = ((CTurret*)m_pPlayer)->m_fRadius; //player radius
    Vector2 direction = m_vPos - m_pPlayer->m_vPos;
    direction.Normalize();
    float dot = direction.Dot(view);


    if (m_pTileManager->Visible(m_vPos, m_pPlayer->m_vPos, r) && m_pGunFireEvent->Triggered() && dot < 0.0f)
    {//player visible
      //RotateTowards(m_pPlayer->m_vPos);

        m_pObjectManager->FireGun(this, eSprite::Bullet2);
    }
        //else m_fRotSpeed = 0.4f; //no target visible, so scan
  } //if
    if (m_pTimer->GetTime() - t > 1.0f)
    {
        if (flip)
        {
            m_vVelocity.x = moveLeft;
            m_fRoll = M_PI;
        }
        else {
            m_vVelocity.x = moveRight;
            m_fRoll = 0.0f;
        }
        flip = !flip;
        t = m_pTimer->GetTime();
    }
    m_vPos += Vector2(1,0) * m_vVelocity.x;
    m_vPos += Vector2(0,-1) * m_vVelocity.y;
  //m_fRoll += 0.2f*m_fRotSpeed*XM_2PI*m_pTimer->GetFrameTime(); //rotate
  //NormalizeAngle(m_fRoll); //normalize to [-pi, pi] for accuracy
} //move

/// Rotate the turret towards a point and file the gun if it is facing
/// sufficiently close to it.
/// \param pos Target point.

void CTurret::RotateTowards(const Vector2& pos){
  const Vector2 v = pos - m_vPos; //vector from target to turret
  const float theta = atan2f(v.y, v.x); //orientation of that vector
  float diff = m_fRoll - theta; //difference with turret orientation
  NormalizeAngle(diff); //normalize to [-pi, pi]

  //set rotation speed from diff

  const float fAngleDelta = 0.05f; //allowable angle discrepancy
  const float fTrackingSpeed = 2.0f; //rotation speed when tracking

  if(diff > fAngleDelta)m_fRotSpeed = -fTrackingSpeed; //clockwise
  else if(diff < -fAngleDelta)m_fRotSpeed = fTrackingSpeed; //counterclockwise
  else m_fRotSpeed = 0; //stop rotating

  //fire gun if pointing approximately towards target

  if(fabsf(diff) < fAngleDelta && m_pGunFireEvent->Triggered())
    m_pObjectManager->FireGun(this, eSprite::Bullet2);
} //RotateTowards

/// Response to collision. 
/// \param norm Collision normal.
/// \param d Overlap distance.
/// \param pObj Pointer to object being collided with (defaults to `nullptr`,
/// which means collision with a wall).

void CTurret::CollisionResponse(const Vector2& norm, float d, CObject* pObj){
  if(m_bDead)return; //already dead, bail out 

  if(pObj && pObj->isBullet()){ //collision with bullet
    if(--m_nHealth == 0){ //health decrements to zero means death 
      m_pAudio->play(eSound::Boom); //explosion
      m_bDead = true; //flag for deletion from object list
      DeathFX(); //particle effects
    } //if

    else{ //not a death blow
      m_pAudio->play(eSound::Clang); //impact sound
      const float f = 0.5f + 0.5f*(float)m_nHealth/m_nMaxHealth; //health fraction
      m_f4Tint = XMFLOAT4(1.0f, f, f, 0); //redden the sprite to indicate damage
    } //else
  } //if

  if (pObj && pObj->isSpike())
  {
      m_pAudio->play(eSound::Boom); //explosion
      m_bDead = true; //flag for deletion from object list
      DeathFX(); //particle effects
  }

  if (pObj == nullptr)
  {
      if (!inAir)
      {
          flip = !flip;
          t = m_pTimer->GetTime();
          inAir = false;
      }
  }
  else
  {
      inAir = true;
  }
  tAir = m_pTimer->GetTime();

  CObject::CollisionResponse(norm, d, pObj);
} //CollisionResponse

/// Perform a particle effect to mark the death of the turret.

void CTurret::DeathFX(){
  LParticleDesc2D d; //particle descriptor
  d.m_vPos = m_vPos; //center particle at turret center

  d.m_nSpriteIndex = (UINT)eSprite::Smoke;
  d.m_fLifeSpan = 2.0f;
  d.m_fMaxScale = 4.0f;
  d.m_fScaleInFrac = 0.5f;
  d.m_fFadeOutFrac = 0.8f;
  d.m_fScaleOutFrac = 0;
  m_pParticleEngine->create(d);

  d.m_nSpriteIndex = (UINT)eSprite::Spark;
  d.m_fLifeSpan = 0.5f;
  d.m_fMaxScale = 1.5f;
  d.m_fScaleInFrac = 0.4f;
  d.m_fScaleOutFrac = 0.3f;
  d.m_fFadeOutFrac = 0.5f;
  d.m_f4Tint = XMFLOAT4(Colors::Orange);
  m_pParticleEngine->create(d);
} //DeathFX