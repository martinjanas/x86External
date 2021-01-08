#include "CPlayer.h"

CPlayer* CPlayer::GetPlayer(int index)
{
	return p::read<CPlayer*>((globals::CLIENT_DLL + 0x4DA2E24) + (index * 0x10)); //dwEntityList
}

CPlayer* CPlayer::GetLocalPlayer()
{
	return p::read<CPlayer*>(globals::CLIENT_DLL + 0xD8B2BC); //dwLocalPlayer
}

bool CPlayer::IsDormant()
{
	if (!this)
		return false;

	return p::read<bool>(reinterpret_cast<uintptr_t>(this) + 0xED); //m_bDormant
}

int CPlayer::GetHealth()
{
	if (!this)
		return 0;

	return p::read<int>(reinterpret_cast<uintptr_t>(this) + 0x100); //iHealth
}

BYTE CPlayer::GetFlags()
{
	if (!this)
		return 0;

	return p::read<BYTE>(reinterpret_cast<uintptr_t>(this) + 0x104); //fFlags
}

bool CPlayer::IsSpotted()
{
	if (!this)
		return false;

	return p::read<bool>(reinterpret_cast<uintptr_t>(this) + 0x93D); //bSpotted
}

int CPlayer::GetTeam()
{
	if (!this)
		return 0;

	return p::read<int>(reinterpret_cast<uintptr_t>(this) + 0xF4); //iTeamNum
}

bool CPlayer::IsScoped()
{
	if (!this)
		return false;

	return p::read<bool>(reinterpret_cast<uintptr_t>(this) + 0x3928); //m_bIsScoped 
}

int CPlayer::GetShotsFired()
{
	if (!this)
		return 0;

	return p::read<int>(reinterpret_cast<uintptr_t>(this) + 0xA390); //m_iShotsFired 
}