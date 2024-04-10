#pragma once
#include "../Common.hpp"
#include "../Hooking/Hooking.hpp"
#include "../GTA/Include.hpp"
#include "../GTA/enums.hpp"
#include "../GTA/datBitBuffer.hpp"

namespace Sentinel
{
    namespace GTA
    {
        extern int NTQueryVirtualMemory(void* _this, HANDLE handle, PVOID base, int iclass, MEMORY_BASIC_INFORMATION* info, int size, std::size_t* length);
        extern rage::eThreadState ScriptVM(std::uint64_t* start_stack, std::uint64_t** scr_globals, rage::scrProgram* program, rage::scrThreadContext* ctx);
        extern bool InitializeNativeTables(rage::scrProgram* program);
        extern void ReceivedEventHandler(rage::netEventMgr* event_manager, CNetGamePlayer* source_player, CNetGamePlayer* target_player, std::uint16_t event_id, int event_index, int event_handled_bitset, int unk, rage::datBitBuffer* bit_buffer);
        extern bool AnimTaskCrashPatch(std::int64_t a1, std::int64_t a2, std::int64_t a3, std::int64_t a4, std::int64_t a5);
        extern bool FragmentCrashPatch(std::uintptr_t a1, std::uint32_t a2, std::uintptr_t a3, std::uintptr_t a4, std::uintptr_t a5);
        extern bool FragmentCrashPatch2(float* a1, float* a2);
        extern std::uint64_t InvalidDecalCrashPatch(std::uintptr_t a1, int a2);
        extern void* InfiniteTrainCrashPatch(std::uint64_t* carriage);
        extern void BDSMCrashPatch(__int64 a1, __int64 a2, unsigned int a3, int a4, __int64 a5);
        extern std::uint64_t KanyeCrashPatch(std::uintptr_t a1, bool a2);
        extern bool ReceivePickup(rage::netObject* object, void* unk, CPed* ped);
        extern bool UpdatePresenceAttributeInt(void* presence_data, int profile_index, char* attr, std::uint64_t value);
        extern bool UpdatePresenceAttributeString(void* presence_data, int profile_index, char* attr, char* value);
        extern bool ReceivedCloneCreate(CNetworkObjectMgr* mgr, CNetGamePlayer* src, CNetGamePlayer* dst, eNetObjType object_type, std::int32_t object_id, std::int32_t object_flag, rage::datBitBuffer* buffer, std::int32_t timestamp);
        extern eAckCode ReceivedCloneSync(CNetworkObjectMgr* mgr, CNetGamePlayer* src, CNetGamePlayer* dst, eNetObjType object_type, std::uint16_t object_id, rage::datBitBuffer* buffer, std::uint16_t unk, std::uint32_t timestamp);
        extern bool CanApplyData(rage::netSyncTree* tree, rage::netObject* object);
        extern void* AssignPhysicalIndex(CNetworkPlayerMgr* netPlayerMgr, CNetGamePlayer* player, std::uint8_t idx);
        extern void NetworkPlayerManagerInitialize(CNetworkPlayerMgr* _this, std::uint64_t a2, std::uint32_t a3, std::uint32_t a4[4]);
        extern void NetworkPlayerManagerDestroy(CNetworkPlayerMgr* _this);
        extern std::uint64_t TaskJumpConstructor(std::uint64_t a1, int a2);
        extern std::uint64_t* FallTaskConstructor(std::uint64_t* _this, std::uint32_t flags);
        extern std::uint64_t WritePedMovementDataNode(CPedMovementDataNode* node, rage::netObject* object);
        extern std::uint64_t WritePedOrientationDataNode(std::uint64_t net_obj, CPedOrientationDataNode* node);
        extern  __int64 WritePedCreationDataNode(std::int64_t ptr, CPedCreationDataNode* node);
        extern void SendNetworkEvent(std::uint64_t net_table, std::uint64_t event);
        extern bool SendChatMessage(void* team_mgr, rage::rlGamerInfo* local_gamer_info, char* message, bool team);
    }

    namespace Native
    {
        extern void Wait(rage::scrNativeCallContext* src);
        extern void SetCurrentPedWeapon(rage::scrNativeCallContext* src);
        extern void DisableControlAction(rage::scrNativeCallContext* src);
        extern void HudForceWeaponWheel(rage::scrNativeCallContext* src);
        extern void SetWarningMessageWithHeader(rage::scrNativeCallContext* src);
    }

    namespace SwapChain
    {
        constexpr auto VMTPresentIdx = 8;
        constexpr auto VMTResizeBuffersIdx = 13;
        extern HRESULT Present(IDXGISwapChain* that, UINT syncInterval, UINT flags);
        extern HRESULT ResizeBuffers(IDXGISwapChain* that, UINT bufferCount, UINT width, UINT height, DXGI_FORMAT newFormat, UINT swapChainFlags);
    }

    namespace Window
    {
        extern LRESULT WndProc(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam);
    }
}