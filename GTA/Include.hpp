#pragma once
#include <cstdint>
#include <GTAV-Classes/rage/atArray.hpp>
#include <GTAV-Classes/script/scrProgram.hpp>
#include <GTAV-Classes/script/scrProgramTable.hpp>
#include <GTAV-Classes/script/GtaThread.hpp>
#include <GTAV-Classes/script/scrThread.hpp>
#include <GTAV-Classes/script/scrNativeHandler.hpp>
#include <GTAV-Classes/script/scrNativeRegistration.hpp>
#include <GTAV-Classes/script/scrNativeRegistrationTable.hpp>
#include <GTAV-Classes/ped/CPedFactory.hpp>
#include <GTAV-Classes/player/CPlayerInfo.hpp>
#include <GTAV-Classes/network/CNetGamePlayer.hpp>
#include <GTAV-Classes/network/CNetworkPlayerMgr.hpp>
#include <GTAV-Classes/network/Network.hpp>
#include <GTAV-Classes/base/HashTable.hpp>
#include <GTAV-Classes/network/ChatData.hpp>
#include <GTAV-Classes/netsync/nodes/ped/CPedOrientationDataNode.hpp>
#include <GTAV-Classes/netsync/nodes/ped/CPedCreationDataNode.hpp>
#include "ReplayInterface.hpp"

#pragma pack(push, 1)
struct VehicleStreamRender
{
	char _0x0000[0x8]; // unk
	float m_tire_height; // unk
	char _0x000C[0xB94]; // unk
	float m_tire_width; // 0x08
};

#pragma pack(push, 4)
class CPedMovementDataNode
{
public:
    char pad_0000[192]; //0x0000
    bool m_has_desired_move_blend_ratio_x; //0x00C0
    bool m_has_desired_move_blend_ratio_y; //0x00C1
    bool unk_00C2; //0x00C2
    float m_desired_move_blend_ratio_x; //0x00C4
    float m_desired_move_blend_ratio_y; //0x00C8
    float unk_00CC; //0x00CC
    float m_desired_pitch; //0x00D0
}; //Size: 0x00D4
static_assert(sizeof(CPedMovementDataNode) == 0xD4);
#pragma pack(pop)

class ListPool
{
public:
	std::uint64_t* list; //0x0000
	std::int32_t capacity; //0x0008
	char pad_000C[4]; //0x000C
	std::int32_t free; //0x0010
	char pad_0014[4]; //0x0014
	std::uint64_t item_size; //0x0018
	char pad_0020[16]; //0x0020
	std::uint32_t* data; //0x0030
	char pad_0038[16]; //0x0038
	std::int32_t occupied; //0x0048
	const std::uint64_t get(std::int32_t index) const {
		if ((this->data[index >> 5] >> (index & 0x1F)) & 1)
			return this->list[index];
		return NULL;
	}
}; // Size: 0x004C

struct QuadInfo
{
    std::uint64_t m_quadPool;
    short m_quadCount;
};

struct WaterQuads
{
    QuadInfo* m_oceanQuads;
};

struct CWheelVfx 
{
	float m_slip_min; // 0x0004
	float m_slip_max; // 0x0008
	float m_pressure_min; // 0x000C
	float m_pressure_max; // 0x0010
	int m_type1; // 0x0014
	BOOL m_unk1; // 0x0018
	int m_type2; // 0x001c
	BOOL m_unk2; // 0x0010
	int m_type3; // 0x0024
	BOOL m_unk3; // 0x0028
	int m_type4; // 0x002c
	BOOL m_unk4; // 0x0030
	std::uint8_t m_r; // 0x0034
	std::uint8_t m_g; // 0x0035
	std::uint8_t m_b; // 0x0036
	char _0x0037[1]; //0x0037
	float m_friction_thresh_min; //0x0038
	float m_friction_thresh_max; //0x003C
	uint32_t m_friction_fx1; //0x0040
	uint32_t m_friction_fx2; //0x0044
	uint32_t m_friction_fx3; //0x0048
	float m_disp_thresh_min; //0x004C
	float m_disp_thresh_max; //0x0050
	uint32_t m_displacement_fx1; //0x0054
	uint32_t m_displacement_fx2; //0x0058
	uint32_t m_displacement_fx3; //0x005C
	uint32_t m_displacement_fx_lod; //0x0060
	float m_burn_friction_evo_min; //0x0064
	float m_burn_friction_evo_max; //0x0068
	float m_burn_temp_evo_min; //0x006C
	float m_burn_temp_evo_max; //0x0070
	uint32_t m_burnout_fx1; //0x0074
	uint32_t m_burnout_fx2; //0x0078
	uint32_t m_burnout_fx3; //0x007C
	bool m_lights_on; //0x0080
	uint8_t m_lights_col_min_r; //0x0081
	uint8_t m_lights_col_min_g; //0x0082
	uint8_t m_lights_col_min_b; //0x0083
	uint8_t m_lights_col_max_r; //0x0084
	uint8_t m_lights_col_max_g; //0x0085
	uint8_t m_lights_col_max_b; //0x0086
	char _0x0087[1]; //0x0087
	float m_lights_intensity_min; //0x0088
	float m_lights_intensity_max; //0x008C
	float m_lights_range_min; //0x0090
	float m_lights_range_max; //0x0094
	float m_lights_falloff_min; //0x0098
	float m_lights_falloff_max; //0x009C
};

