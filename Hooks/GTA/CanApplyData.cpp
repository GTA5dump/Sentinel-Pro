#include "../HookTable.hpp"
#include "../../GTA/GTA.hpp"
#include "../../Features/ProtectionScript.hpp"
#include "../../GUI/Overlays/Overlays.hpp"
#include "../../GTA/Invoker/Natives.hpp"
#include "../../Pointers/Pointers.hpp"
#include "../../Utility/ModelInfo.hpp"
#include "../../GTA/Crashes.hpp"
#include <GTAV-Classes/netsync/netSyncDataNode.hpp>
#include <GTAV-Classes/netsync/netSyncTree.hpp>
#include <GTAV-Classes/netsync/nodes/door/CDoorCreationDataNode.hpp>
#include <GTAV-Classes/netsync/nodes/dynamic_entity/CDynamicEntityGameStateDataNode.hpp>
#include <GTAV-Classes/netsync/nodes/object/CObjectCreationDataNode.hpp>
#include <GTAV-Classes/netsync/nodes/ped/CPedAttachDataNode.hpp>
#include <GTAV-Classes/netsync/nodes/ped/CPedCreationDataNode.hpp>
#include <GTAV-Classes/netsync/nodes/ped/CPedInventoryDataNode.hpp>
#include <GTAV-Classes/netsync/nodes/physical/CPhysicalAttachDataNode.hpp>
#include <GTAV-Classes/netsync/nodes/pickup/CPickupCreationDataNode.hpp>
#include <GTAV-Classes/netsync/nodes/player/CPlayerAppearanceDataNode.hpp>
#include <GTAV-Classes/netsync/nodes/player/CPlayerCreationDataNode.hpp>
#include <GTAV-Classes/netsync/nodes/proximity_migrateable/CSectorDataNode.hpp>
#include <GTAV-Classes/netsync/nodes/train/CTrainGameStateDataNode.hpp>
#include <GTAV-Classes/netsync/nodes/vehicle/CVehicleCreationDataNode.hpp>
#include <GTAV-Classes/netsync/nodes/vehicle/CVehicleGadgetDataNode.hpp>
#include <GTAV-Classes/netsync/nodes/vehicle/CVehicleProximityMigrationDataNode.hpp>

#define RAGE_JOAAT rage::joaat
namespace Sentinel
{
	constexpr std::uint32_t g_CrashVehicles[] = { RAGE_JOAAT("arbitergt"), RAGE_JOAAT("astron2"), RAGE_JOAAT("cyclone2"), RAGE_JOAAT("ignus2"), RAGE_JOAAT("s95") };
	constexpr std::uint32_t g_CrashPeds[] = { RAGE_JOAAT("slod_human"), RAGE_JOAAT("slod_small_quadped"), RAGE_JOAAT("slod_large_quadped") };
	constexpr std::uint32_t g_CageObjects[] = { RAGE_JOAAT("prop_rub_cage01a"), RAGE_JOAAT("prop_fnclink_05crnr1"), RAGE_JOAAT("prop_gold_cont_01"),RAGE_JOAAT("prop_fnclink_02n"), RAGE_JOAAT("prop_fnclink_04g"), RAGE_JOAAT("prop_fnclink_09b"), RAGE_JOAAT("prop_feeder1"), RAGE_JOAAT("stt_prop_stunt_tube_s"), RAGE_JOAAT("prop_feeder1_cr"), RAGE_JOAAT("p_cablecar_s") };
	constexpr std::uint32_t g_CrashObjects[] = { RAGE_JOAAT("prop_dummy_01"), RAGE_JOAAT("prop_dummy_car"), RAGE_JOAAT("prop_dummy_light"), RAGE_JOAAT("prop_dummy_plane"), RAGE_JOAAT("prop_distantcar_night"), RAGE_JOAAT("prop_distantcar_day"), RAGE_JOAAT("hei_bh1_08_details4_em_night"), RAGE_JOAAT("dt1_18_sq_night_slod"), RAGE_JOAAT("ss1_12_night_slod"), -1288391198, RAGE_JOAAT("h4_prop_bush_bgnvla_med_01"), RAGE_JOAAT("h4_prop_bush_bgnvla_lrg_01"), RAGE_JOAAT("h4_prop_bush_buddleia_low_01"), RAGE_JOAAT("h4_prop_bush_ear_aa"), RAGE_JOAAT("h4_prop_bush_ear_ab"), RAGE_JOAAT("h4_prop_bush_fern_low_01"), RAGE_JOAAT("h4_prop_bush_fern_tall_cc"), RAGE_JOAAT("h4_prop_bush_mang_ad"), RAGE_JOAAT("h4_prop_bush_mang_low_aa"), RAGE_JOAAT("h4_prop_bush_mang_low_ab"), RAGE_JOAAT("h4_prop_bush_seagrape_low_01"), RAGE_JOAAT("prop_h4_ground_cover"), RAGE_JOAAT("h4_prop_weed_groundcover_01"), RAGE_JOAAT("h4_prop_grass_med_01"), RAGE_JOAAT("h4_prop_grass_tropical_lush_01"), RAGE_JOAAT("h4_prop_grass_wiregrass_01"), RAGE_JOAAT("h4_prop_weed_01_plant"), RAGE_JOAAT("h4_prop_weed_01_row"), RAGE_JOAAT("urbanweeds02_l1"), RAGE_JOAAT("proc_forest_grass01"), RAGE_JOAAT("prop_small_bushyba"), RAGE_JOAAT("v_res_d_dildo_a"), RAGE_JOAAT("v_res_d_dildo_b"), RAGE_JOAAT("v_res_d_dildo_c"), RAGE_JOAAT("v_res_d_dildo_d"), RAGE_JOAAT("v_res_d_dildo_e"), RAGE_JOAAT("v_res_d_dildo_f"), RAGE_JOAAT("v_res_skateboard"), RAGE_JOAAT("prop_battery_01"), RAGE_JOAAT("prop_barbell_01"), RAGE_JOAAT("prop_barbell_02"), RAGE_JOAAT("prop_bandsaw_01"), RAGE_JOAAT("prop_bbq_3"), RAGE_JOAAT("v_med_curtainsnewcloth2"), RAGE_JOAAT("bh1_07_flagpoles"), 92962485 };

