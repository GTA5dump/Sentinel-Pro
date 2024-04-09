#pragma once 
#include "../Common.hpp"
#include "../GTA/enums.hpp"
#include "../GTA/Invoker/NativeTypes.hpp"

namespace Sentinel::Lists
{
	static constexpr std::array<std::string, 11> g_GTAFontColors =
	{
		"~w~",
		"~r~",
		"~b~",
		"~g~",
		"~y~",
		"~p~",
		"~q~",
		"~o~",
		"~c~",
		"~m~",
		"~u~"
	};

	static constexpr std::array<std::string, 4> g_GTAFontTypes =
	{
		"",
		"~italic~",
		"~bold~",
		"~italic~~bold~"
	};

	static constexpr std::array<std::string, 6> g_Strippers =
	{
		"HCH",
		"INF",
		"JUL",
		"NIK",
		"SAP",
		"TXI"
	};

	static constexpr std::array<std::string, 3> g_SextTypes =
	{
		"1ST",
		"2ND",
		"NEED"
	};

	static constexpr std::array<std::tuple<int, int>, 6> g_BlipColors
	{
		{ 
			{ static_cast<int>(eBlipIcons::Circle), static_cast<int>(eBlipColors::YellowMission) },
			{ static_cast<int>(eBlipIcons::Circle), static_cast<int>(eBlipColors::YellowMission2) },
			{ static_cast<int>(eBlipIcons::Circle), static_cast<int>(eBlipColors::Mission) },
			{ static_cast<int>(eBlipIcons::RaceFinish), static_cast<int>(eBlipColors::None) },
			{ static_cast<int>(eBlipIcons::Circle), static_cast<int>(eBlipColors::Green) },
			{ static_cast<int>(eBlipIcons::Circle), static_cast<int>(eBlipColors::Blue) }
		}
	};

	static constexpr std::array<Hash, 6> g_ShootBulletHashes =
	{
		453432689, // Pistol
		911657153, // Tazer
		-1045183535, // Heavy Revolver
		100416529, // Sniper
		-1074790547, // Rifle
		1198256469 // Lazer
	};

	static constexpr std::array<Hash, 32> g_MexicanHashes =
	{
		-927261102, 321657486, -1635724594, -527186490, 2124742566, 810804565,
		-715445259, -317922106, 648372919, -1059388209, -199280229, 1329576454,
		-1561829034, -1445349730, -2088436577, -162605104, -424905564, -1872961334,
		663522487, 846439045, 62440720, 416176080, -1332260293, 32417469, 193817059,
		1520708641, -995747907, -100858228, 1224690857, 1641152947, -2039163396
		-1029146878
	};

	static constexpr std::array<Hash, 68> g_BlackHashes =
	{
		-781039234, -1513650250, -815646164, 1669696074, 599294057, -1410400252, -1492432238, 361513884, 588969535, -198252413, 1077785853, -408329255,
		-1613485779, 579932932, -1249041111, 1870669624, -459818001, 766375082, -398748745, 866411749, -613248456, -2077218039, 1309468115, 1482427218, 988062523, -1567723049,
		-1692214353, -2078561997, 891398354, 51789996, -396800478, 1099825042, -573920724, 706935758, 1706635382, 1162230285, 666718676, 1985653476, 803106487, 1191548746, -304305299,
		-1010001291, 1746653202, 1596003233, -449965460, -1031795266, 1624626906, -614546432, 416176080, -1332260293, -1519524074, -1398552374, -1007618204, 238213328, -1948675910, 1082572151,
		-1731772337, 469792763, 228715206, 1123963760, 226559113, 1461287021, -567724045, 516505552, 1787764635, 390939205, 1404403376, 1264851357
	};

	static constexpr std::array<Hash, 9> g_PlaneHashes =
	{
		0xF7004C86,
		0xFE0A508C,
		0x6CBD1D6D,
		0xEDA4ED97,
		0x39D6779E,
		0x39D6E83F,
		0x250B0C5E,
		0x9D80F93,
		0xB79C1BF5
	};

	static constexpr std::array<Hash, 9> g_HelicopterHashes =
	{
		0x46699F47,
		0x31F0B376,
		0x11962E49,
		0x2F03547B,
		0xFCFCB68B,
		0x2C634FBD,
		0x1517D4D9,
		0xFB133A17,
		0xEBC24DF2
	};

	static constexpr std::array<Hash, 9> g_BoatHashes =
	{
		0x3D961290,
		0x33581161,
		0xC2974024,
		0x1A144F2A,
		0x82CAC433,
		0x56590FE9,
		0x2DFF622F,
		0xC1CE1183,
		0x9A474B5E
	};

