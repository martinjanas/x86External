#pragma once

#include <Windows.h>
#include <iostream>
#include <algorithm>
#include <thread>
#include <atomic>

#include "MemoryManager.h"
#include "Globals.h"
#include "CPlayer.h"
#include "QAngle.h"
#include "enums.h"
#include "class_definitions.h"

namespace utils
{
	float   curtime(const int& tick_base, CGlobalVarsBase global_vars);
	bool    IsRCSWeapon(int id);
	bool    IsAimbotWeapon(int id);
	float   GetFovToPlayer(const QAngle& current_angles, const QAngle& aim_angles);
	QAngle& GetBone(int iBone, DWORD matrix);
	QAngle& CalcAngle(const QAngle& src, const QAngle& dst);
}
