#include "utils.h"

namespace utils
{
	float curtime(const int& tick_base, CGlobalVarsBase global_vars)
	{
		return global_vars.interval_per_tick * tick_base;
	}

	bool IsRCSWeapon(int id)
	{
		switch (id)
		{
		case WEAPON_AK47:
		case WEAPON_AUG:
		case WEAPON_BIZON:
		case WEAPON_CZ75A:
		case WEAPON_ELITE:
		case WEAPON_FAMAS:
		case WEAPON_FIVESEVEN:
		case WEAPON_GALILAR:
		case WEAPON_M249:
		case WEAPON_M4A1:
		case WEAPON_M4A1_SILENCER:
		case WEAPON_MAC10:
		case WEAPON_MP5SD:
		case WEAPON_MP7:
		case WEAPON_MP9:
		case WEAPON_P90:
		case WEAPON_TEC9:
		case WEAPON_NEGEV:
		case WEAPON_SG556:
		case WEAPON_UMP45:
			return true;
			break;
		default:
			return false;
			break;
		}
	}

	bool IsAimbotWeapon(int id)
	{
		switch (id)
		{
		case WEAPON_AK47:
		case WEAPON_AUG:
		case WEAPON_BIZON:
		case WEAPON_CZ75A:
		case WEAPON_ELITE:
		case WEAPON_FAMAS:
		case WEAPON_FIVESEVEN:
		case WEAPON_GALILAR:
		case WEAPON_M249:
		case WEAPON_M4A1:
		case WEAPON_M4A1_SILENCER:
		case WEAPON_MAC10:
		case WEAPON_MP5SD:
		case WEAPON_MP7:
		case WEAPON_MP9:
		case WEAPON_P90:
		case WEAPON_TEC9:
		case WEAPON_NEGEV:
		case WEAPON_SG556:
		case WEAPON_UMP45:
		case WEAPON_AWP:
		case WEAPON_SSG08:
		case WEAPON_SCAR20:
		case WEAPON_G3SG1:
		case WEAPON_GLOCK:
		case WEAPON_USP_SILENCER:
		case WEAPON_HKP2000:
		case WEAPON_REVOLVER:
		case WEAPON_DEAGLE:
		case WEAPON_MAG7:
		case WEAPON_NOVA:
		case WEAPON_SAWEDOFF:
		case WEAPON_XM1014:
			return true;
			break;
		default:
			return false;
			break;
		}
	}

	float GetFovToPlayer(const QAngle& current_angles, const QAngle& aim_angles)
	{
		QAngle delta = aim_angles - current_angles;

		if (delta.Clamped())
			return sqrtf(powf(delta.pitch, 2.0f) + powf(delta.yaw, 2.0f));
		else return 0.f;
	}

	QAngle& GetBone(int iBone, DWORD matrix)
	{
		QAngle TempBone;

		TempBone.pitch = p::read<float>(matrix + 0x30 * iBone + 0x0C);
		TempBone.yaw = p::read<float>(matrix + 0x30 * iBone + 0x1C);
		TempBone.roll = p::read<float>(matrix + 0x30 * iBone + 0x2C);

		return TempBone;
	}

	QAngle& CalcAngle(const QAngle& src, const QAngle& dst)
	{
		QAngle vAngle;
		QAngle delta{ (src.pitch - dst.pitch), (src.yaw - dst.yaw), (src.roll - dst.roll) };
		double hyp = sqrt(delta.pitch * delta.pitch + delta.yaw * delta.yaw);

		vAngle.pitch = (float)(atan((delta.roll + 64.06f) / hyp) * (180 / 3.1415927f));
		vAngle.yaw = (float)(atan(delta.yaw / delta.pitch) * (180 / 3.1415927f));
		vAngle.roll = 0.0f;

		if (delta.pitch >= 0.0)
			vAngle.yaw += 180.0f;

		return vAngle;
	}
}