	static constexpr std::array<Hash, 9> g_SuperHashes =
	{
		0xB779A091,
		0x9AE6DDA1,
		0x52FF9437,
		0x5502626C,
		0x4992196C,
		0x85E8E76B,
		0x3DA47243,
		0xECA6B6A3,
		0x9734F3EA
	};

	static constexpr std::array<Hash, 9> g_MilitaryHashes =
	{
		0xCEEA3F4B,
		0xFE141DA6,
		0xF34DFB25,
		0x132D5A1A,
		0xAA6F980A,
		0xBBA2A2F7,
		0x2EA68690,
		0x8FD54EBB,
		0xB53C6C52
	};

	static constexpr std::array<Hash, 15> g_MoneyDropHashes =
	{
		0x113FD533,
		0x6B795EBC,
		0x1AFA6A0A,
		0xC079B265,
		0xE3BA450,
		0x772114C9,
		0xF046EA37,
		0xA4E7E0A7,
		0xE6CB661E,
		0x6DB9599A,
		0xF0A61994,
		0xB4861EB7,
		0x6DE6824F,
		0x6F9939C7,
		0x4B3D240F	
	};

	static constexpr std::array<std::pair<const char*, std::pair<const char*, const char*>>, 4> g_SexualAnimations =
	{
		{
			{ "Doggystyle Female", std::make_pair("rcmpaparazzo_2", "shag_loop_poppy") },
			{ "Doggystyle Male", std::make_pair("rcmpaparazzo_2", "shag_loop_a") },
			{ "Shaking Ass", std::make_pair("switch@trevor@mocks_lapdance", "001443_01_trvs_28_idle_stripper") },
			{ "Slow Humping", std::make_pair("misscarsteal2pimpsex", "shagloop_pimp") }
		}
	};

	static constexpr std::array<std::pair<const char*, std::pair<const char*, const char*>>, 3> g_AnimalAnimations =
	{
		{
			{ "Monkey", std::make_pair("missfbi5ig_30monkeys", "monkey_b_freakout_loop") },
			{ "Chop Hump", std::make_pair("missfra0_chop_find", "hump_loop_chop") },
			{ "Chop Swim", std::make_pair("creatures@rottweiler@swim@", "swim") }
		}
	};

	static constexpr std::array<std::pair<const char*, std::pair<const char*, const char*>>, 17> g_ActionAnimations =
	{
		{
			{ "Air Guitar", std::make_pair("anim@mp_player_intcelebrationfemale@air_guitar", "air_guitar") },
			{ "Blow Kiss", std::make_pair("anim@mp_player_intcelebrationfemale@blow_kiss", "blow_kiss") },
			{ "Bro Hug", std::make_pair("anim@mp_player_intcelebrationpaired@f_m_bro_hug", "bro_hug_right") },
			{ "Finger", std::make_pair("anim@mp_player_intcelebrationmale@finger", "finger") },
			{ "Hands Up", std::make_pair("mp_pol_bust_out", "guard_handsup_loop") },
			{ "Hump Air", std::make_pair("anim@mp_player_intcelebrationfemale@air_shagging", "air_shagging") },
			{ "Jazz Hands", std::make_pair("anim@mp_player_intcelebrationmale@jazz_hands", "jazz_hands") },
			{ "Nose Pick", std::make_pair("anim@mp_player_intcelebrationmale@nose_pick", "nose_pick") },
			{ "Photographer", std::make_pair("anim@mp_player_intcelebrationmale@photography", "photography") },
			{ "Salute", std::make_pair("anim@mp_player_intcelebrationmale@salute", "salute") },
			{ "Shush", std::make_pair("anim@mp_player_intcelebrationmale@shush", "shush") },
			{ "Slow Clap", std::make_pair("anim@mp_player_intcelebrationmale@slow_clap", "slow_clap") },
			{ "Smoke", std::make_pair("anim@mp_player_intcelebrationmale@smoke_flick", "smoke_flick") },
			{ "Surrender", std::make_pair("anim@mp_player_intcelebrationmale@surrender", "surrender") },
			{ "Synth", std::make_pair("anim@mp_player_intcelebrationfemale@air_synth", "air_synth") },
			{ "Thumbs Up", std::make_pair("anim@mp_player_intcelebrationmale@thumbs_up", "thumbs_up") },
			{ "Wank", std::make_pair("mp_player_intwank", "mp_player_int_wank") }
		}
	};

