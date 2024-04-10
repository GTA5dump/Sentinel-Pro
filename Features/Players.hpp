#pragma once 
#include "../Common.hpp"
#include "../GTA/Invoker/Natives.hpp"
#include "../Services/Players/PlayerService.hpp"
#include "ProtectionScript.hpp"

namespace Sentinel
{
	class PlayersFeatures
	{
	public:
		void Tick()
		{	
			if (g_PlayerService->GetSelected()->IsValid())
			{
				const auto Ped = PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(g_PlayerService->GetSelected()->ID());
				const auto Coords = ENTITY::GET_ENTITY_COORDS(Ped, TRUE);
				GRAPHICS::DRAW_MARKER(3, Coords.x, Coords.y, Coords.z + 1.3f, 0.f, 0.f, 0.f, 0.f, 180.f, 0.f, 0.3f, 0.3f, 0.3f, 41, 41, 41, 200, TRUE, TRUE, 0, FALSE, NULL, NULL, FALSE);
			}

			PlayerInfo();
			Spectate(m_Spectate);
		}

		bool m_Spectate = false;
		bool m_InvisibleCages = false;

	private:
		void PlayerInfo();
		void Spectate(bool enable);

	public:
		// Seperate Ticks
		void IterateTrollingOptions();
		void IterateShootingOptions();
		void IterateActionDenyingOptions();
		void IterateExplosionCreatorOptions();
		void IterateRemoteOptions();
		void IterateEntitySpammer();
		void IterateChatSpammer();
		void IterateSoundSpammer();
		void IterateAngryPlanes();
		void IterateFriendlyOptions();
		void IterateDropManager();
		void IterateForcefield();
		void IterateWeaponOptions();
		void IterateVehicleOptions();

	};

	inline std::unique_ptr<PlayersFeatures> g_PlayersFeatures;
}