	CObject* GetGameObject(rage::netObject* object)
	{
		return *(CObject**)((__int64)object + 0x50);
	}

	bool IsCrashPed(std::uint32_t model)
	{
		if (!ModelInfo::IsModelOfType(model, eModelType::Ped, eModelType::OnlineOnlyPed))
			return true;
		for (const auto Iterator : g_CrashPeds)
			if (Iterator == model)
				return true;
		return false;
	}

	bool IsCrashVehicle(std::uint32_t model)
	{
		if (!ModelInfo::IsModelOfType(model, eModelType::Vehicle, eModelType::Unk133))
			return true;
		for (const auto Iterator : g_CrashVehicles)
			if (Iterator == model)
				return true;
		return false;
	}

	bool IsCrashObject(std::uint32_t model)
	{
		if (!ModelInfo::IsModelOfType(model, eModelType::Object, eModelType::Time, eModelType::Weapon, eModelType::Destructable, eModelType::WorldObject, eModelType::Sprinkler, eModelType::Unk65, eModelType::Plant, eModelType::LOD, eModelType::Unk132, eModelType::Building))
			return true;
		for (const auto Iterator : g_CrashObjects)
			if (Iterator == model)
				return true;
		return false;
	}

	bool IsCageObject(std::uint32_t model)
	{
		for (const auto Iterator : g_CageObjects)
			if (Iterator == model)
				return true;
		return false;
	}

	bool IsAttachmentInfinite(rage::CDynamicEntity* object, std::uint16_t attached_to_net_id)
	{
		if (object == nullptr)
			return false;

		while (object = Pointers::pGetEntityAttachedTo(object))
		{
			if (object->m_net_object && object->m_net_object->m_object_id == attached_to_net_id)
				return true;
		}

		return false;
	}

	bool IsInVehicle(CPed* ped, CVehicle* vehicle)
	{
		if (!ped || !vehicle)
			return false;

		if (ped == vehicle->m_driver)
			return true;

		for (int i = 0; i < 15; i++)
			if (vehicle->m_passengers[i] == ped)
				return true;

		return false;
	}