	static constexpr std::array<std::pair<const char*, std::pair<const char*, const char*>>, 7> g_DanceAnimations =
	{
		{
			{ "Casual", std::make_pair("rcmnigel1bnmt_1b", "dance_loop_tyler") },
			{ "Clown", std::make_pair("rcm_barry2", "clown_idle_6") },
			{ "Pole", std::make_pair("mini@strip_club@pole_dance@pole_dance3", "pd_dance_03") },
			{ "Private", std::make_pair("mini@strip_club@private_dance@part2", "priv_dance_p2") },
			{ "Receive", std::make_pair("mp_am_stripper", "lap_dance_player") },
			{ "Sexual", std::make_pair("mini@strip_club@pole_dance@pole_a_2_stage", "pole_a_2_stage") },
			{ "Yacht", std::make_pair("oddjobs@assassinate@multi@yachttarget@lapdance", "yacht_ld_f") }
		}
	};

	static constexpr std::array<std::pair<const char*, std::pair<const char*, const char*>>, 15> g_MiscAnimations =
	{
		{
			{ "Electrocute", std::make_pair("ragdoll@human", "electrocute") },
			{ "Hover", std::make_pair("swimming@base", "dive_idle") },
			{ "Jump", std::make_pair("move_jump", "jump_launch_l_to_skydive") },
			{ "Meditate", std::make_pair("rcmcollect_paperleadinout", "meditiate_idle") },
			{ "Party", std::make_pair("rcmfanatic1celebrate", "celebrate") },
			{ "Pissing", std::make_pair("misscarsteal2peeing", "peeing_loop") },
			{ "Push Ups", std::make_pair("rcmfanatic3", "ef_3_rcm_loop_maryann") },
			{ "Run", std::make_pair("move_m@alien", "alien_run") },
			{ "Shitting", std::make_pair("missfbi3ig_0", "shit_loop_trev") },
			{ "Showering", std::make_pair("mp_safehouseshower@male@", "male_shower_idle_b") },
			{ "Swim", std::make_pair("swimming@scuba", "dive_idle") },
			{ "Vomit", std::make_pair("missfam5_blackout", "vomit") },
			{ "Wave Forward", std::make_pair("friends@frj@ig_1", "wave_d") },
			{ "Wave Hands High", std::make_pair("random@prisoner_lift", "arms_waving") },
			{ "Wave One Arm", std::make_pair("random@shop_gunstore", "_greeting") }
		}
	};

	static constexpr std::array<std::pair<const char*, const char*>, 6> g_AmbientScenarios =
	{
		{
			{ "Freeway Bum", "WORLD_HUMAN_BUM_FREEWAY" },
			{ "Sleeping Bum", "WORLD_HUMAN_BUM_SLUMPED" },
			{ "Cheer", "WORLD_HUMAN_CHEERING" },
			{ "Partying", "WORLD_HUMAN_PARTYING" },
			{ "Shine Flashlight", "WORLD_HUMAN_SECURITY_SHINE_TORCH" },
			{ "Tourist Map", "WORLD_HUMAN_TOURIST_MAP" }
		}
	};

	static constexpr std::array<std::pair<const char*, const char*>, 7> g_ActivityScenarios =
	{
		{
			{ "Binoculars", "WORLD_HUMAN_BINOCULARS" },
			{ "Drunk", "WORLD_HUMAN_DRINKING" },
			{ "Human Statue", "WORLD_HUMAN_HUMAN_STATUE" },
			{ "Musician", "WORLD_HUMAN_MUSICIAN" },
			{ "Fishing", "WORLD_HUMAN_STAND_FISHING" },
			{ "BBQ", "PROP_HUMAN_BBQ" },
			{ "Yoga", "WORLD_HUMAN_YOGA" }
		}
	};

	static constexpr std::array<std::pair<const char*, const char*>, 6> g_AnimalScenarios =
	{
		{
			{ "Grazing Boar", "WORLD_BOAR_GRAZING" },
			{ "Grazing Cow", "WORLD_COW_GRAZING" },
			{ "Coyote Howl", "WORLD_COYOTE_HOWL" },
			{ "Coyote Rest", "WORLD_COYOTE_REST" },
			{ "Coyote Wander", "WORLD_COYOTE_WANDER" },
			{ "Rottweiler Bark", "WORLD_DOG_BARKING_ROTTWEILER" }
		}
	};

	static constexpr std::array<std::pair<const char*, const char*>, 5> g_FitnessScenarios =
	{
		{
			{ "Idle Jogger", "WORLD_HUMAN_JOG_STANDING" },
			{ "Muscle Flex", "WORLD_HUMAN_MUSCLE_FLEX" },
			{ "Free Weights", "WORLD_HUMAN_MUSCLE_FREE_WEIGHTS" },
			{ "Tennis", "WORLD_HUMAN_TENNIS_PLAYER" },
			{ "Bench Press", "PROP_HUMAN_SEAT_MUSCLE_BENCH_PRESS" }
		}
	};

