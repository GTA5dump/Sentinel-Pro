#include "../Common.hpp"
#include "Pointers.hpp"
#include "../Hooks/HookTable.hpp"
#include "../Memory/Include.hpp"
#include "../GTA/Include.hpp"

namespace Sentinel
{
	void Pointers::Initialize()
	{
		Memory::Batch MainBatch;
		auto Region = Memory::Module(xorstr_("GTA5.exe"));

		MainBatch.Add(xorstr_("GS"), xorstr_("48 85 C9 74 4B 83 3D"), [](Memory::Handle Pointer) {
			pGameState = Pointer.Add(7).Rip().As<decltype(pGameState)>();
		});

		MainBatch.Add(xorstr_("ISS"), xorstr_("40 38 35 ? ? ? ? 75 0E 4C 8B C3 49 8B D7 49 8B CE"), [](Memory::Handle Pointer) {
			pIsSessionStarted = Pointer.Add(3).Rip().As<decltype(pIsSessionStarted)>();
		});

		MainBatch.Add(xorstr_("FC"), xorstr_("8B 15 ? ? ? ? 41 FF CF"), [](Memory::Handle Pointer) {
			pFrameCount = Pointer.Add(2).Rip().As<decltype(pFrameCount)>();
		});

		MainBatch.Add(xorstr_("SC"), xorstr_("44 38 35 ? ? ? ? 48 8B 01"), [](Memory::Handle Pointer) {
			pSwapChain = Pointer.Sub(13).Rip().As<decltype(pSwapChain)>();
		});

		MainBatch.Add(xorstr_("SPT"), xorstr_("48 8B 1D ? ? ? ? 41 83 F8 FF"), [](Memory::Handle Pointer) {
			pScriptProgramTable = Pointer.Add(3).Rip().As<decltype(pScriptProgramTable)>();
		});

		MainBatch.Add(xorstr_("SG"), xorstr_("48 85 FF 48 89 1D"), [](Memory::Handle Pointer) {
			pScriptGlobals = Pointer.Sub(12).Rip().As<decltype(pScriptGlobals)>();
		});

		MainBatch.Add(xorstr_("SVM"), xorstr_("E8 ? ? ? ? 48 85 FF 48 89 1D"), [](Memory::Handle Pointer) {
			pScriptVM = Pointer.Add(1).Rip().As<decltype(pScriptVM)>();
		});

		MainBatch.Add(xorstr_("INT"), xorstr_("8B CB E8 ? ? ? ? 8B 43 70 ? 03 C4 A9 00 C0 FF FF"), [](Memory::Handle Pointer) {
			pInitializeNativeTables = Pointer.Add(3).Rip().As<decltype(pInitializeNativeTables)>();
		});

		MainBatch.Add(xorstr_("REH"), xorstr_("66 41 83 F9 ? 0F 83"), [](Memory::Handle Pointer) {
			pReceivedEventHandler = Pointer.As<decltype(pReceivedEventHandler)>();
		});

		MainBatch.Add(xorstr_("SEA"), xorstr_("E8 ? ? ? ? 66 83 7B 08 5B"), [](Memory::Handle Pointer) {
			pSendEventAck = Pointer.Add(1).Rip().As<decltype(pSendEventAck)>();
		});

		MainBatch.Add(xorstr_("ATCP"), xorstr_("E8 ? ? ? ? 84 C0 74 8D"), [](Memory::Handle Pointer) {
			pAnimTaskCrashPatch = Pointer.Add(1).Rip().As<decltype(pAnimTaskCrashPatch)>();
		});

		MainBatch.Add(xorstr_("FCP"), xorstr_("E8 ? ? ? ? 44 8B 4D 1C"), [](Memory::Handle Pointer) {
			pFragmentCrashPatch = Pointer.Add(1).Rip().As<decltype(pFragmentCrashPatch)>();
		});

		MainBatch.Add(xorstr_("FCP2"), xorstr_("E8 ? ? ? ? 84 C0 75 0B 41 FF CF"), [](Memory::Handle Pointer) {
			pFragmentCrashPatch2 = Pointer.Add(1).Rip().As<decltype(pFragmentCrashPatch2)>();
		});

		MainBatch.Add(xorstr_("IDCP"), xorstr_("E8 ? ? ? ? 8B 9C 24 B8 00 00 00 4C 8B AC 24 A8 00 00 00"), [](Memory::Handle Pointer) {
			pInvalidDecalCrashPatch = Pointer.Add(1).Rip().As<decltype(pInvalidDecalCrashPatch)>();
		});

		MainBatch.Add(xorstr_("ITCP"), xorstr_("E8 ? ? ? ? F3 44 0F 10 93 90 03 00 00"), [](Memory::Handle Pointer) {
			pInfiniteTrainCrashPatch = Pointer.Add(1).Rip().As<decltype(pInfiniteTrainCrashPatch)>();
			pGetNextCarriage = Pointer.Add(1).Rip().Add(0xF).Rip().As<decltype(pGetNextCarriage)>();
		});

		MainBatch.Add(xorstr_("ITCP2"), xorstr_("48 89 5C 24 ? 57 48 83 EC 30 48 8B 02 48 8B FA"), [](Memory::Handle Pointer) {
			pInfiniteTrainCrashPatch2 = Pointer.As<decltype(pInfiniteTrainCrashPatch2)>();
		});

		MainBatch.Add(xorstr_("BDSMCP"), xorstr_("48 89 5C 24 ? 48 89 74 24 ? 48 89 7C 24 ? 48 8D 05"), [](Memory::Handle Pointer) { // Alternative sig: E8 ? ? ? ? 48 8B 47 10 4C 8B 8C 24 ? ? ? ? -> Add: 1 Relative
			pBDSMCrashPatch = Pointer.As<decltype(pBDSMCrashPatch)>();
		});

		MainBatch.Add(xorstr_("KCP"), xorstr_("E8 ? ? ? ? FE 8F 3B 02 00 00"), [](Memory::Handle Pointer) {
			pKanyeCrashPatch = Pointer.Add(1).Rip().As<decltype(pKanyeCrashPatch)>();
		});

		MainBatch.Add(xorstr_("RPI"), xorstr_("49 8B 80 ? ? ? ? 48 85 C0 74 0C F6 80 ? ? ? ? ? 75 03 32 C0 C3"), [](Memory::Handle Pointer) {
			pReceivePickup = Pointer.As<decltype(pReceivePickup)>();
		});

		MainBatch.Add(xorstr_("RCC"), xorstr_("48 8B C4 66 44 89 48"), [](Memory::Handle Pointer) {
			pReceivedCloneCreate = Pointer.As<decltype(pReceivedCloneCreate)>();
		});

		MainBatch.Add(xorstr_("RCS"), xorstr_("48 89 5C 24 08 48 89 6C 24 10 48 89 74 24 18 57 41 54 41 55 41 56 41 57 48 83 EC 40 4C 8B EA"), [](Memory::Handle Pointer) {
			pReceivedCloneSync = Pointer.As<decltype(pReceivedCloneSync)>();
			pGetSyncTreeForType = Pointer.Add(0x2F).Add(1).Rip().As<decltype(pGetSyncTreeForType)>();
			pGetNetObject = Pointer.Add(0x109).Add(1).Rip().As<decltype(pGetNetObject)>();
		});

		MainBatch.Add(xorstr_("GEAT"), xorstr_("48 83 EC 28 48 8B 51 50 48 85 D2 74 04"), [](Memory::Handle Pointer) {
			pGetEntityAttachedTo = Pointer.As<decltype(pGetEntityAttachedTo)>();
		});

		MainBatch.Add(xorstr_("CAD"), xorstr_("E8 ? ? ? ? 84 C0 0F 84 AF 01 00 00 48 8B 03"), [](Memory::Handle Pointer) {
			pCanApplyData = Pointer.Add(1).Rip().As<decltype(pCanApplyData)>();
		});

		MainBatch.Add(xorstr_("API"), xorstr_("48 89 5C 24 ? 48 89 6C 24 ? 48 89 74 24 ? 57 41 54 41 55 41 56 41 57 48 83 EC 20 41 8A E8"), [](Memory::Handle Pointer) {
			pAssignPhysicalIndex = Pointer.As<decltype(pAssignPhysicalIndex)>();
		});

		MainBatch.Add(xorstr_("NPMI"), xorstr_("41 56 48 83 EC ? 48 8B F1 B9 ? ? ? ? 49 8B F9 41 8B E8 4C 8B F2 E8"), [](Memory::Handle Pointer) {
			pNetPlayerMangerInit = Pointer.Sub(0x13).As<decltype(pNetPlayerMangerInit)>();
		});

		MainBatch.Add(xorstr_("NPMD"), xorstr_("48 8D 9F ? ? ? ? EB ? 48 8B 13 48 85 D2 74 ? 48 8B CB E8 ? ? ? ? 48 83 7B ? ? 75 ? 48 8D 9F"), [](Memory::Handle Pointer) {
			pNetPlayerMangerDest = Pointer.Sub(0x1A).As<decltype(pNetPlayerMangerDest)>();
		});

		MainBatch.Add(xorstr_("TJC"), xorstr_("48 89 5C 24 ? 89 54 24 10 57 48 83 EC 30 0F 29 74 24"), [](Memory::Handle Pointer) {
			pTaskJumpConstructor = Pointer.As<decltype(pTaskJumpConstructor)>();
		});

		MainBatch.Add(xorstr_("FTC"), xorstr_("E8 ? ? ? ? B3 04 08 98 A0 00 00 00"), [](Memory::Handle Pointer) {
			pFallTaskConstructor = Pointer.Add(1).Rip().As<decltype(pFallTaskConstructor)>();
		});

		MainBatch.Add(xorstr_("WPMDN"), xorstr_("48 89 5C 24 18 55 56 57 48 83 EC 30 48 8B 02 48 8D 99 C0 00 00 00"), [](Memory::Handle Pointer) {
			pWritePedMovementDataNode = Pointer.As<decltype(pWritePedMovementDataNode)>();
		});

		MainBatch.Add(xorstr_("WPODN"), xorstr_("4C 8B 81 50 FE FF FF 4D 85 C0 0F 84 97 00 00 00"), [](Memory::Handle Pointer) {
			pWritePedOrientationDataNode = Pointer.As<decltype(pWritePedOrientationDataNode)>();
		});

		MainBatch.Add(xorstr_("WPCDN"), xorstr_("48 8B C4 48 89 58 ? 48 89 68 ? 48 89 70 ? 48 89 78 ? 41 56 48 83 EC ? 48 8B B9 ? ? ? ? 4C 8D B1 ? ? ? ?"), [](Memory::Handle Pointer) {
			pWritePedCreationDataNode = Pointer.As<decltype(pWritePedCreationDataNode)>();
		});

		MainBatch.Add(xorstr_("SNE"), xorstr_("48 8B 5F 08 48 8B 7F 10 49 8B D6 48 8B 03 48 8B CB FF 90 ? ? ? ? 84 C0 0F 85"), [](Memory::Handle Pointer) {
			pSendNetworkEvent = Pointer.Sub(0x3A).As<decltype(pSendNetworkEvent)>();
		});

		MainBatch.Add(xorstr_("CD"), xorstr_("48 8B 05 ? ? ? ? 0F 45 DF"), [](Memory::Handle Pointer) {
			pChatData = Pointer.Add(3).Rip().As<decltype(pChatData)>();
		});

		MainBatch.Add(xorstr_("SCM"), xorstr_("48 83 EC 20 48 8B F1 48 8B CA 41 8A E9"), [](Memory::Handle Pointer) {
			pSendChatMessage = Pointer.Sub(21).As<decltype(pSendChatMessage)>();
		});

		MainBatch.Add(xorstr_("SCMP"), xorstr_("83 7E 1C 01 48 8B 3D"), [](Memory::Handle Pointer) {
			pSendChatMessagePointer = Pointer.Add(7).Rip().As<decltype(pSendChatMessagePointer)>();
		});

		MainBatch.Add(xorstr_("NRT"), xorstr_("76 32 48 8B 53 40 48 8D 0D"), [](Memory::Handle Pointer) {
			pNativeRegistrationTable = Pointer.Add(9).Rip().As<decltype(pNativeRegistrationTable)>();
		});

		MainBatch.Add(xorstr_("GNH"), xorstr_("48 8D 0D ? ? ? ? 48 8B 14 FA E8 ? ? ? ? 48 85 C0 75 0A"), [](Memory::Handle Pointer) {
			pGetNativeHandler = Pointer.Add(12).Rip().As<decltype(pGetNativeHandler)>();
		});

		MainBatch.Add(xorstr_("NRA"), xorstr_("FF E3"), [](Memory::Handle Pointer) {
			pNativeReturnAddress = Pointer.As<decltype(pNativeReturnAddress)>();
		});

		MainBatch.Add(xorstr_("FV"), xorstr_("83 79 18 ? 48 8B D1 74 4A FF 4A 18"), [](Memory::Handle Pointer) {
			pFixVectors = Pointer.As<decltype(pFixVectors)>();
		});

		MainBatch.Add(xorstr_("STA"), xorstr_("45 33 F6 8B E9 85 C9 B8"), [](Memory::Handle Pointer) {
			pScriptThreadArray = Pointer.Sub(4).Rip().Sub(8).As<decltype(pScriptThreadArray)>();
		});

		MainBatch.Add(xorstr_("PF"), xorstr_("48 8B 05 ? ? ? ? 48 8B 48 08 48 85 C9 74 52 8B 81"), [](Memory::Handle Pointer) {
			pPedFactory = Pointer.Add(3).Rip().As<decltype(pPedFactory)>();
		});

		MainBatch.Add(xorstr_("WP"), xorstr_("48 8B 05 ? ? ? ? 45 ? ? ? ? 48 8B 48 08 48 85 C9 74 07"), [](Memory::Handle Pointer) {
			pWorldPointer = (std::uintptr_t)ReadInstruction(Pointer.As<std::uint64_t>(), 0, 3, 7);
		});

		MainBatch.Add(xorstr_("NPM"), xorstr_("48 8B 0D ? ? ? ? 8A D3 48 8B 01 FF 50 ? 4C 8B 07 48 8B CF"), [](Memory::Handle Pointer) {
			pNetworkPlayerMgr = Pointer.Add(3).Rip().As<decltype(pNetworkPlayerMgr)>();
		});

		MainBatch.Add(xorstr_("NOM"), xorstr_("48 8B 0D ? ? ? ? 45 33 C0 E8 ? ? ? ? 33 FF 4C 8B F0"), [](Memory::Handle Pointer) {
			pNetworkObjectMgr = Pointer.Add(3).Rip().As<decltype(pNetworkObjectMgr)>();
		});

		MainBatch.Add(xorstr_("N"), xorstr_("48 8B 0D ? ? ? ? 48 8B D7 E8 ? ? ? ? 84 C0 75 17 48 8B 0D ? ? ? ? 48 8B D7"), [](Memory::Handle Pointer) {
			pNetwork = Pointer.Add(3).Rip().As<decltype(pNetwork)>();
		});

		MainBatch.Add(xorstr_("FR"), xorstr_("3B 0D ? ? ? ? 73 17"), [](Memory::Handle Pointer) {
			pFriendRegistry = Pointer.Add(2).Rip().As<decltype(pFriendRegistry)>();
		});

		MainBatch.Add(xorstr_("RI"), xorstr_("0F B7 44 24 ? 66 89 44 4E"), [](Memory::Handle Pointer) {
			pReplayInterface = Pointer.Add(0x1F).Rip().As<decltype(pReplayInterface)>();
		});

		MainBatch.Add(xorstr_("MSB"), xorstr_("74 48 E8 ? ? ? ? 48 8B 48 48"), [](Memory::Handle Pointer) {
			pModelSpawnBypass = Pointer.Add(3).Rip().Add(30).As<decltype(pModelSpawnBypass)>();
		});

		MainBatch.Add(xorstr_("AOB"), xorstr_("0F 85 ? ? ? ? 48 8B 05 ? ? ? ? 48 8B 48 08 E8"), [](Memory::Handle Pointer) {
			pAddOwnedExplosionBypass = Pointer.As<decltype(pAddOwnedExplosionBypass)>();
		});

		MainBatch.Add(xorstr_("MWL"), xorstr_("8B 43 6C 89 05"), [](Memory::Handle Pointer) {
			pMaxWantedLevel = Memory::BytePatch::Make(Pointer.Add(5).Rip().As<std::uint32_t*>(), 0).get();
			pMaxWantedLevel2 = Memory::BytePatch::Make(Pointer.Add(14).Rip().As<std::uint32_t*>(), 0).get();
		});

		MainBatch.Add(xorstr_("PTH"), xorstr_("48 8B F9 48 83 C1 10 33 DB"), [](Memory::Handle Pointer) {
			pPointerToHandle = Pointer.Sub(0x15).As<decltype(pPointerToHandle)>();
		});

		MainBatch.Add(xorstr_("HTP"), xorstr_("83 F9 FF 74 31 4C 8B 0D"), [](Memory::Handle Pointer) {
			pHandleToPointer = Pointer.As<decltype(pHandleToPointer)>();
		});

		MainBatch.Add(xorstr_("GNP"), xorstr_("48 83 EC 28 33 C0 38 05 ? ? ? ? 74 0A"), [](Memory::Handle Pointer) {
			pGetNetPlayer = Pointer.As<decltype(pGetNetPlayer)>();
		});

		MainBatch.Add(xorstr_("SND"), xorstr_("E8 ? ? ? ? E9 E9 01 00 00 48 8B CB"), [](Memory::Handle Pointer) {
			pSendNetworkDamage = Pointer.Add(1).Rip().As<decltype(pSendNetworkDamage)>();
		});

		MainBatch.Add(xorstr_("CPTN"), xorstr_("E8 ? ? ? ? EB 28 48 8B 8F A0 10 00 00"), [](Memory::Handle Pointer) {
			pClearPedTasksNetworked = Pointer.Add(1).Rip().As<decltype(pClearPedTasksNetworked)>();
		});

		MainBatch.Add(xorstr_("RR"), xorstr_("E8 ? ? ? ? 09 B3 ? ? ? ? 48 8B 5C 24"), [](Memory::Handle Pointer) {
			pRequestRagdoll = Pointer.Add(1).Rip().As<decltype(pRequestRagdoll)>();
		});

		MainBatch.Add(xorstr_("GCP"), xorstr_("8D 42 FF 83 F8 FD 77 3D"), [](Memory::Handle Pointer) {
			pGetConnectionPeer = Pointer.Add(23).Rip().As<decltype(pGetConnectionPeer)>();
		});

		MainBatch.Add(xorstr_("TSE"), xorstr_("45 8B F0 41 8B F9 48 8B EA"), [](Memory::Handle Pointer) {
			pTriggerScriptEvent = Pointer.Sub(0x1C).As<decltype(pTriggerScriptEvent)>();
		});

		MainBatch.Add(xorstr_("SRGC"), xorstr_("8D 42 FF 83 F8 FD 77 3D"), [](Memory::Handle Pointer) {
			pSendRemoveGamerCommand = Pointer.Add(65).Rip().As<decltype(pSendRemoveGamerCommand)>();
		});

		MainBatch.Add(xorstr_("HRGC"), xorstr_("48 85 D2 0F 84 0E 04"), [](Memory::Handle Pointer) {
			pHandleRemoveGamerCommand = Pointer.As<decltype(pHandleRemoveGamerCommand)>();
		});

		MainBatch.Add(xorstr_("GMI"), xorstr_("41 3B 0A 74 54"), [](Memory::Handle Pointer) { // Alternative sig: 0F B7 05 ? ? ? ? 45 33 C9 4C 8B DA 66 85 C0 0F 84 ? ? ? ? 44 0F B7 C0 33 D2 8B C1 41 F7 F0 48 8B 05 ? ? ? ? 4C 8B 14 D0 EB 09 41 3B 0A 74 54
			pGetModelInfo = Pointer.Sub(46).As<decltype(pGetModelInfo)>();
		});

		MainBatch.Add(xorstr_("GPR"), xorstr_("48 8B C8 33 C0 48 85 C9 74 0A 44 8B C3 8B D7 E8"), [](Memory::Handle Pointer) {
			pGivePickupRewards = Pointer.Sub(0x28).As<decltype(pGivePickupRewards)>();
		});

		MainBatch.Add(xorstr_("GEFS"), xorstr_("E8 ? ? ? ? 8D 53 01 33 DB"), [](Memory::Handle Pointer) {
			pGetEntityFromScript = Pointer.Add(1).Rip().As<decltype(pGetEntityFromScript)>();
		});

		MainBatch.Add(xorstr_("CNOO"), xorstr_("48 8B C4 48 89 58 08 48 89 68 10 48 89 70 18 48 89 78 20 41 54 41 56 41 57 48 81 EC ? ? ? ? 44 8A 62 4B"), [](Memory::Handle Pointer) {
			pChangeNetworkObjectOwner = Pointer.As<decltype(pChangeNetworkObjectOwner)>();
		});

		MainBatch.Add(xorstr_("NOMGI"), xorstr_("48 8B 0D ? ? ? ? 45 33 C0 E8 ? ? ? ? 33 FF 4C 8B F0"), [](Memory::Handle Pointer) {
			pNetworkObjectMgrInterface = Pointer.Add(3).Rip().As<decltype(pNetworkObjectMgrInterface)>();
		});

		MainBatch.Add(xorstr_("MT"), xorstr_("4C 03 05 ? ? ? ? EB 03"), [](Memory::Handle Pointer) {
			pModelTable = Pointer.Add(3).Rip().As<decltype(pModelTable)>();
		});

		MainBatch.Add(xorstr_("LP"), xorstr_("4C 03 05 ? ? ? ? EB 09 49 83 C2 04 EB 8B"), [](Memory::Handle Pointer) {
			pListPool = Pointer.Add(3).Rip().As<decltype(pListPool)>();
		});

		MainBatch.Add(xorstr_("OQ"), xorstr_("74 41 4C 8B 05 ? ? ?"), [](Memory::Handle Pointer) {
			pWaterQuads.m_oceanQuads = (QuadInfo*)ReadInstruction(Pointer.As<std::uint64_t>(), 2);
		});

		MainBatch.Add(xorstr_("DST"), xorstr_("48 8D 3D ? ? ? ? 80 3B 00 76 12 48 8B 0F 48 85 C9 74 0A 48 8B 01 FF 50 10 84 C0 75"), [](Memory::Handle Pointer) {
			pDispatchServiceTable = Pointer.Add(3).Rip().As<decltype(pDispatchServiceTable)>();		
		});

		MainBatch.Add(xorstr_("VFX-WHL"), xorstr_("48 8D 05 ? ? ? ? 48 6B FF 45 F3 0F 59 0D ? ? ? ? F3 41 0F 59 9E ? ? ? ? F3 0F 10 BD ? ? ? ? 48 03 FE 48 69 FF ? ? ? ? F3"), [](Memory::Handle Pointer) {
			pVFXWheel.first = Pointer.Add(3).Rip().As<decltype(pVFXWheel.first)>();
			pVFXWheel.second = Pointer.Add(0xA).As<decltype(*(std::uint8_t*)pVFXWheel.second)>();
		});

		MainBatch.Add(xorstr_("VFX-UIW"), xorstr_("48 8D 0D ? ? ? ? 44 8A CE 44 8A C5 8B D3 44 89 64 24 ? 89 7C 24 20 E8 ? ? ? ? 8D"), [](Memory::Handle Pointer) {
			// Base Class -> Add: 3, Relative, Add: 0x60
			pEastAzimuthColor = Pointer.Add(3).Rip().Add(0x60).Add(32).As<decltype(pEastAzimuthColor)>();
			pWestAzimuthColor = Pointer.Add(3).Rip().Add(0x60).Add(80).As<decltype(pWestAzimuthColor)>();
			pAzimuthTransitionColor = Pointer.Add(3).Rip().Add(0x60).Add(128).As<decltype(pAzimuthTransitionColor)>();
			pZenithColor = Pointer.Add(3).Rip().Add(0x60).Add(176).As<decltype(pZenithColor)>();
			pZenithTransitionColor = Pointer.Add(3).Rip().Add(0x60).Add(224).As<decltype(pZenithTransitionColor)>();
			pCloudMidColor = Pointer.Add(3).Rip().Add(0x60).Add(896).As<decltype(pCloudMidColor)>();
			pCloudBaseColor = Pointer.Add(3).Rip().Add(0x60).Add(864).As<decltype(pCloudBaseColor)>();
			pSunColor = Pointer.Add(3).Rip().Add(0x60).Add(448).As<decltype(pSunColor)>();
			pSunHDRColor = Pointer.Add(3).Rip().Add(0x60).Add(496).As<decltype(pSunHDRColor)>();
			pSunDiscHDRColor = Pointer.Add(3).Rip().Add(0x60).Add(672).As<decltype(pSunDiscHDRColor)>();
		});

		MainBatch.Add(xorstr_("VFX-WEA"), xorstr_("4C 8D 35 ? ? ? ? 0F 2F C6"), [](Memory::Handle Pointer) {
			pWeatherVfx = Pointer.Add(3).Rip().As<decltype(pWeatherVfx)>();
		});

		MainBatch.Add(xorstr_("VFX-VIS"), xorstr_("48 8D 0D ? ? ? ? 0F 29 44 24 ? E8 ? ? ? ? 0F 28 05 ? ? ? ? 0F 59 44 24 ? F3 48 0F 2C C0 0F C6 C0 55"), [](Memory::Handle Pointer) {
			pVisualVFX = Pointer.Add(3).Rip().As<decltype(pVisualVFX)>();
		});

		MainBatch.Add(xorstr_("VFX-LIQ"), xorstr_("48 8D 05 ? ? ? ? 4C 8D 1C 7F 48 8B 4A 10 49 C1 E3 05 4C 03 D8 0F B6 85 ? ? ? ? 48 23 C8 8B 42 18 48 3B"), [](Memory::Handle Pointer) {
			pLiquidVFX = (CLiquidVfx*)ReadInstruction(Pointer.As<std::uint64_t>());
		});

		MainBatch.Add(xorstr_("VFX-CGM"), xorstr_("0F 2F 35 ? ? ? ? 0F 86 ? ? ? ? 0F 28 05 ? ? ? ? 48 8D 4D B0 41 0F 28 D9 48 8B D3 0F 28 D7 0F 29 45 A0"), [](Memory::Handle Pointer) {
			pFOGOnMap = Pointer.Add(3).Rip().As<decltype(pFOGOnMap)>();
			pDistanceToSun = Pointer.Add(3).Rip().Add(4).As<decltype(pDistanceToSun)>();
			pAngleFOV = Pointer.Add(3).Rip().Add(8).As<decltype(pAngleFOV)>();
		});

		MainBatch.Add(xorstr_("VFX-WOM"), xorstr_("F3 0F 10 05 ? ? ? ? 0F 57 C9 0F 2F C1 76 1B 48 8B 05 ? ? ? ? 0F 2F 48 70 73 0E 48 8D 0D ? ? ? ? 33"), [](Memory::Handle Pointer) {
			pWaterOnMap = ReadInstruction(Pointer.As<std::uint64_t>(), 0, 4, 8);
		});

		MainBatch.Add(xorstr_("CFOV"), xorstr_("48 8B 05 ? ? ? ? 48 8B 98 ? ? ? ? EB"), [](Memory::Handle Pointer) {
			pCameraFOV = Pointer.Add(3).Rip().As<decltype(pCameraFOV)>();
		});

		MainBatch.Add(xorstr_("WIHL"), xorstr_("44 0F B7 05 ? ? ? ? 44 8B DD 41 FF C8 78 28 48 8B 1D"), [](Memory::Handle Pointer) {
			pWeaponInfoHashList = Pointer.Add(0x10).Add(3).Rip().As<decltype(pWeaponInfoHashList)>();
			pWeaponInfoHashListCount = Pointer.Add(4).Rip().As<decltype(pWeaponInfoHashListCount)>();
		});

		MainBatch.Add(xorstr_("SVG"), xorstr_("F3 0F 59 15 ? ? ? ? F3 41 0F 58 D2 F3 0F 59 D0"), [](Memory::Handle Pointer) {
			pSuperVehicleGrip = Pointer.As<decltype(pSuperVehicleGrip)>();
		});

		MainBatch.Add(xorstr_("VFX-EAP"), xorstr_("0F 29 62 20"), [](Memory::Handle Pointer) {
			pEastAzimuthPatch = Pointer.As<decltype(pEastAzimuthPatch)>();
		});

		MainBatch.Add(xorstr_("VFX-WAP"), xorstr_("0F 29 62 50 8B 81 9C 03 00 00"), [](Memory::Handle Pointer) {
			pWestAzimuthPatch = Pointer.As<decltype(pWestAzimuthPatch)>();
		});

		MainBatch.Add(xorstr_("VFX-ATP"), xorstr_("0F 29 A2 80 00 00 00"), [](Memory::Handle Pointer) {
			pAzimuthTransitionPatch = Pointer.As<decltype(pAzimuthTransitionPatch)>();
		});

		MainBatch.Add(xorstr_("VFX-ZP"), xorstr_("0F 29 A2 B0 00 00 00 8B 81 58 03 00 00"), [](Memory::Handle Pointer) {
			pZenithPatch = Pointer.As<decltype(pZenithPatch)>();
		});

		MainBatch.Add(xorstr_("VFX-ZTP"), xorstr_("0F 29 A2 E0 00 00 00"), [](Memory::Handle Pointer) {
			pZenithTransitionPatch = Pointer.As<decltype(pZenithTransitionPatch)>();
		});

		MainBatch.Add(xorstr_("VFX-CMP"), xorstr_("0F 29 8B 60 03 00 00"), [](Memory::Handle Pointer) {
			pCloudMidPatch = Pointer.As<decltype(pCloudMidPatch)>();
		});

		MainBatch.Add(xorstr_("VFX-CBP"), xorstr_("0F 29 A2 80 03 00 00"), [](Memory::Handle Pointer) {
			pCloudBasePatch = Pointer.As<decltype(pCloudBasePatch)>();
		});

		MainBatch.Add(xorstr_("RBBD"), xorstr_("48 89 74 24 ? 57 48 83 EC 20 48 8B D9 33 C9 41 8B F0 8A"), [](Memory::Handle Pointer) {
			pReadBitBufDWORD = Pointer.Sub(5).As<decltype(pReadBitBufDWORD)>();
		});

		MainBatch.Add(xorstr_("RBBS"), xorstr_("48 89 5C 24 08 48 89 6C 24 18 56 57 41 56 48 83 EC 20 48 8B F2 45"), [](Memory::Handle Pointer) {
			pReadBitBufString = Pointer.As<decltype(pReadBitBufString)>();
		});

		MainBatch.Add(xorstr_("RBBB"), xorstr_("E8 ? ? ? ? 84 C0 74 41 48 8D 56 2C"), [](Memory::Handle Pointer) {
			pReadBitBufBool = Pointer.Add(1).Rip().As<decltype(pReadBitBufBool)>();
		});

		MainBatch.Add(xorstr_("RBBA"), xorstr_("48 89 5C 24 ? 57 48 83 EC 30 41 8B F8 4C"), [](Memory::Handle Pointer) {
			pReadBitBufArray = Pointer.As<decltype(pReadBitBufArray)>();
		});

		MainBatch.Add(xorstr_("WBBQ"), xorstr_("48 89 5C 24 08 48 89 6C 24 10 48 89 74 24 18 57 48 83 EC 20 41 8B F0 48 8B EA 48 8B D9 41 83 F8 20"), [](Memory::Handle Pointer) {
			pWriteBitBufQWORD = Pointer.As<decltype(pWriteBitBufQWORD)>();
		});

		MainBatch.Add(xorstr_("WBBD"), xorstr_("48 8B C4 48 89 58 08 48 89 68 10 48 89 70 18 48 89 78 20 41 56 48 83 EC 20 8B EA BF 01"), [](Memory::Handle Pointer) {
			pWriteBitBufDWORD = Pointer.As<decltype(pWriteBitBufDWORD)>();
		});

		MainBatch.Add(xorstr_("WBBI64"), xorstr_("E8 ? ? ? ? 8A 53 39 48 8B CF"), [](Memory::Handle Pointer) {
			pWriteBitBufInt64 = Pointer.Add(1).Rip().As<decltype(pWriteBitBufInt64)>();
		});

		MainBatch.Add(xorstr_("WBBI32"), xorstr_("E8 ? ? ? ? 8A 53 74"), [](Memory::Handle Pointer) {
			pWriteBitBufInt32 = Pointer.Add(1).Rip().As<decltype(pWriteBitBufInt32)>();
		});

		MainBatch.Add(xorstr_("WBBB"), xorstr_("E8 ? ? ? ? 8A 57 39"), [](Memory::Handle Pointer) {
			pWriteBitBufBool = Pointer.Add(1).Rip().As<decltype(pWriteBitBufBool)>();
		});

		MainBatch.Add(xorstr_("WBBA"), xorstr_("E8 ? ? ? ? 01 7E 08"), [](Memory::Handle Pointer) {
			pWriteBitBufArray = Pointer.Add(1).Rip().As<decltype(pWriteBitBufArray)>();
		});

		MainBatch.Add(xorstr_("NTQVMC"), xorstr_("66 0F 6F 0D ? ? ? ? 66 0F 6F 05 ? ? ? ? 66 0F 66 C4"), [](Memory::Handle Pointer) {
			Memory::BytePatch::Make(Pointer.Add(4).Rip().Sub(32).As<std::uint64_t*>(), (std::uint64_t)&GTA::NTQueryVirtualMemory)->Apply();
		});

		if (auto Patch1 = Region.Scan(xorstr_("3B 0A 0F 83 ? ? ? ? 48 FF C7")))
		{
			Memory::BytePatch::Make(Patch1.Add(2).As<std::uint32_t*>(), 0xC9310272)->Apply();
			Memory::BytePatch::Make(Patch1.Add(6).As<std::uint16_t*>(), 0x9090)->Apply();
		}

		if (auto Patch2 = Region.Scan(xorstr_("3B 0A 0F 83 ? ? ? ? 49 03 FA")))
		{
			Memory::BytePatch::Make(Patch2.Add(2).As<std::uint32_t*>(), 0xC9310272)->Apply();
			Memory::BytePatch::Make(Patch2.Add(6).As<std::uint16_t*>(), 0x9090)->Apply();
		}

		auto Patch3 = Region.ScanAll(xorstr_("3B 11 0F 83 ? ? ? ? 48 FF C7"));
		for (auto& Handle : Patch3)
		{
			Memory::BytePatch::Make(Handle.Add(2).As<std::uint32_t*>(), 0xD2310272)->Apply();
			Memory::BytePatch::Make(Handle.Add(6).As<std::uint16_t*>(), 0x9090)->Apply();
		}

		auto Patch4 = Region.ScanAll(xorstr_("3B 11 0F 83 ? ? ? ? 49 03 FA"));
		for (auto& Handle : Patch4)
		{
			Memory::BytePatch::Make(Handle.Add(2).As<std::uint32_t*>(), 0xD2310272)->Apply();
			Memory::BytePatch::Make(Handle.Add(6).As<std::uint16_t*>(), 0x9090)->Apply();
		}

		LOG(INFO) << xorstr_("[Pointers] Created freemode thread restorer");

		if (!MainBatch.Run(Region))
			throw std::runtime_error(xorstr_("[Memory] GTA5 out of bounds"));

		Memory::Batch SCBatch;

		SCBatch.Add(xorstr_("PD"), xorstr_("48 8D 05 ? ? ? ? 48 8B F1 48 89 01 48 83 C1 08 E8 ? ? ? ? 33 ED"), [](Memory::Handle Pointer) {
			const auto PresenceData = Pointer.Add(3).Rip().As<PVOID*>();
			pUpdatePresenceAttributeInt = PresenceData[1];
			pUpdatePresenceAttributeString = PresenceData[3];
		});

		auto SCModule = Memory::Module(xorstr_("socialclub.dll"));
		if (SCModule.WaitForModule())
		{
			SCBatch.Run(SCModule);
		}
		else
			LOG(FATAL) << xorstr_("[Memory] SC out of bounds");


		pGameWindow = FindWindowA(xorstr_("grcWindow"), xorstr_("Grand Theft Auto V"));

		if (!pGameWindow)
			throw std::runtime_error(xorstr_("[Main] Couldn't find GTA5"));
	}

	void Pointers::Destroy()
	{
		Memory::BytePatch::RestoreAll();
		LOG(INFO) << xorstr_("[Pointers] Restored all Byte Patches");

		if (gResetVFX) 
		{
			std::copy_n("\x0F\x29\x62\x20", 4, pEastAzimuthPatch);
			std::copy_n("\x0F\x29\x62\x50", 4, pWestAzimuthPatch);
			std::copy_n("\x0F\x29\xA2\x80", 7, pAzimuthTransitionPatch);
			std::copy_n("\x0F\x29\xA2\xB0", 7, pZenithPatch);
			std::copy_n("\x0F\x29\xA2\xE0", 7, pZenithTransitionPatch);
			std::copy_n("\x0F\x29\x8B\x60\x03", 7, pCloudMidPatch);
			std::copy_n("\x0F\x29\xA2\x80\x03", 7, pCloudBasePatch);
			//std::copy_n(pOriginalMoonColorPatch, 7, pMoonColorPatch);
			LOG(INFO) << xorstr_("[Pointers] Restored all VFX Patches");
		}
	}
}