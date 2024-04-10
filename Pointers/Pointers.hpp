#pragma once 
#include "../Common.hpp"
#include "../GTA/Include.hpp"
#include "../Memory/BytePatch.hpp"

class CEntity;
class FriendRegistry;
class CNetworkObjectMgr;
namespace rage
{
	class netEventMgr;
	class netConnectionPeer;
	class CDynamicEntity;
}

namespace Sentinel::FunctionTypes
{
	using tScriptVM = rage::eThreadState(std::uint64_t* start_stack, std::uint64_t** scr_globals, rage::scrProgram* program, rage::scrThreadContext* ctx);
	using tSendEventAck = void(rage::netEventMgr* event_manager, CNetGamePlayer* source_player, CNetGamePlayer* target_player, int event_index, int event_handled_bitset);
	using tGetNextCarriage = void*(void* carriage);
	using tGetSyncTreeForType = rage::netSyncTree*(CNetworkObjectMgr* mgr, std::uint16_t sync_type);
	using tGetEntityAttachedTo = rage::CDynamicEntity*(rage::CDynamicEntity* entity);
	using tSendChatMessage = bool(std::int64_t* chat_ptr, rage::rlGamerInfo* gamer_info, char* message, bool team);
	using tGetNativeHandler = rage::scrNativeHandler(rage::scrNativeRegistrationTable*, rage::scrNativeHash);
	using tFixVectors = void(rage::scrNativeCallContext*);
	using tPointerToHandle = std::int32_t(void*);
	using tHandleToPointer = rage::CDynamicEntity*(std::uint32_t ent);
	using tGetNetPlayer = CNetGamePlayer*(std::int32_t id);
	using tSendNetworkDamage = void(CEntity* source, CEntity* target, rage::fvector3* position, int hit_component, bool override_default_damage, int weapon_type, float override_damage, int tire_index, int suspension_index, int flags,
		std::uint32_t action_result_hash, std::int16_t action_result_id, int action_unk, bool hit_weapon, bool hit_weapon_ammo_attachment, bool silenced, bool unk, rage::fvector3* impact_direction);
	using tClearPedTasksNetworked = void(CPed* ped, bool immediately);
	using tRequestRagdoll = void(std::uint16_t object_id);
	using tGetConnectionPeer = rage::netConnectionPeer*(rage::netConnectionManager* manager, int peer_id);
	using tTriggerScriptEvent = void(int event_group, int64_t* args, int arg_count, int player_bits);
	using tSendRemoveGamerCommand = void(rage::netConnectionManager* net_connection_mgr, rage::netConnectionPeer* player, int connection_id, rage::snMsgRemoveGamersFromSessionCmd* cmd, int flags);
	using tHandleRemoveGamerCommand = void*(rage::snSession* session, rage::snPlayer* origin, rage::snMsgRemoveGamersFromSessionCmd* cmd);
	using tGetModelInfo = std::uint64_t(std::uint32_t hash, std::int32_t* index);	
	using tGivePickupRewards = void(int players, std::uint32_t hash);
	using tGetNetObject = rage::netObject*(CNetworkObjectMgr* mgr, std::int16_t id, bool can_delete_be_pending);
	using tGetEntityFromScript = std::uint64_t(std::int32_t script_index);
	using tChangeNetworkObjectOwner = std::uint64_t(std::uint64_t obj_mgr, std::uint64_t net_obj, CNetGamePlayer* player, std::uint64_t unk);
	using tReadBitBufDWORD = bool(rage::datBitBuffer* buffer, PVOID read, int bits);
	using tReadBitBufString = bool(rage::datBitBuffer* buffer, char* read, int bits);
	using tReadBitBufBool = bool(rage::datBitBuffer* buffer, bool* read, int bits);
	using tReadBitBufArray = bool(rage::datBitBuffer* buffer, PVOID read, int bits, int unk);
	using tWriteBitBufQWORD = bool(rage::datBitBuffer* buffer, std::uint64_t val, int bits);
	using tWriteBitBufDWORD = bool(rage::datBitBuffer* buffer, std::uint32_t val, int bits);
	using tWriteBitBufInt64 = bool(rage::datBitBuffer* buffer, std::int64_t val, int bits);
	using tWriteBitBufInt32 = bool(rage::datBitBuffer* buffer, std::int32_t val, int bits);
	using tWriteBitBufBool = bool(rage::datBitBuffer* buffer, bool val, int bits);
	using tWriteBitBufArray = bool(rage::datBitBuffer* buffer, void* val, int bits, int unk);
}

