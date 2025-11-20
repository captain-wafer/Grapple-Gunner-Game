/// \file LaunchPad.h
/// \brief Interface for the spike object class CLaunchPad.

#ifndef __L4RC_GAME_LAUNCHPAD_H__
#define __L4RC_GAME_LAUNCHPAD_H__

#include "Object.h"

/// \brief launch pad object. 
///
/// The abstract representation of the launch pad object. The launch pad is a static object
/// that boosts the player up higher than a jump

class CLaunchPad : public CObject {
public:
	CLaunchPad(const Vector2& p);
};


#endif //__L4RC_GAME_LAUNCHPAD_H__