#pragma once

#include "MemoryManager.h"
#include "Globals.h"

enum life_state : unsigned char
{
	LIFE_ALIVE = 0,// alive
	LIFE_DYING = 1, // playing death animation or still falling off of a ledge waiting to hit ground
	LIFE_DEAD = 2, // dead. lying still.
	MAX_LIFESTATE
};

enum team : int
{
	team_spec = 1,
	team_t = 2,
	team_ct = 3
};

#define	FL_ONGROUND (1<<0)	// At rest / on the ground

class CPlayer
{
public:
	static CPlayer* GetPlayer(int index);
	static CPlayer* GetLocalPlayer();
	int GetHealth();
	int GetTeam();
	int GetShotsFired();
	BYTE GetFlags();
	bool IsSpotted();
	bool IsDormant();
	bool IsScoped();
};