	static std::vector<std::pair<Hash, std::vector<std::uint64_t>>> g_Trees = 
	{
		{RAGE_JOAAT("CAutomobileSyncTree"),
			{
				RAGE_JOAAT("CVehicleCreationDataNode"),
				RAGE_JOAAT("CAutomobileCreationDataNode"),
				RAGE_JOAAT("CGlobalFlagsDataNode"),
				RAGE_JOAAT("CDynamicEntityGameStateDataNode"),
				RAGE_JOAAT("CPhysicalGameStateDataNode"),
				RAGE_JOAAT("CVehicleGameStateDataNode"),
				RAGE_JOAAT("CEntityScriptGameStateDataNode"),
				RAGE_JOAAT("CPhysicalScriptGameStateDataNode"),
				RAGE_JOAAT("CVehicleScriptGameStateDataNode"),
				RAGE_JOAAT("CEntityScriptInfoDataNode"),
				RAGE_JOAAT("CPhysicalAttachDataNode"),
				RAGE_JOAAT("CVehicleAppearanceDataNode"),
				RAGE_JOAAT("CVehicleDamageStatusDataNode"),
				RAGE_JOAAT("CVehicleComponentReservationDataNode"),
				RAGE_JOAAT("CVehicleHealthDataNode"),
				RAGE_JOAAT("CVehicleTaskDataNode"),
				RAGE_JOAAT("CSectorDataNode"),
				RAGE_JOAAT("CSectorPositionDataNode"),
				RAGE_JOAAT("CEntityOrientationDataNode"),
				RAGE_JOAAT("CPhysicalVelocityDataNode"),
				RAGE_JOAAT("CVehicleAngVelocityDataNode"),
				RAGE_JOAAT("CVehicleSteeringDataNode"),
				RAGE_JOAAT("CVehicleControlDataNode"),
				RAGE_JOAAT("CVehicleGadgetDataNode"),
				RAGE_JOAAT("CMigrationDataNode"),
				RAGE_JOAAT("CPhysicalMigrationDataNode"),
				RAGE_JOAAT("CPhysicalScriptMigrationDataNode"),
				RAGE_JOAAT("CVehicleProximityMigrationDataNode"),
			}},
		{RAGE_JOAAT("CBikeSyncTree"),
			{
				RAGE_JOAAT("CVehicleCreationDataNode"),
				RAGE_JOAAT("CGlobalFlagsDataNode"),
				RAGE_JOAAT("CDynamicEntityGameStateDataNode"),
				RAGE_JOAAT("CPhysicalGameStateDataNode"),
				RAGE_JOAAT("CVehicleGameStateDataNode"),
				RAGE_JOAAT("CBikeGameStateDataNode"),
				RAGE_JOAAT("CEntityScriptGameStateDataNode"),
				RAGE_JOAAT("CPhysicalScriptGameStateDataNode"),
				RAGE_JOAAT("CVehicleScriptGameStateDataNode"),
				RAGE_JOAAT("CEntityScriptInfoDataNode"),
				RAGE_JOAAT("CPhysicalAttachDataNode"),
				RAGE_JOAAT("CVehicleAppearanceDataNode"),
				RAGE_JOAAT("CVehicleDamageStatusDataNode"),
				RAGE_JOAAT("CVehicleComponentReservationDataNode"),
				RAGE_JOAAT("CVehicleHealthDataNode"),
				RAGE_JOAAT("CVehicleTaskDataNode"),
				RAGE_JOAAT("CSectorDataNode"),
				RAGE_JOAAT("CSectorPositionDataNode"),
				RAGE_JOAAT("CEntityOrientationDataNode"),
				RAGE_JOAAT("CPhysicalVelocityDataNode"),
				RAGE_JOAAT("CVehicleAngVelocityDataNode"),
				RAGE_JOAAT("CVehicleSteeringDataNode"),
				RAGE_JOAAT("CVehicleControlDataNode"),
				RAGE_JOAAT("CVehicleGadgetDataNode"),
				RAGE_JOAAT("CMigrationDataNode"),
				RAGE_JOAAT("CPhysicalMigrationDataNode"),
				RAGE_JOAAT("CPhysicalScriptMigrationDataNode"),
				RAGE_JOAAT("CVehicleProximityMigrationDataNode"),
			}},
		{RAGE_JOAAT("CBoatSyncTree"),
			{
				RAGE_JOAAT("CVehicleCreationDataNode"),
				RAGE_JOAAT("CGlobalFlagsDataNode"),
				RAGE_JOAAT("CDynamicEntityGameStateDataNode"),
				RAGE_JOAAT("CPhysicalGameStateDataNode"),
				RAGE_JOAAT("CVehicleGameStateDataNode"),
				RAGE_JOAAT("CBoatGameStateDataNode"),
				RAGE_JOAAT("CEntityScriptGameStateDataNode"),
				RAGE_JOAAT("CPhysicalScriptGameStateDataNode"),
				RAGE_JOAAT("CVehicleScriptGameStateDataNode"),
				RAGE_JOAAT("CEntityScriptInfoDataNode"),
				RAGE_JOAAT("CPhysicalAttachDataNode"),
				RAGE_JOAAT("CVehicleAppearanceDataNode"),
				RAGE_JOAAT("CVehicleDamageStatusDataNode"),
				RAGE_JOAAT("CVehicleComponentReservationDataNode"),
				RAGE_JOAAT("CVehicleHealthDataNode"),
				RAGE_JOAAT("CVehicleTaskDataNode"),
				RAGE_JOAAT("CSectorDataNode"),
				RAGE_JOAAT("CSectorPositionDataNode"),
				RAGE_JOAAT("CEntityOrientationDataNode"),
				RAGE_JOAAT("CPhysicalVelocityDataNode"),
				RAGE_JOAAT("CVehicleAngVelocityDataNode"),
				RAGE_JOAAT("CVehicleSteeringDataNode"),
				RAGE_JOAAT("CVehicleControlDataNode"),
				RAGE_JOAAT("CVehicleGadgetDataNode"),
				RAGE_JOAAT("CMigrationDataNode"),
				RAGE_JOAAT("CPhysicalMigrationDataNode"),
				RAGE_JOAAT("CPhysicalScriptMigrationDataNode"),
				RAGE_JOAAT("CVehicleProximityMigrationDataNode"),
			}},
		{RAGE_JOAAT("CDoorSyncTree"),
			{
				RAGE_JOAAT("CDoorCreationDataNode"),
				RAGE_JOAAT("CGlobalFlagsDataNode"),
				RAGE_JOAAT("CDoorScriptInfoDataNode"),
				RAGE_JOAAT("CDoorScriptGameStateDataNode"),
				RAGE_JOAAT("CDoorMovementDataNode"),
				RAGE_JOAAT("CMigrationDataNode"),
				RAGE_JOAAT("CPhysicalScriptMigrationDataNode"),
			}},
		{RAGE_JOAAT("CHeliSyncTree"),
			{
				RAGE_JOAAT("CVehicleCreationDataNode"),
				RAGE_JOAAT("CAutomobileCreationDataNode"),
				RAGE_JOAAT("CGlobalFlagsDataNode"),
				RAGE_JOAAT("CDynamicEntityGameStateDataNode"),
				RAGE_JOAAT("CPhysicalGameStateDataNode"),
				RAGE_JOAAT("CVehicleGameStateDataNode"),
				RAGE_JOAAT("CEntityScriptGameStateDataNode"),
				RAGE_JOAAT("CPhysicalScriptGameStateDataNode"),
				RAGE_JOAAT("CVehicleScriptGameStateDataNode"),
				RAGE_JOAAT("CEntityScriptInfoDataNode"),
				RAGE_JOAAT("CPhysicalAttachDataNode"),
				RAGE_JOAAT("CVehicleAppearanceDataNode"),
				RAGE_JOAAT("CVehicleDamageStatusDataNode"),
				RAGE_JOAAT("CVehicleComponentReservationDataNode"),
				RAGE_JOAAT("CVehicleHealthDataNode"),
				RAGE_JOAAT("CVehicleTaskDataNode"),
				RAGE_JOAAT("CHeliHealthDataNode"),
				RAGE_JOAAT("CSectorDataNode"),
				RAGE_JOAAT("CSectorPositionDataNode"),
				RAGE_JOAAT("CEntityOrientationDataNode"),
				RAGE_JOAAT("CPhysicalVelocityDataNode"),
				RAGE_JOAAT("CVehicleAngVelocityDataNode"),
				RAGE_JOAAT("CVehicleSteeringDataNode"),
				RAGE_JOAAT("CVehicleControlDataNode"),
				RAGE_JOAAT("CVehicleGadgetDataNode"),
				RAGE_JOAAT("CHeliControlDataNode"),
				RAGE_JOAAT("CMigrationDataNode"),
				RAGE_JOAAT("CPhysicalMigrationDataNode"),
				RAGE_JOAAT("CPhysicalScriptMigrationDataNode"),
				RAGE_JOAAT("CVehicleProximityMigrationDataNode"),
			}},
		{RAGE_JOAAT("CObjectSyncTree"),
			{
				RAGE_JOAAT("CObjectCreationDataNode"),
				RAGE_JOAAT("CGlobalFlagsDataNode"),
				RAGE_JOAAT("CDynamicEntityGameStateDataNode"),
				RAGE_JOAAT("CPhysicalGameStateDataNode"),
				RAGE_JOAAT("CObjectGameStateDataNode"),
				RAGE_JOAAT("CEntityScriptGameStateDataNode"),
				RAGE_JOAAT("CPhysicalScriptGameStateDataNode"),
				RAGE_JOAAT("CObjectScriptGameStateDataNode"),
				RAGE_JOAAT("CEntityScriptInfoDataNode"),
				RAGE_JOAAT("CPhysicalAttachDataNode"),
				RAGE_JOAAT("CPhysicalHealthDataNode"),
				RAGE_JOAAT("CSectorDataNode"),
				RAGE_JOAAT("CObjectSectorPosNode"),
				RAGE_JOAAT("CObjectOrientationNode"),
				RAGE_JOAAT("CPhysicalVelocityDataNode"),
				RAGE_JOAAT("CPhysicalAngVelocityDataNode"),
				RAGE_JOAAT("CMigrationDataNode"),
				RAGE_JOAAT("CPhysicalMigrationDataNode"),
				RAGE_JOAAT("CPhysicalScriptMigrationDataNode"),
			}},
		{RAGE_JOAAT("CPedSyncTree"),
			{
				RAGE_JOAAT("CPedCreationDataNode"),
				RAGE_JOAAT("CPedScriptCreationDataNode"),
				RAGE_JOAAT("CGlobalFlagsDataNode"),
				RAGE_JOAAT("CDynamicEntityGameStateDataNode"),
				RAGE_JOAAT("CPhysicalGameStateDataNode"),
				RAGE_JOAAT("CPedGameStateDataNode"),
				RAGE_JOAAT("CPedComponentReservationDataNode"),
				RAGE_JOAAT("CEntityScriptGameStateDataNode"),
				RAGE_JOAAT("CPhysicalScriptGameStateDataNode"),
				RAGE_JOAAT("CPedScriptGameStateDataNode"),
				RAGE_JOAAT("CEntityScriptInfoDataNode"),
				RAGE_JOAAT("CPedAttachDataNode"),
				RAGE_JOAAT("CPedHealthDataNode"),
				RAGE_JOAAT("CPedMovementGroupDataNode"),
				RAGE_JOAAT("CPedAIDataNode"),
				RAGE_JOAAT("CPedAppearanceDataNode"),
				RAGE_JOAAT("CPedOrientationDataNode"),
				RAGE_JOAAT("CPedMovementDataNode"),
				RAGE_JOAAT("CPedTaskTreeDataNode"),
				RAGE_JOAAT("CPedTaskSpecificDataNode"),
				RAGE_JOAAT("CPedTaskSpecificDataNode"),
				RAGE_JOAAT("CPedTaskSpecificDataNode"),
				RAGE_JOAAT("CPedTaskSpecificDataNode"),
				RAGE_JOAAT("CPedTaskSpecificDataNode"),
				RAGE_JOAAT("CPedTaskSpecificDataNode"),
				RAGE_JOAAT("CPedTaskSpecificDataNode"),
				RAGE_JOAAT("CPedTaskSpecificDataNode"),
				RAGE_JOAAT("CSectorDataNode"),
				RAGE_JOAAT("CPedSectorPosMapNode"),
				RAGE_JOAAT("CPedSectorPosNavMeshNode"),
				RAGE_JOAAT("CMigrationDataNode"),
				RAGE_JOAAT("CPhysicalMigrationDataNode"),
				RAGE_JOAAT("CPhysicalScriptMigrationDataNode"),
				RAGE_JOAAT("CPedInventoryDataNode"),
				RAGE_JOAAT("CPedTaskSequenceDataNode"),
			}},
		{RAGE_JOAAT("CPickupSyncTree"),
			{
				RAGE_JOAAT("CPickupCreationDataNode"),
				RAGE_JOAAT("CGlobalFlagsDataNode"),
				RAGE_JOAAT("CDynamicEntityGameStateDataNode"),
				RAGE_JOAAT("CPickupScriptGameStateNode"),
				RAGE_JOAAT("CPhysicalGameStateDataNode"),
				RAGE_JOAAT("CEntityScriptGameStateDataNode"),
				RAGE_JOAAT("CPhysicalScriptGameStateDataNode"),
				RAGE_JOAAT("CEntityScriptInfoDataNode"),
				RAGE_JOAAT("CPhysicalHealthDataNode"),
				RAGE_JOAAT("CPhysicalAttachDataNode"),
				RAGE_JOAAT("CSectorDataNode"),
				RAGE_JOAAT("CPickupSectorPosNode"),
				RAGE_JOAAT("CEntityOrientationDataNode"),
				RAGE_JOAAT("CPhysicalVelocityDataNode"),
				RAGE_JOAAT("CPhysicalAngVelocityDataNode"),
				RAGE_JOAAT("CMigrationDataNode"),
				RAGE_JOAAT("CPhysicalMigrationDataNode"),
				RAGE_JOAAT("CPhysicalScriptMigrationDataNode"),
			}},
		{RAGE_JOAAT("CPickupPlacementSyncTree"),
			{
				RAGE_JOAAT("CPickupPlacementCreationDataNode"),
				RAGE_JOAAT("CMigrationDataNode"),
				RAGE_JOAAT("CGlobalFlagsDataNode"),
				RAGE_JOAAT("CPickupPlacementStateDataNode"),
			}},
		{RAGE_JOAAT("CPlaneSyncTree"),
			{
				RAGE_JOAAT("CVehicleCreationDataNode"),
				RAGE_JOAAT("CGlobalFlagsDataNode"),
				RAGE_JOAAT("CDynamicEntityGameStateDataNode"),
				RAGE_JOAAT("CPhysicalGameStateDataNode"),
				RAGE_JOAAT("CVehicleGameStateDataNode"),
				RAGE_JOAAT("CEntityScriptGameStateDataNode"),
				RAGE_JOAAT("CPhysicalScriptGameStateDataNode"),
				RAGE_JOAAT("CVehicleScriptGameStateDataNode"),
				RAGE_JOAAT("CEntityScriptInfoDataNode"),
				RAGE_JOAAT("CPhysicalAttachDataNode"),
				RAGE_JOAAT("CVehicleAppearanceDataNode"),
				RAGE_JOAAT("CVehicleDamageStatusDataNode"),
				RAGE_JOAAT("CVehicleComponentReservationDataNode"),
				RAGE_JOAAT("CVehicleHealthDataNode"),
				RAGE_JOAAT("CVehicleTaskDataNode"),
				RAGE_JOAAT("CPlaneGameStateDataNode"),
				RAGE_JOAAT("CSectorDataNode"),
				RAGE_JOAAT("CSectorPositionDataNode"),
				RAGE_JOAAT("CEntityOrientationDataNode"),
				RAGE_JOAAT("CPhysicalVelocityDataNode"),
				RAGE_JOAAT("CVehicleAngVelocityDataNode"),
				RAGE_JOAAT("CVehicleSteeringDataNode"),
				RAGE_JOAAT("CVehicleControlDataNode"),
				RAGE_JOAAT("CVehicleGadgetDataNode"),
				RAGE_JOAAT("CPlaneControlDataNode"),
				RAGE_JOAAT("CMigrationDataNode"),
				RAGE_JOAAT("CPhysicalMigrationDataNode"),
				RAGE_JOAAT("CPhysicalScriptMigrationDataNode"),
				RAGE_JOAAT("CVehicleProximityMigrationDataNode"),
			}},
		{RAGE_JOAAT("CSubmarineSyncTree"),
			{
				RAGE_JOAAT("CVehicleCreationDataNode"),
				RAGE_JOAAT("CGlobalFlagsDataNode"),
				RAGE_JOAAT("CDynamicEntityGameStateDataNode"),
				RAGE_JOAAT("CPhysicalGameStateDataNode"),
				RAGE_JOAAT("CVehicleGameStateDataNode"),
				RAGE_JOAAT("CSubmarineGameStateDataNode"),
				RAGE_JOAAT("CEntityScriptGameStateDataNode"),
				RAGE_JOAAT("CPhysicalScriptGameStateDataNode"),
				RAGE_JOAAT("CVehicleScriptGameStateDataNode"),
				RAGE_JOAAT("CEntityScriptInfoDataNode"),
				RAGE_JOAAT("CPhysicalAttachDataNode"),
				RAGE_JOAAT("CVehicleAppearanceDataNode"),
				RAGE_JOAAT("CVehicleDamageStatusDataNode"),
				RAGE_JOAAT("CVehicleComponentReservationDataNode"),
				RAGE_JOAAT("CVehicleHealthDataNode"),
				RAGE_JOAAT("CVehicleTaskDataNode"),
				RAGE_JOAAT("CSectorDataNode"),
				RAGE_JOAAT("CSectorPositionDataNode"),
				RAGE_JOAAT("CEntityOrientationDataNode"),
				RAGE_JOAAT("CPhysicalVelocityDataNode"),
				RAGE_JOAAT("CVehicleAngVelocityDataNode"),
				RAGE_JOAAT("CVehicleSteeringDataNode"),
				RAGE_JOAAT("CVehicleControlDataNode"),
				RAGE_JOAAT("CVehicleGadgetDataNode"),
				RAGE_JOAAT("CSubmarineControlDataNode"),
				RAGE_JOAAT("CMigrationDataNode"),
				RAGE_JOAAT("CPhysicalMigrationDataNode"),
				RAGE_JOAAT("CPhysicalScriptMigrationDataNode"),
				RAGE_JOAAT("CVehicleProximityMigrationDataNode"),
			}},
		{RAGE_JOAAT("CPlayerSyncTree"),
			{
				RAGE_JOAAT("CPlayerCreationDataNode"),
				RAGE_JOAAT("CGlobalFlagsDataNode"),
				RAGE_JOAAT("CDynamicEntityGameStateDataNode"),
				RAGE_JOAAT("CPhysicalGameStateDataNode"),
				RAGE_JOAAT("CPedGameStateDataNode"),
				RAGE_JOAAT("CPedComponentReservationDataNode"),
				RAGE_JOAAT("CEntityScriptGameStateDataNode"),
				RAGE_JOAAT("CPlayerGameStateDataNode"),
				RAGE_JOAAT("CPedAttachDataNode"),
				RAGE_JOAAT("CPedHealthDataNode"),
				RAGE_JOAAT("CPedMovementGroupDataNode"),
				RAGE_JOAAT("CPedAIDataNode"),
				RAGE_JOAAT("CPlayerAppearanceDataNode"),
				RAGE_JOAAT("CPlayerPedGroupDataNode"),
				RAGE_JOAAT("CPlayerAmbientModelStreamingNode"),
				RAGE_JOAAT("CPlayerGamerDataNode"),
				RAGE_JOAAT("CPlayerExtendedGameStateNode"),
				RAGE_JOAAT("CPedOrientationDataNode"),
				RAGE_JOAAT("CPedMovementDataNode"),
				RAGE_JOAAT("CPedTaskTreeDataNode"),
				RAGE_JOAAT("CPedTaskSpecificDataNode"),
				RAGE_JOAAT("CPedTaskSpecificDataNode"),
				RAGE_JOAAT("CPedTaskSpecificDataNode"),
				RAGE_JOAAT("CPedTaskSpecificDataNode"),
				RAGE_JOAAT("CPedTaskSpecificDataNode"),
				RAGE_JOAAT("CPedTaskSpecificDataNode"),
				RAGE_JOAAT("CPedTaskSpecificDataNode"),
				RAGE_JOAAT("CPedTaskSpecificDataNode"),
				RAGE_JOAAT("CSectorDataNode"),
				RAGE_JOAAT("CPlayerSectorPosNode"),
				RAGE_JOAAT("CPlayerCameraDataNode"),
				RAGE_JOAAT("CPlayerWantedAndLOSDataNode"),
				RAGE_JOAAT("CMigrationDataNode"),
				RAGE_JOAAT("CPhysicalMigrationDataNode"),
				RAGE_JOAAT("CPhysicalScriptMigrationDataNode"),
			}},
		{RAGE_JOAAT("CAutomobileSyncTree"),
			{
				RAGE_JOAAT("CVehicleCreationDataNode"),
				RAGE_JOAAT("CAutomobileCreationDataNode"),
				RAGE_JOAAT("CGlobalFlagsDataNode"),
				RAGE_JOAAT("CDynamicEntityGameStateDataNode"),
				RAGE_JOAAT("CPhysicalGameStateDataNode"),
				RAGE_JOAAT("CVehicleGameStateDataNode"),
				RAGE_JOAAT("CEntityScriptGameStateDataNode"),
				RAGE_JOAAT("CPhysicalScriptGameStateDataNode"),
				RAGE_JOAAT("CVehicleScriptGameStateDataNode"),
				RAGE_JOAAT("CEntityScriptInfoDataNode"),
				RAGE_JOAAT("CPhysicalAttachDataNode"),
				RAGE_JOAAT("CVehicleAppearanceDataNode"),
				RAGE_JOAAT("CVehicleDamageStatusDataNode"),
				RAGE_JOAAT("CVehicleComponentReservationDataNode"),
				RAGE_JOAAT("CVehicleHealthDataNode"),
				RAGE_JOAAT("CVehicleTaskDataNode"),
				RAGE_JOAAT("CSectorDataNode"),
				RAGE_JOAAT("CSectorPositionDataNode"),
				RAGE_JOAAT("CEntityOrientationDataNode"),
				RAGE_JOAAT("CPhysicalVelocityDataNode"),
				RAGE_JOAAT("CVehicleAngVelocityDataNode"),
				RAGE_JOAAT("CVehicleSteeringDataNode"),
				RAGE_JOAAT("CVehicleControlDataNode"),
				RAGE_JOAAT("CVehicleGadgetDataNode"),
				RAGE_JOAAT("CMigrationDataNode"),
				RAGE_JOAAT("CPhysicalMigrationDataNode"),
				RAGE_JOAAT("CPhysicalScriptMigrationDataNode"),
				RAGE_JOAAT("CVehicleProximityMigrationDataNode"),
			}},
		{RAGE_JOAAT("CTrainSyncTree"),
			{
				RAGE_JOAAT("CVehicleCreationDataNode"),
				RAGE_JOAAT("CGlobalFlagsDataNode"),
				RAGE_JOAAT("CDynamicEntityGameStateDataNode"),
				RAGE_JOAAT("CPhysicalGameStateDataNode"),
				RAGE_JOAAT("CVehicleGameStateDataNode"),
				RAGE_JOAAT("CTrainGameStateDataNode"),
				RAGE_JOAAT("CEntityScriptGameStateDataNode"),
				RAGE_JOAAT("CPhysicalScriptGameStateDataNode"),
				RAGE_JOAAT("CVehicleScriptGameStateDataNode"),
				RAGE_JOAAT("CEntityScriptInfoDataNode"),
				RAGE_JOAAT("CPhysicalAttachDataNode"),
				RAGE_JOAAT("CVehicleAppearanceDataNode"),
				RAGE_JOAAT("CVehicleDamageStatusDataNode"),
				RAGE_JOAAT("CVehicleComponentReservationDataNode"),
				RAGE_JOAAT("CVehicleHealthDataNode"),
				RAGE_JOAAT("CVehicleTaskDataNode"),
				RAGE_JOAAT("CSectorDataNode"),
				RAGE_JOAAT("CSectorPositionDataNode"),
				RAGE_JOAAT("CEntityOrientationDataNode"),
				RAGE_JOAAT("CPhysicalVelocityDataNode"),
				RAGE_JOAAT("CVehicleAngVelocityDataNode"),
				RAGE_JOAAT("CVehicleSteeringDataNode"),
				RAGE_JOAAT("CVehicleControlDataNode"),
				RAGE_JOAAT("CVehicleGadgetDataNode"),
				RAGE_JOAAT("CMigrationDataNode"),
				RAGE_JOAAT("CPhysicalMigrationDataNode"),
				RAGE_JOAAT("CPhysicalScriptMigrationDataNode"),
				RAGE_JOAAT("CVehicleProximityMigrationDataNode"),
			}},
	};