class CUIElementColor 
{
public:
    int m_index; //0x0000
    char pad_0004[12]; //0x0004
    rage::fvector4 m_value; //0x0010
}; //Size: 0x0020
static_assert(sizeof(CUIElementColor) == 0x20);

class CUIElementColorConstant 
{
public:
    rage::fvector4 m_value; //0x0000
    char pad_0010[12]; //0x0010
    int m_index; //0x001C
}; //Size: 0x0020
static_assert(sizeof(CUIElementColorConstant) == 0x20);

class CUIIndex 
{
public:
    int m_index; //0x0000
}; //Size: 0x0004
static_assert(sizeof(CUIIndex) == 0x4);

class CUIElementInt 
{
public:
    CUIIndex m_index; //0x0000
    int m_value; //0x0004
}; //Size: 0x0008
static_assert(sizeof(CUIElementInt) == 0x8);

struct grc_texture_dx11 
{
    char _0x0000[0x28];
    char* m_name;
    char _0x0030[0x8];
    void* m_texture; // ID3D11Resource
    char _0x0040[0x10];
    uint16_t m_width;
    uint16_t m_height;
    uint16_t m_depth;
    char _0x0056[0xA];
    grc_texture_dx11* m_previous;
    grc_texture_dx11* m_next;
    char _0x0070[0x8];
    void* m_shader; // ID3D11ShaderResourceView
    char _0x0080[0x20];
};

class CUIElementShader 
{
public:
    uint64_t m_raw_element; //0x0000
    grc_texture_dx11* m_shader; //0x0008
}; //Size: 0x0010
static_assert(sizeof(CUIElementShader) == 0x10);

class CUIWeather 
{
public:
    char pad_0000[16]; //0x0000
    CUIElementColor m_azimuth_east_color; //0x0010
    char pad_0030[16]; //0x0030
    CUIElementColor m_azimuth_west_color; //0x0040
    char pad_0060[16]; //0x0060
    CUIElementColor m_azimuth_transition_color; //0x0070
    char pad_0090[4]; //0x0090
    CUIElementInt m_azimuth_transition_position; //0x0094
    char pad_009C[4]; //0x009C
    CUIElementColor m_zenith_color; //0x00B8
    char pad_00C0[16]; //0x00C0
    CUIElementColor m_zenith_transition_color; //0x00D0
    char pad_00F0[4]; //0x00F0
    CUIElementColorConstant m_zenith_constants; //0x00F0
    char pad_0114[28]; //0x0114
    CUIElementColor m_sky_plane_color; //0x0130
    char pad_0150[16]; //0x0150
    CUIIndex m_sky_plane_params; //0x0160
    char pad_0164[52]; //0x0164
    CUIIndex m_sun_direction; //0x0198
    CUIIndex m_sun_position; //0x019C
    char pad_01A0[16]; //0x01A0
    CUIElementColor m_sun_color; //0x01B0
    CUIElementColor m_sun_color_hdr; //0x01D0
    char pad_01F0[16]; //0x01F0
    CUIElementColor m_sun_disc_color_hdr; //0x0200
    char pad_0220[32]; //0x0220
    CUIElementColor m_sun_constants; //0x0240
    CUIElementShader m_high_detail_noise; //0x0260
    CUIElementColorConstant m_cloud_constant1; //0x0270
    char pad_0290[16]; //0x0290
    CUIElementColorConstant m_cloud_constant2; //0x02A0
    char pad_02C0[16]; //0x02C0
    CUIElementColorConstant m_cloud_constant3; //0x02D0
    char pad_02F0[16]; //0x02F0
    CUIElementColorConstant m_cloud_detail_constants; //0x0300
    char pad_0320[48]; //0x0320
    CUIElementColor m_cloud_base_minus_mid_colour; //0x0350
    CUIElementColor m_cloud_mid_color; //0x0370
    CUIElementColor m_cloud_shadow_minus_base_colour_times_shadow_strength; //0x0390
    CUIElementColorConstant m_small_cloud_constants; //0x03B0
    char pad_03D0[32]; //0x03D0
    CUIElementColor m_small_cloud_color_hdr; //0x03F0
    char pad_0410[16]; //0x0410
    CUIIndex m_cloudgen_frequency; //0x0420
    char pad_0424[28]; //0x0424
    CUIElementColor m_noise_phase; //0x0440
    char pad_0460[32]; //0x0460
    CUIIndex m_speed_constants; //0x0480
    char pad_0484[28]; //0x0484
    CUIElementInt m_horizon_level; //0x04A0
    char pad_04A8[16]; //0x04A8
    CUIElementShader m_dither; //0x04B8
    char pad_04C8[24]; //0x04C8
    CUIElementShader m_star_field; //0x04E0
    CUIElementInt m_starfield_intensity; //0x04F0
    CUIElementShader m_moon; //0x04F8
    char pad_0508[40]; //0x0508
    CUIElementInt m_moon_intensity; //0x0530
    char pad_0538[8]; //0x0538
    CUIElementColor m_moon_color; //0x0540
    char pad_0560[16]; //0x0560
    CUIIndex m_lunar_cycle; //0x0570
    char pad_0574[28]; //0x0574
    CUIIndex m_moon_direction; //0x0590
    CUIIndex m_moon_position; //0x0594
    char pad_0598[24]; //0x0598
    CUIElementInt m_noise_frequency; //0x05A8
    CUIElementInt m_noise_scale; //0x05B0
    CUIElementInt m_noise_threshold; //0x05B8
    CUIElementInt m_noise_softness; //0x05C0
    CUIElementInt m_noise_density_offset; //0x05C8
    CUIElementShader m_noise; //0x05D8

