/// \file Object.cpp
/// \brief Code for the game object class CObject.

#include "Object.h"
#include "ComponentIncludes.h"
#include "GameDefines.h"
#include "Particle.h"
#include "ParticleEngine.h"
#include "Helpers.h"

/// Create and initialize an object given its sprite type and initial position.
/// \param t Type of sprite.
/// \param p Initial position of object.

CObject::CObject(eSprite t, const Vector2& p):
  LBaseObject(t, p)
{ 
  m_bIsTarget = false; //not a target

  const float w = m_pRenderer->GetWidth(t); //sprite width
  const float h = m_pRenderer->GetHeight(t); //sprite height
  m_fRadius = std::max(w, h)/2; //bounding circle radius

  m_pGunFireEvent = new LEventTimer(1.0f); //timer for firing gun
} //constructor

/// Destructor.

CObject::~CObject(){
  delete m_pGunFireEvent;
} //destructor

/// Move object an amount that depends on its velocity and the frame time.

void CObject::move(){
  if(!m_bDead && !m_bStatic)
    m_vPos += m_vVelocity*m_pTimer->GetFrameTime();
} //move

/// Ask the renderer to draw the sprite described in the sprite descriptor.
/// Note that `CObject` is derived from `LBaseObject` which is inherited from
/// `LSpriteDesc2D`. Therefore `LSpriteRenderer::Draw(const LSpriteDesc2D*)`
/// will accept `this` as a parameter, automatically down-casting it from
/// `CObject*` to `LSpriteDesc2D*`, effectively drawing the object from its
/// sprite descriptor.

void CObject::draw(){ 
  m_pRenderer->Draw(this);

  if (m_bHealthPercent < 1.0f && m_bHealthPercent > 0.0f) {
      drawHealthBar();
  }
} //draw

void CObject::drawHealthBar() {
    // Set green line
    float greenLength = 70.0f * m_bHealthPercent;

    Vector2 greenStartVector = m_vPos;
    greenStartVector.x -= 35.0f;
    greenStartVector.y += 60.0f;

    Vector2 greenEndVector = greenStartVector;

    greenEndVector.x += greenLength;

    // Set red line
    float redLength = 70.0f - greenLength;

    Vector2 redStartVector = m_vPos;
    redStartVector.x += 35.0f;
    redStartVector.y += 60.0f;

    Vector2 redEndVector = redStartVector;
    redEndVector.x -= redLength;

    m_pRenderer->DrawLine(eSprite::HealthBar_Green, greenStartVector, greenEndVector);
    m_pRenderer->DrawLine(eSprite::HealthBar_Red, redStartVector, redEndVector);
}

Vector2 CObject::convertGameToScreenSpace(Vector2& gameVector) {
    Vector2 screenSpaceVector;

    Vector3 cameraSpacePosition = m_pRenderer->GetCameraPos();
    screenSpaceVector.x = (gameVector.x - cameraSpacePosition.x) + (m_nWinHeight / 1.5f);
    screenSpaceVector.y = ((gameVector.y - cameraSpacePosition.y) * -1.0f) + (m_nWinHeight / 2.0f);

    return screenSpaceVector;
}

/// Response to collision. Move back the overlap distance along the collision
/// normal. 
/// \param norm Collision normal.
/// \param d Overlap distance.
/// \param pObj Pointer to object being collided with (defaults to `nullptr`,
/// which means collision with a wall).

void CObject::CollisionResponse(const Vector2& norm, float d, CObject* pObj){
  if(m_bDead)return; //dead, bail out

  if (m_bIsGrappler)return;

  const Vector2 vOverlap = d*norm; //overlap in direction of this
  const bool bStatic = !pObj || pObj->m_bStatic; //whether other object is static

  if(!m_bStatic && !bStatic) //both objects are dynamic
    m_vPos += vOverlap/2; //back off this object by half

  else if (!m_bStatic && bStatic) { //only this object is dynamic
      m_vPos += vOverlap; //back off this object
  }
} //CollisionResponse

/// Create a particle effect to mark the death of the object.
/// This function is a stub intended to be overridden by various object classes
/// derived from this class.

void CObject::DeathFX(){
 //stub
} //DeathFX

/// Compute the view vector from the object orientation.
/// \return The view vector.

const Vector2 CObject::GetViewVector() const{
  return AngleToVector(m_fRoll);
} //ViewVector

/// Reader function for bullet flag.
/// \return true if a bullet.

const bool CObject::isBullet() const{
  return m_bIsBullet;
} //isBullet

/// Reader function for spike flag.
/// \return true if a spike.

const bool CObject::isSpike() const {
    return m_bIsSpike;
} //isSpike

/// Reader function for door flag.
/// \return true if a door.

const bool CObject::isDoor() const {
    return m_bIsDoor;
} //isBullet

const bool CObject::isDoorLocked() const {
    return m_bIsDoorLocked;
}

const bool CObject::isStar() const {
    return m_bIsStar;
} // isStar

const bool CObject::isHealthPack() const {
    return m_bIsHealthpack;
}

const bool CObject::isPlayer() const {
    return m_bIsPlayer;
} // isPlayer

const bool CObject::isLaunchPad() const {
    return m_bIsLaunchPad;
} //isLaunchPad

const bool CObject::isGrappler() const {
    return m_bIsGrappler;
} //isGrappler

const bool CObject::isOneUp() const {
    return m_bIsOneUp;
} //isGrappler

const bool CObject::isShotgun() const {
    return m_bIsShotgun;
} //isGrappler

const bool CObject::isCreeper() const {
    return m_bIsCreeper;
} // isCreeper

const bool CObject::isSwooper() const {
    return m_bIsSwooper;
}

void CObject::setDoorOpen() {
    m_bIsDoorLocked = false;
    m_nSpriteIndex = (UINT)eSprite::DoorOpen;
}
