#pragma once 
#include "../Common.hpp"
#include "System.hpp"
#include "Lists.hpp"
#include "../GTA/enums.hpp"
#include "../GTA/GTA.hpp"
#include "../GTA/Invoker/Natives.hpp"
#include "../GTA/Script/ScriptManager.hpp"
#include "../GTA/Script/ScriptGlobal.hpp"
#include "../GTA/Script/ScriptFunction.hpp"
#include "../Pointers/Pointers.hpp"
#include "../Hooks/HookTable.hpp"
#include "../Services/Players/PlayerService.hpp"
#include <GTAV-Classes/script/HudColor.hpp>
#include <curl/curl.h>

namespace Sentinel::System
{
	inline void ModelSpawnBypass(bool toggle)
	{
		*(unsigned short*)Pointers::pModelSpawnBypass = toggle ? 0x9090 : 0x0574;
	}

	inline void AddOwnedExplosionBypass(bool toggle)
	{
		*(unsigned short*)Pointers::pAddOwnedExplosionBypass = toggle ? 0xE990 : 0x850F;
	}

	inline void DrawChat(char* msg, const char* player_name, bool is_team)
	{
		const auto Scaleform = GRAPHICS::REQUEST_SCALEFORM_MOVIE("MULTIPLAYER_CHAT");

		GRAPHICS::BEGIN_SCALEFORM_MOVIE_METHOD(Scaleform, "ADD_MESSAGE");
		GRAPHICS::SCALEFORM_MOVIE_METHOD_ADD_PARAM_PLAYER_NAME_STRING(player_name); // player name
		GRAPHICS::SCALEFORM_MOVIE_METHOD_ADD_PARAM_LITERAL_STRING(msg);             // content
		GRAPHICS::SCALEFORM_MOVIE_METHOD_ADD_PARAM_TEXTURE_NAME_STRING(HUD::GET_FILENAME_FOR_AUDIO_CONVERSATION(is_team ? "MP_CHAT_TEAM" : "MP_CHAT_ALL")); // Scope
		GRAPHICS::SCALEFORM_MOVIE_METHOD_ADD_PARAM_BOOL(false);                               // Team Only
		GRAPHICS::SCALEFORM_MOVIE_METHOD_ADD_PARAM_INT((int)HudColor::HUD_COLOUR_PURE_WHITE); // eHudColour
		GRAPHICS::END_SCALEFORM_MOVIE_METHOD();

		GRAPHICS::BEGIN_SCALEFORM_MOVIE_METHOD(Scaleform, "SET_FOCUS");
		GRAPHICS::SCALEFORM_MOVIE_METHOD_ADD_PARAM_INT(1);                                    // VISIBLE_STATE_DEFAULT
		GRAPHICS::SCALEFORM_MOVIE_METHOD_ADD_PARAM_INT(0);                                    // Scope Type (unused)
		GRAPHICS::SCALEFORM_MOVIE_METHOD_ADD_PARAM_INT(0);                                    // Scope (unused)
		GRAPHICS::SCALEFORM_MOVIE_METHOD_ADD_PARAM_PLAYER_NAME_STRING(player_name);           // Player
		GRAPHICS::SCALEFORM_MOVIE_METHOD_ADD_PARAM_INT((int)HudColor::HUD_COLOUR_PURE_WHITE); // eHudColour
		GRAPHICS::END_SCALEFORM_MOVIE_METHOD();

		GRAPHICS::DRAW_SCALEFORM_MOVIE_FULLSCREEN(Scaleform, 255, 255, 255, 255, 0);

		// Fix broken scaleforms, when chat already opened
		if (const auto ChatData = *Pointers::pChatData; ChatData && (ChatData->m_chat_open || ChatData->m_timer_two))
			HUD::CLOSE_MP_TEXT_CHAT();
	}

	inline void SendPublicChatMessage(char* message, bool view_local)
	{
		if (const auto GamePlayer = GTA::GetNetworkPlayerMgr()->m_local_net_player; GamePlayer)
		{
			if (Hooking::GetHook<DetourHook>("SCM")->Original<decltype(&GTA::SendChatMessage)>()(*Pointers::pSendChatMessagePointer, GamePlayer->get_net_data(), message, false))
			{
				if (view_local)
					System::DrawChat(message, GamePlayer->get_name(), false);
			}
		}
	}

