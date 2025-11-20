#ifndef __L4RC_GAME_CREEPER_H__
#define __L4RC_GAME_CREEPER_H__

#include "Object.h"

class CCreeper : public CObject {
protected:
    const UINT m_nMaxHealth = 3; ///< Maximum health.
    UINT m_nHealth = m_nMaxHealth; ///< Current health.

    const float TOP_SPEED = 8.0f;
    const float explosionDistance = 80.0f;

    void RotateTowards(const Vector2&); ///< Swivel towards position.
    virtual void CollisionResponse(const Vector2&, float, CObject* = nullptr); ///< Collision response.
    virtual void DeathFX(); ///< Death special effects.

    void Explode(); ///< Creeper explodes.
public:
    CCreeper(const Vector2& p); ///< Constructor.
    virtual void move(); ///< Move creeper.
};

#endif