	static constexpr std::array<std::pair<const char*, const char*>, 7> g_IndustrialScenarios =
	{
		{
			{ "Parking Attendant", "WORLD_HUMAN_CAR_PARK_ATTENDANT" },
			{ "Drill", "WORLD_HUMAN_CONST_DRILL" },
			{ "Leaf Blower", "WORLD_HUMAN_GARDENER_LEAF_BLOWER" },
			{ "Hammering", "WORLD_HUMAN_HAMMERING" },
			{ "Janitor", "WORLD_HUMAN_JANITOR" },
			{ "Maid", "WORLD_HUMAN_MAID_CLEAN" },
			{ "Welding", "WORLD_HUMAN_WELDING" }
		}
	};

	static constexpr std::array<std::pair<const char*, const char*>, 9> g_ClipsetMotion =
	{
		{
			{ "Businessman", "move_m@business@a" },
			{ "Gangster", "move_m@gangster@generic" },
			{ "Hero", "move_m@brave@a" },
			{ "Hiker", "move_m@hiking" },
			{ "Hobo", "move_m@hobo@a" },
			{ "Injured", "move_m@injured" },
			{ "Quick", "move_m@quick" },
			{ "Wasted", "move_m@drunk@slightlydrunk" },
			{ "Very Wasted", "move_m@drunk@verydrunk" }
		}
	};

	static constexpr std::array<std::pair<const char*, const char*>, 7> g_WeaponClipset =
	{
		{
			{ "Bag", "missfbi4prepp1_garbageman" },
			{ "Bucket", "move_ped_wpn_bucket" },
			{ "Cane", "move_lester_CaneUp" },
			{ "Jerry Can", "move_ped_wpn_jerrycan_generic" },
			{ "Mop", "move_ped_wpn_mop" },
			{ "Rifle", "Wpn_AssaultRifle_WeaponHoldingCrouched" },
			{ "Sack", "MOVE_P_M_ZERO_RUCKSACK" }
		}
	};

	static constexpr std::array<std::pair<const char*, Hash>, 9> g_PopularModels =
	{
		{
			{ "Alien", 0x64611296 },
			{ "Astronaut", 0xE7B31432 },
			{ "Playboy (Naked)", 0x4498DDE },
			{ "Hooker", 0x28ABF95 },
			{ "Pogo", 0xDC59940D },
			{ "Stripper", 0x52580019 },
			{ "Swat", 0x8D8F1B10 },
			{ "Topless", 0x9CF26183 },
			{ "Zombie", 0xAC4B4506 }
		}
	};

	static constexpr std::array<std::pair<const char*, Hash>, 25> g_StoryModels =
	{
		{
			{ "Michael", 0xD7114C9 },
			{ "Franklin", 0x9B22DBAF },
			{ "Trevor", 0x9B810FA2 },
			{ "Amanda", 0x6D1E15F7 },
			{ "Ashley", 0x7EF440DB },
			{ "Brad", 0xBDBB4922 },
			{ "Chef", 0x49EADBF6 },
			{ "Chris Formage", 0x286E54A7 },
			{ "Clag", 0x6CCFE08A },
			{ "Cletus", 0xE6631195 },
			{ "Dave Norton", 0x15CD4C33 },
			{ "Denise", 0x820B33BD },
			{ "Fabien", 0xD090C350 },
			{ "Floyd", 0xB1B196B2 },
			{ "Jay Norris", 0x7A32EE74 },
			{ "Jimmy", 0x570462B9 },
			{ "Lamar", 0x65B93076 },
			{ "Lazlow", 0xDFE443E5 },
			{ "Lester", 0x4DA6E849 },
			{ "Mrs Phillips", 0x3862EEA8 },
			{ "Stretch", 0x36984358 },
			{ "Tao Cheng", 0xDC5C5EA5 },
			{ "Tennis Coach", 0xA23B5F57 },
			{ "Tracy", 0xDE352A35 },
			{ "Wade", 0x92991B72 }
		}
	};

	static constexpr std::array<std::pair<const char*, Hash>, 20> g_AnimalModels =
	{
		{
			{ "Boar", 0xCE5FF074 },
			{ "Chimp", 0xA8683715 },
			{ "Chop", 0x14EC17EA },
			{ "Cow", 0xFCFA9E1E },
			{ "Coyote", 0x644AC75E },
			{ "Crow", 0x18012A9F },
			{ "Deer", 0xD86B5A95 },
			{ "Fish", 0x2FD800B7 },
			{ "Chicken Hawk", 0xAAB71F62 },
			{ "Hen", 0x6AF51FAF },
			{ "Husky", 0x4E8F95A2 },
			{ "Mountain Lion", 0x1250D7BA },
			{ "Pig", 0xB11BAB56 },
			{ "Pigeon", 0x6A20728 },
			{ "Retriever", 0x349F33E1 },
			{ "Rhesus", 0xC2D06F53 },
			{ "Rottweiler", 0x9563221D },
			{ "Seagull", 0xD3939DFD },
			{ "Tiger Shark", 0x6C3F072 },
			{ "Shepherd", 0x431FC24C }
		}
	};