	static auto g_Nodes = std::to_array<std::pair<Hash, std::uint64_t>>({ {RAGE_JOAAT("CAutomobileCreationDataNode"), 0}, {RAGE_JOAAT("CBikeGameStateDataNode"), 0}, {RAGE_JOAAT("CBoatGameStateDataNode"), 0}, {RAGE_JOAAT("CDoorCreationDataNode"), 0}, {RAGE_JOAAT("CDoorMovementDataNode"), 0}, {RAGE_JOAAT("CDoorScriptGameStateDataNode"), 0}, {RAGE_JOAAT("CDoorScriptInfoDataNode"), 0}, {RAGE_JOAAT("CDynamicEntityGameStateDataNode"), 0}, {RAGE_JOAAT("CEntityOrientationDataNode"), 0}, {RAGE_JOAAT("CEntityScriptGameStateDataNode"), 0}, {RAGE_JOAAT("CEntityScriptInfoDataNode"), 0}, {RAGE_JOAAT("CGlobalFlagsDataNode"), 0}, {RAGE_JOAAT("CHeliControlDataNode"), 0}, {RAGE_JOAAT("CHeliHealthDataNode"), 0}, {RAGE_JOAAT("CMigrationDataNode"), 0}, {RAGE_JOAAT("CObjectCreationDataNode"), 0}, {RAGE_JOAAT("CObjectGameStateDataNode"), 0}, {RAGE_JOAAT("CObjectOrientationNode"), 0}, {RAGE_JOAAT("CObjectScriptGameStateDataNode"), 0}, {RAGE_JOAAT("CObjectSectorPosNode"), 0}, {RAGE_JOAAT("CPedAIDataNode"), 0}, {RAGE_JOAAT("CPedAppearanceDataNode"), 0}, {RAGE_JOAAT("CPedAttachDataNode"), 0}, {RAGE_JOAAT("CPedComponentReservationDataNode"), 0}, {RAGE_JOAAT("CPedCreationDataNode"), 0}, {RAGE_JOAAT("CPedGameStateDataNode"), 0}, {RAGE_JOAAT("CPedHealthDataNode"), 0}, {RAGE_JOAAT("CPedInventoryDataNode"), 0}, {RAGE_JOAAT("CPedMovementDataNode"), 0}, {RAGE_JOAAT("CPedMovementGroupDataNode"), 0}, {RAGE_JOAAT("CPedOrientationDataNode"), 0}, {RAGE_JOAAT("CPedScriptCreationDataNode"), 0}, {RAGE_JOAAT("CPedScriptGameStateDataNode"), 0}, {RAGE_JOAAT("CPedSectorPosMapNode"), 0}, {RAGE_JOAAT("CPedSectorPosNavMeshNode"), 0}, {RAGE_JOAAT("CPedTaskSequenceDataNode"), 0}, {RAGE_JOAAT("CPedTaskSpecificDataNode"), 0}, {RAGE_JOAAT("CPedTaskTreeDataNode"), 0}, {RAGE_JOAAT("CPhysicalAngVelocityDataNode"), 0}, {RAGE_JOAAT("CPhysicalAttachDataNode"), 0}, {RAGE_JOAAT("CPhysicalGameStateDataNode"), 0}, {RAGE_JOAAT("CPhysicalHealthDataNode"), 0}, {RAGE_JOAAT("CPhysicalMigrationDataNode"), 0}, {RAGE_JOAAT("CPhysicalScriptGameStateDataNode"), 0}, {RAGE_JOAAT("CPhysicalScriptMigrationDataNode"), 0}, {RAGE_JOAAT("CPhysicalVelocityDataNode"), 0}, {RAGE_JOAAT("CPickupCreationDataNode"), 0}, {RAGE_JOAAT("CPickupPlacementCreationDataNode"), 0}, {RAGE_JOAAT("CPickupPlacementStateDataNode"), 0}, {RAGE_JOAAT("CPickupScriptGameStateNode"), 0}, {RAGE_JOAAT("CPickupSectorPosNode"), 0}, {RAGE_JOAAT("CPlaneControlDataNode"), 0}, {RAGE_JOAAT("CPlaneGameStateDataNode"), 0}, {RAGE_JOAAT("CPlayerAmbientModelStreamingNode"), 0}, {RAGE_JOAAT("CPlayerAppearanceDataNode"), 0}, {RAGE_JOAAT("CPlayerCameraDataNode"), 0}, {RAGE_JOAAT("CPlayerCreationDataNode"), 0}, {RAGE_JOAAT("CPlayerExtendedGameStateNode"), 0}, {RAGE_JOAAT("CPlayerGameStateDataNode"), 0}, {RAGE_JOAAT("CPlayerGamerDataNode"), 0}, {RAGE_JOAAT("CPlayerPedGroupDataNode"), 0}, {RAGE_JOAAT("CPlayerSectorPosNode"), 0}, {RAGE_JOAAT("CPlayerWantedAndLOSDataNode"), 0}, {RAGE_JOAAT("CSectorDataNode"), 0}, {RAGE_JOAAT("CSectorPositionDataNode"), 0}, {RAGE_JOAAT("CSubmarineControlDataNode"), 0}, {RAGE_JOAAT("CSubmarineGameStateDataNode"), 0}, {RAGE_JOAAT("CTrainGameStateDataNode"), 0}, {RAGE_JOAAT("CVehicleAngVelocityDataNode"), 0}, {RAGE_JOAAT("CVehicleAppearanceDataNode"), 0}, {RAGE_JOAAT("CVehicleComponentReservationDataNode"), 0}, {RAGE_JOAAT("CVehicleControlDataNode"), 0}, {RAGE_JOAAT("CVehicleCreationDataNode"), 0}, {RAGE_JOAAT("CVehicleDamageStatusDataNode"), 0}, {RAGE_JOAAT("CVehicleGadgetDataNode"), 0}, {RAGE_JOAAT("CVehicleGameStateDataNode"), 0}, {RAGE_JOAAT("CVehicleHealthDataNode"), 0}, {RAGE_JOAAT("CVehicleProximityMigrationDataNode"), 0}, {RAGE_JOAAT("CVehicleScriptGameStateDataNode"), 0}, {RAGE_JOAAT("CVehicleSteeringDataNode"), 0}, {RAGE_JOAAT("CVehicleTaskDataNode"), 0} });

