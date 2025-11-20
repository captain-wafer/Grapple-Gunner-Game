/// \file Door.h
/// \brief Interface for the door object class Door.

#ifndef __L4RC_GAME_DOOR_H__
#define __L4RC_GAME_DOOR_H__


#include "Object.h"

/// \brief The door object. 
///
/// The abstract representation of the door object. The door is a static object
/// that ends the level upon player collision

class CDoor : public CObject {
public:
	CDoor(eSprite doorSprite, const Vector2& p);
};


#endif //__L4RC_GAME_DOOR_H__