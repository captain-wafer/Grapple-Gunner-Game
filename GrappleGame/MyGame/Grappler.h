/// \file Grappler.h
/// \brief Interface for the player object class CPlayer.

#ifndef __L4RC_GAME_GRAPPLER_H__
#define __L4RC_GAME_GRAPPLER_H__

#include "Object.h"

/// \brief The player object. 
///
/// The abstract representation of the player object. The player differs from
/// the other objects in the game in that it moves in respond to device inputs.

class CGrappler : public CObject {
protected:

    virtual void CollisionResponse(const Vector2&, float, CObject* = nullptr); ///< Collision response.
    bool shotgun = false;

public:
    CGrappler(eSprite directionSprite, const Vector2& p); ///< Constructor.

    Vector2 cursorPos;
    float cooldown;
    

    void Shoot();   ///< Fire gun

    const Vector2& GetPos() const; ///< Get position.

    void AimGun(float direction);    ///< Point gun to mouse cursor
    void RotateTowards(const Vector2& pos);
    void setShotgun();
    void normalGun();
    void CGrappler::move();
}; //CGrappler

#endif //__L4RC_GAME_GRAPPLER_H__
