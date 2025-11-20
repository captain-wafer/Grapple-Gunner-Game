/// \file Object.h
/// \brief Interface for the game object class CObject.

#ifndef __L4RC_GAME_OBJECT_H__
#define __L4RC_GAME_OBJECT_H__

#include "GameDefines.h"
#include "SpriteRenderer.h"
#include "Common.h"
#include "Component.h"
#include "SpriteDesc.h"
#include "BaseObject.h"
#include "EventTimer.h"

/// \brief The game object. 
///
/// The abstract representation of an object. `CObjectManager` is a friend of
/// this class so that it can access any private members as needed to manage
/// the objects without the need for reader and set functions for each private
/// or protected member variable. This class must contain public member
/// functions `move()` and `draw()` to move and draw the object, respectively.

class CObject:
  public CCommon,
  public LSettings,
  public LBaseObject
{
  friend class CObjectManager; ///< Object manager needs access so it can manage.

  protected:
    float m_fRadius = 0; ///< Bounding circle radius.

    float m_fSpeed = 0; ///< Speed.
    float m_fRotSpeed = 0; ///< Rotational speed.
    Vector2 m_vVelocity; ///< Velocity.
    bool m_bStatic = true; ///< Is static (does not move).
    bool m_bIsTarget = true; ///< Is a target.
    bool m_bIsBullet = false; ///< Is a bullet.
    bool m_bIsSpike = false; ///< Is a spike.
    bool m_bIsDoor = false; ///< Is a door.
    bool m_bIsDoorLocked = false; ///< Is Door Locked.
    bool m_bIsStar = false; ///< Is a star.
    bool m_bIsHealthpack = false;
    bool m_bIsPlayer = false; ///< Is a player.
    bool m_bIsLaunchPad = false; ///< Is a Launch pad.
    bool m_bIsGrappler = false;
    bool m_bIsOneUp = false;
    bool m_bIsShotgun = false;
    bool m_bIsCreeper = false;
    bool m_bIsSwooper = false;

    std::string m_bHealthString = "";
    float m_bHealthPercent = 1.0f;

    LEventTimer* m_pGunFireEvent = nullptr; ///< Gun fire event.
    
    virtual void CollisionResponse(const Vector2&, float,
      CObject* = nullptr); ///< Collision response.
    virtual void DeathFX(); ///< Death special effects.

    const Vector2 GetViewVector() const; ///< Compute view vector.

  public:
    CObject(eSprite, const Vector2&); ///< Constructor.
    virtual ~CObject(); ///< Destructor.

    void move(); ///< Move object.
    void draw(); ///< Draw object.

    void drawHealthBar(); ///< Draw the healthbar if applicable.

    const bool isBullet() const; ///< Is a bullet.
    const bool isSpike() const; ///< Is a spike
    const bool isDoor() const; ///< Is a door
    const bool isDoorLocked() const; ///< Is the door locked
    const bool isStar() const; ///< Is a star
    const bool isHealthPack() const; ///Is a healthpack
    const bool isPlayer() const; ///< Is a player.
    const bool isLaunchPad() const; ///< Is a launch pad.
    const bool isGrappler() const; ///< Is a grappler
    const bool isOneUp() const; ///< Is a oneup
    const bool isShotgun() const; ///< Is a shotgun
    const bool isCreeper() const; ///< Is a creeper
    const bool isSwooper() const; ///< Is a Swooper

    Vector2 convertGameToScreenSpace(Vector2& gameVector); ///< Converts Game Vector2 to Screen Vector2.

    void setDoorOpen();
}; //CObject

#endif //__L4RC_GAME_OBJECT_H__