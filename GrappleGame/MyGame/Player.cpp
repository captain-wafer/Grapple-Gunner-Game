/// \file Player.cpp
/// \brief Code for the player object class CPlayer.

#include "Player.h"
#include "Game.h"
#include "ComponentIncludes.h"
#include "Helpers.h"
#include "Particle.h"
#include "ParticleEngine.h"

/// Create and initialize an player object given its initial position.
/// \param p Initial position of player.

CPlayer::CPlayer(eSprite directionSprite, const Vector2& p): CObject(eSprite::Standright, p){ 
  m_bIsTarget = true;
  m_bStatic = false;
  m_bIsPlayer = true;
  t = m_pTimer->GetTime();
  // tInvincible = m_pTimer->GetTime();
} //constructor

/// Move and rotate in response to device input. The amount of motion and
/// rotation speed is proportional to the frame time.

void CPlayer::move(){
  const float t = m_pTimer->GetFrameTime(); //time
  const Vector2 view = GetViewVector(); //view vector
  
  const Vector2 norm = VectorNormalCC(view); //normal to view vector
  const float delta = 40.0f*t; //change in position for strafing

  if (m_vVelocity.x != 0.00f) m_vPos += delta * view * m_vVelocity.x; // Player X Movement
  m_vPos -= delta * norm * m_vVelocity.y; // Player Y Movement

  // Idle Animation
  if (m_vVelocity.x == 0.00f && m_vVelocity.y == 0.00f && canJump &&
      (m_nSpriteIndex != (UINT)eSprite::Standright || m_nSpriteIndex != (UINT)eSprite::Standleft)) {
      facingLeft ? m_nSpriteIndex = (UINT)eSprite::Standleft : m_nSpriteIndex = (UINT)eSprite::Standright;
  }

  // Walking Animation
  if (m_vVelocity.x > 0.00f && canJump) m_nSpriteIndex = (UINT)eSprite::Walkright;
  else if (m_vVelocity.x < 0.00f && canJump) m_nSpriteIndex = (UINT)eSprite::Walkleft;

  // Left and Right Movement
  if (m_bStrafeRight) {
      if (m_vVelocity.x <= 0) m_vVelocity.x += PLAYER_TURN_AROUND;
      else if (m_vVelocity.x < PLAYER_TOP_SPEED) m_vVelocity.x += PLAYER_ACCELERATION;
      else if (m_vVelocity.x > PLAYER_TOP_SPEED) m_vVelocity.x == PLAYER_TOP_SPEED;
  }
  else if (m_bStrafeLeft) {
      if (m_vVelocity.x >= 0) m_vVelocity.x -= PLAYER_TURN_AROUND;
      else if (m_vVelocity.x > -PLAYER_TOP_SPEED) m_vVelocity.x -= PLAYER_ACCELERATION;
      else if (m_vVelocity.x < -PLAYER_TOP_SPEED) m_vVelocity.x == -PLAYER_TOP_SPEED;
  }

  // Player Jumping
  if (canJump && m_bJump) {
      m_vVelocity.y = JUMP_HEIGHT;
      m_nSpriteIndex = (UINT)eSprite::Jump;
  }

  // Constant Gravity
  m_vVelocity.y += PLAYER_GRAVITY * delta;

  // Max Player Y Velocity
  /*if (m_vVelocity.y > -JUMP_HEIGHT) m_vVelocity.y = -JUMP_HEIGHT;
  else if (m_vVelocity.y < JUMP_HEIGHT) m_vVelocity.y = JUMP_HEIGHT;*/
  // In Case we need it again

  // Friction
  if (!m_bStrafeRight && !m_bStrafeLeft) {
      if ((m_vVelocity.x < 0.00f) && ((m_vVelocity.x + PLAYER_FRICTION) > 0.00f)) m_vVelocity.x = 0.00f;
      if ((m_vVelocity.x > 0.00f) && ((m_vVelocity.x - PLAYER_FRICTION) < 0.00f)) m_vVelocity.x = 0.00f;

      if (m_vVelocity.x < 0.00f) m_vVelocity.x += PLAYER_FRICTION;
      else if (m_vVelocity.x > 0.00f) m_vVelocity.x -= PLAYER_FRICTION;
  }

  m_bStrafeLeft = m_bStrafeRight = m_bStrafeBack = m_bJump = canJump = false; //reset strafe flags
} //move