	static constexpr std::array<std::pair<const char*, Hash>, 16> g_EmergencyModels =
	{
		{
			{ "Black Ops", 0xB3F3EE34 },
			{ "Chem Security", 0x2EFEAFD5 },
			{ "City Cop", 0x5E3DA4A4 },
			{ "County Cop", 0x9AB35F63 },
			{ "Fireman", 0xB6B1EDA8 },
			{ "Security Guard", 0x709220C7 },
			{ "Highway Cop", 0x739B1EF5 },
			{ "Lifeguard", 0xB4A6862 },
			{ "Marine", 0x72C0CAD2 },
			{ "Marine (Senior)", 0xF0259D83 },
			{ "Paramedic", 0xB353629E },
			{ "Ranger", 0xEF7135AE },
			{ "Prison Guard", 0x56C96FC6 },
			{ "Security Guard", 0xD768B228 },
			{ "Sheriff", 0xB144F9B9 },
			{ "Snow Cop", 0x1AE8BB58 }
		}
	};

	static constexpr std::array<std::pair<const char*, Hash>, 14> g_RoleModels =
	{
		{
			{ "Bouncer", 0x9FD4292D },
			{ "Bride", 0x6162EC47 },
			{ "Pest Control", 0x48114518 },
			{ "Doctor", 0xD47303AC },
			{ "Factory Worker", 0x4163A158 },
			{ "Garbage Man", 0xEE75A00F },
			{ "Groom", 0xFECE8B85 },
			{ "Hunter", 0xCE1324DE },
			{ "Jesus", 0xCE2CB751 },
			{ "Mime", 0x3CDCA742 },
			{ "Priest", 0x6437E77D },
			{ "Prisoner", 0xB1BB9B59 },
			{ "Scientist", 0x4117D39B },
			{ "Spy", 0x5B81D86C }
		}
	};

	static constexpr std::array<std::pair<const char*, Hash>, 6> g_AmbientModels =
	{
		{
			{ "Body Builder", 0xDA116E7E },
			{ "Nonce", 0x7DC3908F },
			{ "Guido", 0xC6B49A2F },
			{ "Statue", 0x795AC7A8 },
			{ "Mani", 0xC8BB1E52 },
			{ "Griff", 0xC454BCBB }
		}
	};

	static constexpr std::array<std::pair<const char*, Hash>, 5> g_MiscModels =
	{
		{
			{ "Dead Hooker", 0x73DEA88B },
			{ "Imporage", 0x348065F5 },
			{ "Marston", 0x38430167 },
			{ "Misty", 0xD128FF9D },
			{ "Niko", 0xEEDACFC9 }
		}
	};

