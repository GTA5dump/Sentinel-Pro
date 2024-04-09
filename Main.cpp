#include "Common.hpp"
#include "Pointers/Pointers.hpp"
#include "GTA/Invoker/Invoker.hpp"
#include "Renderer/Renderer.hpp"
#include "GUI/GUI.hpp"
#include "GUI/Overlays/Overlays.hpp"
#include "GUI/Menu/Menu.hpp"
#include "Services/Friends/FriendsService.hpp"
#include "Services/Players/PlayerService.hpp"
#include "Services/ScriptPatcher/ScriptPatcher.hpp"
#include "Features/FeatureManager.hpp"
#include "GTA/Script/ScriptManager.hpp"
#include "GTA/Script/ScriptQueue.hpp"
#include "GTA/GTA.hpp"
#include "Hooking/Hooking.hpp"
#include "Hooking/NativeHooks.hpp"
#include "Utility/System.hpp"
#include "Logger/Logger.hpp"
#include "Logger/ExceptionHandler.hpp"

namespace Sentinel
{
	DWORD __stdcall main(void*)
	{
		auto Handler = ExceptionHandler();

		while (!FindWindow(L"grcWindow", nullptr))
			std::this_thread::sleep_for(100ms);

		std::filesystem::path BaseDirectory = std::getenv("USERPROFILE");
		BaseDirectory /= "Documents";
		BaseDirectory /= "Sentinel";
		BaseDirectory /= "Sentinel Pro";
		BaseDirectory /= "Cheat.log";

		std::filesystem::path BackupDirectory = std::getenv("USERPROFILE");
		BackupDirectory /= "Documents";
		BackupDirectory /= "Sentinel";
		BackupDirectory /= "Sentinel Pro";
		BackupDirectory /= "Backup";

		auto LoggerInstance = std::make_unique<Log>("Sentinel Pro: Open Beta", BaseDirectory, BackupDirectory);

		EnableMenuItem(GetSystemMenu(GetConsoleWindow(), 0), SC_CLOSE, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);

		std::srand(std::chrono::system_clock::now().time_since_epoch().count());

		try
		{	
			/*LOG(INFO) << "[Auth] Making a connection to Sentinel's API...";
			std::filesystem::path CredentialsPath;
			CredentialsPath.append(std::getenv("USERPROFILE"));
			CredentialsPath.append("Documents");
			CredentialsPath.append("Sentinel");
			CredentialsPath.append("Credentials.json");
			if (!std::filesystem::exists(CredentialsPath))
				LOG(FATAL) << "[Auth] Vectored Exception: Credentials.json couldn't be located.";

			WorldShield::Initialize();
			WorldShield::AuthenticateFromJSON();

			for (const auto& Message : WorldShield::GetMessages())
			{
				LOG(FATAL) << "[Auth] Vectored Exception: " << Message;
				WorldShield::GetMessages().erase(WorldShield::GetMessages().begin());
			}*/

			/*if (WorldShield::g_KeyLevel < 3) 
				LOG(FATAL) << "[Auth] Vectored Exception: You're not allowed to use this version";*/

			//LOG(INFO) << "[Auth] Welcome back, " << WorldShield::GetUsername() << "!";

			Pointers::Initialize();
			Invoker::CacheHandlers();

			if (*Pointers::pGameState != eGameState::Playing)
			{
				LOG(INFO) << "[Main] Waiting for the game to load, before continuing";
				do
				{
					std::this_thread::sleep_for(100ms);
				} while (*Pointers::pGameState != eGameState::Playing);

				LOG(INFO) << "[Main] Game has finished loading, continuing";
			}

			Renderer::Initialize();
			GUI::Initialize();

			auto PlayerServiceInstance = std::make_unique<PlayerService>();
			auto ScriptPatcherInstance = std::make_unique<ScriptPatcher>();

			ScriptManager::Initialize(std::make_unique<Script>(&Menu::ScriptMain));
			auto ScriptQueueInstance = std::make_unique<ScriptQueue>(10);
			FeatureManager::Initialize();

			Hooking::Initialize();
			auto g_NativeHooks = std::make_unique<NativeHooks>();

			//if (System::Killswitch())
			//	__fastfail(420);

			/*if (System::Killswitch())
				__fastfail(420);

			WorldShield::Heartbeat();

			for (const auto& Message : WorldShield::GetMessages())
			{
				LOG(FATAL) << "[Auth] Vectored Exception: " << Message;
				WorldShield::GetMessages().erase(WorldShield::GetMessages().begin());
			}*/

			while (gRunning)
			{
				/*WorldShield::DebuggerChecks();
				for (const auto& Message : WorldShield::GetMessages())
				{
					LOG(FATAL) << "[Auth] Vectored Exception: " << Message;
					WorldShield::GetMessages().erase(WorldShield::GetMessages().begin());
				}
				if (const auto Atom = GlobalFindAtomA(xorstr_("z17#nEgmqXngCRZa7TC39o*^%BsZVVqf7hg#DYqg1DxszdoFc6")); Atom == 0 && !gAtomFound) {
					LOG(FATAL) << "[Auth] Vectored Exception: You're doing something shady";
					__fastfail(420);
				}
				else if (gShouldCheck)
				{
					GlobalDeleteAtom(Atom);
					gAtomFound = true;
					gShouldCheck = false;
				}*/
				std::this_thread::yield();
			}

			g_NativeHooks.reset();
			Hooking::Destroy();

			FeatureManager::Destroy();
			ScriptQueueInstance.reset();
			ScriptManager::Destroy();
			ScriptPatcherInstance.reset();
			PlayerServiceInstance.reset();
			Renderer::Destroy();
			Pointers::Destroy();
		}
		catch (std::exception const& Exception)
		{
			MessageBoxA(nullptr, Exception.what(), nullptr, MB_OK | MB_ICONEXCLAMATION);
		}

		LOG(INFO) << "[Main] We hope to see you soon again!";
		LoggerInstance->Destroy();
		LoggerInstance.reset();

		CloseHandle(gMainThread);
		FreeLibraryAndExitThread(gInstance, 0);

		return 0;
	}
}

bool __stdcall DllMain(HINSTANCE hinstance, std::uint32_t reason, void** reserved)
{
	switch (reason)
	{
	case DLL_PROCESS_ATTACH:
	{
		DisableThreadLibraryCalls(hinstance);

		Sentinel::gMainThread = CreateThread(nullptr, 0, Sentinel::main, nullptr, 0, nullptr);
		Sentinel::gInstance = hinstance;

		break;
	}
	}

	return true;
}