    void set_sky(bool enabled) {
        *(bool*)((uint64_t)this - 0x60) = enabled;
    }
}; //Size: 0x05E8
static_assert(sizeof(CUIWeather) == 0x5E8);

namespace rage
{
    class grcTexture;
}

struct CWeatherVfx 
{
    uint32_t m_name_hash;
    char pad_1[4];
    float m_velocity;
    float m_gravity;
    char pad_2[16];
    rage::fvector4 m_box_centre_offset;
    rage::fvector4 m_box_size;
    rage::fvector4 m_life_min_max;
    rage::fvector4 m_velocity_min;
    rage::fvector4 m_velocity_max;
    char pad_3[32];
    rage::grcTexture* m_texture_1;
    rage::grcTexture* m_texture_2;
    rage::fvector4 m_texture_rows_cols_start_end;
    rage::fvector4 m_texture_anim_rate_scale_over_life_start_2_end_2;
    rage::fvector4 m_size_min_max;
    rage::fvector4 m_color;
    rage::fvector2 m_fade_in_out;
    rage::fvector2 m_fade_near_far;
    rage::fvector4 m_fade_grd_off_lo_hi;
    rage::fvector4 m_rot_speed_min_max;
    rage::fvector4 m_directional_z_offset_min_max;
    rage::fvector4 m_dir_vel_add_cam_speed_min_max_mult;
    float m_edge_softness;
    char pad_4[8];
    float m_particle_color_percentage;
    float m_background_distortion_visibility_percentage;
    float m_background_distortion_alpha_booster;
    float m_background_distortion_amount;
    uint32_t m_background_distortion_blur_level;
    float m_local_lights_multiplier;
    char pad_5[12];
};

struct Shader 
{
    CWeatherVfx* m_Drop;
    float m_Color[3];
    float m_Luminance = 1.f;
};

struct CVisualVfx 
{
    char pad_0000[0x2874];
    float m_shadow_light;
    float m_reflection;
    float m_gamma;
    float m_blur;
    float m_blur2;
    float m_sky_intensity;
    float m_saturation;
};

struct CLiquidVfx 
{
    float m_duration_foot;
    float m_duration_wheel_in;
    float m_life_foot;
    float m_life_wheel;
    char _0x0010[0x48];
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t a;
    char _0x005C[0x4];
};

class SniperRifleDOF
{
public:
    float m_near_start; //0x0000
    float m_near_end; //0x0004
    float m_far_start; //0x0008
    float m_far_end; //0x000C
}; //Size: 0x0010
static_assert(sizeof(SniperRifleDOF) == 0x10);
#pragma pack(pop)