	static constexpr std::array<std::pair<const char*, const char*>, 64> g_TimecycleVisions =
	{
		{
			{ "Bank", "Bank_HLWD" },
			{ "Stoned", "Barry1_Stoned" },
			{ "Fade Out", "BarryFadeOut" },
			{ "Bikers", "Bikers" },
			{ "Bikers Splash", "BikersSPLASH" },
			{ "Black and White", "blackNwhite" },
			{ "Blackout", "BlackOut" },
			{ "Bloom", "Bloom" },
			{ "Bloom Light", "BloomLight" },
			{ "Building Top", "buildingTOP" },
			{ "Bullet Time Light", "BulletTimeLight" },
			{ "Bullet Time Dark", "BulletTimeDark" },
			{ "Camera Black and White", "CAMERA_BW" },
			{ "CCTV", "CAMERA_secuirity" },
			{ "Fuzzy CCTV", "CAMERA_secuirity_FUZZ" },
			{ "Canyon Mission", "canyon_mission" },
			{ "Carmod Underpass", "carMOD_underpass" },
			{ "Carpark 1", "carpark" },
			{ "Carpark 2", "carpark_dt1_02" },
			{ "Carpark 3", "carpark_dt1_03" },
			{ "Cash Depot", "cashdepot" },
			{ "Cash Depot Emergency", "cashdepotEMERGENCY" },
			{ "Bank Back", "cBank_back" },
			{ "Bank Front", "cBank_front" },
			{ "Tunnel Whitelight", "ch2_tunnel_whitelight" },
			{ "Water", "CH3_06_water" },
			{ "Chop", "CHOP" },
			{ "Cinema 1", "cinema" },
			{ "Cinema 2", "cinema_001" },
			{ "Cops", "cops" },
			{ "Cops Splash", "CopsSPLASH" },
			{ "Crane Cam", "crane_cam" },
			{ "Crane Cam Cinematic", "crane_cam_cinematic" },
			{ "Railway Tunnel", "CS1_railwayB_tunnel" },
			{ "Tunnel", "CS3_rail_tunnel" },
			{ "Streetlight", "CUSTOM_streetlight" },
			{ "Damage", "damage" },
			{ "Death", "death" },
			{ "Default Color Code", "DefaultColorCode" },
			{ "Overide Sunpos", "DONT_overide_sunpos" },
			{ "Dont Taze Me Bro", "Dont_tazeme_bro" },
			{ "FIB Cascades", "downtown_FIB_cascades_opt" },
			{ "Driving Focus Light", "DrivingFocusLight" },
			{ "Driving Focus Dark", "DrivingFocusDark" },
			{ "Drug Drive", "DRUG_2_drive" },
			{ "Deadman", "Drug_deadman" },
			{ "Deadman Blend", "Drug_deadman_blend" },
			{ "Drive Blend", "drug_drive_blend01" },
			{ "Drive Flying", "drug_flying_01" },
			{ "Huffin Gas", "DRUG_gas_huffin" },
			{ "Wobbly", "drug_wobbly" },
			{ "Drunk", "Drunk" },
			{ "Dying", "dying" },
			{ "Epsilion", "epsilion" },
			{ "Epsilion Josh", "ExplosionJosh" },
			{ "Facebook New", "Facebook_NEW" },
			{ "Facebook Server Room", "facebook_serveroom" },
			{ "Bank Finale", "FinaleBank" },
			{ "Forest", "Forest" },
			{ "Prologue", "prologue" },
			{ "Pulse", "pulse" },
			{ "Race Turbo Light", "RaceTurboLight" },
			{ "Race Turbo Dark", "RaceTurboDark" },
			{ "Race Turbo Flash", "RaceTurboFlash" }
		}
	};

	static constexpr std::array<Hash, 8> g_BodyguardModelHashes =
	{
		0x64611296,
		0x04498DDE,
		0xDC59940D,
		0xCE2CB751,
		0xDA116E7E,
		0xAC4B4506,
		0x28ABF95,
		0x52580019
	};

	static constexpr std::array<Hash, 8> g_BodyguardWeaponHashes =
	{
		0x1B06D571,
		0x3656C8C1,
		0xB1CA77B1,
		0x42BF8A85,
		0xBFEFFF6D,
		0xB62D1F67,
		0x476BF155,
		0x61012683
	};

	static constexpr std::array<Hash, 14> g_BulletChangerHashes =
	{
		0x7F7497E5,
		0x93E220BD,
		0x24B17070,
		0xBEFDC581,
		0xB1CA77B1,
		1945616459,
		1566990507,
		0xEFFD014B,
		0x23C9F95C,
		0x787F0BB,
		0xFDBC8A50,
		0x497FACC3,
		0x2C3731D9,
		0xAB564B93
	};

	static constexpr std::array<Hash, 14> g_EntityGunObjects =
	{
		0xDF9841D7,
		0x757C28D,
		0x6B795EBC,
		0xE2FBA8D8,
		0xC079B265,
		0xC79A987E,
		0xE6CB661E,
		0xD5C0BC07,
		0xF046EA37,
		0xB2274905,
		0x6F9939C7,
		0x113FD533,
		0x4B3D240F,
		0x83C07621
	};

	static constexpr std::array<Hash, 23> g_EntityGunVehicles =
	{
		0x5A82F9AE,
		0x5BA0FF1E,
		0x18619B7E,
		0x322CF98F,
		0xB2E046FB,
		0xE83C17,
		0xA29F78B0,
		0x42BC5E19,
		0x1FD824AF,
		0x8B213907,
		0x2C1FEA99,
		0xEA6A047F,
		0x432AA566,
		0xEB298297,
		0xB44F0582,
		0x9472CD24,
		0x23CA25F2,
		0x619C1B82,
		0xB472D2B5,
		0xC2974024,
		0x761E2AD3,
		0xB779A091,
		0x6322B39A
	};

	static constexpr std::array<Hash, 12> g_EntityGunPeds =
	{
		0x8D8AC8B9,
		0xFCFA9E1E,
		0xD3939DFD,
		0x9CF26183,
		0x6162EC47,
		0x28ABF95,
		0xE7B31432,
		0x795AC7A8,
		0x3CDCA742,
		0xCE2CB751,
		0xDC59940D,
		0xAC4B4506
	};

	static constexpr std::array<Hash, 7> g_AimAssistBones =
	{
		0x796E,
		0x9995,
		0xE0FD,
		0x49D9,
		0xDEAD,
		0x3779,
		0xCC4D
	};