void CPlayer::playerLogic() {
    // Controls state of invincibility
    if (isInvincible && (m_pTimer->GetTime() - tInvincible > POWERUP_TIMER)) {
        isInvincible = false;
        m_fAlpha = 1.0f;
        m_pAudio->play(eSound::PowerDown);
    }

    // Flashes the player if about to lose invincibility
    if (isInvincible && (m_pTimer->GetTime() - tInvincible > (POWERUP_TIMER - 3.0f))) {
        flashPlayer();
    }
} //playerLogic

/// Response to collision. If the object being collided with is a bullet, then
/// play a sound, otherwise call `CObject::CollisionResponse` for the default
/// collision response.
/// \param norm Collision normal.
/// \param d Overlap distance.
/// \param pObj Pointer to object being collided with (defaults to `nullptr`,
/// which means collision with a wall).

void CPlayer::CollisionResponse(const Vector2& norm, float d, CObject* pObj) {
    if (m_bDead)return; //already dead, bail out 

    if (pObj && pObj->isGrappler())
    {
        return;
    }

    if (pObj && pObj->isSpike())
    {
        m_nHealth = 0;
        m_pAudio->play(eSound::Boom);
        m_bDead = true;
        DeathFX();
        m_pPlayer = nullptr;
    }

    if (pObj && pObj->isDoor())
    {
        if (!pObj->isDoorLocked()) {
            m_bIsWinner = true;
        }
    }

    if (pObj && pObj->isStar()) {
        m_pAudio->play(eSound::Star);
        isInvincible = true;
        tInvincible = m_pTimer->GetTime();
    }

    if (pObj && pObj->isLaunchPad()) {
        m_pAudio->play(eSound::Bounce);
        m_vVelocity.y = LAUNCHPAD_VELOCITY;
    }

    if (pObj && pObj->isHealthPack())
    {
        m_pAudio->play(eSound::Star);
        if (m_nHealth < m_nMaxHealth)
        {
            if ((m_nMaxHealth - m_nHealth) < 4)
                m_nHealth = m_nMaxHealth;
            else
                m_nHealth += 4;

            m_bHealthPercent = (float)m_nHealth / m_nMaxHealth;
            const float f = 0.5f + 0.5f * (float)m_nHealth / m_nMaxHealth; //health fraction
            m_f4Tint = XMFLOAT4(1.0f, f, f, 0); //redden the health indicator
        }
    }
    if (pObj && pObj->isOneUp())
    {
        m_pAudio->play(eSound::Star);
        hasOneUp = true;
    }
    if (pObj && pObj->isShotgun())
    {
        m_pAudio->play(eSound::Star);
        shotgun = true;
    }
    if (pObj && pObj->isBullet() && !isInvincible) { //collision with bullet
        //if(m_bGodMode) //god mode, does no damage
         // m_pAudio->play(eSound::Grunt); //impact sound

        if (--m_nHealth == 0) { //health decrements to zero means death 
            m_pAudio->play(eSound::Boom); //explosion
            m_bHealthPercent = 0.0f;
            m_bDead = true; //flag for deletion from object list
            DeathFX(); //particle effects
            m_pPlayer = nullptr; //clear common player pointer
        } //if

        else { //not a death blow
            m_pAudio->play(eSound::Grunt); //impact sound
            m_bHealthPercent = (float)m_nHealth / m_nMaxHealth;
            const float f = 0.5f + 0.5f * (float)m_nHealth / m_nMaxHealth; //health fraction
            m_f4Tint = XMFLOAT4(1.0f, f, f, 0); //redden the health indicator
        } //else

    }

    if (pObj && pObj->isSwooper() && !isInvincible) { //collision with swooper
        //if(m_bGodMode) //god mode, does no damage
         // m_pAudio->play(eSound::Grunt); //impact sound

        if (m_pTimer->GetTime() - t > 1.5f)
        {
            m_nHealth -= 2;
            t = m_pTimer->GetTime();
        }

        if (m_nHealth <= 0) { //health decrements to zero means death 
            m_pAudio->play(eSound::Boom); //explosion
            m_bDead = true; //flag for deletion from object list
            DeathFX(); //particle effects
            m_pPlayer = nullptr; //clear common player pointer
        } //if

        else { //not a death blow
            m_pAudio->play(eSound::Grunt); //impact sound
            m_bHealthPercent = (float)m_nHealth / m_nMaxHealth;
            const float f = 0.5f + 0.5f * (float)m_nHealth / m_nMaxHealth; //health fraction
            m_f4Tint = XMFLOAT4(1.0f, f, f, 0); //redden the health indicator
        } //else
    } //if

    if (pObj == nullptr) {
        if (abs(norm.Dot(Vector2(1, 0))) >= 0.95f)
        {
            m_vVelocity.x = 0.00f;
        }

        if (norm.Dot(Vector2(0, 1)) >= 0.95f)
        {
            m_vVelocity.y = 0.00f;
            canJump = true;
        }

        if (norm.Dot(Vector2(0, -1)) >= 0.95f) {
            m_vVelocity.y = 0.00f;
            canJump = false;
        }
    } // colliding with floor

        CObject::CollisionResponse(norm, d, pObj); //default collision response
} //CollisionResponse


