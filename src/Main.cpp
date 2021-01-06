#include <Windows.h>
#include <iostream>
#include <algorithm>

#include "MemoryManager.h"
#include "Globals.h"
#include "CPlayer.h"
#include "QAngle.h"

QAngle OldAngle, FixedAngle;

enum EItemDefinitionIndex
{
	WEAPON_NONE = 0,
	WEAPON_DEAGLE,
	WEAPON_ELITE,
	WEAPON_FIVESEVEN,
	WEAPON_GLOCK,
	WEAPON_AK47 = 7,
	WEAPON_AUG,
	WEAPON_AWP,
	WEAPON_FAMAS,
	WEAPON_G3SG1,
	WEAPON_GALILAR = 13,
	WEAPON_M249,
	WEAPON_M4A1 = 16,
	WEAPON_MAC10,
	WEAPON_P90 = 19,
	WEAPON_MP5SD = 23,
	WEAPON_UMP45,
	WEAPON_XM1014,
	WEAPON_BIZON,
	WEAPON_MAG7,
	WEAPON_NEGEV,
	WEAPON_SAWEDOFF,
	WEAPON_TEC9,
	WEAPON_TASER,
	WEAPON_HKP2000,
	WEAPON_MP7,
	WEAPON_MP9,
	WEAPON_NOVA,
	WEAPON_P250,
	WEAPON_SHIELD,
	WEAPON_SCAR20,
	WEAPON_SG556,
	WEAPON_SSG08,
	WEAPON_KNIFEGG,
	WEAPON_KNIFE,
	WEAPON_FLASHBANG,
	WEAPON_HEGRENADE,
	WEAPON_SMOKEGRENADE,
	WEAPON_MOLOTOV,
	WEAPON_DECOY,
	WEAPON_INCGRENADE,
	WEAPON_C4,
	WEAPON_HEALTHSHOT = 57,
	WEAPON_KNIFE_T = 59,
	WEAPON_M4A1_SILENCER,
	WEAPON_USP_SILENCER,
	WEAPON_CZ75A = 63,
	WEAPON_REVOLVER,
	WEAPON_TAGRENADE = 68,
	WEAPON_FISTS,
	WEAPON_BREACHCHARGE,
	WEAPON_TABLET = 72,
	WEAPON_MELEE = 74,
	WEAPON_AXE,
	WEAPON_HAMMER,
	WEAPON_SPANNER = 78,
	WEAPON_KNIFE_GHOST = 80,
	WEAPON_FIREBOMB,
	WEAPON_DIVERSION,
	WEAPON_FRAG_GRENADE,
	WEAPON_SNOWBALL,
	WEAPON_BUMPMINE,
	WEAPON_KNIFE_BAYONET = 500,
	WEAPON_KNIFE_CSS = 503,
	WEAPON_KNIFE_FLIP = 505,
	WEAPON_KNIFE_GUT = 506,
	WEAPON_KNIFE_KARAMBIT = 507,
	WEAPON_KNIFE_M9_BAYONET = 508,
	WEAPON_KNIFE_TACTICAL = 509,
	WEAPON_KNIFE_FALCHION = 512,
	WEAPON_KNIFE_SURVIVAL_BOWIE = 514,
	WEAPON_KNIFE_BUTTERFLY = 515,
	WEAPON_KNIFE_PUSH = 516,
	WEAPON_KNIFE_CORD = 517,
	WEAPON_KNIFE_CANIS = 518,
	WEAPON_KNIFE_URSUS = 519,
	WEAPON_KNIFE_GYPSY_JACKKNIFE = 520,
	WEAPON_KNIFE_OUTDOOR = 521,
	WEAPON_KNIFE_STILETTO = 522,
	WEAPON_KNIFE_WIDOWMAKER = 523,
	WEAPON_KNIFE_SKELETON = 525,
	GLOVE_STUDDED_BLOODHOUND = 5027,
	GLOVE_T_SIDE = 5028,
	GLOVE_CT_SIDE = 5029,
	GLOVE_SPORTY = 5030,
	GLOVE_SLICK = 5031,
	GLOVE_LEATHER_WRAP = 5032,
	GLOVE_MOTORCYCLE = 5033,
	GLOVE_SPECIALIST = 5034,
	GLOVE_HYDRA = 5035
};

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
	delta.y = std::clamp(delta.y, -180.f, 180.f);
	delta.x = std::clamp(delta.x, -89.f, 89.f);
	delta.z = 0.f;

	return sqrtf(powf(delta.x, 2.0f) + powf(delta.y, 2.0f));
}