	static constexpr std::array<Hash, 17> g_MeleeWeapons =
	{
		0x92A27487, 
		0x958A4A8F, 
		0xF9E6AA4B, 
		0x84BD7BFD, 
		0x8BB05FD7, 
		0x440E4788, 
		0x4E875F73, 
		0xF9DCBF2D, 
		0xD8DF3C3C, 
		0x99B507EA, 
		0xDD5DF8D9, 
		0xDFE37640, 
		0x678B81B1, 
		0x19044EE0,
		0xCD274149,
		0x94117305, 
		0x3813FC08
	};

	static constexpr std::array<Hash, 19> g_Handguns =
	{
		0x1B06D571,
		0xBFE256D4,
		0x5EF9FEC4,
		0x22D8FE39,
		0x3656C8C1,
		0x99AEEB3B,
		0xBFD21232,
		0x88374054,
		0xD205520E,
		0x83839C4,
		0x47757124,
		0xDC4DB296,
		0xC1B3C3D1,
		0xCB96392F,
		0x97EA20B8,
		0xAF3696A1,
		0x2B5EF5EC,
		0x917F6C8C,
		0x57A4368C
	};

	static constexpr std::array<Hash, 8> g_SubmachineGuns =
	{
		0x13532244, 
		0x2BE6766B, 
		0x78A97CD0, 
		0xEFE7E2DF, 
		0x0A3D4D34, 
		0xDB1AA450, 
		0xBD248B55, 
		0x476BF155
	};

	static constexpr std::array<Hash, 10> g_Shotguns =
	{
		0x1D073A89, 
		0x555AF99A, 
		0x7846A318,
		0xE284C527, 
		0x9D61E50F, 
		0xA89CB99E, 
		0x3AABBBAA, 
		0xEF951FBB, 
		0x12E82D3D, 
		0x5A96BA4
	};

	static constexpr std::array<Hash, 13> g_AssaultRifles =
	{
		0xBFEFFF6D, 
		0x394F415C, 
		0x83BF0278, 
		0xFAD1F1C9, 
		0xAF113F99, 
		0xC0A3098D, 
		0x969C3D67, 
		0x7F229F94, 
		0x84D6FAFD, 
		0x624FE830,
		0x9D1F17E6, 
		0xC78D71B4, 
		0xD1D5F52B
	};

	static constexpr std::array<Hash, 4> g_LightMachineGuns =
	{
		0x9D07F764, 
		0x7FD62962, 
		0xDBBD7280, 
		0x61012683
	};

	static constexpr std::array<Hash, 6> g_SniperRifles =
	{
		0x05FC3C11, 
		0x0C472FE2, 
		0xA914799,
		0xC734385A, 
		0x6A6C02E0,
		0x6E7DDDEC
	};

	static constexpr std::array<Hash, 10> g_HeavyWeapons =
	{
		0xB1CA77B1, 
		0xA284510B,
		0x4DD2DC56, 
		0x42BF8A85, 
		0x7F7497E5, 
		0x6D544C99, 
		0x63AB0442, 
		0x0781FE4A, 
		0xB62D1F67, 
		0xDB26713A
	};

	static constexpr std::array<Hash, 10> g_Throwables =
	{
		0x93E220BD, 
		0xA0973D5E, 
		0x24B17070, 
		0x2C3731D9,
		0xAB564B93, 
		0x787F0BB, 
		0xBA45E8B8, 
		0x23C9F95C, 
		0xFDBC8A50, 
		0x497FACC3
	};

	static constexpr std::array<Hash, 5> g_MiscellaneousWeapons =
	{
		0x34A67B97, 
		0xFBAB5776, 
		0x060EC506, 
		0xBA536372, 
		0x184140A1
	};

	static constexpr std::array<std::string, 9> g_VehicleBones = 
	{
		"CHASSIS",
		"WHEEL_LF",
		"WHEEL_RF",
		"WHEEL_LR",
		"WHEEL_RR",
		"HEADLIGHT_L",
		"HEADLIGHT_R",
		"TAILLIGHT_L",
		"TAILLIGHT_R"
	};

	static constexpr std::array<std::pair<const char*, const char*>, 7> g_VehicleTrailParticles =
	{
		{
			{ "core", "muz_rpg" },
			{ "scr_rcbarry1", "scr_alien_teleport" },
			{ "scr_rcbarry1", "scr_alien_disintegrate" },
			{ "scr_rcbarry2", "scr_clown_death" },
			{ "scr_rcbarry2", "scr_clown_appears" },
			{ "scr_martin1", "ent_amb_elec_crackle_sp" },
			{ "scr_rcbarry1", "scr_alien_impact_bul" }
		}
	};

