/// \file Turret.h
/// \brief Interface for the turret object class CTurret.

#ifndef __L4RC_GAME_TURRET_H__
#define __L4RC_GAME_TURRET_H__

#include "Object.h"


/// \brief The turret object. 
///
/// CTurret is the abstract representation of a turret object.

class CTurret: public CObject{
  protected:
    const UINT m_nMaxHealth = 3; ///< Maximum health.
    UINT m_nHealth = m_nMaxHealth; ///< Current health.
    const float moveRight = 5.0;
    const float moveLeft = -5.0;
    const float gravity = 3.0;
    
    void RotateTowards(const Vector2&); ///< Swivel towards position.
    virtual void CollisionResponse(const Vector2&, float, CObject* = nullptr); ///< Collision response.
    virtual void DeathFX(); ///< Death special effects.
    bool flip = true;
    float t;
    float tAir;
    bool start = true;
    bool inAir = true;
  public:
    CTurret(const Vector2& p); ///< Constructor.
    virtual void move(); ///< Move turret.
}; //CBullet

#endif //__L4RC_GAME_TURRET_H__
