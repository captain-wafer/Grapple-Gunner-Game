
#ifndef __L4RC_GAME_BAT_H__
#define __L4RC_GAME_BAT_H__

#include "Object.h"

class CBat : public CObject {
protected:
    const UINT m_nMaxHealth = 3; ///< Maximum health.
    UINT m_nHealth = m_nMaxHealth; ///< Current health.
    const float moveRight = 5.0;
    const float moveLeft = -5.0;
    const float moveUp = 1.0;
    const float moveDown = -1.0;
    const float gravity = 3.0;

    void RotateTowards(const Vector2&); ///< Swivel towards position.
    virtual void CollisionResponse(const Vector2&, float, CObject* = nullptr); ///< Collision response.
    virtual void DeathFX(); ///< Death special effects.
    bool flip = true;
    bool flipAim = true;
    float t;
    float tAir;
    bool start = true;
    bool inAir = true;
public:
    CBat(const Vector2& p); ///< Constructor.
    virtual void move(); ///< Move turret.
};

#endif