DWORD global_vars = p::read<DWORD>(globals::ENGINE_DLL + 0x58ECE8);
float interval_per_tick = p::read<float>(global_vars + 0x20);

QAngle& GetBone(int iBone, DWORD matrix)
{
	QAngle TempBone;

	TempBone.x = p::read<float>(matrix + 0x30 * iBone + 0x0C);
	TempBone.y = p::read<float>(matrix + 0x30 * iBone + 0x1C);
	TempBone.z = p::read<float>(matrix + 0x30 * iBone + 0x2C);

	return TempBone;
}

QAngle& CalcAngle(const QAngle& src, const QAngle& dst)
{
	QAngle vAngle;
	QAngle delta{ (src.x - dst.x), (src.y - dst.y), (src.z - dst.z) };
	double hyp = sqrt(delta.x * delta.x + delta.y * delta.y);

	vAngle.x = (float)(atan((delta.z + 64.06f) / hyp) * (180 / 3.1415927f));
	vAngle.y = (float)(atan(delta.y / delta.x) * (180 / 3.1415927f));
	vAngle.z = 0.0f;

	if (delta.x >= 0.0)
		vAngle.y += 180.0f;

	return vAngle;
}

class glow_object_t {
public:
	void* entity;
	float r;
	float g;
	float b;
	float   m_flAlpha;                 //0x0010
	uint8_t pad_0014[4];               //0x0014
	float   m_flSomeFloat;             //0x0018
	uint8_t pad_001C[4];               //0x001C
	float   m_flBloomAmount;          //0x0020
	bool    m_bRenderWhenOccluded;     //0x0024
	bool    m_bRenderWhenUnoccluded;   //0x0025
	bool    m_bFullBloomRender;        //0x0026
	uint8_t pad_0027[5];               //0x0027
	int32_t m_nGlowStyle;              //0x002C
	int32_t m_nSplitScreenSlot;        //0x0030
	int32_t m_nNextFreeSlot;           //0x0034
};