	static constexpr std::array<std::pair<Vector3, Vector3>, 10> g_VehicleAcrobatics =
	{
		{
			{ { 0.f, 0.f, 1.f }, { 0.f, 5.0f, 0.f } },
			{ { 0.f, 0.0f, 11.5f }, { 5.f, 0.f, 0.f } },
			{ { 0.f, 0.0f, 21.f }, { 10.f, 0.f, 0.f } },
			{ { 0.f, 0.0f, 33.f }, { 15.f, 0.f, 0.f } },
			{ { 0.f, 0.f, 20.f }, { 0.f, 0.f, 0.f } },
			{ { 0.f, 0.f, 200.f }, { 0.f, 0.f, 0.f } },
			{ { 0.f, 0.f, 15.5f }, { 0.f, 1.3f, 0.f } },
			{ { 0.f, 0.f, 15.5f }, { 0.f, -1.f, 0.f } },
			{ { 0.f, 0.f, 2.8f }, { -3.f, 0.f, 0.f } },
			{ { 0.f, 0.f, 2.8f }, { 3.f, 0.f, 0.f } }
		}
	};

	static constexpr std::array<Hash, 9> g_VehicleEngines =
	{
		0x52FF9437,
		0x2EA68690,
		0xB779A091,
		0x831A21D5,
		0xCFCA3668,
		0xF9300CC5,
		0x5A82F9AE,
		0xCD93A7DB,
		0x1446590A
	};

	static constexpr std::array<std::uint64_t, 10> g_OnlineNativeChecks =
	{
		0xA4A79DD2D9600654, // NETWORK_GET_NUM_CONNECTED_PLAYERS
		0xA6C90FBC38E395EE, // NETWORK_GET_NUM_PARTICIPANTS
		0xCF61D4B4702EE9EB, // NETWORK_GET_TOTAL_NUM_PLAYERS
		0x10FAB35428CCC9D7, // NETWORK_IS_GAME_IN_PROGRESS
		0xCA97246103B63917, // NETWORK_IS_IN_SESSION
		0xB8DFD30D6973E135, // NETWORK_IS_PLAYER_ACTIVE
		0x93DC1BE4E1ABE9D1, // NETWORK_IS_PLAYER_CONNECTED
		0xD83C2B94E7508980, // NETWORK_IS_SESSION_ACTIVE
		0x9DE624D2FC4B603F, // NETWORK_IS_SESSION_STARTED
		0x1077788E268557C2	// NETWORK_IS_SIGNED_ONLINE
	};

	static constexpr std::array<std::uint64_t, 5> g_ModelNatives =
	{
		0xAF35D0D2583051B0, // CREATE_VEHICLE
		0x509D5878EB39E842, // CREATE_OBJECT
		0x9A294B2138ABB884, // CREATE_OBJECT_NO_OFFSET
		0xD49F9B0955C367DE, // CREATE_PED
		0x7DD959874C1FD534	// CREATE_PED_INSIDE_VEHICLE
	};

	static constexpr std::array<std::string, 15> g_WeatherTypes =
	{
		"EXTRASUNNY",
		"CLEAR",
		"CLOUDS",
		"SMOG",
		"FOGGY",
		"OVERCAST",
		"RAIN",
		"THUNDER",
		"CLEARING",
		"NEUTRAL",
		"SNOW",
		"BLIZZARD",
		"SNOWLIGHT",
		"XMAS",
		"HALLOWEEN"
	};

	static constexpr std::array<std::string, 20> g_CloudTypes =
	{
		"Cloudy 01",
		"RAIN",
		"Horizon",
		"horizonband1",
		"horizonband2",
		"horizonband3",
		"Puffs",
		"Wispy",
		"Stormy 01",
		"Clear 01",
		"Snowy 01",
		"Contrails",
		"Altostratus",
		"Nimbus",
		"Cirrus",
		"cirrocumulus",
		"stratocumulus",
		"Stripey",
		"horsey",
		"shower"
	};

	inline std::pair<uint32_t, std::string> g_VFXDropNames[] =
	{
		{ 0xE947723D, "Rainstorm Drop" },
		{ 0xF4698119, "Thunder Drop" },
		{ 0xB13E1842, "Snow Light Drop" },
		{ 0x7F84E74E, "Snow Heavy Drop" },
		{ 0xFA37E4E2, "Blizzard Drop" },
		{ 0xC11D1EC6, "Underwater Drop" },
		{ 0x69163FA7, "Lake Fog Drop" },
		{ 0x91C32C69, "Woodland Pollen Drop" },
		{ 0x8DE5FE6A, "Woodland Firefly Drop" },
		{ 0x36196668, "Wetland Flies Drop" },
	};
}
