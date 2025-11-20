#include "Creeper.h"
#include "ComponentIncludes.h"
#include "ObjectManager.h"
#include "TileManager.h"
#include "Player.h"
#include "Helpers.h"
#include "Particle.h"
#include "ParticleEngine.h"

CCreeper::CCreeper(const Vector2& p) : CObject(eSprite::Creeper, p) {
    m_bStatic = false;
    m_bIsCreeper = true;
} //constructor

void CCreeper::move() {
    const float moveT = m_pTimer->GetFrameTime(); //time

    Vector2 view = GetViewVector(); //view vector
    Vector2 norm = VectorNormalCC(view); //normal to view vector

    const float delta = 40.0f * moveT;

    if (m_pPlayer) {
        const float r = ((CCreeper*)m_pPlayer)->m_fRadius; //player radius

        if (m_pTileManager->Visible(m_vPos, m_pPlayer->m_vPos, r)) {
            RotateTowards(m_pPlayer->m_vPos);

            float distance = m_vPos.Distance(m_pPlayer->m_vPos, m_vPos);
            if (abs(distance) <= explosionDistance) {
                Explode();
            }
        }//player visible
    }
    else
    {
        m_fRotSpeed = 0.0f;
    }


    m_vPos += m_fSpeed * delta * view;

    m_fRoll += 0.2f * m_fRotSpeed * XM_2PI * m_pTimer->GetFrameTime(); //rotate
    NormalizeAngle(m_fRoll); //normalize to [-pi, pi] for accuracy
} //move

void CCreeper::RotateTowards(const Vector2& pos) {
    const Vector2 v = pos - m_vPos; //vector from target to turret
    const float theta = atan2f(v.y, v.x); //orientation of that vector
    float diff = m_fRoll - theta; //difference with turret orientation
    NormalizeAngle(diff); //normalize to [-pi, pi]

    //set rotation speed from diff

    const float fAngleDelta = 0.05f; //allowable angle discrepancy
    const float fTrackingSpeed = 6.0f; //rotation speed when tracking

    if (diff > fAngleDelta) {
        m_fRotSpeed = -fTrackingSpeed; //clockwise
    }
    else if (diff < -fAngleDelta) {
        m_fRotSpeed = fTrackingSpeed; //counterclockwise
    }
    else {
        m_fRotSpeed = 0; //stop rotating
        m_fSpeed = TOP_SPEED;
    }

    

} //RotateTowards


/// Response to collision. 
/// \param norm Collision normal.
/// \param d Overlap distance.
/// \param pObj Pointer to object being collided with (defaults to `nullptr`,
/// which means collision with a wall).
void CCreeper::CollisionResponse(const Vector2& norm, float d, CObject* pObj) {
    if (m_bDead)return; //already dead, bail out 

    if (pObj && pObj->isBullet()) { //collision with bullet
        if (--m_nHealth == 0) { //health decrements to zero means death 
            m_pAudio->play(eSound::Boom); //explosion
            m_bHealthPercent = 0.0f;
            m_bDead = true; //flag for deletion from object list
            DeathFX(); //particle effects
        } //if

        else { //not a death blow
            m_pAudio->play(eSound::Clang); //impact sound
            m_bHealthPercent = (float)m_nHealth / m_nMaxHealth;
            const float f = 0.5f + 0.5f * (float)m_nHealth / m_nMaxHealth; //health fraction
            m_f4Tint = XMFLOAT4(1.0f, f, f, 0); //redden the sprite to indicate damage
        } //else
    } //if

    if (pObj && pObj->isSpike())
    {
        m_pAudio->play(eSound::Boom); //explosion
        m_bHealthPercent = 0.0f;
        m_bDead = true; //flag for deletion from object list
        DeathFX(); //particle effects
    }

    CObject::CollisionResponse(norm, d, pObj);
} //CollisionResponse

/// Perform a particle effect to mark the death of the turret.
void CCreeper::DeathFX() {
    LParticleDesc2D d; //particle descriptor
    d.m_vPos = m_vPos; //center particle at turret center

    d.m_nSpriteIndex = (UINT)eSprite::CreeperExplosion;
    d.m_fLifeSpan = 2.0f;
    d.m_fMaxScale = 8.0f;
    d.m_fScaleInFrac = 0.01f;
    d.m_fFadeOutFrac = 0.8f;
    d.m_fScaleOutFrac = 0;
    m_pParticleEngine->create(d);
} //DeathFX

void CCreeper::Explode() {
    m_pAudio->play(eSound::Boom); //explosion

    m_pPlayer->hitByCreeper();
    m_bHealthPercent = 0.0f;
    m_bDead = true; //flag for deletion from object list
    DeathFX();
}