	inline void DrawSprite(const char* dict, const char* texture, float x, float y, float width, float height, Color color, float rotation)
	{
		if (GRAPHICS::HAS_STREAMED_TEXTURE_DICT_LOADED(dict))
			GRAPHICS::DRAW_SPRITE(dict, texture, x, y, width, height, rotation, color.r, color.g, color.b, color.a, FALSE, NULL);
		else
			GRAPHICS::REQUEST_STREAMED_TEXTURE_DICT(dict, FALSE);
	}

	typedef std::size_t(*Write)(char*, std::size_t, std::size_t, std::string*);

	inline std::string Request(std::string url)
	{
		CURLcode ResultCode = CURLE_FAILED_INIT;
		CURL* Instance = curl_easy_init();
		std::string Result;

		curl_global_init(CURL_GLOBAL_ALL);

		if (Instance) {
			curl_easy_setopt(Instance,
				CURLOPT_WRITEFUNCTION,
				static_cast <Write> ([](char* contents, std::size_t size,
					std::size_t nmemb, std::string* data) -> std::size_t {
						std::size_t new_size = size * nmemb;
						if (data == NULL) {
							return 0;
						}
						data->append(contents, new_size);
						return new_size;
			}));
			curl_easy_setopt(Instance, CURLOPT_WRITEDATA, &Result);
			curl_easy_setopt(Instance, CURLOPT_URL, url.c_str());
			curl_easy_setopt(Instance, CURLOPT_USERAGENT, xorstr_("Old Master"));

			ResultCode = curl_easy_perform(Instance);

			if (ResultCode != CURLE_OK)
				MessageBoxA(0, curl_easy_strerror(ResultCode), 0, MB_ICONERROR);

			curl_easy_cleanup(Instance);
		}

		curl_global_cleanup();

		return Result;
	}

	inline bool Killswitch()
	{
		return Request(xorstr_("https://pastebin.com/raw/B2Ez5sny")) == xorstr_("YES");
	}

	inline RGB g_SlowRainbowColor;
	inline void SlowRainbowLoop()
	{
		static int Color[] = { 255, 0, 0 }; // Red
		static int Stage = 0;

		if (Stage == 0)
		{
			Color[2]++;
			if (Color[2] == 255)
				Stage = 1;
		}
		else if (Stage == 1)
		{
			Color[0]--;
			if (Color[0] == 0)
				Stage = 2;
		}
		else if (Stage == 2)
		{
			Color[1]++;
			if (Color[1] == 255)
				Stage = 3;
		}
		else if (Stage == 3)
		{
			Color[2]--;
			if (Color[2] == 0)
				Stage = 4;
		}
		else if (Stage == 4)
		{
			Color[0]++;
			if (Color[0] == 255)
				Stage = 5;
		}
		else if (Stage == 5)
		{
			Color[1]--;
			if (Color[1] == 0)
				Stage = 0;
		}

		g_SlowRainbowColor.r = Color[0];
		g_SlowRainbowColor.g = Color[1];
		g_SlowRainbowColor.b = Color[2];
	}

	inline RGB g_FastRainbowColor;
	inline void FastRainbowLoop()
	{
		static int Color[] = { 255, 0, 0 }; // Red
		static int Stage = 0;

		switch (Stage)
		{
		case 0:
			Color[2] = Color[2] + 15;
			if (Color[2] == 255)
				Stage = 1;
			break;
		case 1:
			Color[0] = Color[0] - 15;
			if (Color[0] == 0)
				Stage = 2;
			break;
		case 2:
			Color[1] = Color[1] + 15;
			if (Color[1] == 255)
				Stage = 3;
			break;
		case 3:
			Color[2] = Color[2] - 15;
			if (Color[2] == 0)
				Stage = 4;
			break;
		case 4:
			Color[0] = Color[0] + 15;
			if (Color[0] == 255)
				Stage = 5;
			break;
		case 5:
			Color[1] = Color[1] - 15;
			if (Color[1] == 0)
				Stage = 0;
			break;
		}

		g_FastRainbowColor.r = Color[0];
		g_FastRainbowColor.g = Color[1];
		g_FastRainbowColor.b = Color[2];
	}

	inline std::string GetShaderName(std::uint32_t name_hash) 
	{
		auto vit = std::find_if(begin(Lists::g_VFXDropNames), end(Lists::g_VFXDropNames), [=](std::pair<uint32_t, std::string> e) { return e.first == name_hash; });
		if (vit != end(Lists::g_VFXDropNames)) {
			return vit->second;
		}

		return "Unknown";
	}
}