	static void CacheNodes()
	{
		for (int i = (int)eNetObjType::NET_OBJ_TYPE_AUTOMOBILE; i <= (int)eNetObjType::NET_OBJ_TYPE_TRAIN; i++)
		{
			if (i == (int)eNetObjType::NET_OBJ_TYPE_TRAILER)
				continue;
			rage::netSyncTree* tree = Pointers::pGetSyncTreeForType(*Pointers::pNetworkObjectMgr, i);
			if (tree->m_child_node_count > g_Trees[i].second.size())
			{
				throw std::runtime_error("Failed to cache data nodes.");
			}
			for (int j = 0; j < tree->m_child_node_count; j++)
			{
				PVOID vft = *(PVOID*)tree->m_child_nodes[j];
				for (auto& n : g_Nodes)
				{
					if (n.first == g_Trees[i].second[j])
					{
						n.second = (std::uint64_t)vft;
						break;
					}
				}
			}
		}
	}

	bool CheckNode(rage::netSyncNodeBase* node, CNetGamePlayer* sender, rage::netObject* object)
	{
		const auto Name = sender->get_name();
		const auto LocalPlayer = g_PlayerService->GetSelf()->GetPed();
		if (node->IsParentNode())
		{
			for (auto Child = node->m_first_child; Child; Child = Child->m_next_sibling)
			{
				if (CheckNode(Child, sender, object))
					return true;
			}
		}
		else if (node->IsDataNode())
		{
			auto vtable = *(void**)node;
			Hash NodeHash = 0;

			for (const auto& n : g_Nodes)
			{
				if ((void*)n.second == vtable)
				{
					NodeHash = n.first;
					break;
				}
			}

			switch (NodeHash)
			{
			case (RAGE_JOAAT("CVehicleCreationDataNode")):
			{
				if (g_ProtectionScript->m_InvalidVehicles) {
					const auto CreationNode = (CVehicleCreationDataNode*)(node);
					if (IsCrashVehicle(CreationNode->m_model))
					{
						if (g_ProtectionScript->m_DataNodesNotify)
							Overlays::ProtectionMessage(Name, "Invalid Vehicles");
						return true;
					}
				}
				break;
			}
			case RAGE_JOAAT("CDoorCreationDataNode"):
			{
				if (g_ProtectionScript->m_InvalidDoorModel) {
					const auto CreationNode = (CDoorCreationDataNode*)(node);
					if (IsCrashObject(CreationNode->m_model))
					{
						if (g_ProtectionScript->m_DataNodesNotify)
							Overlays::ProtectionMessage(Name, "Invalid Door Model");
						return true;
					}
				}
				break;
			}
			case RAGE_JOAAT("CPickupCreationDataNode"):
			{
				if (g_ProtectionScript->m_InvalidPickupModel) {
					const auto CreationNode = (CPickupCreationDataNode*)(node);
					if (CreationNode->m_custom_model && IsCrashObject(CreationNode->m_custom_model))
					{
						if (g_ProtectionScript->m_DataNodesNotify)
							Overlays::ProtectionMessage(Name, "Invalid Pickup");
						return true;
					}
				}
				break;
			}
			case RAGE_JOAAT("CPedCreationDataNode"):
			{
				const auto CreationNode = (CPedCreationDataNode*)(node);
				if (IsCrashPed(CreationNode->m_model))
				{
					if (g_ProtectionScript->m_InvalidPedModel && !Crashes::g_PogoCrash) {
						if (g_ProtectionScript->m_DataNodesNotify)
							Overlays::ProtectionMessage(Name, "Invalid Ped Model");
						return true;
					}
				}
				else if (CreationNode->m_has_prop && IsCrashObject(CreationNode->m_prop_model))
				{
					if (g_ProtectionScript->m_InvalidPedPropModel && !Crashes::g_PogoCrash) {
						if (g_ProtectionScript->m_DataNodesNotify)
							Overlays::ProtectionMessage(Name, "Invalid Ped Prop Model");
						return true;
					}
				}
				break;
			}
			case RAGE_JOAAT("CPedAttachDataNode"):
			{
				if (g_ProtectionScript->m_InvalidAttachment) {
					const auto AttachNode = (CPedAttachDataNode*)(node);
					if (AttachNode->m_attached && AttachNode->m_attached_to == object->m_object_id)
					{
						if (g_ProtectionScript->m_DataNodesNotify)
							Overlays::ProtectionMessage(Name, "Invalid Ped Attachment");
						return true;
					}
				}

				break;
			}
			case RAGE_JOAAT("CObjectCreationDataNode"):
			{
				const auto CreationNode = (CObjectCreationDataNode*)(node);
				if (IsCrashObject(CreationNode->m_model))
				{
					if (g_ProtectionScript->m_InvalidObjects) {
						if (g_ProtectionScript->m_DataNodesNotify)
							Overlays::ProtectionMessage(Name, "Invalid Object Model");
						return true;
					}
				}
				if (IsCageObject(CreationNode->m_model))
				{
					if (g_ProtectionScript->m_BlockCages) {
						if (g_ProtectionScript->m_DataNodesNotify)
							Overlays::ProtectionMessage(Name, "Cage");
						return true;
					}
				}
				break;
			}
			case RAGE_JOAAT("CPlayerAppearanceDataNode"):
			{
				if (g_ProtectionScript->m_InvalidPlayerModel) {
					const auto CreationNode = (CPlayerAppearanceDataNode*)(node);
					if (IsCrashPed(CreationNode->m_model_hash))
					{
						if (g_ProtectionScript->m_DataNodesNotify)
							Overlays::ProtectionMessage(Name, "Invalid Player Model");
						return true;
					}
				}
				break;
			}
			case RAGE_JOAAT("CPlayerCreationDataNode"):
			{
				if (g_ProtectionScript->m_InvalidPlayerModel) {
					const auto CreationNode = (CPlayerCreationDataNode*)(node);
					if (IsCrashPed(CreationNode->m_model))
					{
						if (g_ProtectionScript->m_DataNodesNotify)
							Overlays::ProtectionMessage(Name, "Invalid Player Model");
						return true;
					}
				}
				break;
			}
			case RAGE_JOAAT("CSectorDataNode"):
			{
				if (g_ProtectionScript->m_InvalidSector) {
					const auto SectorNode = (CSectorDataNode*)(node);
					if (SectorNode->m_pos_x == 712 || SectorNode->m_pos_y == 712 || SectorNode->m_pos_z == 712)
					{
						if (g_ProtectionScript->m_DataNodesNotify)
							Overlays::ProtectionMessage(Name, "Invalid Sector");
						return true;
					}
				}
				break;
			}
			case RAGE_JOAAT("CVehicleProximityMigrationDataNode"):
			{
				if (g_ProtectionScript->m_VehicleManipulation) {
					if (object && LocalPlayer && LocalPlayer->m_net_object)
					{
						const auto MigrationNode = (CVehicleProximityMigrationDataNode*)(node);
						if (IsInVehicle(LocalPlayer, LocalPlayer->m_vehicle) && LocalPlayer->m_vehicle->m_net_object
							&& LocalPlayer->m_vehicle->m_net_object->m_object_id == object->m_object_id)
							return false; // Vehicle kick?

						if (!LocalPlayer->m_vehicle || !LocalPlayer->m_vehicle->m_net_object
							|| LocalPlayer->m_vehicle->m_net_object->m_object_id != object->m_object_id
							|| !IsInVehicle(LocalPlayer, LocalPlayer->m_vehicle))
						{
							for (int Iterator = 0; Iterator < 16; Iterator++)
							{
								if (MigrationNode->m_has_occupants[Iterator]
									&& MigrationNode->m_occupants[Iterator] == LocalPlayer->m_net_object->m_object_id)
									return true; // Remote Teleport
							}
						}
					}
				}
				break;
			}
			case RAGE_JOAAT("CTrainGameStateDataNode"): 
			{
				if (g_ProtectionScript->m_InfiniteTrainCrash) {
					const auto NetObjectID = g_ProtectionScript->m_SyncingPlayer->m_player_info->m_ped->m_net_object->m_object_id;
					const auto TrainNode = (CTrainGameStateDataNode*)(node);
					if (sizeof(TrainNode->m_track_id) >= 27) {
						if (g_ProtectionScript->m_ManualPatchesNotify)
							Overlays::ProtectionMessage(Name, "Steamroller Crash");
						return true;
					}
					if (++TrainNode->m_carriage_config_index > 20) {
						if (g_ProtectionScript->m_ManualPatchesNotify)
							Overlays::ProtectionMessage(Name, "Steamroller Crash");
						return true;
					}
					if (NetObjectID == TrainNode->m_linked_to_backward_id && NetObjectID == TrainNode->m_linked_to_forward_id && NetObjectID == TrainNode->m_engine_id) {
						if (g_ProtectionScript->m_ManualPatchesNotify)
							Overlays::ProtectionMessage(Name, "Demon Crash");
						return true;
					}
				}
				break;
			}
			}
		}

		return false;
	}

	bool GTA::CanApplyData(rage::netSyncTree* tree, rage::netObject* object)
	{
		static bool Initialize = ([] { CacheNodes(); }(), true);

		if (g_ProtectionScript->m_SyncingPlayer != nullptr) {
			if (g_ProtectionScript->m_SyncingPlayer->is_valid()) {
				if (tree->m_child_node_count && CheckNode(tree->m_next_sync_node, g_ProtectionScript->m_SyncingPlayer, object))
				{
					return false;
				}
			}
		}

		return Hooking::GetHook<DetourHook>("CAD")->Original<decltype(&CanApplyData)>()(tree, object);
	}
}