/// Perform a particle effect to mark the death of the player.

void CPlayer::DeathFX(){
  LParticleDesc2D d; //particle descriptor
  d.m_vPos = m_vPos; //center particle at player center

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
  d.m_f4Tint = XMFLOAT4(Colors::OrangeRed);
  m_pParticleEngine->create(d);
} //DeathFX

/// Set the strafe left flag. This function will be called in response to
/// device inputs.

void CPlayer::StrafeLeft(){
  m_bStrafeLeft = true;
  facingLeft = true;
} //StrafeLeft

/// Set the strafe right flag. This function will be called in response to
/// device inputs.

void CPlayer::StrafeRight(){
  m_bStrafeRight = true;
  facingLeft = false;
} //StrafeRight

/// Set the strafe back flag. This function will be called in response to
/// device inputs.

void CPlayer::StrafeBack(){
  m_bStrafeBack = true;
} //StrafeBack

void CPlayer::Jump() {
    m_bJump = true;
} // Jump

/// Set the object's speed, assuming that the object moves in the direction of
/// its view vector. This function will be called in response to device inputs.
/// \param speed Speed.

void CPlayer::SetSpeed(const float speed){
  m_fSpeed = speed;
} //SetSpeed

/// Set the object's rotational speed in revolutions per second. This function
/// will be called in response to device inputs.
/// \param speed Rotational speed in RPS.

void CPlayer::SetRotSpeed(const float speed){
  m_fRotSpeed = speed;
} //SetRotSpeed

/// Reader function for position.
/// \return Position.

const Vector2& CPlayer::GetPos() const{
  return m_vPos;
} //GetPos

void CPlayer::flashPlayer() {
    float fractionalPart = (m_pTimer->GetTime() - tInvincible) - static_cast<int>(m_pTimer->GetTime() - tInvincible);
    int interval = static_cast<int>(fractionalPart / 0.2f);
    if (interval % 2 == 0) {
        m_fAlpha = 1.0f;
    }
    else {
        m_fAlpha = 0.0f;
    }
}

bool CPlayer::GetHasOneUp() {
    return hasOneUp;
}

void CPlayer::ClearHasOneUp() {
    hasOneUp = false;
}

bool CPlayer::hasShotgun() {
    return shotgun;
}

void CPlayer::resetShotgun()
{
    shotgun = false;
}

void CPlayer::hitByCreeper() {
    if(!isInvincible) {
        if (m_nHealth <= 3) {
            m_pAudio->play(eSound::Boom); //explosion
            m_bDead = true; //flag for deletion from object list
            DeathFX(); //particle effects
            m_pPlayer = nullptr; //clear common player pointer
        }

        else {
            m_nHealth -= 4;
            m_pAudio->play(eSound::Grunt); //impact sound
            m_bHealthPercent = (float)m_nHealth / m_nMaxHealth;
            const float f = 0.5f + 0.5f * (float)m_nHealth / m_nMaxHealth; //health fraction
            m_f4Tint = XMFLOAT4(1.0f, f, f, 0); //redden the health indicator
        }
    }
}