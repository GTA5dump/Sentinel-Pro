#include "../HookTable.hpp"
#include "../../GTA/GTA.hpp"
#include "../../Features/ProtectionScript.hpp"
#include "../../GUI/Overlays/Overlays.hpp"

#define RAGE_JOAAT rage::joaat
namespace Sentinel
{
	bool GTA::ReceivedCloneCreate(CNetworkObjectMgr* mgr, CNetGamePlayer* src, CNetGamePlayer* dst, eNetObjType object_type, std::int32_t object_id, std::int32_t object_flag, rage::datBitBuffer* buffer, std::int32_t timestamp)
	{
		if (g_ProtectionScript->m_BlockMaliciousSyncs && src != nullptr) {
			if (src->is_valid()) {
				if (object_type < eNetObjType::NET_OBJ_TYPE_AUTOMOBILE || object_type > eNetObjType::NET_OBJ_TYPE_TRAIN)
				{
					if (g_ProtectionScript->m_MiscellaneousNotify)
						Overlays::ProtectionMessage(src->get_name(), "Sync Crash");
					return true;
				}
			}
		}
		g_ProtectionScript->m_SyncingPlayer = src;
		return Hooking::GetHook<DetourHook>("RCC")->Original<decltype(&ReceivedCloneCreate)>()(mgr, src, dst, object_type, object_id, object_flag, buffer, timestamp);
	}

	eAckCode GTA::ReceivedCloneSync(CNetworkObjectMgr* mgr, CNetGamePlayer* src, CNetGamePlayer* dst, eNetObjType object_type, std::uint16_t object_id, rage::datBitBuffer* buffer, std::uint16_t unk, std::uint32_t timestamp)
	{
		if (g_ProtectionScript->m_BlockMaliciousSyncs && src != nullptr) {
			if (src->is_valid()) {
				if (object_type < eNetObjType::NET_OBJ_TYPE_AUTOMOBILE || object_type > eNetObjType::NET_OBJ_TYPE_TRAIN)
				{
					if (g_ProtectionScript->m_MiscellaneousNotify)
						Overlays::ProtectionMessage(src->get_name(), "Sync Crash");
					return eAckCode::ACKCODE_FAIL;
				}

				if (const auto NetworkObject = Pointers::pGetNetObject(mgr, object_id, true); NetworkObject && NetworkObject->m_object_type != (std::int16_t)object_type)
				{
					if (g_ProtectionScript->m_MiscellaneousNotify)
						Overlays::ProtectionMessage(src->get_name(), "Sync Crash");
					return eAckCode::ACKCODE_FAIL;
				}
			}
		}

		g_ProtectionScript->m_SyncingPlayer = src;
		return Hooking::GetHook<DetourHook>("RCS")->Original<decltype(&ReceivedCloneSync)>()(mgr, src, dst, object_type, object_id, buffer, unk, timestamp);
	}
}