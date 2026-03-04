/// \file Star.h
/// \brief Interface for the spike object class CStar.

#ifndef __L4RC_GAME_STAR_H__
#define __L4RC_GAME_STAR_H__

#include "Object.h"

/// \brief The spike object. 
///
/// The abstract representation of the star object. The star is a static object
/// gives the player invincibility for a short time

class CStar : public CObject {\
protected:
	virtual void CollisionResponse(const Vector2&, float, CObject* = nullptr); ///< Collision response.
public:
	CStar(const Vector2& p);
};


#endif //__L4RC_GAME_STAR_H__