/// \file Player.h
/// \brief Interface for the player object class CPlayer.

#ifndef __L4RC_GAME_PLAYER_H__
#define __L4RC_GAME_PLAYER_H__

#include "Object.h"

/// \brief The player object. 
///
/// The abstract representation of the player object. The player differs from
/// the other objects in the game in that it moves in respond to device inputs.

class CPlayer: public CObject{
  protected:  
    const UINT m_nMaxHealth = 12; ///< Maximum health.
    UINT m_nHealth = m_nMaxHealth; ///< Current health.

    bool m_bStrafeLeft = false; ///< Strafe left.
    bool m_bStrafeRight = false; ///< Strafe right.
    bool m_bStrafeBack = false; ///< Strafe back.
    bool m_bJump = false; ///< Jump
    bool canJump = false;
    bool facingLeft = false;
    bool hasOneUp = false;
    bool shotgun = false;

    bool isInvincible = false;
    float tInvincible = 0.0f;

    float t;

    const float JUMP_HEIGHT = -20.00f; ///< Player Jump Height Constant
    const float PLAYER_GRAVITY = 1.50f; ///< Player Gravity Constant
    const float PLAYER_TOP_SPEED = 15.00f; ///< Player Top Speed Constant
    const float PLAYER_ACCELERATION = 0.3f; ///< Player Acceleration Constant
    const float PLAYER_TURN_AROUND = 0.8f; ///< Player Turn Around Acceleration Constant
    const float PLAYER_FRICTION = 0.8f; ///< Player Friction Constant
    const float LAUNCHPAD_VELOCITY = -35.0f; ///< Launch Pad Velocity

    const float POWERUP_TIMER = 10.00f; ///< Power Up Timer for a player
    
    virtual void CollisionResponse(const Vector2&, float, CObject* = nullptr); ///< Collision response.
    virtual void DeathFX(); ///< Death special effects.

    void flashPlayer(); ///< Flash Player Sprite

  public:
    CPlayer(eSprite directionSprite, const Vector2& p); ///< Constructor.
    bool m_bIsWinner = false;
    virtual void move(); ///< Move player object.

    void playerLogic(); ///< Player logic that controls different parts of the player.

    void SetSpeed(const float speed); ///< Set speed.
    void SetRotSpeed(const float speed); ///< Set rotational velocity.
    
    void StrafeLeft(); ///< Strafe left.
    void StrafeRight(); ///< Strafe right.
    void StrafeBack(); ///< Strafe back.
    void Jump(); ///< Jump.

    bool GetHasOneUp();
    void ClearHasOneUp();
    bool hasShotgun();
    void resetShotgun();
    void hitByCreeper();
    
    const Vector2& GetPos() const; ///< Get position.
}; //CPlayer

#endif //__L4RC_GAME_PLAYER_H__
