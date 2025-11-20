/// \file Grappler.cpp
/// \brief Code for the player object class CPlayer.

#include "Grappler.h"
#include "ComponentIncludes.h"
#include "Helpers.h"
#include "Particle.h"
#include "ParticleEngine.h"
#include "Player.h"
#include "ObjectManager.h"
#include <iostream>

/// Create and initialize a grappler object given its initial position.
/// \param p Initial position of player.

CGrappler::CGrappler(eSprite directionSprite, const Vector2& p) : CObject(eSprite::Grappler, p) {
    m_bIsGrappler = true;
    m_bIsTarget = false;
    m_bStatic = false;
    cooldown = 0.5;
    m_pGunFireEvent = new LEventTimer(0.2f);
} //constructor

/// Move and rotate in response to device input. The amount of motion and
/// rotation speed is proportional to the frame time.

void CGrappler::move() {
    if (m_pPlayer) {
        m_bDead = false;
        m_pGrappler->m_vPos = m_pPlayer->GetPos();
    }
    else {
        m_bDead = true;
    }
} //move

/// Rotate the turret towards a point and file the gun if it is facing
/// sufficiently close to it.
/// \param pos Target point.

void CGrappler::RotateTowards(const Vector2& cursorPos) {
    const Vector2 v = cursorPos - GetPos(); //vector from cursor to grappler
    const float theta = atan2f(-v.y, v.x); //orientation of that vector
    float diff = m_fRoll - theta; //difference with grappler orientation
    NormalizeAngle(diff); //normalize to [-pi, pi]

    //set rotation speed from diff

    const float fAngleDelta = 0.05f; //allowable angle discrepancy
    const float fTrackingSpeed = 10.0f; //rotation speed when tracking

    if (diff > fAngleDelta)m_fRotSpeed = -fTrackingSpeed; //clockwise
    else if (diff < -fAngleDelta)m_fRotSpeed = fTrackingSpeed; //counterclockwise
    else m_fRotSpeed = 0; //stop rotating
} //RotateTowards

void CGrappler::AimGun(float direction) {
    if (m_bDead)return; //already dead, bail out 
        m_fRoll = direction;
}



void CGrappler::CollisionResponse(const Vector2& norm, float d, CObject* pObj) {
    if (m_bDead)return; //already dead, bail out 

    CObject::CollisionResponse(norm, d, pObj); //default collision response
} //CollisionResponse

void CGrappler::Shoot()
{
    if (m_bDead)return; //already dead, bail out 
    if(m_pGunFireEvent->Triggered())
        if(shotgun)
            m_pObjectManager->FireShotgun(this, eSprite::Bullet2);
        else
            m_pObjectManager->FireGun(this, eSprite::Bullet2);
}

/// Reader function for position.
/// \return Position.

const Vector2& CGrappler::GetPos() const {
    return m_vPos;
} //GetPos

void CGrappler::setShotgun() {
    shotgun = true;
    m_nSpriteIndex = (UINT)eSprite::Shotgun;
}

void CGrappler::normalGun() {
    shotgun = false;
    m_nSpriteIndex = (UINT)eSprite::Grappler;
}