namespace Sentinel::Pointers
{
	extern void Initialize();
	extern void Destroy();

	inline HWND pGameWindow;
	inline eGameState* pGameState;
	inline bool* pIsSessionStarted;
	inline std::uint32_t* pFrameCount;
	inline IDXGISwapChain** pSwapChain;
	inline ::rage::scrProgramTable* pScriptProgramTable;
	inline std::int64_t** pScriptGlobals;
	inline FunctionTypes::tScriptVM* pScriptVM;
	inline PVOID pInitializeNativeTables;
	inline PVOID pReceivedEventHandler;
	inline FunctionTypes::tSendEventAck* pSendEventAck;
	inline PVOID pAnimTaskCrashPatch;
	inline PVOID pFragmentCrashPatch;
	inline PVOID pFragmentCrashPatch2;
	inline PVOID pInvalidDecalCrashPatch;
	inline PVOID pInfiniteTrainCrashPatch;
	inline PVOID pInfiniteTrainCrashPatch2;
	inline FunctionTypes::tGetNextCarriage* pGetNextCarriage;
	inline PVOID pBDSMCrashPatch;
	inline PVOID pKanyeCrashPatch;
	inline PVOID pReceivePickup;
	inline PVOID pUpdatePresenceAttributeInt;
	inline PVOID pUpdatePresenceAttributeString;
	inline PVOID pReceivedCloneCreate;
	inline PVOID pReceivedCloneSync;
	inline FunctionTypes::tGetSyncTreeForType* pGetSyncTreeForType;
	inline FunctionTypes::tGetEntityAttachedTo* pGetEntityAttachedTo;
	inline PVOID pCanApplyData;
	inline PVOID pBountyPresenceEvent;
	inline PVOID pAssignPhysicalIndex;
	inline PVOID pNetPlayerMangerInit;
	inline PVOID pNetPlayerMangerDest;
	inline PVOID pTaskJumpConstructor;
	inline PVOID pFallTaskConstructor;
	inline PVOID pWritePedMovementDataNode;
	inline PVOID pWritePedOrientationDataNode;
	inline PVOID pWritePedCreationDataNode;
	inline PVOID pSendNetworkEvent;
	inline ChatData** pChatData;
	inline FunctionTypes::tSendChatMessage* pSendChatMessage;
	inline std::int64_t** pSendChatMessagePointer;
	inline rage::scrNativeRegistrationTable* pNativeRegistrationTable;
	inline FunctionTypes::tGetNativeHandler* pGetNativeHandler;
	inline void* pNativeReturnAddress;
	inline FunctionTypes::tFixVectors* pFixVectors;
	inline rage::atArray<GtaThread*>* pScriptThreadArray;
	inline CPedFactory** pPedFactory;
	inline std::uintptr_t pWorldPointer;
	inline CNetworkPlayerMgr** pNetworkPlayerMgr;
	inline CNetworkObjectMgr** pNetworkObjectMgr;
	inline Network** pNetwork;
	inline FriendRegistry* pFriendRegistry;
	inline rage::CReplayInterface** pReplayInterface;
	inline PVOID pModelSpawnBypass;
	inline std::uint16_t* pAddOwnedExplosionBypass;
	inline Memory::BytePatch* pMaxWantedLevel;
	inline Memory::BytePatch* pMaxWantedLevel2;
	inline FunctionTypes::tPointerToHandle* pPointerToHandle;
	inline FunctionTypes::tHandleToPointer* pHandleToPointer;
	inline FunctionTypes::tGetNetPlayer* pGetNetPlayer;
	inline FunctionTypes::tSendNetworkDamage* pSendNetworkDamage;
	inline FunctionTypes::tClearPedTasksNetworked* pClearPedTasksNetworked;
	inline FunctionTypes::tRequestRagdoll* pRequestRagdoll;
	inline FunctionTypes::tGetConnectionPeer* pGetConnectionPeer;
	inline FunctionTypes::tTriggerScriptEvent* pTriggerScriptEvent;
	inline FunctionTypes::tSendRemoveGamerCommand* pSendRemoveGamerCommand;
	inline FunctionTypes::tHandleRemoveGamerCommand* pHandleRemoveGamerCommand;
	inline FunctionTypes::tGetModelInfo* pGetModelInfo;
	inline FunctionTypes::tGivePickupRewards* pGivePickupRewards;
	inline FunctionTypes::tGetNetObject* pGetNetObject;
	inline FunctionTypes::tGetEntityFromScript* pGetEntityFromScript;
	inline FunctionTypes::tChangeNetworkObjectOwner* pChangeNetworkObjectOwner;
	inline std::uintptr_t pNetworkObjectMgrInterface;
	inline HashTable<CBaseModelInfo*>* pModelTable;
	inline ListPool* pListPool;
	inline WaterQuads pWaterQuads;
	inline std::uint64_t pDispatchServiceTable;
	inline std::pair<CWheelVfx*, std::uint8_t> pVFXWheel;
	inline CWeatherVfx* pWeatherVfx;
	inline CVisualVfx* pVisualVFX;
	inline CLiquidVfx* pLiquidVFX;
	inline std::uint64_t pFOGOnMap;
	inline std::uint64_t pDistanceToSun;
	inline std::uint64_t pAngleFOV;
	inline std::uint64_t pWaterOnMap;
	inline std::uint64_t pCameraFOV;
	inline std::uint64_t pWeaponInfoHashList;
	inline std::uint64_t pWeaponInfoHashListCount;
	inline std::uintptr_t pSuperVehicleGrip;
	inline char* pEastAzimuthPatch;
	inline std::uintptr_t pEastAzimuthColor;
	inline char* pWestAzimuthPatch;
	inline std::uintptr_t pWestAzimuthColor;
	inline char* pAzimuthTransitionPatch;
	inline std::uintptr_t pAzimuthTransitionColor;
	inline char* pZenithPatch;
	inline std::uintptr_t pZenithColor;
	inline char* pZenithTransitionPatch;
	inline std::uintptr_t pZenithTransitionColor;
	inline char* pCloudMidPatch;
	inline std::uintptr_t pCloudMidColor;
	inline char* pCloudBasePatch;
	inline std::uintptr_t pCloudBaseColor;
	inline std::uintptr_t pSunColor;
	inline std::uintptr_t pSunHDRColor;
	inline std::uintptr_t pSunDiscHDRColor;
	inline FunctionTypes::tReadBitBufDWORD* pReadBitBufDWORD;
	inline FunctionTypes::tReadBitBufString* pReadBitBufString;
	inline FunctionTypes::tReadBitBufBool* pReadBitBufBool;
	inline FunctionTypes::tReadBitBufArray* pReadBitBufArray;
	inline FunctionTypes::tWriteBitBufQWORD* pWriteBitBufQWORD;
	inline FunctionTypes::tWriteBitBufDWORD* pWriteBitBufDWORD;
	inline FunctionTypes::tWriteBitBufInt64* pWriteBitBufInt64;
	inline FunctionTypes::tWriteBitBufInt32* pWriteBitBufInt32;
	inline FunctionTypes::tWriteBitBufBool* pWriteBitBufBool;
	inline FunctionTypes::tWriteBitBufArray* pWriteBitBufArray;

	inline std::uint64_t ReadInstruction(std::uint64_t address, int offset = 0, int opcode_size = 3, int opcode_length = 7) {
		std::uint64_t _address = (address + offset);
		return (std::uint64_t)(*(int*)(_address + opcode_size) + _address + opcode_length);
	}

	inline void PrintRVA(std::uint64_t address) {
		LOG(INFO) << "Base: " << HEX_TO_UPPER(reinterpret_cast<std::uint64_t>(GetModuleHandleA(NULL)));
		LOG(INFO) << "RV: " << HEX_TO_UPPER(address);
	}

	inline std::uintptr_t Add(std::uintptr_t address, std::uint64_t offset)
	{
		return (address + offset);
	}

	inline DWORD64 GetCamera() {
		if (pCameraFOV)
			return *(DWORD64*)(pCameraFOV + 0x0);
	}
}