int main()
{
	globals::proc = p::get_process(L"csgo.exe");
	globals::CLIENT_DLL = p::get_module(globals::proc, L"client.dll");
	globals::ENGINE_DLL = p::get_module(globals::proc, L"engine.dll");

	glow_object_t glow_object;

	while (globals::proc != NULL)
	{
		CPlayer* localplayer = CPlayer::GetLocalPlayer();

		if (!localplayer)
			continue; 
		
		DWORD client_state = p::read<DWORD>(globals::ENGINE_DLL + 0x58EFE4); //dwClientState 

		QAngle AimPunch = p::read<QAngle>(reinterpret_cast<uintptr_t>(localplayer) + 0x302C); //m_aimPunchAngle
		QAngle local_ang = p::read<QAngle>(client_state + 0x4D90);

		DWORD active_wep = p::read<DWORD>(reinterpret_cast<uintptr_t>(localplayer) + 0x2EF8); //activeWep

		int pWeaponEnt = active_wep & 0xFFF;
		DWORD pWeapon = p::read<DWORD>(globals::CLIENT_DLL + 0x4D9FBD4 + (pWeaponEnt - 1) * 0x10); //dwEntityList
		int id = p::read<int>(pWeapon + 0x2FAA); //iItemDefinitonIndex

		for (int i = 1; i < 65; i++)
		{
			CPlayer* player = CPlayer::GetPlayer(i);

			if (!player)
				continue;

			if (player->IsDormant())
				continue;

			if (player->GetHealth() <= 0)
				continue;

			if (player->GetTeam() != localplayer->GetTeam()) //ClRender chams
			{
				/*p::write<BYTE>(reinterpret_cast<uintptr_t>(player) + 0x70, 255);
				p::write<BYTE>(reinterpret_cast<uintptr_t>(player) + 0x71, 0);
				p::write<BYTE>(reinterpret_cast<uintptr_t>(player) + 0x72, 0);
				float brightness = p::write<int>(reinterpret_cast<uintptr_t>(player) + 0x73, 10);

				DWORD thisPtr = (globals::ENGINE_DLL + 0x59205C - 0x2c); //ambient_min
				DWORD xored = *(DWORD*)&brightness ^ thisPtr;
				p::write<int>(globals::ENGINE_DLL + 0x59205C, xored); //ambient_min*/

				int glow_index = p::read<int>(reinterpret_cast<uintptr_t>(player) + 0xA438); //m_iGlowIndex

				DWORD glow_object_manager = p::read<DWORD>(globals::CLIENT_DLL + 0x52E81B0); //dwGlowObjectManager 

				glow_object = p::read<glow_object_t>(glow_object_manager + (glow_index * 0x38)); //dwGlowObjectManager

				glow_object.entity = player;
				glow_object.r = 1.0f;
				glow_object.g = 0.f;
				glow_object.b = 0.4f;
				glow_object.m_flAlpha = 1.0f;
				glow_object.m_bRenderWhenOccluded = true;
				glow_object.m_bRenderWhenUnoccluded = false;
				glow_object.m_nGlowStyle = 1;

				p::write<glow_object_t>(glow_object_manager + (glow_index * 0x38), glow_object);
			}
			
			if (player->GetTeam() == localplayer->GetTeam()) //ClRender chams
			{
				if (player != localplayer)
				{
					/*p::write<BYTE>(reinterpret_cast<uintptr_t>(player) + 0x70, 0);
					p::write<BYTE>(reinterpret_cast<uintptr_t>(player) + 0x71, 255);
					p::write<BYTE>(reinterpret_cast<uintptr_t>(player) + 0x72, 0);
					float brightness = p::write<int>(reinterpret_cast<uintptr_t>(player) + 0x73, 10);

					DWORD thisPtr = (globals::ENGINE_DLL + 0x59205C - 0x2c); //ambient_min
					DWORD xored = *(DWORD*)&brightness ^ thisPtr;
					p::write<int>(globals::ENGINE_DLL + 0x59205C, xored); //ambient_min*/
				}
			}

			QAngle localpos = p::read<QAngle>(reinterpret_cast<uintptr_t>(localplayer) + 0x138);
			QAngle enemypos = p::read<QAngle>(reinterpret_cast<uintptr_t>(player) + 0x138);

			QAngle local_ang = p::read<QAngle>(client_state + 0x4D90);

			switch (id)
			{
			case 1:
			case 2:
			case 3:
			case 4:
			case 7:
			case 8:
			case 9:
			case 10:
			case 11:
			case 13:
			case 14:
			case 16:
			case 17:
			case 19:
			case 23:
			case 24:
			case 25:
			case 26:
			case 27:
			case 28:
			case 29:
			case 30:
			case 32:
			case 33:
			case 34:
			case 35:
			case 36:
			case 38:
			case 39:
			case 40:
				p::write<int>(pWeapon + 0x2FC0, -1); //iItemIDHigh
				p::write<int>(pWeapon + 0x31C8, 3); //m_nFallbackPaintkit
				break;
			}

			if (GetAsyncKeyState(VK_LEFT))
				p::write<int>(client_state + 0x174, -1);

			if (!player->IsSpotted())
				p::write<bool>(reinterpret_cast<uintptr_t>(player) + 0x93D, true);

			float dist = localpos.DistTo(enemypos);

			if (IsAimbotWeapon(id))
			{
				if (GetAsyncKeyState(VK_LBUTTON) & 0x8000)
				{
					if (player->IsDormant())
						continue;

					if (player->GetHealth() <= 0)
						continue;

					if (player->GetTeam() == localplayer->GetTeam())
						continue;

					DWORD matrix = p::read<DWORD>(reinterpret_cast<uintptr_t>(player) + 0x26A8);

					QAngle test = CalcAngle(localpos, GetBone(8, matrix));

					float fov = GetFovToPlayer(local_ang, test);

					if (fov < 4.f)
					{
						test.y = std::clamp(test.y, -180.f, 180.f);
						test.x = std::clamp(test.x, -89.f, 89.f);
						test.z = 0.f;

						QAngle qDelta(test - local_ang);

						test.x = local_ang.x + qDelta.x / 4.f;
						test.y = local_ang.y + qDelta.y / 4.f;

						p::write<QAngle>(client_state + 0x4D90, test);
						Sleep(16);
					}
				}
			}
		}

		if (IsRCSWeapon(id))
		{
			if (GetAsyncKeyState(VK_LBUTTON) && localplayer->GetShotsFired() > 1)
			{
				AimPunch.x *= 2.f;
				AimPunch.y *= 2.f;

				FixedAngle.x = local_ang.x - (AimPunch.x - OldAngle.x);
				FixedAngle.y = local_ang.y - (AimPunch.y - OldAngle.y);

				p::write<QAngle>(client_state + 0x4D90, FixedAngle);
				Sleep(16);
			}
			OldAngle = AimPunch;
		}

		Sleep(1);
	}
}