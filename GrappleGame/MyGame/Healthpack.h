/// \file Healthpack.h
/// \contains class for health packs

#ifndef __L4RC_GAME_HEALTHPACK_H__
#define __L4RC_GAME_HEALTHPACK_H__

#include "Object.h"

class CHealthPack : public CObject {
	protected:
		virtual void CollisionResponse(const Vector2&, float, CObject* = nullptr); ///< Collision response.
public:
	CHealthPack(const Vector2& p);
};

#endif