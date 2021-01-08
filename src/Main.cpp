#include "utils.h"

QAngle OldAngle, FixedAngle;

int paintkit = 0;

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

		QAngle aimpunch = p::read<QAngle>(reinterpret_cast<uintptr_t>(localplayer) + 0x302C); //m_aimPunchAngle
		QAngle localpos = p::read<QAngle>(reinterpret_cast<uintptr_t>(localplayer) + 0x138);
		QAngle local_viewangles = p::read<QAngle>(client_state + 0x4D90);

		DWORD active_wep = p::read<DWORD>(reinterpret_cast<uintptr_t>(localplayer) + 0x2EF8); //m_hActiveWeapon

		DWORD wpn = p::read<DWORD>(globals::CLIENT_DLL + 0x4DA2E24 + ((active_wep & 0xFFF) - 1) * 0x10); //dwEntityList

		int id = p::read<int>(wpn + 0x2FAA); //iItemDefinitonIndex

		CGlobalVarsBase global_vars = p::read<CGlobalVarsBase>(globals::ENGINE_DLL + 0x58ECE8); //dwGlobalVars

		if (GetAsyncKeyState(VK_SPACE) && localplayer->GetFlags() & FL_ONGROUND)
			p::write<int>(globals::CLIENT_DLL + 0x524CD84, 6); //dwForceJump

		int tick_base = p::read<int>(reinterpret_cast<unsigned int>(localplayer) + 0x3430); //m_nTickBase

		for (int i = 0; i < 1024; i++)
		{
			CPlayer* entity = CPlayer::GetPlayer(i);

			if (!entity)
				continue;

			if (entity == localplayer)
				continue;

			if (entity->IsDormant())
				continue;

			int icn = p::read<int>(reinterpret_cast<unsigned int>(entity) + 0x8); //IClientNetworkable vtable
			int get_client_class = p::read<int>(icn + 0x8); //3rd function in the vtable (GetClientClass)
			int client_class_struct = p::read<int>(get_client_class + 0x1); //pointer to the ClientClass struct out of the mov eax
			int classid = p::read<int>(client_class_struct + 0x14); //classid

			if (classid == ClassID::CPlantedC4)
			{
				bool is_bomb_ticking = p::read<bool>(reinterpret_cast<unsigned int>(entity) + 0x2980); //m_bBombTicking

				float timer = p::read<float>(reinterpret_cast<unsigned int>(entity) + 0x2990); //m_flTimerLength

				float final_timer = (timer - utils::curtime(tick_base, global_vars));

				if(is_bomb_ticking && final_timer >= 0.1f)
				   printf("Bomb time left: %0.1f\n", final_timer <= 0.0f ? 0.0f : final_timer);
			}
		}

		for (int i = 1; i < 65; i++)
		{
			CPlayer* player = CPlayer::GetPlayer(i);

			if (!player)
				continue;

			if (player->IsDormant())
				continue;

			if (player->GetHealth() <= 0)
				continue;

			bool is_defusing = p::read<bool>(reinterpret_cast<unsigned int>(player) + 0x3930); //m_bIsDefusing

			int glow_index = p::read<int>(reinterpret_cast<uintptr_t>(player) + 0xA438); //m_iGlowIndex

			DWORD glow_object_manager = p::read<DWORD>(globals::CLIENT_DLL + 0x52EB418); //dwGlowObjectManager 

			glow_object = p::read<glow_object_t>(glow_object_manager + (glow_index * 0x38)); //dwGlowObjectManager

			BYTE color_r = 0;
			BYTE color_g = 0;
			BYTE color_b = 0;
			float color_a = 0;

			if (player->GetTeam() != localplayer->GetTeam()) //ClRender chams
			{
				if (player != localplayer)
				{
					color_r = 255;
					color_g = 0;
					color_b = 0;
					color_a = 2.f;

					glow_object.entity = player;
					glow_object.r = 1.0f;
					glow_object.g = is_defusing ? 0.f : 1.f;
					glow_object.b = 0.f;
					glow_object.a = 1.0f;
					glow_object.render_when_occluded = true;
					glow_object.render_when_unoccluded = false;
					glow_object.glow_style = is_defusing ? 0 : 1;
				}
			}
			
			if (player->GetTeam() == localplayer->GetTeam()) //ClRender chams
			{
				if (player != localplayer)
				{
					color_r = 0;
					color_g = 0;
					color_b = 255;
					color_a = 2.f;

					glow_object.entity = player;
					glow_object.r = 1.f;
					glow_object.g = 0.f;
					glow_object.b = 0.f;
					glow_object.a = 1.0f;
					glow_object.render_when_occluded = true;
					glow_object.render_when_unoccluded = false;
					glow_object.glow_style = 1;
				}
			}
			
			p::write<BYTE>(reinterpret_cast<uintptr_t>(player) + 0x70, color_r);
			p::write<BYTE>(reinterpret_cast<uintptr_t>(player) + 0x71, color_g);
			p::write<BYTE>(reinterpret_cast<uintptr_t>(player) + 0x72, color_b);

			DWORD ambient_min = (globals::ENGINE_DLL + 0x59205C - 0x2c); //ambient_min
			DWORD xored = *(DWORD*)&color_a ^ ambient_min;
			p::write<int>(globals::ENGINE_DLL + 0x59205C, xored);        //ambient_min

			p::write<glow_object_t>(glow_object_manager + (glow_index * 0x38), glow_object);

			if (!player->IsSpotted())
				p::write<bool>(reinterpret_cast<uintptr_t>(player) + 0x93D, true);

			if (utils::IsAimbotWeapon(id))
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

					QAngle test = utils::CalcAngle(localpos, utils::GetBone(8, matrix));

					float fov = utils::GetFovToPlayer(local_viewangles, test);

					if (fov < 2.f)
					{
						if (test.Clamped())
						{
							QAngle qDelta(test - local_viewangles);

							test.pitch = local_viewangles.pitch + qDelta.pitch / 12.f;
							test.yaw = local_viewangles.yaw + qDelta.yaw / 12.f;

							p::write<QAngle>(client_state + 0x4D90, test);
							Sleep(16);
						}
					}
				}
			}
		}

		for (int i = 0; i < 8; i++)
		{
			DWORD pw = p::read<DWORD>(reinterpret_cast<uintptr_t>(localplayer) + 0x2DF8 + i * 0x4) & 0xfff; //m_hMyWeapons
			pw = p::read<DWORD>(globals::CLIENT_DLL + 0x4DA2E24 + (pw - 1) * 0x10); //dwEntityList
			
			if (!pw)
				continue;

			int id = p::read<int>(pw + 0x2FAA); //iItemDefinitonIndex

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
				paintkit = 3;
				break;
			}

			p::write<int>(pw + 0x2FC0, -1); //iItemIDHigh
			p::write<int>(pw + 0x31C8, paintkit); //m_nFallbackPaintkit

			if(GetAsyncKeyState(VK_LEFT))
			   p::write<int>(client_state + 0x174, -1);
		}

		if (utils::IsRCSWeapon(id))
		{
			if (GetAsyncKeyState(VK_LBUTTON) && localplayer->GetShotsFired() > 1)
			{
				aimpunch.pitch *= 2.f;
				aimpunch.yaw *= 2.f;

				FixedAngle.pitch = local_viewangles.pitch - (aimpunch.pitch - OldAngle.pitch);
				FixedAngle.yaw = local_viewangles.yaw - (aimpunch.yaw - OldAngle.yaw);

				if (FixedAngle.Clamped())
				{
					p::write<QAngle>(client_state + 0x4D90, FixedAngle);
					Sleep(16);
				}
			}
			OldAngle = aimpunch;
		}

		Sleep(1);
	}
}