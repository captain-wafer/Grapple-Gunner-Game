/// \file Spike.h
/// \brief Interface for the spike object class CSpike.

#ifndef __L4RC_GAME_SPIKE_H__
#define __L4RC_GAME_SPIKE_H__

#include "Object.h"

/// \brief The spike object. 
///
/// The abstract representation of the spike object. The spike is a static object
/// that kills the player instantly upon collision

class CSpike : public CObject {
	public:
		CSpike(const Vector2& p);
};


#endif //__L4RC_GAME_SPIKE_H__