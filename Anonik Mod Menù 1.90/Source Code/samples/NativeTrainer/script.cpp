/*
THIS FILE IS A PART OF GTA V SCRIPT HOOK SDK
http://dev-c.com
(C) Alexander Blade 2015
*/

/*
F4                  activate
NUM2/8/4/6          navigate thru the menus and lists (numlock must be on)
NUM5                select
NUM0/BACKSPACE/F4   back
NUM9/3              use vehicle boost when active
NUM+                use vehicle rockets when active
*/

#include "script.h"
#include "keyboard.h"

#include <string>
#include <ctime>

#pragma warning(disable : 4244 4305) // double <-> float conversions

int selectedPlayer;

enum ControllerInputs
{

	INPUT_FRONTEND_X = 203,
	INPUT_FRONTEND_Y = 204,
	INPUT_FRONTEND_LB = 205,
	INPUT_FRONTEND_RB = 206,
	INPUT_FRONTEND_LT = 207,
	INPUT_FRONTEND_RT = 208,
	INPUT_FRONTEND_LS = 209,
	INPUT_FRONTEND_RS = 210,
	INPUT_FRONTEND_DOWN = 187,
	INPUT_FRONTEND_UP = 188,
	INPUT_FRONTEND_LEFT = 189,
	INPUT_FRONTEND_RIGHT = 190,
	INPUT_FRONTEND_RDOWN = 191,
	INPUT_FRONTEND_RUP = 192,
	INPUT_FRONTEND_RLEFT = 193,
	INPUT_FRONTEND_RRIGHT = 194,
	INPUT_FRONTEND_ACCEPT = 201,
	INPUT_FRONTEND_CANCEL = 202,
	INPUT_PHONE = 27,
	INPUT_SCRIPT_LT = 228,
	INPUT_SCRIPT_RT = 229,
	INPUT_CELLPHONE_LEFT = 174,
	INPUT_CELLPHONE_RIGHT = 175,

};



void drawNotification(char* msg)
{
	UI::_SET_NOTIFICATION_TEXT_ENTRY("STRING");
	UI::_ADD_TEXT_COMPONENT_STRING(msg);
	UI::_DRAW_NOTIFICATION(2000, 1);
}

void Freeze(Ped selectedPed)
{
	AI::CLEAR_PED_TASKS_IMMEDIATELY(selectedPed);
	AI::CLEAR_PED_SECONDARY_TASK(selectedPed);
}

void draw_rect(float A_0, float A_1, float A_2, float A_3, int A_4, int A_5, int A_6, int A_7)
{
	GRAPHICS::DRAW_RECT((A_0 + (A_2 * 0.5f)), (A_1 + (A_3 * 0.5f)), A_2, A_3, A_4, A_5, A_6, A_7);
}



void draw_menu_line(std::string caption, float lineWidth, float lineHeight, float lineTop, float lineLeft, float textLeft, bool active, bool title, bool rescaleText = true)
{
	// default values
	int text_col[4] = { 255, 255, 255, 250.0f },
		rect_col[4] = { 0, 0, 0, 180.f };
	float text_scale = 0.35;
	int font = 0;
	bool outline = false;
	bool dropShadow = false;

	// correcting values for active line
	if (active)
	{
		text_col[0] = 0;
		text_col[1] = 0;
		text_col[2] = 0;
		text_col[3] = 215.f;

		rect_col[0] = 48;
		rect_col[1] = 122;
		rect_col[2] = 238;
		rect_col[3] = 180;//

						  //outline = true;

		if (rescaleText) text_scale = 0.40;

	}
	else if (title)
	{
		text_col[0] = 255;
		text_col[1] = 255;
		text_col[2] = 255;

		rect_col[0] = 48;
		rect_col[1] = 122;
		rect_col[2] = 238;
		rect_col[3] = 180;

		if (rescaleText) text_scale = 0.78;
		outline = true;

		font = 1;
	}
	else
	{
		outline = true;
	}


	int screen_w, screen_h;
	GRAPHICS::GET_SCREEN_RESOLUTION(&screen_w, &screen_h);

	textLeft += lineLeft;

	float lineWidthScaled = lineWidth / (float)screen_w; // line width
	float lineTopScaled = lineTop / (float)screen_h; // line top offset
	float textLeftScaled = textLeft / (float)screen_w; // text left offset
	float lineHeightScaled = lineHeight / (float)screen_h; // line height

	float lineLeftScaled = lineLeft / (float)screen_w;

	// this is how it's done in original scripts

	// text upper part
	UI::SET_TEXT_FONT(font);
	UI::SET_TEXT_SCALE(0.0, text_scale);
	UI::SET_TEXT_COLOUR(text_col[0], text_col[1], text_col[2], text_col[3]);
	UI::SET_TEXT_CENTRE(0);
	UI::SET_TEXT_DROPSHADOW(0, 0, 0, 0, 0);
	UI::SET_TEXT_EDGE(0, 0, 0, 0, 0);
	UI::_SET_TEXT_ENTRY("STRING");
	UI::_ADD_TEXT_COMPONENT_STRING((LPSTR)caption.c_str());
	UI::_DRAW_TEXT(textLeftScaled, (((lineTopScaled + 0.00278f) + lineHeightScaled) - 0.005f));

	// text lower part
	UI::SET_TEXT_FONT(font);
	UI::SET_TEXT_SCALE(0.0, text_scale);
	UI::SET_TEXT_COLOUR(text_col[0], text_col[1], text_col[2], text_col[3]);
	UI::SET_TEXT_CENTRE(0);
	UI::SET_TEXT_DROPSHADOW(0, 0, 0, 0, 0);
	UI::SET_TEXT_EDGE(0, 0, 0, 0, 0);
	UI::_SET_TEXT_GXT_ENTRY("STRING");
	UI::_ADD_TEXT_COMPONENT_STRING((LPSTR)caption.c_str());
	int num25 = UI::_0x9040DFB09BE75706(textLeftScaled, (((lineTopScaled + 0.00278f) + lineHeightScaled) - 0.005f));

	// rect
	draw_rect(lineLeftScaled, lineTopScaled + (0.00278f),
		lineWidthScaled, ((((float)(num25)* UI::_0xDB88A37483346780(text_scale, 0)) + (lineHeightScaled * 1.5f)) + 0.006f),
		rect_col[0], rect_col[1], rect_col[2], rect_col[3]);
}


bool trainer_switch_pressed()
{
	return IsKeyJustUp(VK_F4);
}

void get_button_state(bool *a, bool *b, bool *up, bool *down, bool *l, bool *r)
{
	if (a) *a = IsKeyDown(VK_NUMPAD5);
	if (b) *b = IsKeyDown(VK_NUMPAD0) || trainer_switch_pressed() || IsKeyDown(VK_BACK);
	if (up) *up = IsKeyDown(VK_NUMPAD8);
	if (down) *down = IsKeyDown(VK_NUMPAD2);
	if (r) *r = IsKeyDown(VK_NUMPAD6);
	if (l) *l = IsKeyDown(VK_NUMPAD4);
}

void menu_beep()
{
	AUDIO::PLAY_SOUND_FRONTEND(-1, "NAV_UP_DOWN", "HUD_FRONTEND_DEFAULT_SOUNDSET", 0);
}

std::string statusText;
DWORD statusTextDrawTicksMax;
bool statusTextGxtEntry;

void update_status_text()
{
	if (GetTickCount() < statusTextDrawTicksMax)
	{
		UI::SET_TEXT_FONT(0);
		UI::SET_TEXT_SCALE(0.55, 0.55);
		UI::SET_TEXT_COLOUR(255, 255, 255, 255);
		UI::SET_TEXT_WRAP(0.0, 1.0);
		UI::SET_TEXT_CENTRE(1);
		UI::SET_TEXT_DROPSHADOW(0, 0, 0, 0, 0);
		UI::SET_TEXT_EDGE(1, 0, 0, 0, 205);
		if (statusTextGxtEntry)
		{
			UI::_SET_TEXT_ENTRY((char *)statusText.c_str());
		}
		else
		{
			UI::_SET_TEXT_ENTRY("STRING");
			UI::_ADD_TEXT_COMPONENT_STRING((char *)statusText.c_str());
		}
		UI::_DRAW_TEXT(0.5, 0.5);
	}
}

void set_status_text(std::string str, DWORD time = 2500, bool isGxtEntry = false)
{
	statusText = str;
	statusTextDrawTicksMax = GetTickCount() + time;
	statusTextGxtEntry = isGxtEntry;
}

// features
bool featurePlayerInvincible = false;
bool featurePlayerInvincibleUpdated = false;
bool featurePlayerNeverWanted = false;
bool featurePlayerIgnored = false;
bool featurePlayerIgnoredUpdated = false;
bool featurePlayerUnlimitedAbility = false;
bool featurePlayerNoNoise = false;
bool featurePlayerNoNoiseUpdated = false;
bool featurePlayerFastSwim = false;
bool featurePlayerFastSwimUpdated = false;
bool featurePlayerFastRun = false;
bool featurePlayerFastRunUpdated = false;
bool featurePlayerSuperJump = false;
bool featurePlayerUltraJump = false;

bool featureWeaponNoReload = false;
bool featureWeaponFireAmmo = false;
bool featureWeaponExplosiveAmmo = false;
bool featureWeaponExplosiveMelee = false;
bool featureWeaponVehRockets = false;

DWORD featureWeaponVehShootLastTime = 0;

bool featureVehInvincible = false;
bool featureVehInvincibleUpdated = false;
bool featureVehInvincibleWheels = false;
bool featureVehInvincibleWheelsUpdated = false;
bool featureVehSeatbelt = false;
bool featureVehSeatbeltUpdated = false;
bool featureVehSpeedBoost = false;
bool featureVehWrapInSpawned = false;
bool featureWeaponVehFire = false;
bool featureWeaponVehSnow = false;
bool featureWeaponVehGreen = false;
bool featureWeaponVehRed = false;
bool featureWeaponVehTank = false;

bool featureWorldMoonGravity = false;
bool featureWorldRandomCops = true;
bool featureWorldRandomTrains = true;
bool featureWorldRandomBoats = true;
bool featureWorldGarbageTrucks = true;

bool featureTimePaused = false;
bool featureTimePausedUpdated = false;
bool featureTimeSynced = false;

bool featureWeatherWind = false;
bool featureWeatherPers = false;

bool featureMiscLockRadio = false;
bool featureMiscHideHud = false;
bool featureMiscMobile = false;

bool featureVisionNight = false;
bool featureVisionThermal = false;
bool featureVisionDrunk = false;
bool featureVisionDrunkUpdated = false;

bool featureESP = false;
bool featureNameESP = false;
bool featurePlayerESPTrace = false;
bool freezeplayer = false;

bool featurePedestrianInvincibleBodyguard = false;
bool featurePedestrianInvincibleBodyguardUpdate = false;
bool featureWeaponVehFlare = false;
bool featureWeaponMoneyGun = false;


DWORD featureWeaponVehShootLastTimeFire = 0;



// player model control, switching on normal ped model when needed 
void check_player_model()
{
	// common variables
	Player player = PLAYER::PLAYER_ID();
	Ped playerPed = PLAYER::PLAYER_PED_ID();

	if (!ENTITY::DOES_ENTITY_EXIST(playerPed)) return;

	Hash model = ENTITY::GET_ENTITY_MODEL(playerPed);
	if (ENTITY::IS_ENTITY_DEAD(playerPed) || PLAYER::IS_PLAYER_BEING_ARRESTED(player, TRUE))
		if (model != GAMEPLAY::GET_HASH_KEY("player_zero") &&
			model != GAMEPLAY::GET_HASH_KEY("player_one") &&
			model != GAMEPLAY::GET_HASH_KEY("player_two"))
		{
			set_status_text("turning to normal");
			WAIT(1000);

			model = GAMEPLAY::GET_HASH_KEY("player_zero");
			STREAMING::REQUEST_MODEL(model);
			while (!STREAMING::HAS_MODEL_LOADED(model))
				WAIT(0);
			PLAYER::SET_PLAYER_MODEL(PLAYER::PLAYER_ID(), model);
			PED::SET_PED_DEFAULT_COMPONENT_VARIATION(PLAYER::PLAYER_PED_ID());
			STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(model);

			// wait until player is ressurected
			while (ENTITY::IS_ENTITY_DEAD(PLAYER::PLAYER_PED_ID()) || PLAYER::IS_PLAYER_BEING_ARRESTED(player, TRUE))
				WAIT(0);

		}
}

void update_vehicle_guns()
{
	Player player = PLAYER::PLAYER_ID();
	Ped playerPed = PLAYER::PLAYER_PED_ID();

	if (!ENTITY::DOES_ENTITY_EXIST(playerPed) || !featureWeaponVehRockets) return;

	bool bSelect = IsKeyDown(0x6B); // num plus
	if (bSelect && featureWeaponVehShootLastTime + 150 < GetTickCount() &&
		PLAYER::IS_PLAYER_CONTROL_ON(player) && PED::IS_PED_IN_ANY_VEHICLE(playerPed, 0))
	{
		Vehicle veh = PED::GET_VEHICLE_PED_IS_USING(playerPed);

		Vector3 v0, v1;
		GAMEPLAY::GET_MODEL_DIMENSIONS(ENTITY::GET_ENTITY_MODEL(veh), &v0, &v1);

		Hash weaponAssetRocket = GAMEPLAY::GET_HASH_KEY("WEAPON_VEHICLE_ROCKET");
		if (!WEAPON::HAS_WEAPON_ASSET_LOADED(weaponAssetRocket))
		{
			WEAPON::REQUEST_WEAPON_ASSET(weaponAssetRocket, 31, 0);
			while (!WEAPON::HAS_WEAPON_ASSET_LOADED(weaponAssetRocket))
				WAIT(0);
		}

		Vector3 coords0from = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(veh, -(v1.x + 0.25f), v1.y + 1.25f, 0.1);
		Vector3 coords1from = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(veh, (v1.x + 0.25f), v1.y + 1.25f, 0.1);
		Vector3 coords0to = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(veh, -v1.x, v1.y + 100.0f, 0.1f);
		Vector3 coords1to = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(veh, v1.x, v1.y + 100.0f, 0.1f);

		GAMEPLAY::SHOOT_SINGLE_BULLET_BETWEEN_COORDS(coords0from.x, coords0from.y, coords0from.z,
			coords0to.x, coords0to.y, coords0to.z,
			250, 1, weaponAssetRocket, playerPed, 1, 0, -1.0);
		GAMEPLAY::SHOOT_SINGLE_BULLET_BETWEEN_COORDS(coords1from.x, coords1from.y, coords1from.z,
			coords1to.x, coords1to.y, coords1to.z,
			250, 1, weaponAssetRocket, playerPed, 1, 0, -1.0);

		featureWeaponVehShootLastTime = GetTickCount();
	}
}

bool skinchanger_used = false;

// Updates all features that can be turned off by the game, being called each game frame
void update_features()
{
	update_status_text();

	update_vehicle_guns();

	// changing player model if died/arrested while being in another skin, since it can cause inf loading loop
	if (skinchanger_used)
		check_player_model();

	// wait until player is ready, basicly to prevent using the trainer while player is dead or arrested
	while (ENTITY::IS_ENTITY_DEAD(PLAYER::PLAYER_PED_ID()) || PLAYER::IS_PLAYER_BEING_ARRESTED(PLAYER::PLAYER_ID(), TRUE))
		WAIT(0);

	// read default feature values from the game
	featureWorldRandomCops = PED::CAN_CREATE_RANDOM_COPS() == TRUE;

	// common variables
	Player player = PLAYER::PLAYER_ID();
	Ped playerPed = PLAYER::PLAYER_PED_ID();
	BOOL bPlayerExists = ENTITY::DOES_ENTITY_EXIST(playerPed);

	// player invincible
	if (featurePlayerInvincibleUpdated)
	{
		if (bPlayerExists && !featurePlayerInvincible)
			PLAYER::SET_PLAYER_INVINCIBLE(player, FALSE);
		featurePlayerInvincibleUpdated = false;
	}
	if (featurePlayerInvincible)
	{
		if (bPlayerExists)
			PLAYER::SET_PLAYER_INVINCIBLE(player, TRUE);
	}

	// player never wanted
	if (featurePlayerNeverWanted)
	{
		if (bPlayerExists)
			PLAYER::CLEAR_PLAYER_WANTED_LEVEL(player);
	}

	// police ignore player
	if (featurePlayerIgnoredUpdated)
	{
		if (bPlayerExists)
			PLAYER::SET_POLICE_IGNORE_PLAYER(player, featurePlayerIgnored);
		featurePlayerIgnoredUpdated = false;
	}

	// player special ability
	if (featurePlayerUnlimitedAbility)
	{
		if (bPlayerExists)
			PLAYER::SPECIAL_ABILITY_FILL_METER(player, 1);
	}

	// player no noise
	if (featurePlayerNoNoiseUpdated)
	{
		if (bPlayerExists && !featurePlayerNoNoise)
			PLAYER::SET_PLAYER_NOISE_MULTIPLIER(player, 1.0);
		featurePlayerNoNoiseUpdated = false;
	}
	if (featurePlayerNoNoise)
		PLAYER::SET_PLAYER_NOISE_MULTIPLIER(player, 0.0);

	// player fast swim
	if (featurePlayerFastSwimUpdated)
	{
		if (bPlayerExists && !featurePlayerFastSwim)
			PLAYER::SET_SWIM_MULTIPLIER_FOR_PLAYER(player, 1.0);
		featurePlayerFastSwimUpdated = false;
	}
	if (featurePlayerFastSwim)
		PLAYER::SET_SWIM_MULTIPLIER_FOR_PLAYER(player, 1.49);

	// player fast run
	if (featurePlayerFastRunUpdated)
	{
		if (bPlayerExists && !featurePlayerFastRun)
			PLAYER::SET_RUN_SPRINT_MULTIPLIER_FOR_PLAYER(player, 1.0);
		featurePlayerFastRunUpdated = false;
	}
	if (featurePlayerFastRun)
		PLAYER::SET_RUN_SPRINT_MULTIPLIER_FOR_PLAYER(player, 1.49);

	// player super jump
	if (featurePlayerSuperJump)
	{
		if (bPlayerExists)
			GAMEPLAY::SET_SUPER_JUMP_THIS_FRAME(player);
	}

	// weapon
	if (featureWeaponFireAmmo)
	{
		if (bPlayerExists)
			GAMEPLAY::SET_FIRE_AMMO_THIS_FRAME(player);
	}
	if (featureWeaponExplosiveAmmo)
	{
		if (bPlayerExists)
			GAMEPLAY::SET_EXPLOSIVE_AMMO_THIS_FRAME(player);
	}
	if (featureWeaponExplosiveMelee)
	{
		if (bPlayerExists)
			GAMEPLAY::SET_EXPLOSIVE_MELEE_THIS_FRAME(player);
	}

	// weapon no reload
	if (bPlayerExists && featureWeaponNoReload)
	{
		Hash cur;
		if (WEAPON::GET_CURRENT_PED_WEAPON(playerPed, &cur, 1))
		{
			if (WEAPON::IS_WEAPON_VALID(cur))
			{
				int maxAmmo;
				if (WEAPON::GET_MAX_AMMO(playerPed, cur, &maxAmmo))
				{
					WEAPON::SET_PED_AMMO(playerPed, cur, maxAmmo);

					maxAmmo = WEAPON::GET_MAX_AMMO_IN_CLIP(playerPed, cur, 1);
					if (maxAmmo > 0)
						WEAPON::SET_AMMO_IN_CLIP(playerPed, cur, maxAmmo);
				}
			}
		}
	}

	// weapon money gun
	if (featureWeaponMoneyGun && bPlayerExists)
	{
		if (ENTITY::DOES_ENTITY_EXIST(playerPed) && PED::IS_PED_SHOOTING(playerPed))
		{
			float xD[6];
			WEAPON::GET_PED_LAST_WEAPON_IMPACT_COORD(playerPed, (Vector3*)xD);
			if (xD[0] != 0 || xD[2] != 0 || xD[4] != 0)
			{
				STREAMING::REQUEST_MODEL(GAMEPLAY::GET_HASH_KEY("prop_money_bag_01"));
				while (!STREAMING::HAS_MODEL_LOADED(GAMEPLAY::GET_HASH_KEY("prop_money_bag_01"))) WAIT(0);
				{
					OBJECT::CREATE_AMBIENT_PICKUP(GAMEPLAY::GET_HASH_KEY("pickup_money_case"), xD[0], xD[2], xD[4], 0.0, 40000.0, GAMEPLAY::GET_HASH_KEY("prop_money_bag_01"), false, true);
					STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(GAMEPLAY::GET_HASH_KEY("prop_money_bag_01"));
				}
			}
		}
	}

	// player's vehicle invincible
	if (featureVehInvincibleUpdated)
	{
		if (bPlayerExists && !featureVehInvincible && PED::IS_PED_IN_ANY_VEHICLE(playerPed, 0))
		{
			Vehicle veh = PED::GET_VEHICLE_PED_IS_USING(playerPed);
			ENTITY::SET_ENTITY_INVINCIBLE(veh, FALSE);
			ENTITY::SET_ENTITY_PROOFS(veh, 0, 0, 0, 0, 0, 0, 0, 0);
			VEHICLE::SET_VEHICLE_TYRES_CAN_BURST(veh, 1);
			VEHICLE::SET_VEHICLE_WHEELS_CAN_BREAK(veh, 1);
			VEHICLE::SET_VEHICLE_CAN_BE_VISIBLY_DAMAGED(veh, 1);
		}
		featureVehInvincibleUpdated = false;
	}
	if (featureVehInvincible)
	{
		if (bPlayerExists && PED::IS_PED_IN_ANY_VEHICLE(playerPed, 0))
		{
			Vehicle veh = PED::GET_VEHICLE_PED_IS_USING(playerPed);
			ENTITY::SET_ENTITY_INVINCIBLE(veh, TRUE);
			ENTITY::SET_ENTITY_PROOFS(veh, 1, 1, 1, 1, 1, 1, 1, 1);
			VEHICLE::SET_VEHICLE_TYRES_CAN_BURST(veh, 0);
			VEHICLE::SET_VEHICLE_WHEELS_CAN_BREAK(veh, 0);
			VEHICLE::SET_VEHICLE_CAN_BE_VISIBLY_DAMAGED(veh, 0);
		}
	}

	// player's vehicle invincible wheels, usefull with custom handling
	if (featureVehInvincibleWheelsUpdated)
	{
		if (bPlayerExists && !featureVehInvincibleWheels && PED::IS_PED_IN_ANY_VEHICLE(playerPed, 0))
		{
			Vehicle veh = PED::GET_VEHICLE_PED_IS_USING(playerPed);
			VEHICLE::SET_VEHICLE_TYRES_CAN_BURST(veh, TRUE);
			VEHICLE::SET_VEHICLE_WHEELS_CAN_BREAK(veh, TRUE);
			VEHICLE::SET_VEHICLE_HAS_STRONG_AXLES(veh, FALSE);
		}
		featureVehInvincibleWheelsUpdated = false;
	}
	if (featureVehInvincibleWheels)
	{
		if (bPlayerExists && PED::IS_PED_IN_ANY_VEHICLE(playerPed, 0))
		{
			Vehicle veh = PED::GET_VEHICLE_PED_IS_USING(playerPed);
			VEHICLE::SET_VEHICLE_TYRES_CAN_BURST(veh, FALSE);
			VEHICLE::SET_VEHICLE_WHEELS_CAN_BREAK(veh, FALSE);
			VEHICLE::SET_VEHICLE_HAS_STRONG_AXLES(veh, TRUE);
		}
	}

	if (featurePlayerUltraJump)
	{
		GAMEPLAY::SET_SUPER_JUMP_THIS_FRAME(player);
		WEAPON::REMOVE_WEAPON_FROM_PED(playerPed, GAMEPLAY::GET_HASH_KEY("GADGET_PARACHUTE"));
		if (PED::IS_PED_JUMPING(playerPed))
		{
			ENTITY::APPLY_FORCE_TO_ENTITY(playerPed, true, 0, 0, 5, 0, 0, 0, true, true, true, true, false, true);
		}
	}

	// seat belt
	const int PED_FLAG_CAN_FLY_THRU_WINDSCREEN = 32;
	if (featureVehSeatbeltUpdated)
	{
		if (bPlayerExists && !featureVehSeatbelt)
			PED::SET_PED_CONFIG_FLAG(playerPed, PED_FLAG_CAN_FLY_THRU_WINDSCREEN, TRUE);
		featureVehSeatbeltUpdated = false;
	}
	if (featureVehSeatbelt)
	{
		if (bPlayerExists)
		{
			if (PED::GET_PED_CONFIG_FLAG(playerPed, PED_FLAG_CAN_FLY_THRU_WINDSCREEN, TRUE))
				PED::SET_PED_CONFIG_FLAG(playerPed, PED_FLAG_CAN_FLY_THRU_WINDSCREEN, FALSE);
		}
	}

	// player's vehicle boost
	if (featureVehSpeedBoost && bPlayerExists && PED::IS_PED_IN_ANY_VEHICLE(playerPed, 0))
	{
		Vehicle veh = PED::GET_VEHICLE_PED_IS_USING(playerPed);
		DWORD model = ENTITY::GET_ENTITY_MODEL(veh);

		bool bUp = IsKeyDown(VK_NUMPAD9);
		bool bDown = IsKeyDown(VK_NUMPAD3);

		if (bUp || bDown)
		{
			float speed = ENTITY::GET_ENTITY_SPEED(veh);
			if (bUp)
			{
				if (speed < 3.0f) speed = 3.0f;
				speed += speed * 0.05f;
				VEHICLE::SET_VEHICLE_FORWARD_SPEED(veh, speed);
			}
			else
				if (ENTITY::IS_ENTITY_IN_AIR(veh) || speed > 5.0)
					VEHICLE::SET_VEHICLE_FORWARD_SPEED(veh, 0.0);
		}
	}

	// time pause
	if (featureTimePausedUpdated)
	{
		TIME::PAUSE_CLOCK(featureTimePaused);
		featureTimePausedUpdated = false;
	}

	// time sync
	if (featureTimeSynced)
	{
		time_t now = time(0);
		tm t;
		localtime_s(&t, &now);
		TIME::SET_CLOCK_TIME(t.tm_hour, t.tm_min, t.tm_sec);
	}

	// hide hud
	if (featureMiscHideHud)
		UI::HIDE_HUD_AND_RADAR_THIS_FRAME();
}

LPCSTR pedModels[70][10] = {
	{ "player_zero", "player_one", "player_two", "a_c_boar", "a_c_chimp", "a_c_cow", "a_c_coyote", "a_c_deer", "a_c_fish", "a_c_hen" },
	{ "a_c_cat_01", "a_c_chickenhawk", "a_c_cormorant", "a_c_crow", "a_c_dolphin", "a_c_humpback", "a_c_killerwhale", "a_c_pigeon", "a_c_seagull", "a_c_sharkhammer" },
	{ "a_c_pig", "a_c_rat", "a_c_rhesus", "a_c_chop", "a_c_husky", "a_c_mtlion", "a_c_retriever", "a_c_sharktiger", "a_c_shepherd", "s_m_m_movalien_01" },
	{ "a_f_m_beach_01", "a_f_m_bevhills_01", "a_f_m_bevhills_02", "a_f_m_bodybuild_01", "a_f_m_business_02", "a_f_m_downtown_01", "a_f_m_eastsa_01", "a_f_m_eastsa_02", "a_f_m_fatbla_01", "a_f_m_fatcult_01" },
	{ "a_f_m_fatwhite_01", "a_f_m_ktown_01", "a_f_m_ktown_02", "a_f_m_prolhost_01", "a_f_m_salton_01", "a_f_m_skidrow_01", "a_f_m_soucentmc_01", "a_f_m_soucent_01", "a_f_m_soucent_02", "a_f_m_tourist_01" },
	{ "a_f_m_trampbeac_01", "a_f_m_tramp_01", "a_f_o_genstreet_01", "a_f_o_indian_01", "a_f_o_ktown_01", "a_f_o_salton_01", "a_f_o_soucent_01", "a_f_o_soucent_02", "a_f_y_beach_01", "a_f_y_bevhills_01" },
	{ "a_f_y_bevhills_02", "a_f_y_bevhills_03", "a_f_y_bevhills_04", "a_f_y_business_01", "a_f_y_business_02", "a_f_y_business_03", "a_f_y_business_04", "a_f_y_eastsa_01", "a_f_y_eastsa_02", "a_f_y_eastsa_03" },
	{ "a_f_y_epsilon_01", "a_f_y_fitness_01", "a_f_y_fitness_02", "a_f_y_genhot_01", "a_f_y_golfer_01", "a_f_y_hiker_01", "a_f_y_hippie_01", "a_f_y_hipster_01", "a_f_y_hipster_02", "a_f_y_hipster_03" },
	{ "a_f_y_hipster_04", "a_f_y_indian_01", "a_f_y_juggalo_01", "a_f_y_runner_01", "a_f_y_rurmeth_01", "a_f_y_scdressy_01", "a_f_y_skater_01", "a_f_y_soucent_01", "a_f_y_soucent_02", "a_f_y_soucent_03" },
	{ "a_f_y_tennis_01", "a_f_y_topless_01", "a_f_y_tourist_01", "a_f_y_tourist_02", "a_f_y_vinewood_01", "a_f_y_vinewood_02", "a_f_y_vinewood_03", "a_f_y_vinewood_04", "a_f_y_yoga_01", "a_m_m_acult_01" },
	{ "a_m_m_afriamer_01", "a_m_m_beach_01", "a_m_m_beach_02", "a_m_m_bevhills_01", "a_m_m_bevhills_02", "a_m_m_business_01", "a_m_m_eastsa_01", "a_m_m_eastsa_02", "a_m_m_farmer_01", "a_m_m_fatlatin_01" },
	{ "a_m_m_genfat_01", "a_m_m_genfat_02", "a_m_m_golfer_01", "a_m_m_hasjew_01", "a_m_m_hillbilly_01", "a_m_m_hillbilly_02", "a_m_m_indian_01", "a_m_m_ktown_01", "a_m_m_malibu_01", "a_m_m_mexcntry_01" },
	{ "a_m_m_mexlabor_01", "a_m_m_og_boss_01", "a_m_m_paparazzi_01", "a_m_m_polynesian_01", "a_m_m_prolhost_01", "a_m_m_rurmeth_01", "a_m_m_salton_01", "a_m_m_salton_02", "a_m_m_salton_03", "a_m_m_salton_04" },
	{ "a_m_m_skater_01", "a_m_m_skidrow_01", "a_m_m_socenlat_01", "a_m_m_soucent_01", "a_m_m_soucent_02", "a_m_m_soucent_03", "a_m_m_soucent_04", "a_m_m_stlat_02", "a_m_m_tennis_01", "a_m_m_tourist_01" },
	{ "a_m_m_trampbeac_01", "a_m_m_tramp_01", "a_m_m_tranvest_01", "a_m_m_tranvest_02", "a_m_o_acult_01", "a_m_o_acult_02", "a_m_o_beach_01", "a_m_o_genstreet_01", "a_m_o_ktown_01", "a_m_o_salton_01" },
	{ "a_m_o_soucent_01", "a_m_o_soucent_02", "a_m_o_soucent_03", "a_m_o_tramp_01", "a_m_y_acult_01", "a_m_y_acult_02", "a_m_y_beachvesp_01", "a_m_y_beachvesp_02", "a_m_y_beach_01", "a_m_y_beach_02" },
	{ "a_m_y_beach_03", "a_m_y_bevhills_01", "a_m_y_bevhills_02", "a_m_y_breakdance_01", "a_m_y_busicas_01", "a_m_y_business_01", "a_m_y_business_02", "a_m_y_business_03", "a_m_y_cyclist_01", "a_m_y_dhill_01" },
	{ "a_m_y_downtown_01", "a_m_y_eastsa_01", "a_m_y_eastsa_02", "a_m_y_epsilon_01", "a_m_y_epsilon_02", "a_m_y_gay_01", "a_m_y_gay_02", "a_m_y_genstreet_01", "a_m_y_genstreet_02", "a_m_y_golfer_01" },
	{ "a_m_y_hasjew_01", "a_m_y_hiker_01", "a_m_y_hippy_01", "a_m_y_hipster_01", "a_m_y_hipster_02", "a_m_y_hipster_03", "a_m_y_indian_01", "a_m_y_jetski_01", "a_m_y_juggalo_01", "a_m_y_ktown_01" },
	{ "a_m_y_ktown_02", "a_m_y_latino_01", "a_m_y_methhead_01", "a_m_y_mexthug_01", "a_m_y_motox_01", "a_m_y_motox_02", "a_m_y_musclbeac_01", "a_m_y_musclbeac_02", "a_m_y_polynesian_01", "a_m_y_roadcyc_01" },
	{ "a_m_y_runner_01", "a_m_y_runner_02", "a_m_y_salton_01", "a_m_y_skater_01", "a_m_y_skater_02", "a_m_y_soucent_01", "a_m_y_soucent_02", "a_m_y_soucent_03", "a_m_y_soucent_04", "a_m_y_stbla_01" },
	{ "a_m_y_stbla_02", "a_m_y_stlat_01", "a_m_y_stwhi_01", "a_m_y_stwhi_02", "a_m_y_sunbathe_01", "a_m_y_surfer_01", "a_m_y_vindouche_01", "a_m_y_vinewood_01", "a_m_y_vinewood_02", "a_m_y_vinewood_03" },
	{ "a_m_y_vinewood_04", "a_m_y_yoga_01", "u_m_y_proldriver_01", "u_m_y_rsranger_01", "u_m_y_sbike", "u_m_y_staggrm_01", "u_m_y_tattoo_01", "csb_abigail", "csb_anita", "csb_anton" },
	{ "csb_ballasog", "csb_bride", "csb_burgerdrug", "csb_car3guy1", "csb_car3guy2", "csb_chef", "csb_chin_goon", "csb_cletus", "csb_cop", "csb_customer" },
	{ "csb_denise_friend", "csb_fos_rep", "csb_g", "csb_groom", "csb_grove_str_dlr", "csb_hao", "csb_hugh", "csb_imran", "csb_janitor", "csb_maude" },
	{ "csb_mweather", "csb_ortega", "csb_oscar", "csb_porndudes", "csb_porndudes_p", "csb_prologuedriver", "csb_prolsec", "csb_ramp_gang", "csb_ramp_hic", "csb_ramp_hipster" },
	{ "csb_ramp_marine", "csb_ramp_mex", "csb_reporter", "csb_roccopelosi", "csb_screen_writer", "csb_stripper_01", "csb_stripper_02", "csb_tonya", "csb_trafficwarden", "cs_amandatownley" },
	{ "cs_andreas", "cs_ashley", "cs_bankman", "cs_barry", "cs_barry_p", "cs_beverly", "cs_beverly_p", "cs_brad", "cs_bradcadaver", "cs_carbuyer" },
	{ "cs_casey", "cs_chengsr", "cs_chrisformage", "cs_clay", "cs_dale", "cs_davenorton", "cs_debra", "cs_denise", "cs_devin", "cs_dom" },
	{ "cs_dreyfuss", "cs_drfriedlander", "cs_fabien", "cs_fbisuit_01", "cs_floyd", "cs_guadalope", "cs_gurk", "cs_hunter", "cs_janet", "cs_jewelass" },
	{ "cs_jimmyboston", "cs_jimmydisanto", "cs_joeminuteman", "cs_johnnyklebitz", "cs_josef", "cs_josh", "cs_lamardavis", "cs_lazlow", "cs_lestercrest", "cs_lifeinvad_01" },
	{ "cs_magenta", "cs_manuel", "cs_marnie", "cs_martinmadrazo", "cs_maryann", "cs_michelle", "cs_milton", "cs_molly", "cs_movpremf_01", "cs_movpremmale" },
	{ "cs_mrk", "cs_mrsphillips", "cs_mrs_thornhill", "cs_natalia", "cs_nervousron", "cs_nigel", "cs_old_man1a", "cs_old_man2", "cs_omega", "cs_orleans" },
	{ "cs_paper", "cs_paper_p", "cs_patricia", "cs_priest", "cs_prolsec_02", "cs_russiandrunk", "cs_siemonyetarian", "cs_solomon", "cs_stevehains", "cs_stretch" },
	{ "cs_tanisha", "cs_taocheng", "cs_taostranslator", "cs_tenniscoach", "cs_terry", "cs_tom", "cs_tomepsilon", "cs_tracydisanto", "cs_wade", "cs_zimbor" },
	{ "g_f_y_ballas_01", "g_f_y_families_01", "g_f_y_lost_01", "g_f_y_vagos_01", "g_m_m_armboss_01", "g_m_m_armgoon_01", "g_m_m_armlieut_01", "g_m_m_chemwork_01", "g_m_m_chemwork_01_p", "g_m_m_chiboss_01" },
	{ "g_m_m_chiboss_01_p", "g_m_m_chicold_01", "g_m_m_chicold_01_p", "g_m_m_chigoon_01", "g_m_m_chigoon_01_p", "g_m_m_chigoon_02", "g_m_m_korboss_01", "g_m_m_mexboss_01", "g_m_m_mexboss_02", "g_m_y_armgoon_02" },
	{ "g_m_y_azteca_01", "g_m_y_ballaeast_01", "g_m_y_ballaorig_01", "g_m_y_ballasout_01", "g_m_y_famca_01", "g_m_y_famdnf_01", "g_m_y_famfor_01", "g_m_y_korean_01", "g_m_y_korean_02", "g_m_y_korlieut_01" },
	{ "g_m_y_lost_01", "g_m_y_lost_02", "g_m_y_lost_03", "g_m_y_mexgang_01", "g_m_y_mexgoon_01", "g_m_y_mexgoon_02", "g_m_y_mexgoon_03", "g_m_y_mexgoon_03_p", "g_m_y_pologoon_01", "g_m_y_pologoon_01_p" },
	{ "g_m_y_pologoon_02", "g_m_y_pologoon_02_p", "g_m_y_salvaboss_01", "g_m_y_salvagoon_01", "g_m_y_salvagoon_02", "g_m_y_salvagoon_03", "g_m_y_salvagoon_03_p", "g_m_y_strpunk_01", "g_m_y_strpunk_02", "hc_driver" },
	{ "hc_gunman", "hc_hacker", "ig_abigail", "ig_amandatownley", "ig_andreas", "ig_ashley", "ig_ballasog", "ig_bankman", "ig_barry", "ig_barry_p" },
	{ "ig_bestmen", "ig_beverly", "ig_beverly_p", "ig_brad", "ig_bride", "ig_car3guy1", "ig_car3guy2", "ig_casey", "ig_chef", "ig_chengsr" },
	{ "ig_chrisformage", "ig_clay", "ig_claypain", "ig_cletus", "ig_dale", "ig_davenorton", "ig_denise", "ig_devin", "ig_dom", "ig_dreyfuss" },
	{ "ig_drfriedlander", "ig_fabien", "ig_fbisuit_01", "ig_floyd", "ig_groom", "ig_hao", "ig_hunter", "ig_janet", "ig_jay_norris", "ig_jewelass" },
	{ "ig_jimmyboston", "ig_jimmydisanto", "ig_joeminuteman", "ig_johnnyklebitz", "ig_josef", "ig_josh", "ig_kerrymcintosh", "ig_lamardavis", "ig_lazlow", "ig_lestercrest" },
	{ "ig_lifeinvad_01", "ig_lifeinvad_02", "ig_magenta", "ig_manuel", "ig_marnie", "ig_maryann", "ig_maude", "ig_michelle", "ig_milton", "ig_molly" },
	{ "ig_mrk", "ig_mrsphillips", "ig_mrs_thornhill", "ig_natalia", "ig_nervousron", "ig_nigel", "ig_old_man1a", "ig_old_man2", "ig_omega", "ig_oneil" },
	{ "ig_orleans", "ig_ortega", "ig_paper", "ig_patricia", "ig_priest", "ig_prolsec_02", "ig_ramp_gang", "ig_ramp_hic", "ig_ramp_hipster", "ig_ramp_mex" },
	{ "ig_roccopelosi", "ig_russiandrunk", "ig_screen_writer", "ig_siemonyetarian", "ig_solomon", "ig_stevehains", "ig_stretch", "ig_talina", "ig_tanisha", "ig_taocheng" },
	{ "ig_taostranslator", "ig_taostranslator_p", "ig_tenniscoach", "ig_terry", "ig_tomepsilon", "ig_tonya", "ig_tracydisanto", "ig_trafficwarden", "ig_tylerdix", "ig_wade" },
	{ "ig_zimbor", "mp_f_deadhooker", "mp_f_freemode_01", "mp_f_misty_01", "mp_f_stripperlite", "mp_g_m_pros_01", "mp_headtargets", "mp_m_claude_01", "mp_m_exarmy_01", "mp_m_famdd_01" },
	{ "mp_m_fibsec_01", "mp_m_freemode_01", "mp_m_marston_01", "mp_m_niko_01", "mp_m_shopkeep_01", "mp_s_m_armoured_01", "", "", "", "" },
	{ "", "s_f_m_fembarber", "s_f_m_maid_01", "s_f_m_shop_high", "s_f_m_sweatshop_01", "s_f_y_airhostess_01", "s_f_y_bartender_01", "s_f_y_baywatch_01", "s_f_y_cop_01", "s_f_y_factory_01" },
	{ "s_f_y_hooker_01", "s_f_y_hooker_02", "s_f_y_hooker_03", "s_f_y_migrant_01", "s_f_y_movprem_01", "s_f_y_ranger_01", "s_f_y_scrubs_01", "s_f_y_sheriff_01", "s_f_y_shop_low", "s_f_y_shop_mid" },
	{ "s_f_y_stripperlite", "s_f_y_stripper_01", "s_f_y_stripper_02", "s_f_y_sweatshop_01", "s_m_m_ammucountry", "s_m_m_armoured_01", "s_m_m_armoured_02", "s_m_m_autoshop_01", "s_m_m_autoshop_02", "s_m_m_bouncer_01" },
	{ "s_m_m_chemsec_01", "s_m_m_ciasec_01", "s_m_m_cntrybar_01", "s_m_m_dockwork_01", "s_m_m_doctor_01", "s_m_m_fiboffice_01", "s_m_m_fiboffice_02", "s_m_m_gaffer_01", "s_m_m_gardener_01", "s_m_m_gentransport" },
	{ "s_m_m_hairdress_01", "s_m_m_highsec_01", "s_m_m_highsec_02", "s_m_m_janitor", "s_m_m_lathandy_01", "s_m_m_lifeinvad_01", "s_m_m_linecook", "s_m_m_lsmetro_01", "s_m_m_mariachi_01", "s_m_m_marine_01" },
	{ "s_m_m_marine_02", "s_m_m_migrant_01", "u_m_y_zombie_01", "s_m_m_movprem_01", "s_m_m_movspace_01", "s_m_m_paramedic_01", "s_m_m_pilot_01", "s_m_m_pilot_02", "s_m_m_postal_01", "s_m_m_postal_02" },
	{ "s_m_m_prisguard_01", "s_m_m_scientist_01", "s_m_m_security_01", "s_m_m_snowcop_01", "s_m_m_strperf_01", "s_m_m_strpreach_01", "s_m_m_strvend_01", "s_m_m_trucker_01", "s_m_m_ups_01", "s_m_m_ups_02" },
	{ "s_m_o_busker_01", "s_m_y_airworker", "s_m_y_ammucity_01", "s_m_y_armymech_01", "s_m_y_autopsy_01", "s_m_y_barman_01", "s_m_y_baywatch_01", "s_m_y_blackops_01", "s_m_y_blackops_02", "s_m_y_busboy_01" },
	{ "s_m_y_chef_01", "s_m_y_clown_01", "s_m_y_construct_01", "s_m_y_construct_02", "s_m_y_cop_01", "s_m_y_dealer_01", "s_m_y_devinsec_01", "s_m_y_dockwork_01", "s_m_y_doorman_01", "s_m_y_dwservice_01" },
	{ "s_m_y_dwservice_02", "s_m_y_factory_01", "s_m_y_fireman_01", "s_m_y_garbage", "s_m_y_grip_01", "s_m_y_hwaycop_01", "s_m_y_marine_01", "s_m_y_marine_02", "s_m_y_marine_03", "s_m_y_mime" },
	{ "s_m_y_pestcont_01", "s_m_y_pilot_01", "s_m_y_prismuscl_01", "s_m_y_prisoner_01", "s_m_y_ranger_01", "s_m_y_robber_01", "s_m_y_sheriff_01", "s_m_y_shop_mask", "s_m_y_strvend_01", "s_m_y_swat_01" },
	{ "s_m_y_uscg_01", "s_m_y_valet_01", "s_m_y_waiter_01", "s_m_y_winclean_01", "s_m_y_xmech_01", "s_m_y_xmech_02", "u_f_m_corpse_01", "u_f_m_miranda", "u_f_m_promourn_01", "u_f_o_moviestar" },
	{ "u_f_o_prolhost_01", "u_f_y_bikerchic", "u_f_y_comjane", "u_f_y_corpse_01", "u_f_y_corpse_02", "u_f_y_hotposh_01", "u_f_y_jewelass_01", "u_f_y_mistress", "u_f_y_poppymich", "u_f_y_princess" },
	{ "u_f_y_spyactress", "u_m_m_aldinapoli", "u_m_m_bankman", "u_m_m_bikehire_01", "u_m_m_fibarchitect", "u_m_m_filmdirector", "u_m_m_glenstank_01", "u_m_m_griff_01", "u_m_m_jesus_01", "u_m_m_jewelsec_01" },
	{ "u_m_m_jewelthief", "u_m_m_markfost", "u_m_m_partytarget", "u_m_m_prolsec_01", "u_m_m_promourn_01", "u_m_m_rivalpap", "u_m_m_spyactor", "u_m_m_willyfist", "u_m_o_finguru_01", "u_m_o_taphillbilly" },
	{ "u_m_o_tramp_01", "u_m_y_abner", "u_m_y_antonb", "u_m_y_babyd", "u_m_y_baygor", "u_m_y_burgerdrug_01", "u_m_y_chip", "u_m_y_cyclist_01", "u_m_y_fibmugger_01", "u_m_y_guido_01" },
	{ "u_m_y_gunvend_01", "u_m_y_hippie_01", "u_m_y_imporage", "u_m_y_justin", "u_m_y_mani", "u_m_y_militarybum", "u_m_y_paparazzi", "u_m_y_party_01", "u_m_y_pogo_01", "u_m_y_prisoner_01" },
	{ "ig_benny", "ig_g", "ig_vagspeak", "mp_m_g_vagfun_01", "mp_m_boatstaff_01", "mp_f_boatstaff_01", "", "", "", "" }
};

LPCSTR pedModelNames[70][10] = {
	{ "MICHAEL", "FRANKLIN", "TREVOR", "BOAR", "CHIMP", "COW", "COYOTE", "DEER", "FISH", "HEN" },
	{ "CAT", "HAWK", "CORMORANT", "CROW", "DOLPHIN", "HUMPBACK", "WHALE", "PIGEON", "SEAGULL", "SHARKHAMMER" },
	{ "PIG", "RAT", "RHESUS", "CHOP", "HUSKY", "MTLION", "RETRIEVER", "SHARKTIGER", "SHEPHERD", "ALIEN" },
	{ "BEACH", "BEVHILLS", "BEVHILLS", "BODYBUILD", "BUSINESS", "DOWNTOWN", "EASTSA", "EASTSA", "FATBLA", "FATCULT" },
	{ "FATWHITE", "KTOWN", "KTOWN", "PROLHOST", "SALTON", "SKIDROW", "SOUCENTMC", "SOUCENT", "SOUCENT", "TOURIST" },
	{ "TRAMPBEAC", "TRAMP", "GENSTREET", "INDIAN", "KTOWN", "SALTON", "SOUCENT", "SOUCENT", "BEACH", "BEVHILLS" },
	{ "BEVHILLS", "BEVHILLS", "BEVHILLS", "BUSINESS", "BUSINESS", "BUSINESS", "BUSINESS", "EASTSA", "EASTSA", "EASTSA" },
	{ "EPSILON", "FITNESS", "FITNESS", "GENHOT", "GOLFER", "HIKER", "HIPPIE", "HIPSTER", "HIPSTER", "HIPSTER" },
	{ "HIPSTER", "INDIAN", "JUGGALO", "RUNNER", "RURMETH", "SCDRESSY", "SKATER", "SOUCENT", "SOUCENT", "SOUCENT" },
	{ "TENNIS", "TOPLESS", "TOURIST", "TOURIST", "VINEWOOD", "VINEWOOD", "VINEWOOD", "VINEWOOD", "YOGA", "ACULT" },
	{ "AFRIAMER", "BEACH", "BEACH", "BEVHILLS", "BEVHILLS", "BUSINESS", "EASTSA", "EASTSA", "FARMER", "FATLATIN" },
	{ "GENFAT", "GENFAT", "GOLFER", "HASJEW", "HILLBILLY", "HILLBILLY", "INDIAN", "KTOWN", "MALIBU", "MEXCNTRY" },
	{ "MEXLABOR", "OG_BOSS", "PAPARAZZI", "POLYNESIAN", "PROLHOST", "RURMETH", "SALTON", "SALTON", "SALTON", "SALTON" },
	{ "SKATER", "SKIDROW", "SOCENLAT", "SOUCENT", "SOUCENT", "SOUCENT", "SOUCENT", "STLAT", "TENNIS", "TOURIST" },
	{ "TRAMPBEAC", "TRAMP", "TRANVEST", "TRANVEST", "ACULT", "ACULT", "BEACH", "GENSTREET", "KTOWN", "SALTON" },
	{ "SOUCENT", "SOUCENT", "SOUCENT", "TRAMP", "ACULT", "ACULT", "BEACHVESP", "BEACHVESP", "BEACH", "BEACH" },
	{ "BEACH", "BEVHILLS", "BEVHILLS", "BREAKDANCE", "BUSICAS", "BUSINESS", "BUSINESS", "BUSINESS", "CYCLIST", "DHILL" },
	{ "DOWNTOWN", "EASTSA", "EASTSA", "EPSILON", "EPSILON", "GAY", "GAY", "GENSTREET", "GENSTREET", "GOLFER" },
	{ "HASJEW", "HIKER", "HIPPY", "HIPSTER", "HIPSTER", "HIPSTER", "INDIAN", "JETSKI", "JUGGALO", "KTOWN" },
	{ "KTOWN", "LATINO", "METHHEAD", "MEXTHUG", "MOTOX", "MOTOX", "MUSCLBEAC", "MUSCLBEAC", "POLYNESIAN", "ROADCYC" },
	{ "RUNNER", "RUNNER", "SALTON", "SKATER", "SKATER", "SOUCENT", "SOUCENT", "SOUCENT", "SOUCENT", "STBLA" },
	{ "STBLA", "STLAT", "STWHI", "STWHI", "SUNBATHE", "SURFER", "VINDOUCHE", "VINEWOOD", "VINEWOOD", "VINEWOOD" },
	{ "VINEWOOD", "YOGA", "PROLDRIVER", "RSRANGER", "SBIKE", "STAGGRM", "TATTOO", "ABIGAIL", "ANITA", "ANTON" },
	{ "BALLASOG", "BRIDE", "BURGERDRUG", "CAR3GUY1", "CAR3GUY2", "CHEF", "CHIN_GOON", "CLETUS", "COP", "CUSTOMER" },
	{ "DENISE_FRIEND", "FOS_REP", "G", "GROOM", "DLR", "HAO", "HUGH", "IMRAN", "JANITOR", "MAUDE" },
	{ "MWEATHER", "ORTEGA", "OSCAR", "PORNDUDES", "PORNDUDES_P", "PROLOGUEDRIVER", "PROLSEC", "GANG", "HIC", "HIPSTER" },
	{ "MARINE", "MEX", "REPORTER", "ROCCOPELOSI", "SCREEN_WRITER", "STRIPPER", "STRIPPER", "TONYA", "TRAFFICWARDEN", "AMANDATOWNLEY" },
	{ "ANDREAS", "ASHLEY", "BANKMAN", "BARRY", "BARRY_P", "BEVERLY", "BEVERLY_P", "BRAD", "BRADCADAVER", "CARBUYER" },
	{ "CASEY", "CHENGSR", "CHRISFORMAGE", "CLAY", "DALE", "DAVENORTON", "DEBRA", "DENISE", "DEVIN", "DOM" },
	{ "DREYFUSS", "DRFRIEDLANDER", "FABIEN", "FBISUIT", "FLOYD", "GUADALOPE", "GURK", "HUNTER", "JANET", "JEWELASS" },
	{ "JIMMYBOSTON", "JIMMYDISANTO", "JOEMINUTEMAN", "JOHNNYKLEBITZ", "JOSEF", "JOSH", "LAMARDAVIS", "LAZLOW", "LESTERCREST", "LIFEINVAD" },
	{ "MAGENTA", "MANUEL", "MARNIE", "MARTINMADRAZO", "MARYANN", "MICHELLE", "MILTON", "MOLLY", "MOVPREMF", "MOVPREMMALE" },
	{ "MRK", "MRSPHILLIPS", "MRS_THORNHILL", "NATALIA", "NERVOUSRON", "NIGEL", "OLD_MAN1A", "OLD_MAN2", "OMEGA", "ORLEANS" },
	{ "PAPER", "PAPER_P", "PATRICIA", "PRIEST", "PROLSEC", "RUSSIANDRUNK", "SIEMONYETARIAN", "SOLOMON", "STEVEHAINS", "STRETCH" },
	{ "TANISHA", "TAOCHENG", "TAOSTRANSLATOR", "TENNISCOACH", "TERRY", "TOM", "TOMEPSILON", "TRACYDISANTO", "WADE", "ZIMBOR" },
	{ "BALLAS", "FAMILIES", "LOST", "VAGOS", "ARMBOSS", "ARMGOON", "ARMLIEUT", "CHEMWORK", "CHEMWORK_P", "CHIBOSS" },
	{ "CHIBOSS_P", "CHICOLD", "CHICOLD_P", "CHIGOON", "CHIGOON_P", "CHIGOON", "KORBOSS", "MEXBOSS", "MEXBOSS", "ARMGOON" },
	{ "AZTECA", "BALLAEAST", "BALLAORIG", "BALLASOUT", "FAMCA", "FAMDNF", "FAMFOR", "KOREAN", "KOREAN", "KORLIEUT" },
	{ "LOST", "LOST", "LOST", "MEXGANG", "MEXGOON", "MEXGOON", "MEXGOON", "MEXGOON_P", "POLOGOON", "POLOGOON_P" },
	{ "POLOGOON", "POLOGOON_P", "SALVABOSS", "SALVAGOON", "SALVAGOON", "SALVAGOON", "SALVAGOON_P", "STRPUNK", "STRPUNK", "HC_DRIVER" },
	{ "HC_GUNMAN", "HC_HACKER", "ABIGAIL", "AMANDATOWNLEY", "ANDREAS", "ASHLEY", "BALLASOG", "BANKMAN", "BARRY", "BARRY_P" },
	{ "BESTMEN", "BEVERLY", "BEVERLY_P", "BRAD", "BRIDE", "CAR3GUY1", "CAR3GUY2", "CASEY", "CHEF", "CHENGSR" },
	{ "CHRISFORMAGE", "CLAY", "CLAYPAIN", "CLETUS", "DALE", "DAVENORTON", "DENISE", "DEVIN", "DOM", "DREYFUSS" },
	{ "DRFRIEDLANDER", "FABIEN", "FBISUIT", "FLOYD", "GROOM", "HAO", "HUNTER", "JANET", "JAY_NORRIS", "JEWELASS" },
	{ "JIMMYBOSTON", "JIMMYDISANTO", "JOEMINUTEMAN", "JOHNNYKLEBITZ", "JOSEF", "JOSH", "KERRYMCINTOSH", "LAMARDAVIS", "LAZLOW", "LESTERCREST" },
	{ "LIFEINVAD", "LIFEINVAD", "MAGENTA", "MANUEL", "MARNIE", "MARYANN", "MAUDE", "MICHELLE", "MILTON", "MOLLY" },
	{ "MRK", "MRSPHILLIPS", "MRS_THORNHILL", "NATALIA", "NERVOUSRON", "NIGEL", "OLD_MAN1A", "OLD_MAN2", "OMEGA", "ONEIL" },
	{ "ORLEANS", "ORTEGA", "PAPER", "PATRICIA", "PRIEST", "PROLSEC", "GANG", "HIC", "HIPSTER", "MEX" },
	{ "ROCCOPELOSI", "RUSSIANDRUNK", "SCREEN_WRITER", "SIEMONYETARIAN", "SOLOMON", "STEVEHAINS", "STRETCH", "TALINA", "TANISHA", "TAOCHENG" },
	{ "TAOSTRANSLATOR", "TAOSTRANSLATOR_P", "TENNISCOACH", "TERRY", "TOMEPSILON", "TONYA", "TRACYDISANTO", "TRAFFICWARDEN", "TYLERDIX", "WADE" },
	{ "ZIMBOR", "DEADHOOKER", "FREEMODE", "MISTY", "STRIPPERLITE", "PROS", "MP_HEADTARGETS", "CLAUDE", "EXARMY", "FAMDD" },
	{ "FIBSEC", "FREEMODE", "MARSTON", "NIKO", "SHOPKEEP", "ARMOURED", "NONE", "NONE", "NONE", "NONE" },
	{ "NONE", "FEMBARBER", "MAID", "SHOP_HIGH", "SWEATSHOP", "AIRHOSTESS", "BARTENDER", "BAYWATCH", "COP", "FACTORY" },
	{ "HOOKER", "HOOKER", "HOOKER", "MIGRANT", "MOVPREM", "RANGER", "SCRUBS", "SHERIFF", "SHOP_LOW", "SHOP_MID" },
	{ "STRIPPERLITE", "STRIPPER", "STRIPPER", "SWEATSHOP", "AMMUCOUNTRY", "ARMOURED", "ARMOURED", "AUTOSHOP", "AUTOSHOP", "BOUNCER" },
	{ "CHEMSEC", "CIASEC", "CNTRYBAR", "DOCKWORK", "DOCTOR", "FIBOFFICE", "FIBOFFICE", "GAFFER", "GARDENER", "GENTRANSPORT" },
	{ "HAIRDRESS", "HIGHSEC", "HIGHSEC", "JANITOR", "LATHANDY", "LIFEINVAD", "LINECOOK", "LSMETRO", "MARIACHI", "MARINE" },
	{ "MARINE", "MIGRANT", "ZOMBIE", "MOVPREM", "MOVSPACE", "PARAMEDIC", "PILOT", "PILOT", "POSTAL", "POSTAL" },
	{ "PRISGUARD", "SCIENTIST", "SECURITY", "SNOWCOP", "STRPERF", "STRPREACH", "STRVEND", "TRUCKER", "UPS", "UPS" },
	{ "BUSKER", "AIRWORKER", "AMMUCITY", "ARMYMECH", "AUTOPSY", "BARMAN", "BAYWATCH", "BLACKOPS", "BLACKOPS", "BUSBOY" },
	{ "CHEF", "CLOWN", "CONSTRUCT", "CONSTRUCT", "COP", "DEALER", "DEVINSEC", "DOCKWORK", "DOORMAN", "DWSERVICE" },
	{ "DWSERVICE", "FACTORY", "FIREMAN", "GARBAGE", "GRIP", "HWAYCOP", "MARINE", "MARINE", "MARINE", "MIME" },
	{ "PESTCONT", "PILOT", "PRISMUSCL", "PRISONER", "RANGER", "ROBBER", "SHERIFF", "SHOP_MASK", "STRVEND", "SWAT" },
	{ "USCG", "VALET", "WAITER", "WINCLEAN", "XMECH", "XMECH", "CORPSE", "MIRANDA", "PROMOURN", "MOVIESTAR" },
	{ "PROLHOST", "BIKERCHIC", "COMJANE", "CORPSE", "CORPSE", "HOTPOSH", "JEWELASS", "MISTRESS", "POPPYMICH", "PRINCESS" },
	{ "SPYACTRESS", "ALDINAPOLI", "BANKMAN", "BIKEHIRE", "FIBARCHITECT", "FILMDIRECTOR", "GLENSTANK", "GRIFF", "JESUS", "JEWELSEC" },
	{ "JEWELTHIEF", "MARKFOST", "PARTYTARGET", "PROLSEC", "PROMOURN", "RIVALPAP", "SPYACTOR", "WILLYFIST", "FINGURU", "TAPHILLBILLY" },
	{ "TRAMP", "ABNER", "ANTONB", "BABYD", "BAYGOR", "BURGERDRUG", "CHIP", "CYCLIST", "FIBMUGGER", "GUIDO" },
	{ "GUNVEND", "HIPPIE", "IMPORAGE", "JUSTIN", "MANI", "MILITARYBUM", "PAPARAZZI", "PARTY", "POGO", "PRISONER" },
	{ "BENNY", "G", "VAGSPEAK", "VAGFUN", "BOATSTAFF", "FEMBOATSTAFF", "", "", "", "" }
};

int skinchangerActiveLineIndex = 0;
int skinchangerActiveItemIndex = 0;

bool process_skinchanger_menu()
{
	DWORD waitTime = 150;
	const int lineCount = 70;
	const int itemCount = 10;
	const int itemCountLastLine = 6;
	while (true)
	{
		// timed menu draw, used for pause after active line switch
		DWORD maxTickCount = GetTickCount() + waitTime;
		do
		{
			// draw menu
			char caption[32];
			sprintf_s(caption, "SKIN CHANGER   %d / %d", skinchangerActiveLineIndex + 1, lineCount);
			draw_menu_line(caption, 350.0, 15.0, 18.0, 0.0, 5.0, false, true);
			for (int i = 0; i < itemCount; i++)
				if (strlen(pedModels[skinchangerActiveLineIndex][i]) || strcmp(pedModelNames[skinchangerActiveLineIndex][i], "NONE") == 0)
					draw_menu_line(pedModelNames[skinchangerActiveLineIndex][i], 100.0f, 5.0f, 200.0f, 100.0f + i * 110.0f, 5.0f, i == skinchangerActiveItemIndex, false, false);

			update_features();
			WAIT(0);
		} while (GetTickCount() < maxTickCount);
		waitTime = 0;

		bool bSelect, bBack, bUp, bDown, bLeft, bRight;
		get_button_state(&bSelect, &bBack, &bUp, &bDown, &bLeft, &bRight);

		if (bSelect)
		{
			menu_beep();
			DWORD model = GAMEPLAY::GET_HASH_KEY((char *)pedModels[skinchangerActiveLineIndex][skinchangerActiveItemIndex]);
			if (STREAMING::IS_MODEL_IN_CDIMAGE(model) && STREAMING::IS_MODEL_VALID(model))
			{
				STREAMING::REQUEST_MODEL(model);
				while (!STREAMING::HAS_MODEL_LOADED(model)) WAIT(0);
				//STREAMING::LOAD_ALL_OBJECTS_NOW();
				PLAYER::SET_PLAYER_MODEL(PLAYER::PLAYER_ID(), model);
				//PED::SET_PED_RANDOM_COMPONENT_VARIATION(PLAYER::PLAYER_PED_ID(), FALSE);
				PED::SET_PED_DEFAULT_COMPONENT_VARIATION(PLAYER::PLAYER_PED_ID());
				WAIT(0);
				for (int i = 0; i < 12; i++)
					for (int j = 0; j < 100; j++)
					{
						int drawable = rand() % 10;
						int texture = rand() % 10;
						if (PED::IS_PED_COMPONENT_VARIATION_VALID(PLAYER::PLAYER_PED_ID(), i, drawable, texture))
						{
							PED::SET_PED_COMPONENT_VARIATION(PLAYER::PLAYER_PED_ID(), i, drawable, texture, 0);
							break;
						}
					}
				skinchanger_used = true;
				WAIT(100);
				STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(model);
				waitTime = 200;
			}
		}
		else
			if (bBack)
			{
				menu_beep();
				break;
			}
			else
				if (bRight)
				{
					menu_beep();
					skinchangerActiveItemIndex++;
					int itemsMax = (skinchangerActiveLineIndex == (lineCount - 1)) ? itemCountLastLine : itemCount;
					if (skinchangerActiveItemIndex == itemsMax)
						skinchangerActiveItemIndex = 0;
					waitTime = 100;
				}
				else
					if (bLeft)
					{
						menu_beep();
						if (skinchangerActiveItemIndex == 0)
							skinchangerActiveItemIndex = (skinchangerActiveLineIndex == (lineCount - 1)) ? itemCountLastLine : itemCount;
						skinchangerActiveItemIndex--;
						waitTime = 100;
					}
					else
						if (bUp)
						{
							menu_beep();
							if (skinchangerActiveLineIndex == 0)
								skinchangerActiveLineIndex = lineCount;
							skinchangerActiveLineIndex--;
							waitTime = 200;
						}
						else
							if (bDown)
							{
								menu_beep();
								skinchangerActiveLineIndex++;
								if (skinchangerActiveLineIndex == lineCount)
									skinchangerActiveLineIndex = 0;
								waitTime = 200;
							}
		if (skinchangerActiveLineIndex == (lineCount - 1))
			if (skinchangerActiveItemIndex >= itemCountLastLine)
				skinchangerActiveItemIndex = 0;
	}
	return false;
}

int teleportActiveLineIndex = 0;

bool process_teleport_menu()
{
	const float lineWidth = 250.0;
	const int lineCount = 17;

	std::string caption = "TELEPORT";

	static struct {
		LPCSTR  text;
		float x;
		float y;
		float z;
	} lines[lineCount] = {
		{ "MARKER" },
		{ "MICHAEL'S HOUSE", -852.4f, 160.0f, 65.6f },
		{ "FRANKLIN'S HOUSE", 7.9f, 548.1f, 175.5f },
		{ "TREVOR'S TRAILER", 1985.7f, 3812.2f, 32.2f },
		{ "AIRPORT ENTRANCE", -1034.6f, -2733.6f, 13.8f },
		{ "AIRPORT FIELD", -1336.0f, -3044.0f, 13.9f },
		{ "ELYSIAN ISLAND", 338.2f, -2715.9f, 38.5f },
		{ "JETSAM", 760.4f, -2943.2f, 5.8f },
		{ "STRIPCLUB", 127.4f, -1307.7f, 29.2f },
		{ "ELBURRO HEIGHTS", 1384.0f, -2057.1f, 52.0f },
		{ "FERRIS WHEEL", -1670.7f, -1125.0f, 13.0f },
		{ "CHUMASH", -3192.6f, 1100.0f, 20.2f },
		{ "WINDFARM", 2354.0f, 1830.3f, 101.1f },
		{ "MILITARY BASE", -2047.4f, 3132.1f, 32.8f },
		{ "MCKENZIE AIRFIELD", 2121.7f, 4796.3f, 41.1f },
		{ "DESERT AIRFIELD", 1747.0f, 3273.7f, 41.1f },
		{ "CHILLIAD", 425.4f, 5614.3f, 766.5f }
	};

	DWORD waitTime = 150;
	while (true)
	{
		// timed menu draw, used for pause after active line switch
		DWORD maxTickCount = GetTickCount() + waitTime;
		do
		{
			// draw menu
			draw_menu_line(caption, lineWidth, 15.0, 18.0, 0.0, 5.0, false, true);
			for (int i = 0; i < lineCount; i++)
				if (i != teleportActiveLineIndex)
					draw_menu_line(lines[i].text, lineWidth, 12.0, 60.0 + i * 36.0, 0.0, 9.0, false, false);
			draw_menu_line(lines[teleportActiveLineIndex].text, lineWidth + 1.0, 12.0, 60.0 + teleportActiveLineIndex * 36.0, 0.0, 9.0, true, false);

			update_features();
			WAIT(0);
		} while (GetTickCount() < maxTickCount);
		waitTime = 0;

		// process buttons
		bool bSelect, bBack, bUp, bDown;
		get_button_state(&bSelect, &bBack, &bUp, &bDown, NULL, NULL);
		if (bSelect)
		{
			menu_beep();

			// get entity to teleport
			Entity e = PLAYER::PLAYER_PED_ID();
			if (PED::IS_PED_IN_ANY_VEHICLE(e, 0))
				e = PED::GET_VEHICLE_PED_IS_USING(e);

			// get coords
			Vector3 coords;
			bool success = false;
			if (teleportActiveLineIndex == 0) // marker
			{
				bool blipFound = false;
				// search for marker blip
				int blipIterator = UI::_GET_BLIP_INFO_ID_ITERATOR();
				for (Blip i = UI::GET_FIRST_BLIP_INFO_ID(blipIterator); UI::DOES_BLIP_EXIST(i) != 0; i = UI::GET_NEXT_BLIP_INFO_ID(blipIterator))
				{
					if (UI::GET_BLIP_INFO_ID_TYPE(i) == 4)
					{
						coords = UI::GET_BLIP_INFO_ID_COORD(i);
						blipFound = true;
						break;
					}
				}
				if (blipFound)
				{
					// load needed map region and check height levels for ground existence
					bool groundFound = false;
					static float groundCheckHeight[] = {
						100.0, 150.0, 50.0, 0.0, 200.0, 250.0, 300.0, 350.0, 400.0,
						450.0, 500.0, 550.0, 600.0, 650.0, 700.0, 750.0, 800.0
					};
					for (int i = 0; i < sizeof(groundCheckHeight) / sizeof(float); i++)
					{
						ENTITY::SET_ENTITY_COORDS_NO_OFFSET(e, coords.x, coords.y, groundCheckHeight[i], 0, 0, 1);
						WAIT(100);
						if (GAMEPLAY::GET_GROUND_Z_FOR_3D_COORD(coords.x, coords.y, groundCheckHeight[i], &coords.z, FALSE))
						{
							groundFound = true;
							coords.z += 3.0;
							break;
						}
					}
					// if ground not found then set Z in air and give player a parachute
					if (!groundFound)
					{
						coords.z = 1000.0;
						WEAPON::GIVE_DELAYED_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0xFBAB5776, 1, 0);
					}
					success = true;
				}
				else
				{
					set_status_text("map marker isn't set");
				}

			}
			else // predefined coords
			{
				coords.x = lines[teleportActiveLineIndex].x;
				coords.y = lines[teleportActiveLineIndex].y;
				coords.z = lines[teleportActiveLineIndex].z;
				success = true;
			}

			// set player pos
			if (success)
			{
				ENTITY::SET_ENTITY_COORDS_NO_OFFSET(e, coords.x, coords.y, coords.z, 0, 0, 1);
				WAIT(0);
				set_status_text("teleported");
				return true;
			}

			waitTime = 200;
		}
		else
			if (bBack || trainer_switch_pressed())
			{
				menu_beep();
				break;
			}
			else
				if (bUp)
				{
					menu_beep();
					if (teleportActiveLineIndex == 0)
						teleportActiveLineIndex = lineCount;
					teleportActiveLineIndex--;
					waitTime = 150;
				}
				else
					if (bDown)
					{
						menu_beep();
						teleportActiveLineIndex++;
						if (teleportActiveLineIndex == lineCount)
							teleportActiveLineIndex = 0;
						waitTime = 150;
					}
	}
	return false;
}

std::string line_as_str(std::string text, bool *pState)
{
	while (text.size() < 18) text += " ";
	return text + (pState ? (*pState ? " ~b~On" : " ~r~Off") : "");
}

int activeLineIndexPlayer = 0;

void process_player_menu()
{
	const float lineWidth = 250.0;
	const int lineCount = 16;

	std::string caption = "PLAYER  OPTIONS";

	static struct {
		LPCSTR      text;
		bool        *pState;
		bool        *pUpdated;
	} lines[lineCount] = {
		{ "SKIN CHANGER", NULL, NULL }, //case 0
		{ "TELEPORT", NULL, NULL },
		{ "FIX PLAYER", NULL, NULL },
		{ "RESET SKIN", NULL, NULL },
		{ "ADD CASH", NULL, NULL },
		{ "WANTED UP", NULL, NULL },
		{ "WANTED DOWN", NULL, NULL },
		{ "NEVER WANTED", &featurePlayerNeverWanted, NULL },
		{ "INVINCIBLE", &featurePlayerInvincible, &featurePlayerInvincibleUpdated },
		{ "POLICE IGNORED", &featurePlayerIgnored, &featurePlayerIgnoredUpdated },
		{ "UNLIM ABILITY", &featurePlayerUnlimitedAbility, NULL },
		{ "NOISELESS", &featurePlayerNoNoise, &featurePlayerNoNoiseUpdated },
		{ "FAST SWIM", &featurePlayerFastSwim, &featurePlayerFastSwimUpdated },
		{ "FAST RUN", &featurePlayerFastRun, &featurePlayerFastRunUpdated },
		{ "SUPER JUMP", &featurePlayerSuperJump, NULL },
		{ "ULTRA JUMP", &featurePlayerUltraJump, NULL },
	};

	DWORD waitTime = 150;
	while (true)
	{
		// timed menu draw, used for pause after active line switch
		DWORD maxTickCount = GetTickCount() + waitTime;
		do
		{
			// draw menu
			draw_menu_line(caption, lineWidth, 7.0, 13.6, 4.0, 4.0, false, true);
			for (int i = 0; i < lineCount; i++)
				if (i != activeLineIndexPlayer)
					draw_menu_line(line_as_str(lines[i].text, lines[i].pState),
						lineWidth, 4.0, 60.0 + i * 24.0, 4.0, 9.0, false, false);
			draw_menu_line(line_as_str(lines[activeLineIndexPlayer].text, lines[activeLineIndexPlayer].pState),
				lineWidth + 1.0, 2.0, 60.0 + activeLineIndexPlayer * 24.0, 4.0, 7.0, true, false);

			update_features();
			WAIT(0);
		} while (GetTickCount() < maxTickCount);
		waitTime = 0;

		// process buttons
		bool bSelect, bBack, bUp, bDown;
		get_button_state(&bSelect, &bBack, &bUp, &bDown, NULL, NULL);
		if (bSelect)
		{
			menu_beep();

			// common variables
			BOOL bPlayerExists = ENTITY::DOES_ENTITY_EXIST(PLAYER::PLAYER_PED_ID());
			Player player = PLAYER::PLAYER_ID();
			Ped playerPed = PLAYER::PLAYER_PED_ID();

			switch (activeLineIndexPlayer)
			{
				// skin changer
			case 0:
				if (process_skinchanger_menu()) return;
				break;
				// teleport
			case 1:
				if (process_teleport_menu()) return;
				break;
				// fix player
			case 2:
			{
				ENTITY::SET_ENTITY_HEALTH(playerPed, ENTITY::GET_ENTITY_MAX_HEALTH(playerPed));
				PED::ADD_ARMOUR_TO_PED(playerPed, PLAYER::GET_PLAYER_MAX_ARMOUR(player) - PED::GET_PED_ARMOUR(playerPed));
				if (PED::IS_PED_IN_ANY_VEHICLE(playerPed, 0))
				{
					Vehicle playerVeh = PED::GET_VEHICLE_PED_IS_USING(playerPed);
					if (ENTITY::DOES_ENTITY_EXIST(playerVeh) && !ENTITY::IS_ENTITY_DEAD(playerVeh))
						VEHICLE::SET_VEHICLE_FIXED(playerVeh);
				}
				set_status_text("player fixed");
			}
			break;
			// reset model skin
			case 3:
			{
				PED::SET_PED_DEFAULT_COMPONENT_VARIATION(playerPed);
				set_status_text("using default model skin");
			}
			break;
			// add cash
			case 4:
				for (int i = 0; i < 3; i++)
				{
					char statNameFull[32];
					sprintf_s(statNameFull, "SP%d_TOTAL_CASH", i);
					Hash hash = GAMEPLAY::GET_HASH_KEY(statNameFull);
					int val;
					STATS::STAT_GET_INT(hash, &val, -1);
					val += 1000000;
					STATS::STAT_SET_INT(hash, val, 1);
				}
				set_status_text("cash added");
				break;
				// wanted up
			case 5:
				if (bPlayerExists && PLAYER::GET_PLAYER_WANTED_LEVEL(player) < 5)
				{
					PLAYER::SET_PLAYER_WANTED_LEVEL(player, PLAYER::GET_PLAYER_WANTED_LEVEL(player) + 1, 0);
					PLAYER::SET_PLAYER_WANTED_LEVEL_NOW(player, 0);
					set_status_text("wanted up");
				}
				break;
				// wanted down
			case 6:
				if (bPlayerExists && PLAYER::GET_PLAYER_WANTED_LEVEL(player) > 0)
				{
					PLAYER::SET_PLAYER_WANTED_LEVEL(player, PLAYER::GET_PLAYER_WANTED_LEVEL(player) - 1, 0);
					PLAYER::SET_PLAYER_WANTED_LEVEL_NOW(player, 0);
					set_status_text("wanted down");
				}
				break;
				// switchable features
			default:
				if (lines[activeLineIndexPlayer].pState)
					*lines[activeLineIndexPlayer].pState = !(*lines[activeLineIndexPlayer].pState);
				if (lines[activeLineIndexPlayer].pUpdated)
					*lines[activeLineIndexPlayer].pUpdated = true;
			}
			waitTime = 200;
		}
		else
			if (bBack || trainer_switch_pressed())
			{
				menu_beep();
				break;
			}
			else
				if (bUp)
				{
					menu_beep();
					if (activeLineIndexPlayer == 0)
						activeLineIndexPlayer = lineCount;
					activeLineIndexPlayer--;
					waitTime = 150;
				}
				else
					if (bDown)
					{
						menu_beep();
						activeLineIndexPlayer++;
						if (activeLineIndexPlayer == lineCount)
							activeLineIndexPlayer = 0;
						waitTime = 150;
					}
	}
}

int activeLineIndexWeapon = 0;

void process_weapon_menu()
{
	const float lineWidth = 250.0;
	const int lineCount = 7;

	std::string caption = "WEAPON  OPTIONS";

	static struct {
		LPCSTR      text;
		bool        *pState;
		bool        *pUpdated;
	} lines[lineCount] = {
		{ "GET ALL WEAPON",  NULL,                         NULL },
		{ "INFINITE AMMO",       &featureWeaponNoReload,       NULL },
		{ "FIRE AMMO",       &featureWeaponFireAmmo,       NULL },
		{ "MONEY GUN", &featureWeaponMoneyGun, NULL },
		{ "EXPLOSIVE AMMO",  &featureWeaponExplosiveAmmo,  NULL },
		{ "EXPLOSIVE MELEE", &featureWeaponExplosiveMelee, NULL },
		{ "VEHICLE ROCKETS", &featureWeaponVehRockets,     NULL }
	};

	static LPCSTR weaponNames[] = {
		"WEAPON_KNIFE", "WEAPON_NIGHTSTICK", "WEAPON_HAMMER", "WEAPON_BAT", "WEAPON_GOLFCLUB", "WEAPON_CROWBAR",
		"WEAPON_PISTOL", "WEAPON_COMBATPISTOL", "WEAPON_APPISTOL", "WEAPON_PISTOL50", "WEAPON_MICROSMG", "WEAPON_SMG",
		"WEAPON_ASSAULTSMG", "WEAPON_ASSAULTRIFLE", "WEAPON_CARBINERIFLE", "WEAPON_ADVANCEDRIFLE", "WEAPON_MG",
		"WEAPON_COMBATMG", "WEAPON_PUMPSHOTGUN", "WEAPON_SAWNOFFSHOTGUN", "WEAPON_ASSAULTSHOTGUN", "WEAPON_BULLPUPSHOTGUN",
		"WEAPON_STUNGUN", "WEAPON_SNIPERRIFLE", "WEAPON_HEAVYSNIPER", "WEAPON_GRENADELAUNCHER", "WEAPON_GRENADELAUNCHER_SMOKE",
		"WEAPON_RPG", "WEAPON_MINIGUN", "WEAPON_GRENADE", "WEAPON_STICKYBOMB", "WEAPON_SMOKEGRENADE", "WEAPON_BZGAS",
		"WEAPON_MOLOTOV", "WEAPON_FIREEXTINGUISHER", "WEAPON_PETROLCAN",
		"WEAPON_SNSPISTOL", "WEAPON_SPECIALCARBINE", "WEAPON_HEAVYPISTOL", "WEAPON_BULLPUPRIFLE", "WEAPON_HOMINGLAUNCHER",
		"WEAPON_PROXMINE", "WEAPON_SNOWBALL", "WEAPON_VINTAGEPISTOL", "WEAPON_DAGGER", "WEAPON_FIREWORK", "WEAPON_MUSKET",
		"WEAPON_MARKSMANRIFLE", "WEAPON_HEAVYSHOTGUN", "WEAPON_GUSENBERG", "WEAPON_HATCHET", "WEAPON_RAILGUN",
		"WEAPON_COMBATPDW", "WEAPON_KNUCKLE", "WEAPON_MARKSMANPISTOL",
		"WEAPON_FLASHLIGHT", "WEAPON_MACHETE", "WEAPON_MACHINEPISTOL",
		"WEAPON_SWITCHBLADE", "WEAPON_REVOLVER"
	};

	DWORD waitTime = 150;
	while (true)
	{
		// timed menu draw, used for pause after active line switch
		DWORD maxTickCount = GetTickCount() + waitTime;
		do
		{
			// draw menu
			draw_menu_line(caption, lineWidth, 7.0, 13.6, 4.0, 4.0, false, true);
			for (int i = 0; i < lineCount; i++)
				if (i != activeLineIndexWeapon)
					draw_menu_line(line_as_str(lines[i].text, lines[i].pState),
						lineWidth, 4.0, 60.0 + i * 24.0, 4.0, 9.0, false, false);
			draw_menu_line(line_as_str(lines[activeLineIndexWeapon].text, lines[activeLineIndexWeapon].pState),
				lineWidth + 1.0, 2.0, 60.0 + activeLineIndexWeapon * 24.0, 4.0, 7.0, true, false);

			update_features();
			WAIT(0);
		} while (GetTickCount() < maxTickCount);
		waitTime = 0;

		// process buttons
		bool bSelect, bBack, bUp, bDown;
		get_button_state(&bSelect, &bBack, &bUp, &bDown, NULL, NULL);
		if (bSelect)
		{
			menu_beep();

			// common variables
			BOOL bPlayerExists = ENTITY::DOES_ENTITY_EXIST(PLAYER::PLAYER_PED_ID());
			Player player = PLAYER::PLAYER_ID();
			Ped playerPed = PLAYER::PLAYER_PED_ID();

			switch (activeLineIndexWeapon)
			{
			case 0:
				for (int i = 0; i < sizeof(weaponNames) / sizeof(weaponNames[0]); i++)
					WEAPON::GIVE_DELAYED_WEAPON_TO_PED(playerPed, GAMEPLAY::GET_HASH_KEY((char *)weaponNames[i]), 1000, 0);
				set_status_text("all weapon added");
				break;
				// switchable features
			default:
				if (lines[activeLineIndexWeapon].pState)
					*lines[activeLineIndexWeapon].pState = !(*lines[activeLineIndexWeapon].pState);
				if (lines[activeLineIndexWeapon].pUpdated)
					*lines[activeLineIndexWeapon].pUpdated = true;
			}
			waitTime = 200;
		}
		else
			if (bBack || trainer_switch_pressed())
			{
				menu_beep();
				break;
			}
			else
				if (bUp)
				{
					menu_beep();
					if (activeLineIndexWeapon == 0)
						activeLineIndexWeapon = lineCount;
					activeLineIndexWeapon--;
					waitTime = 150;
				}
				else
					if (bDown)
					{
						menu_beep();
						activeLineIndexWeapon++;
						if (activeLineIndexWeapon == lineCount)
							activeLineIndexWeapon = 0;
						waitTime = 150;
					}
	}
}

LPCSTR vehicleModels[40][10] = {
	{ "NINEF", "NINEF2", "BLISTA", "ASEA", "ASEA2", "BOATTRAILER", "BUS", "ARMYTANKER", "ARMYTRAILER", "ARMYTRAILER2" },
	{ "SUNTRAP", "COACH", "AIRBUS", "ASTEROPE", "AIRTUG", "AMBULANCE", "BARRACKS", "BARRACKS2", "BALLER", "BALLER2" },
	{ "BJXL", "BANSHEE", "BENSON", "BFINJECTION", "BIFF", "BLAZER", "BLAZER2", "BLAZER3", "BISON", "BISON2" },
	{ "BISON3", "BOXVILLE", "BOXVILLE2", "BOXVILLE3", "BOBCATXL", "BODHI2", "BUCCANEER", "BUFFALO", "BUFFALO2", "BULLDOZER" },
	{ "BULLET", "BLIMP", "BURRITO", "BURRITO2", "BURRITO3", "BURRITO4", "BURRITO5", "CAVALCADE", "CAVALCADE2", "POLICET" },
	{ "GBURRITO", "CABLECAR", "CADDY", "CADDY2", "CAMPER", "CARBONIZZARE", "CHEETAH", "COMET2", "COGCABRIO", "COQUETTE" },
	{ "CUTTER", "GRESLEY", "DILETTANTE", "DILETTANTE2", "DUNE", "DUNE2", "HOTKNIFE", "DLOADER", "DUBSTA", "DUBSTA2" },
	{ "DUMP", "RUBBLE", "DOCKTUG", "DOMINATOR", "EMPEROR", "EMPEROR2", "EMPEROR3", "ENTITYXF", "EXEMPLAR", "ELEGY2" },
	{ "F620", "FBI", "FBI2", "FELON", "FELON2", "FELTZER2", "FIRETRUK", "FLATBED", "FORKLIFT", "FQ2" },
	{ "FUSILADE", "FUGITIVE", "FUTO", "GRANGER", "GAUNTLET", "HABANERO", "HAULER", "HANDLER", "INFERNUS", "INGOT" },
	{ "INTRUDER", "ISSI2", "JACKAL", "JOURNEY", "JB700", "KHAMELION", "LANDSTALKER", "LGUARD", "MANANA", "MESA" },
	{ "MESA2", "MESA3", "CRUSADER", "MINIVAN", "MIXER", "MIXER2", "MONROE", "MOWER", "MULE", "MULE2" },
	{ "ORACLE", "ORACLE2", "PACKER", "PATRIOT", "PBUS", "PENUMBRA", "PEYOTE", "PHANTOM", "PHOENIX", "PICADOR" },
	{ "POUNDER", "POLICE", "POLICE4", "POLICE2", "POLICE3", "POLICEOLD1", "POLICEOLD2", "PONY", "PONY2", "PRAIRIE" },
	{ "PRANGER", "PREMIER", "PRIMO", "PROPTRAILER", "RANCHERXL", "RANCHERXL2", "RAPIDGT", "RAPIDGT2", "RADI", "RATLOADER" },
	{ "REBEL", "REGINA", "REBEL2", "RENTALBUS", "RUINER", "RUMPO", "RUMPO2", "RHINO", "RIOT", "RIPLEY" },
	{ "ROCOTO", "ROMERO", "SABREGT", "SADLER", "SADLER2", "SANDKING", "SANDKING2", "SCHAFTER2", "SCHWARZER", "SCRAP" },
	{ "SEMINOLE", "SENTINEL", "SENTINEL2", "ZION", "ZION2", "SERRANO", "SHERIFF", "SHERIFF2", "SPEEDO", "SPEEDO2" },
	{ "STANIER", "STINGER", "STINGERGT", "STOCKADE", "STOCKADE3", "STRATUM", "SULTAN", "SUPERD", "SURANO", "SURFER" },
	{ "SURFER2", "SURGE", "TACO", "TAILGATER", "TAXI", "TRASH", "TRACTOR", "TRACTOR2", "TRACTOR3", "GRAINTRAILER" },
	{ "BALETRAILER", "TIPTRUCK", "TIPTRUCK2", "TORNADO", "TORNADO2", "TORNADO3", "TORNADO4", "TOURBUS", "TOWTRUCK", "TOWTRUCK2" },
	{ "UTILLITRUCK", "UTILLITRUCK2", "UTILLITRUCK3", "VOODOO2", "WASHINGTON", "STRETCH", "YOUGA", "ZTYPE", "SANCHEZ", "SANCHEZ2" },
	{ "SCORCHER", "TRIBIKE", "TRIBIKE2", "TRIBIKE3", "FIXTER", "CRUISER", "BMX", "POLICEB", "AKUMA", "CARBONRS" },
	{ "BAGGER", "BATI", "BATI2", "RUFFIAN", "DAEMON", "DOUBLE", "PCJ", "VADER", "VIGERO", "FAGGIO2" },
	{ "HEXER", "ANNIHILATOR", "BUZZARD", "BUZZARD2", "CARGOBOB", "CARGOBOB2", "CARGOBOB3", "SKYLIFT", "POLMAV", "MAVERICK" },
	{ "NEMESIS", "FROGGER", "FROGGER2", "CUBAN800", "DUSTER", "STUNT", "MAMMATUS", "JET", "SHAMAL", "LUXOR" },
	{ "TITAN", "LAZER", "CARGOPLANE", "SQUALO", "MARQUIS", "DINGHY", "DINGHY2", "JETMAX", "PREDATOR", "TROPIC" },
	{ "SEASHARK", "SEASHARK2", "SUBMERSIBLE", "TRAILERS", "TRAILERS2", "TRAILERS3", "TVTRAILER", "RAKETRAILER", "TANKER", "TRAILERLOGS" },
	{ "TR2", "TR3", "TR4", "TRFLAT", "TRAILERSMALL", "VELUM", "ADDER", "VOLTIC", "VACCA", "BIFTA" },
	{ "SPEEDER", "PARADISE", "KALAHARI", "JESTER", "TURISMOR", "VESTRA", "ALPHA", "HUNTLEY", "THRUST", "MASSACRO" },
	{ "MASSACRO2", "ZENTORNO", "BLADE", "GLENDALE", "PANTO", "PIGALLE", "WARRENER", "RHAPSODY", "DUBSTA3", "MONSTER" },
	{ "SOVEREIGN", "INNOVATION", "HAKUCHOU", "FUROREGT", "MILJET", "COQUETTE2", "BTYPE", "BUFFALO3", "DOMINATOR2", "GAUNTLET2" },
	{ "MARSHALL", "DUKES", "DUKES2", "STALION", "STALION2", "BLISTA2", "BLISTA3", "DODO", "SUBMERSIBLE2", "HYDRA" },
	{ "INSURGENT", "INSURGENT2", "TECHNICAL", "SAVAGE", "VALKYRIE", "KURUMA", "KURUMA2", "JESTER2", "CASCO", "VELUM2" },
	{ "GUARDIAN", "ENDURO", "LECTRO", "SLAMVAN", "SLAMVAN2", "RATLOADER2", "SWIFT2", "LUXOR2", "FELTZER3", "OSIRIS" },
	{ "VIRGO", "WINDSOR", "BESRA", "SWIFT", "BLIMP2", "VINDICATOR", "TORO", "T20", "COQUETTE3", "CHINO" },
	{ "BRAWLER", "BUCCANEER2", "CHINO2", "FACTION", "FACTION2", "MOONBEAM", "MOONBEAM2", "PRIMO2", "VOODOO", "LURCHER" },
	{ "BTYPE2", "BALLER3", "BALLER4", "BALLER5", "BALLER6", "CARGOBOB4", "COG55", "COG552", "COGNOSCENTI", "COGNOSCENTI2" },
	{ "DINGHY4", "LIMO2", "MAMBA", "NIGHTSHADE", "SCHAFTER3", "SCHAFTER4", "SCHAFTER5", "SCHAFTER6", "SEASHARK3", "SPEEDER2" },
	{ "SUPERVOLITO", "SUPERVOLITO2", "TORO2", "TROPIC2", "VALKYRIE2", "VERLIERER2", "TAMPA", "BANSHEE2", "SULTANRS", "BTYPE3" }
};

int carspawnActiveLineIndex = 0;
int carspawnActiveItemIndex = 0;

bool process_carspawn_menu()
{
	DWORD waitTime = 150;
	const int lineCount = 40;
	const int itemCount = 10;
	const int itemCountLastLine = 10;
	while (true)
	{
		// timed menu draw, used for pause after active line switch
		DWORD maxTickCount = GetTickCount() + waitTime;
		do
		{
			// draw menu
			char caption[32];
			sprintf_s(caption, "Car Spawner   %d / %d", carspawnActiveLineIndex + 1, lineCount);
			draw_menu_line(caption, 350.0, 15.0, 18.0, 0.0, 5.0, false, true);
			for (int i = 0; i < itemCount; i++)
				if (strlen(vehicleModels[carspawnActiveLineIndex][i]))
					draw_menu_line(vehicleModels[carspawnActiveLineIndex][i], 100.0, 5.0, 200.0, 100.0 + i * 110.0, 5.0, i == carspawnActiveItemIndex, false, false);

			update_features();
			WAIT(0);
		} while (GetTickCount() < maxTickCount);
		waitTime = 0;

		bool bSelect, bBack, bUp, bDown, bLeft, bRight;
		get_button_state(&bSelect, &bBack, &bUp, &bDown, &bLeft, &bRight);

		if (bSelect)
		{
			menu_beep();
			LPCSTR modelName = vehicleModels[carspawnActiveLineIndex][carspawnActiveItemIndex];
			DWORD model = GAMEPLAY::GET_HASH_KEY((char *)modelName);
			if (STREAMING::IS_MODEL_IN_CDIMAGE(model) && STREAMING::IS_MODEL_A_VEHICLE(model))
			{
				STREAMING::REQUEST_MODEL(model);
				while (!STREAMING::HAS_MODEL_LOADED(model)) WAIT(0);
				Vector3 coords = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(PLAYER::PLAYER_PED_ID(), 0.0, 5.0, 0.0);
				Vehicle veh = VEHICLE::CREATE_VEHICLE(model, coords.x, coords.y, coords.z, 0.0, 1, 1);
				VEHICLE::SET_VEHICLE_ON_GROUND_PROPERLY(veh);

				if (featureVehWrapInSpawned)
				{
					ENTITY::SET_ENTITY_HEADING(veh, ENTITY::GET_ENTITY_HEADING(PLAYER::PLAYER_PED_ID()));
					PED::SET_PED_INTO_VEHICLE(PLAYER::PLAYER_PED_ID(), veh, -1);
				}

				WAIT(0);
				STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(model);
				ENTITY::SET_VEHICLE_AS_NO_LONGER_NEEDED(&veh);

				char statusText[32];
				sprintf_s(statusText, "%s spawned", modelName);
				set_status_text(statusText);

				return true;
			}
		}
		else
			if (bBack)
			{
				menu_beep();
				break;
			}
			else
				if (bRight)
				{
					menu_beep();
					carspawnActiveItemIndex++;
					int itemsMax = (carspawnActiveLineIndex == (lineCount - 1)) ? itemCountLastLine : itemCount;
					if (carspawnActiveItemIndex == itemsMax)
						carspawnActiveItemIndex = 0;
					waitTime = 100;
				}
				else
					if (bLeft)
					{
						menu_beep();
						if (carspawnActiveItemIndex == 0)
							carspawnActiveItemIndex = (carspawnActiveLineIndex == (lineCount - 1)) ? itemCountLastLine : itemCount;
						carspawnActiveItemIndex--;
						waitTime = 100;
					}
					else
						if (bUp)
						{
							menu_beep();
							if (carspawnActiveLineIndex == 0)
								carspawnActiveLineIndex = lineCount;
							carspawnActiveLineIndex--;
							waitTime = 200;
						}
						else
							if (bDown)
							{
								menu_beep();
								carspawnActiveLineIndex++;
								if (carspawnActiveLineIndex == lineCount)
									carspawnActiveLineIndex = 0;
								waitTime = 200;
							}
		if (carspawnActiveLineIndex == (lineCount - 1))
			if (carspawnActiveItemIndex >= itemCountLastLine)
				carspawnActiveItemIndex = 0;
	}
	return false;
}

int activeLineIndexVeh = 0;

void process_veh_menu()
{
	const float lineWidth = 250.0;
	const int lineCount = 8;

	std::string caption = "VEHICLE  OPTIONS";

	static struct {
		LPCSTR      text;
		bool        *pState;
		bool        *pUpdated;
	} lines[lineCount] = {
		{ "CAR SPAWNER",     NULL, NULL },
		{ "PAINT RANDOM",    NULL, NULL },
		{ "FIX",             NULL, NULL },
		{ "SEAT BELT",       &featureVehSeatbelt, &featureVehSeatbeltUpdated },
		{ "WRAP IN SPAWNED", &featureVehWrapInSpawned, NULL },
		{ "INVINCIBLE",      &featureVehInvincible, &featureVehInvincibleUpdated },
		{ "STRONG WHEELS",   &featureVehInvincibleWheels, &featureVehInvincibleWheelsUpdated },
		{ "SPEED BOOST",     &featureVehSpeedBoost, NULL }
	};

	DWORD waitTime = 150;
	while (true)
	{
		// timed menu draw, used for pause after active line switch
		DWORD maxTickCount = GetTickCount() + waitTime;
		do
		{
			// draw menu
			draw_menu_line(caption, lineWidth, 7.0, 13.6, 4.0, 4.0, false, true);
			for (int i = 0; i < lineCount; i++)
				if (i != activeLineIndexVeh)
					draw_menu_line(line_as_str(lines[i].text, lines[i].pState), lineWidth, 4.0, 60.0 + i * 24.0, 4.0, 9.0, false, false);
			draw_menu_line(line_as_str(lines[activeLineIndexVeh].text,
				lines[activeLineIndexVeh].pState), lineWidth + 1.0, 2.0, 60.0 + activeLineIndexVeh * 24.0, 4.0, 7.0, true, false);

			update_features();
			WAIT(0);
		} while (GetTickCount() < maxTickCount);
		waitTime = 0;

		// process buttons
		bool bSelect, bBack, bUp, bDown;
		get_button_state(&bSelect, &bBack, &bUp, &bDown, NULL, NULL);
		if (bSelect)
		{
			menu_beep();

			// common variables
			BOOL bPlayerExists = ENTITY::DOES_ENTITY_EXIST(PLAYER::PLAYER_PED_ID());
			Player player = PLAYER::PLAYER_ID();
			Ped playerPed = PLAYER::PLAYER_PED_ID();

			switch (activeLineIndexVeh)
			{
			case 0:
				if (process_carspawn_menu()) return;
				break;
			case 1:
				if (bPlayerExists)
				{
					if (PED::IS_PED_IN_ANY_VEHICLE(playerPed, 0))
					{
						Vehicle veh = PED::GET_VEHICLE_PED_IS_USING(playerPed);
						VEHICLE::SET_VEHICLE_CUSTOM_PRIMARY_COLOUR(veh, rand() % 255, rand() % 255, rand() % 255);
						if (VEHICLE::GET_IS_VEHICLE_PRIMARY_COLOUR_CUSTOM(veh))
							VEHICLE::SET_VEHICLE_CUSTOM_SECONDARY_COLOUR(veh, rand() % 255, rand() % 255, rand() % 255);
					}
					else
					{
						set_status_text("player isn't in a vehicle");
					}
				}
				break;
			case 2:
				if (bPlayerExists)
					if (PED::IS_PED_IN_ANY_VEHICLE(playerPed, 0))
						VEHICLE::SET_VEHICLE_FIXED(PED::GET_VEHICLE_PED_IS_USING(playerPed));
					else
						set_status_text("player isn't in a vehicle");
				break;
				// switchable features
			default:
				if (lines[activeLineIndexVeh].pState)
					*lines[activeLineIndexVeh].pState = !(*lines[activeLineIndexVeh].pState);
				if (lines[activeLineIndexVeh].pUpdated)
					*lines[activeLineIndexVeh].pUpdated = true;
			}
			waitTime = 200;
		}
		else
			if (bBack || trainer_switch_pressed())
			{
				menu_beep();
				break;
			}
			else
				if (bUp)
				{
					menu_beep();
					if (activeLineIndexVeh == 0)
						activeLineIndexVeh = lineCount;
					activeLineIndexVeh--;
					waitTime = 150;
				}
				else
					if (bDown)
					{
						menu_beep();
						activeLineIndexVeh++;
						if (activeLineIndexVeh == lineCount)
							activeLineIndexVeh = 0;
						waitTime = 150;
					}
	}
}

int activeLineIndexWorld = 0;

void process_world_menu()
{
	const float lineWidth = 250.0;
	const int lineCount = 5;

	std::string caption = "WORLD  OPTIONS";

	static struct {
		LPCSTR      text;
		bool        *pState;
		bool        *pUpdated;
	} lines[lineCount] = {
		{ "MOON GRAVITY",    &featureWorldMoonGravity,   NULL },
		{ "RANDOM COPS",     &featureWorldRandomCops,    NULL },
		{ "RANDOM TRAINS",   &featureWorldRandomTrains,  NULL },
		{ "RANDOM BOATS",    &featureWorldRandomBoats,   NULL },
		{ "GARBAGE TRUCKS",  &featureWorldGarbageTrucks, NULL }
	};

	DWORD waitTime = 150;
	while (true)
	{
		// timed menu draw, used for pause after active line switch
		DWORD maxTickCount = GetTickCount() + waitTime;
		do
		{
			// draw menu
			draw_menu_line(caption, lineWidth, 7.0, 13.6, 4.0, 4.0, false, true);
			for (int i = 0; i < lineCount; i++)
				if (i != activeLineIndexWorld)
					draw_menu_line(line_as_str(lines[i].text, lines[i].pState), lineWidth, 4.0, 60.0 + i * 24.0, 4.0, 9.0, false, false);
			draw_menu_line(line_as_str(lines[activeLineIndexWorld].text,
				lines[activeLineIndexWorld].pState), lineWidth + 1.0, 2.0, 60.0 + activeLineIndexWorld * 24.0, 4.0, 7.0, true, false);

			update_features();
			WAIT(0);
		} while (GetTickCount() < maxTickCount);
		waitTime = 0;

		// process buttons
		bool bSelect, bBack, bUp, bDown;
		get_button_state(&bSelect, &bBack, &bUp, &bDown, NULL, NULL);
		if (bSelect)
		{
			menu_beep();
			switch (activeLineIndexWorld)
			{
			case 0:
				featureWorldMoonGravity = !featureWorldMoonGravity;
				GAMEPLAY::SET_GRAVITY_LEVEL(featureWorldMoonGravity ? 2 : 0);
				break;
			case 1:
				// featureWorldRandomCops being set in update_features
				PED::SET_CREATE_RANDOM_COPS(!featureWorldRandomCops);
				break;
			case 2:
				featureWorldRandomTrains = !featureWorldRandomTrains;
				VEHICLE::SET_RANDOM_TRAINS(featureWorldRandomTrains);
				break;
			case 3:
				featureWorldRandomBoats = !featureWorldRandomBoats;
				VEHICLE::SET_RANDOM_BOATS(featureWorldRandomBoats);
				break;
			case 4:
				featureWorldGarbageTrucks = !featureWorldGarbageTrucks;
				VEHICLE::SET_GARBAGE_TRUCKS(featureWorldGarbageTrucks);
				break;
			}
			waitTime = 200;
		}
		else
			if (bBack || trainer_switch_pressed())
			{
				menu_beep();
				break;
			}
			else
				if (bUp)
				{
					menu_beep();
					if (activeLineIndexWorld == 0)
						activeLineIndexWorld = lineCount;
					activeLineIndexWorld--;
					waitTime = 150;
				}
				else
					if (bDown)
					{
						menu_beep();
						activeLineIndexWorld++;
						if (activeLineIndexWorld == lineCount)
							activeLineIndexWorld = 0;
						waitTime = 150;
					}
	}
}

int activeLineIndexTime = 0;

void process_time_menu()
{
	const float lineWidth = 250.0;
	const int lineCount = 4;

	std::string caption = "TIME  OPTIONS";

	static struct {
		LPCSTR      text;
		bool        *pState;
		bool        *pUpdated;
	} lines[lineCount] = {
		{ "HOUR FORWARD",     NULL,               NULL },
		{ "HOUR BACKWARD",    NULL,               NULL },
		{ "CLOCK PAUSED",     &featureTimePaused, &featureTimePausedUpdated },
		{ "SYNC WITH SYSTEM", &featureTimeSynced, NULL }
	};

	DWORD waitTime = 150;
	while (true)
	{
		// timed menu draw, used for pause after active line switch
		DWORD maxTickCount = GetTickCount() + waitTime;
		do
		{
			// draw menu
			draw_menu_line(caption, lineWidth, 7.0, 13.6, 4.0, 4.0, false, true);
			for (int i = 0; i < lineCount; i++)
				if (i != activeLineIndexTime)
					draw_menu_line(line_as_str(lines[i].text, lines[i].pState), lineWidth, 4.0, 60.0 + i * 24.0, 4.0, 9.0, false, false);
			draw_menu_line(line_as_str(lines[activeLineIndexTime].text,
				lines[activeLineIndexTime].pState), lineWidth + 1.0, 2.0, 60.0 + activeLineIndexTime * 24.0, 4.0, 7.0, true, false);

			update_features();
			WAIT(0);
		} while (GetTickCount() < maxTickCount);
		waitTime = 0;

		// process buttons
		bool bSelect, bBack, bUp, bDown;
		get_button_state(&bSelect, &bBack, &bUp, &bDown, NULL, NULL);
		if (bSelect)
		{
			menu_beep();
			switch (activeLineIndexTime)
			{
				// hour forward/backward
			case 0:
			case 1:
			{
				int h = TIME::GET_CLOCK_HOURS();
				if (activeLineIndexTime == 0) h = (h == 23) ? 0 : h + 1; else h = (h == 0) ? 23 : h - 1;
				int m = TIME::GET_CLOCK_MINUTES();
				TIME::SET_CLOCK_TIME(h, m, 0);
				char text[32];
				sprintf_s(text, "time %d:%d", h, m);
				set_status_text(text);
			}
			break;
			// switchable features
			default:
				if (lines[activeLineIndexTime].pState)
					*lines[activeLineIndexTime].pState = !(*lines[activeLineIndexTime].pState);
				if (lines[activeLineIndexTime].pUpdated)
					*lines[activeLineIndexTime].pUpdated = true;
			}
			waitTime = 200;
		}
		else
			if (bBack || trainer_switch_pressed())
			{
				menu_beep();
				break;
			}
			else
				if (bUp)
				{
					menu_beep();
					if (activeLineIndexTime == 0)
						activeLineIndexTime = lineCount;
					activeLineIndexTime--;
					waitTime = 150;
				}
				else
					if (bDown)
					{
						menu_beep();
						activeLineIndexTime++;
						if (activeLineIndexTime == lineCount)
							activeLineIndexTime = 0;
						waitTime = 150;
					}
	}
}

int activeLineIndexWeather = 0;

void process_weather_menu()
{
	const float lineWidth = 250.0;
	const int lineCount = 16;

	std::string caption = "WEATHER  OPTIONS";

	static struct {
		LPCSTR      text;
		bool        *pState;
		bool        *pUpdated;
	} lines[lineCount] = {
		{ "WIND",         &featureWeatherWind,   NULL },
		{ "SET PERSIST",  &featureWeatherPers,   NULL },
		{ "EXTRASUNNY",   NULL,                  NULL },
		{ "CLEAR",        NULL,                  NULL },
		{ "CLOUDS",       NULL,                  NULL },
		{ "SMOG",         NULL,                  NULL },
		{ "FOGGY",        NULL,                  NULL },
		{ "OVERCAST",     NULL,                  NULL },
		{ "RAIN",         NULL,                  NULL },
		{ "THUNDER",      NULL,                  NULL },
		{ "CLEARING",     NULL,                  NULL },
		{ "NEUTRAL",      NULL,                  NULL },
		{ "SNOW",         NULL,                  NULL },
		{ "BLIZZARD",     NULL,                  NULL },
		{ "SNOWLIGHT",    NULL,                  NULL },
		{ "XMAS",         NULL,                  NULL }
	};


	DWORD waitTime = 150;
	while (true)
	{
		// timed menu draw, used for pause after active line switch
		DWORD maxTickCount = GetTickCount() + waitTime;
		do
		{
			// draw menu
			draw_menu_line(caption, lineWidth, 7.0, 13.6, 4.0, 4.0, false, true);
			for (int i = 0; i < lineCount; i++)
				if (i != activeLineIndexWeather)
					draw_menu_line(line_as_str(lines[i].text, lines[i].pState), lineWidth, 4.0, 60.0 + i * 24.0, 4.0, 9.0, false, false);
			draw_menu_line(line_as_str(lines[activeLineIndexWeather].text,
				lines[activeLineIndexWeather].pState), lineWidth + 1.0, 2.0, 60.0 + activeLineIndexWeather * 24.0, 4.0, 7.0, true, false);

			update_features();
			WAIT(0);
		} while (GetTickCount() < maxTickCount);
		waitTime = 0;

		// process buttons
		bool bSelect, bBack, bUp, bDown;
		get_button_state(&bSelect, &bBack, &bUp, &bDown, NULL, NULL);
		if (bSelect)
		{
			menu_beep();
			switch (activeLineIndexWeather)
			{
				// wind
			case 0:
				featureWeatherWind = !featureWeatherWind;
				if (featureWeatherWind)
				{
					GAMEPLAY::SET_WIND(1.0);
					GAMEPLAY::SET_WIND_SPEED(11.99);
					GAMEPLAY::SET_WIND_DIRECTION(ENTITY::GET_ENTITY_HEADING(PLAYER::PLAYER_PED_ID()));
				}
				else
				{
					GAMEPLAY::SET_WIND(0.0);
					GAMEPLAY::SET_WIND_SPEED(0.0);
				}
				break;
				// set persist
			case 1:
				featureWeatherPers = !featureWeatherPers;
				break;
				// set weather
			default:
				GAMEPLAY::CLEAR_OVERRIDE_WEATHER();
				if (featureWeatherPers)
				{
					GAMEPLAY::SET_OVERRIDE_WEATHER((char *)lines[activeLineIndexWeather].text);
				}
				else
				{
					GAMEPLAY::SET_WEATHER_TYPE_NOW_PERSIST((char *)lines[activeLineIndexWeather].text);
					GAMEPLAY::CLEAR_WEATHER_TYPE_PERSIST();
				}
				set_status_text(lines[activeLineIndexWeather].text);
			}
			waitTime = 200;
		}
		else
			if (bBack || trainer_switch_pressed())
			{
				menu_beep();
				break;
			}
			else
				if (bUp)
				{
					menu_beep();
					if (activeLineIndexWeather == 0)
						activeLineIndexWeather = lineCount;
					activeLineIndexWeather--;
					waitTime = 150;
				}
				else
					if (bDown)
					{
						menu_beep();
						activeLineIndexWeather++;
						if (activeLineIndexWeather == lineCount)
							activeLineIndexWeather = 0;
						waitTime = 150;
					}
	}
}

int activeLineIndexObj = 0;

void process_obj_menu()
{
	Ped playerPed = PLAYER::PLAYER_PED_ID();
	Vector3 position = ENTITY::GET_ENTITY_COORDS(playerPed, 1);

	const float lineWidth = 260.0;
	const int lineCount = 18;

	std::string caption = "Object Spawner";

	static struct {
		LPCSTR		text;
		bool		*pState;
		bool		*pUpdated;
	} lines[lineCount] = {
		{ "Pumpkin", NULL, NULL },
		{ "ufo", NULL, NULL },
		{ "Weed Leaves", NULL, NULL },
		{ "Christmas Tree", NULL, NULL },
		{ "Windmill", NULL, NULL },
		{ "Alien Egg", NULL, NULL },
		{ "Dildo", NULL, NULL },
		{ "Toilet", NULL, NULL },
		{ "Ferris Wheel", NULL, NULL },
		{ "Cable Car", NULL, NULL },
		{ "Half Pipe", NULL, NULL },
		{ "Giant Orange", NULL, NULL },
		{ "Drugs", NULL, NULL },
		{ "Roller Car", NULL, NULL },
		{ "Laptop", NULL, NULL },
		{ "ATM Machine", NULL, NULL },
		{ "Trolly Of Gold", NULL, NULL },
		{ "Input Object ~h~~o~>>", NULL, NULL }


	};


	DWORD waitTime = 150;
	while (true)
	{
		// timed menu draw, used for pause after active line switch
		DWORD maxTickCount = GetTickCount() + waitTime;
		do
		{
			// draw menu
			draw_menu_line(caption, lineWidth, 7.0, 13.6, 4.0, 4.0, false, true);
			for (int i = 0; i < lineCount; i++)
				if (i != activeLineIndexObj)
					draw_menu_line(line_as_str(lines[i].text, lines[i].pState), lineWidth, 4.0, 60.0 + i * 24.0, 4.0, 9.0, false, false);
			draw_menu_line(line_as_str(lines[activeLineIndexObj].text,
				lines[activeLineIndexObj].pState), lineWidth + 1.0, 2.0, 60.0 + activeLineIndexObj * 24.0, 4.0, 7.0, true, false);


			update_features();
			WAIT(0);
		} while (GetTickCount() < maxTickCount);
		waitTime = 0;


		// process buttons
		bool bSelect, bBack, bUp, bDown;
		get_button_state(&bSelect, &bBack, &bUp, &bDown, NULL, NULL);
		if (bSelect)
		{
			menu_beep();
			switch (activeLineIndexObj)
			{
				// next radio track
			case 0:
				if (ENTITY::DOES_ENTITY_EXIST(PLAYER::PLAYER_PED_ID()))
				{
					STREAMING::REQUEST_MODEL(0x83C07621);
					while (!STREAMING::HAS_MODEL_LOADED(0x83C07621))
						WAIT(0);
					OBJECT::CREATE_OBJECT((0x83C07621), position.x, position.y, position.z, 0, 3000, 1000);
					return;
					break;
				}
			case 1:
				if (ENTITY::DOES_ENTITY_EXIST(PLAYER::PLAYER_PED_ID()))
				{
					STREAMING::REQUEST_MODEL(GAMEPLAY::GET_HASH_KEY("p_spinning_anus_s"));
					while (!STREAMING::HAS_MODEL_LOADED(GAMEPLAY::GET_HASH_KEY("p_spinning_anus_s")))
						WAIT(0);
					OBJECT::CREATE_OBJECT(GAMEPLAY::GET_HASH_KEY("p_spinning_anus_s"), position.x, position.y, position.z, 0, 3000, 1000);
					return;
					break;
				}
			case 2:
				if (ENTITY::DOES_ENTITY_EXIST(PLAYER::PLAYER_PED_ID()))
				{
					STREAMING::REQUEST_MODEL(GAMEPLAY::GET_HASH_KEY("prop_weed_01"));
					while (!STREAMING::HAS_MODEL_LOADED(GAMEPLAY::GET_HASH_KEY("prop_weed_01")))
						WAIT(0);
					OBJECT::CREATE_OBJECT(GAMEPLAY::GET_HASH_KEY("prop_weed_01"), position.x, position.y, position.z, 0, 3000, 1000);
					return;
					break;
				}
			case 3:
				if (ENTITY::DOES_ENTITY_EXIST(PLAYER::PLAYER_PED_ID()))
				{
					STREAMING::REQUEST_MODEL(GAMEPLAY::GET_HASH_KEY("prop_xmas_tree_int"));
					while (!STREAMING::HAS_MODEL_LOADED(GAMEPLAY::GET_HASH_KEY("prop_xmas_tree_int")))
						WAIT(0);
					OBJECT::CREATE_OBJECT(GAMEPLAY::GET_HASH_KEY("prop_xmas_tree_int"), position.x, position.y, position.z, 0, 3000, 1000);
					return;
					break;
				}
				break;
			case 4:
				if (ENTITY::DOES_ENTITY_EXIST(PLAYER::PLAYER_PED_ID()))
				{
					STREAMING::REQUEST_MODEL(GAMEPLAY::GET_HASH_KEY("prop_windmill1"));
					while (!STREAMING::HAS_MODEL_LOADED(GAMEPLAY::GET_HASH_KEY("prop_windmill1")))
						WAIT(0);
					OBJECT::CREATE_OBJECT(GAMEPLAY::GET_HASH_KEY("prop_windmill1"), position.x, position.y, position.z, 0, 3000, 1000);
					return;
					break;
				}
				break;
			case 5:
				if (ENTITY::DOES_ENTITY_EXIST(PLAYER::PLAYER_PED_ID()))
				{
					STREAMING::REQUEST_MODEL(GAMEPLAY::GET_HASH_KEY("prop_alien_egg_01"));
					while (!STREAMING::HAS_MODEL_LOADED(GAMEPLAY::GET_HASH_KEY("prop_alien_egg_01")))
						WAIT(0);
					OBJECT::CREATE_OBJECT(GAMEPLAY::GET_HASH_KEY("prop_alien_egg_01"), position.x, position.y, position.z, 0, 3000, 1000);
					return;
					break;
				}
			case 6:
				if (ENTITY::DOES_ENTITY_EXIST(PLAYER::PLAYER_PED_ID()))
				{
					STREAMING::REQUEST_MODEL(GAMEPLAY::GET_HASH_KEY("prop_cs_dildo_01"));
					while (!STREAMING::HAS_MODEL_LOADED(GAMEPLAY::GET_HASH_KEY("prop_cs_dildo_01")))
						WAIT(0);
					OBJECT::CREATE_OBJECT(GAMEPLAY::GET_HASH_KEY("prop_cs_dildo_01"), position.x, position.y, position.z, 0, 3000, 1000);
					return;
					break;
				}
				break;
			case 7:
				if (ENTITY::DOES_ENTITY_EXIST(PLAYER::PLAYER_PED_ID()))
				{
					STREAMING::REQUEST_MODEL(GAMEPLAY::GET_HASH_KEY("prop_ld_toilet_01"));
					while (!STREAMING::HAS_MODEL_LOADED(GAMEPLAY::GET_HASH_KEY("prop_ld_toilet_01")))
						WAIT(0);
					OBJECT::CREATE_OBJECT(GAMEPLAY::GET_HASH_KEY("prop_ld_toilet_01"), position.x, position.y, position.z, 0, 3000, 1000);
					return;
					break;
				}
				break;
			case 8:
				if (ENTITY::DOES_ENTITY_EXIST(PLAYER::PLAYER_PED_ID()))
				{
					STREAMING::REQUEST_MODEL(GAMEPLAY::GET_HASH_KEY("prop_ld_ferris_wheel"));
					while (!STREAMING::HAS_MODEL_LOADED(GAMEPLAY::GET_HASH_KEY("prop_ld_ferris_wheel")))
						WAIT(0);
					OBJECT::CREATE_OBJECT(GAMEPLAY::GET_HASH_KEY("prop_ld_ferris_wheel"), position.x, position.y, position.z, 0, 3000, 1000);
					return;
					break;
				}
				break;
			case 9:
				if (ENTITY::DOES_ENTITY_EXIST(PLAYER::PLAYER_PED_ID()))
				{
					STREAMING::REQUEST_MODEL(GAMEPLAY::GET_HASH_KEY("p_cablecar_s"));
					while (!STREAMING::HAS_MODEL_LOADED(GAMEPLAY::GET_HASH_KEY("p_cablecar_s")))
						WAIT(0);
					OBJECT::CREATE_OBJECT(GAMEPLAY::GET_HASH_KEY("p_cablecar_s"), position.x, position.y, position.z, 0, 3000, 1000);
					return;
					break;
				}
				break;
			case 10:
				if (ENTITY::DOES_ENTITY_EXIST(PLAYER::PLAYER_PED_ID()))
				{
					STREAMING::REQUEST_MODEL(GAMEPLAY::GET_HASH_KEY("prop_skate_halfpipe_cr"));
					while (!STREAMING::HAS_MODEL_LOADED(GAMEPLAY::GET_HASH_KEY("prop_skate_halfpipe_cr")))
						WAIT(0);
					OBJECT::CREATE_OBJECT(GAMEPLAY::GET_HASH_KEY("prop_skate_halfpipe_cr"), position.x, position.y, position.z, 0, 3000, 1000);
					return;
					break;
				}
				break;
			case 11:
				if (ENTITY::DOES_ENTITY_EXIST(PLAYER::PLAYER_PED_ID()))
				{
					STREAMING::REQUEST_MODEL(GAMEPLAY::GET_HASH_KEY("prop_juicestand"));
					while (!STREAMING::HAS_MODEL_LOADED(GAMEPLAY::GET_HASH_KEY("prop_juicestand")))
						WAIT(0);
					OBJECT::CREATE_OBJECT(GAMEPLAY::GET_HASH_KEY("prop_juicestand"), position.x, position.y, position.z, 0, 3000, 1000);
					return;
					break;
				}
				break;
			case 12:
				if (ENTITY::DOES_ENTITY_EXIST(PLAYER::PLAYER_PED_ID()))
				{
					STREAMING::REQUEST_MODEL(GAMEPLAY::GET_HASH_KEY("prop_weed_pallet"));
					while (!STREAMING::HAS_MODEL_LOADED(GAMEPLAY::GET_HASH_KEY("prop_weed_pallet")))
						WAIT(0);
					OBJECT::CREATE_OBJECT(GAMEPLAY::GET_HASH_KEY("prop_weed_pallet"), position.x, position.y, position.z, 0, 3000, 1000);
					return;
					break;
				}
				break;
			case 13:
				if (ENTITY::DOES_ENTITY_EXIST(PLAYER::PLAYER_PED_ID()))
				{
					STREAMING::REQUEST_MODEL(GAMEPLAY::GET_HASH_KEY("prop_roller_car_01"));
					while (!STREAMING::HAS_MODEL_LOADED(GAMEPLAY::GET_HASH_KEY("prop_roller_car_01")))
						WAIT(0);
					OBJECT::CREATE_OBJECT(GAMEPLAY::GET_HASH_KEY("prop_roller_car_01"), position.x, position.y, position.z, 0, 3000, 1000);
					return;
					break;
				}
				break;
			case 14:
				if (ENTITY::DOES_ENTITY_EXIST(PLAYER::PLAYER_PED_ID()))
				{
					STREAMING::REQUEST_MODEL(GAMEPLAY::GET_HASH_KEY("hei_prop_hst_laptop"));
					while (!STREAMING::HAS_MODEL_LOADED(GAMEPLAY::GET_HASH_KEY("hei_prop_hst_laptop")))
						WAIT(0);
					OBJECT::CREATE_OBJECT(GAMEPLAY::GET_HASH_KEY("hei_prop_hst_laptop"), position.x, position.y, position.z, 0, 3000, 1000);
					return;
					break;
				}
				break;
			case 15:
				if (ENTITY::DOES_ENTITY_EXIST(PLAYER::PLAYER_PED_ID()))
				{
					STREAMING::REQUEST_MODEL(GAMEPLAY::GET_HASH_KEY("prop_large_gold"));
					while (!STREAMING::HAS_MODEL_LOADED(GAMEPLAY::GET_HASH_KEY("prop_large_gold")))
						WAIT(0);
					OBJECT::CREATE_OBJECT(GAMEPLAY::GET_HASH_KEY("prop_large_gold"), position.x, position.y, position.z, 0, 3000, 1000);
					return;
					break;
				}
				break;
			case 16:
				if (ENTITY::DOES_ENTITY_EXIST(PLAYER::PLAYER_PED_ID()))
				{
					STREAMING::REQUEST_MODEL(GAMEPLAY::GET_HASH_KEY("prop_sculpt_fix"));
					while (!STREAMING::HAS_MODEL_LOADED(GAMEPLAY::GET_HASH_KEY("prop_sculpt_fix")))
						WAIT(0);
					OBJECT::CREATE_OBJECT(GAMEPLAY::GET_HASH_KEY("prop_sculpt_fix"), position.x, position.y, position.z, 0, 3000, 1000);
					return;
					break;
				}
				break;
			case 17:
				// create object
				if (ENTITY::DOES_ENTITY_EXIST(PLAYER::PLAYER_PED_ID()))
				{
					GAMEPLAY::DISPLAY_ONSCREEN_KEYBOARD(true, "FMMC_KEY_TIP8", "", "p_spinning_anus_s", "", "", "", 20);
					while (GAMEPLAY::UPDATE_ONSCREEN_KEYBOARD() == 0) WAIT(0);
					if (!STREAMING::IS_MODEL_IN_CDIMAGE(GAMEPLAY::GET_HASH_KEY(GAMEPLAY::GET_ONSCREEN_KEYBOARD_RESULT())))
					{
						return;
					}
					else
					{
						Vector3 get_entity_coords = ENTITY::GET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), 0);
						char* get_onscreen_keyboard_result = GAMEPLAY::GET_ONSCREEN_KEYBOARD_RESULT();
						Hash get_hash_key = GAMEPLAY::GET_HASH_KEY(get_onscreen_keyboard_result);
						STREAMING::REQUEST_MODEL(get_hash_key);
						while (!STREAMING::HAS_MODEL_LOADED(get_hash_key))
							WAIT(0);
						Object create_object = OBJECT::CREATE_OBJECT(get_hash_key, get_entity_coords.x, get_entity_coords.y + 10.0f, get_entity_coords.z + 10.0f, true, true, false);
						STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(get_hash_key);
					}
					std::string playerName = PLAYER::GET_PLAYER_NAME(PLAYER::PLAYER_ID());
					std::string text = " Spawned ~w~A Model ~g~Successfuly";
					set_status_text(playerName + text);
				}
				break;
				// switchable features
			default:
				if (lines[activeLineIndexObj].pState)
					*lines[activeLineIndexObj].pState = !(*lines[activeLineIndexObj].pState);
				if (lines[activeLineIndexObj].pUpdated)
					*lines[activeLineIndexObj].pUpdated = true;
			}
			waitTime = 200;
		}
		else
			if (bBack || trainer_switch_pressed())
			{
				menu_beep();
				break;
			}
			else
				if (bUp)
				{
					menu_beep();
					if (activeLineIndexObj == 0)
						activeLineIndexObj = lineCount;
					activeLineIndexObj--;
					waitTime = 150;
				}
				else
					if (bDown)
					{
						menu_beep();
						activeLineIndexObj++;
						if (activeLineIndexObj == lineCount)
							activeLineIndexObj = 0;
						waitTime = 150;
					}
	}
}

int activeLineIndexMisc = 0;

void process_misc_menu()
{
	const float lineWidth = 250.0;
	const int lineCount = 2;

	std::string caption = "MISC  OPTIONS";

	static struct {
		LPCSTR      text;
		bool        *pState;
		bool        *pUpdated;
	} lines[lineCount] = {
		{ "NEXT RADIO TRACK",    NULL,                   NULL },
		{ "HIDE HUD",            &featureMiscHideHud,    NULL }
	};


	DWORD waitTime = 150;
	while (true)
	{
		// timed menu draw, used for pause after active line switch
		DWORD maxTickCount = GetTickCount() + waitTime;
		do
		{
			// draw menu
			draw_menu_line(caption, lineWidth, 15.0, 18.0, 0.0, 5.0, false, true);
			for (int i = 0; i < lineCount; i++)
				if (i != activeLineIndexMisc)
					draw_menu_line(line_as_str(lines[i].text, lines[i].pState),
						lineWidth, 9.0, 60.0 + i * 36.0, 0.0, 9.0, false, false);
			draw_menu_line(line_as_str(lines[activeLineIndexMisc].text, lines[activeLineIndexMisc].pState),
				lineWidth + 1.0, 11.0, 56.0 + activeLineIndexMisc * 36.0, 0.0, 7.0, true, false);

			update_features();
			WAIT(0);
		} while (GetTickCount() < maxTickCount);
		waitTime = 0;

		// process buttons
		bool bSelect, bBack, bUp, bDown;
		get_button_state(&bSelect, &bBack, &bUp, &bDown, NULL, NULL);
		if (bSelect)
		{
			menu_beep();
			switch (activeLineIndexMisc)
			{
				// next radio track
			case 0:
				if (ENTITY::DOES_ENTITY_EXIST(PLAYER::PLAYER_PED_ID()) &&
					PED::IS_PED_IN_ANY_VEHICLE(PLAYER::PLAYER_PED_ID(), 0))
					AUDIO::SKIP_RADIO_FORWARD();
				break;
				// switchable features
			default:
				if (lines[activeLineIndexMisc].pState)
					*lines[activeLineIndexMisc].pState = !(*lines[activeLineIndexMisc].pState);
				if (lines[activeLineIndexMisc].pUpdated)
					*lines[activeLineIndexMisc].pUpdated = true;
			}
			waitTime = 200;
		}
		else
			if (bBack || trainer_switch_pressed())
			{
				menu_beep();
				break;
			}
			else
				if (bUp)
				{
					menu_beep();
					if (activeLineIndexMisc == 0)
						activeLineIndexMisc = lineCount;
					activeLineIndexMisc--;
					waitTime = 150;
				}
				else
					if (bDown)
					{
						menu_beep();
						activeLineIndexMisc++;
						if (activeLineIndexMisc == lineCount)
							activeLineIndexMisc = 0;
						waitTime = 150;
					}
	}
}

int activeLineIndexWalk = 0;

void process_walk_menu()
{
	const float lineWidth = 260.0;
	const int lineCount = 14;

	std::string caption = "WALK STYLES";

	static struct {
		LPCSTR		text;
		bool		*pState;
		bool		*pUpdated;
	} lines[lineCount] = {
		{ "Tipsy ", NULL, NULL },
		{ "Drunk", NULL, NULL },
		{ "Tazered", NULL, NULL },
		{ "Cane", NULL, NULL },
		{ "Limp", NULL, NULL },
		{ "Crouched", NULL, NULL },
		{ "Heavy", NULL, NULL },
		{ "Female", NULL, NULL },
		{ "Female sexy", NULL, NULL },
		{ "Male casual", NULL, NULL },
		{ "Slouched", NULL, NULL },
		{ "Gangster", NULL, NULL },
		{ "Garbageman", NULL, NULL },
		{ "Very drunk", NULL, NULL },

	};

	DWORD waitTime = 150;
	while (true)
	{
		// timed menu draw, used for pause after active line switch
		DWORD maxTickCount = GetTickCount() + waitTime;
		do
		{
			// draw menu
			draw_menu_line(caption, lineWidth, 7.0, 13.6, 4.0, 4.0, false, true);
			for (int i = 0; i < lineCount; i++)
				if (i != activeLineIndexWalk)
					draw_menu_line(line_as_str(lines[i].text, lines[i].pState), lineWidth, 4.0, 60.0 + i * 24.0, 4.0, 9.0, false, false);
			draw_menu_line(line_as_str(lines[activeLineIndexWalk].text,
				lines[activeLineIndexWalk].pState), lineWidth, 13.6, 38.0 + activeLineIndexWalk * 26.0, 600.0, 4.0, true, false);

			update_features();
			WAIT(0);
		} while (GetTickCount() < maxTickCount);
		waitTime = 0;

		// process buttons
		bool bSelect, bBack, bUp, bDown;
		get_button_state(&bSelect, &bBack, &bUp, &bDown, NULL, NULL);
		if (bSelect)
		{
			menu_beep();

			// common variables
			BOOL bPlayerExists = ENTITY::DOES_ENTITY_EXIST(PLAYER::PLAYER_PED_ID());
			Player player = PLAYER::PLAYER_ID();
			Ped playerPed = PLAYER::PLAYER_PED_ID();

			switch (activeLineIndexWalk)
			{
				// Pole Dance 1
			case 0:
				if (bPlayerExists)
				{
					STREAMING::REQUEST_ANIM_SET("MOVE_M@DRUNK@SLIGHTLYDRUNK");
					while (!STREAMING::HAS_ANIM_SET_LOADED("MOVE_M@DRUNK@SLIGHTLYDRUNK"))
					{
						WAIT(0);
					}
					PED::SET_PED_MOVEMENT_CLIPSET(playerPed, "MOVE_M@DRUNK@SLIGHTLYDRUNK", 1.0f);
				}
				break;
			case 1:
				if (bPlayerExists)
				{
					STREAMING::REQUEST_ANIM_SET("MOVE_M@DRUNK@MODERATEDRUNK");
					while (!STREAMING::HAS_ANIM_SET_LOADED("MOVE_M@DRUNK@MODERATEDRUNK"))
					{
						WAIT(0);
					}
					PED::SET_PED_MOVEMENT_CLIPSET(playerPed, "MOVE_M@DRUNK@MODERATEDRUNK", 1.0f);
				}
				break;
			case 2:
				if (bPlayerExists)
				{
					STREAMING::REQUEST_ANIM_SET("MOVE_M@BAIL_BOND_TAZERED");
					while (!STREAMING::HAS_ANIM_SET_LOADED("MOVE_M@BAIL_BOND_TAZERED"))
					{
						WAIT(0);
					}
					PED::SET_PED_MOVEMENT_CLIPSET(playerPed, "MOVE_M@BAIL_BOND_TAZERED", 1.0f);
				}
				break;

				// switchable features
			default:
				if (lines[activeLineIndexWalk].pState)
					*lines[activeLineIndexWalk].pState = !(*lines[activeLineIndexWalk].pState);
				if (lines[activeLineIndexWalk].pUpdated)
					*lines[activeLineIndexWalk].pUpdated = true;
			}
			waitTime = 200;
		}
		else
			if (bBack || trainer_switch_pressed())
			{
				menu_beep();
				break;
			}
			else
				if (bUp)
				{
					menu_beep();
					if (activeLineIndexWalk == 0)
						activeLineIndexWalk = lineCount;
					activeLineIndexWalk--;
					waitTime = 150;
				}
				else
					if (bDown)
					{
						menu_beep();
						activeLineIndexWalk++;
						if (activeLineIndexWalk == lineCount)
							activeLineIndexWalk = 0;
						waitTime = 150;
					}
	}
}

int activeLineIndexAnimation = 0;

void process_animation_menu()
{
	const float lineWidth = 260.0;
	const int lineCount = 17;

	std::string caption = "Animation Options";

	static struct {
		LPCSTR		text;
		bool		*pState;
		bool		*pUpdated;
	} lines[lineCount] = {
		{ "Pole Dance 1", NULL, NULL },
		{ "Pole Dance 2", NULL, NULL },
		{ "Pole Dance 3", NULL, NULL },
		{ "Stunned", NULL, NULL },
		{ "Situps", NULL, NULL },
		{ "Pushups", NULL, NULL },
		{ "Wave Arms", NULL, NULL },
		{ "Suicide", NULL, NULL },
		{ "On The Can", NULL, NULL },
		{ "On Fire", NULL, NULL },
		{ "Cower", NULL, NULL },
		{ "Private Dance", NULL, NULL },
		{ "BJ", NULL, NULL },
		{ "Stungun", NULL, NULL },
		{ "Air Fuck", NULL, NULL },
		{ "Air Fuck 2", NULL, NULL },
		{ "Stop Animation", NULL, NULL }
	};

	DWORD waitTime = 150;
	while (true)
	{
		// timed menu draw, used for pause after active line switch
		DWORD maxTickCount = GetTickCount() + waitTime;
		do
		{
			// draw menu
			draw_menu_line(caption, lineWidth, 7.0, 13.6, 4.0, 4.0, false, true);
			for (int i = 0; i < lineCount; i++)
				if (i != activeLineIndexAnimation)
					draw_menu_line(line_as_str(lines[i].text, lines[i].pState), lineWidth, 4.0, 60.0 + i * 24.0, 4.0, 9.0, false, false);
			draw_menu_line(line_as_str(lines[activeLineIndexAnimation].text,
				lines[activeLineIndexAnimation].pState), lineWidth + 1.0, 2.0, 60.0 + activeLineIndexAnimation * 24.0, 4.0, 7.0, true, false);


			update_features();
			WAIT(0);
		} while (GetTickCount() < maxTickCount);
		waitTime = 0;

		// process buttons
		bool bSelect, bBack, bUp, bDown;
		get_button_state(&bSelect, &bBack, &bUp, &bDown, NULL, NULL);
		if (bSelect)
		{
			menu_beep();

			// common variables
			BOOL bPlayerExists = ENTITY::DOES_ENTITY_EXIST(PLAYER::PLAYER_PED_ID());
			Player player = PLAYER::PLAYER_ID();
			Ped playerPed = PLAYER::PLAYER_PED_ID();

			switch (activeLineIndexAnimation)
			{
				// Pole Dance 1
			case 0:
				if (bPlayerExists)
				{
					Ped playerPed = PLAYER::PLAYER_PED_ID();
					char *dict0 = "mini@strip_club@pole_dance@pole_dance1";
					char *anim0 = "pd_dance_01";
					STREAMING::REQUEST_ANIM_SET(anim0);
					STREAMING::REQUEST_ANIM_DICT(dict0);
					while (!STREAMING::HAS_ANIM_DICT_LOADED(dict0) && !STREAMING::HAS_ANIM_DICT_LOADED(dict0)) WAIT(0);
					{
						AI::TASK_PLAY_ANIM(playerPed, dict0, anim0, 1, 1, -1, 47, 0, false, 0, false);
					}
				}
				break;
				// Pole Dance 2
			case 1:
				if (bPlayerExists)
				{
					Ped playerPed = PLAYER::PLAYER_PED_ID();
					char *dict1 = "mini@strip_club@pole_dance@pole_dance2";
					char *anim1 = "pd_dance_02";
					STREAMING::REQUEST_ANIM_SET(anim1);
					STREAMING::REQUEST_ANIM_DICT(dict1);
					while (!STREAMING::HAS_ANIM_DICT_LOADED(dict1) && !STREAMING::HAS_ANIM_DICT_LOADED(dict1)) WAIT(0);
					{
						AI::TASK_PLAY_ANIM(playerPed, dict1, anim1, 1, 1, -1, 47, 0, false, 0, false);
					}
				}
				break;
				// Pole Dance 3
			case 2:
				if (bPlayerExists)
				{
					Ped playerPed = PLAYER::PLAYER_PED_ID();
					char *dict2 = "mini@strip_club@pole_dance@pole_dance3";
					char *anim2 = "pd_dance_03";
					STREAMING::REQUEST_ANIM_SET(anim2);
					STREAMING::REQUEST_ANIM_DICT(dict2);
					while (!STREAMING::HAS_ANIM_DICT_LOADED(dict2) && !STREAMING::HAS_ANIM_DICT_LOADED(dict2)) WAIT(0);
					{
						AI::TASK_PLAY_ANIM(playerPed, dict2, anim2, 1, 1, -1, 47, 0, false, 0, false);
					}
				}
				break;
				// Stun
			case 3:
				if (bPlayerExists)
				{
					Ped playerPed = PLAYER::PLAYER_PED_ID();
					char *dict3 = "ragdoll@human";
					char *anim3 = "electrocute";
					STREAMING::REQUEST_ANIM_SET(anim3);
					STREAMING::REQUEST_ANIM_DICT(dict3);
					while (!STREAMING::HAS_ANIM_DICT_LOADED(dict3) && !STREAMING::HAS_ANIM_DICT_LOADED(dict3)) WAIT(0);
					{
						AI::TASK_PLAY_ANIM(playerPed, dict3, anim3, 1, 1, -1, 47, 0, false, 0, false);
					}
				}
				break;


			case 4:
				if (bPlayerExists)
				{
					Ped playerPed = PLAYER::PLAYER_PED_ID();
					char *dict5 = "amb@world_human_sit_ups@male@base";
					char *anim5 = "base";
					STREAMING::REQUEST_ANIM_SET(anim5);
					STREAMING::REQUEST_ANIM_DICT(dict5);
					while (!STREAMING::HAS_ANIM_DICT_LOADED(dict5) && !STREAMING::HAS_ANIM_DICT_LOADED(dict5)) WAIT(0);
					{
						AI::TASK_PLAY_ANIM(playerPed, dict5, anim5, 1, 1, -1, 47, 0, false, 0, false);
					}
				}
				break;

			case 5:
				if (bPlayerExists)
				{
					Ped playerPed = PLAYER::PLAYER_PED_ID();
					char *dict6 = "amb@world_human_push_ups@male@base";
					char *anim6 = "base";
					STREAMING::REQUEST_ANIM_SET(anim6);
					STREAMING::REQUEST_ANIM_DICT(dict6);
					while (!STREAMING::HAS_ANIM_DICT_LOADED(dict6) && !STREAMING::HAS_ANIM_DICT_LOADED(dict6)) WAIT(0);
					{
						AI::TASK_PLAY_ANIM(playerPed, dict6, anim6, 1, 1, -1, 47, 0, false, 0, false);
					}
				}
				break;

			case 6:
				if (bPlayerExists)
				{
					Ped playerPed = PLAYER::PLAYER_PED_ID();
					char *dict7 = "random@car_thief@waving_ig_1";
					char *anim7 = "waving";
					STREAMING::REQUEST_ANIM_SET(anim7);
					STREAMING::REQUEST_ANIM_DICT(dict7);
					while (!STREAMING::HAS_ANIM_DICT_LOADED(dict7) && !STREAMING::HAS_ANIM_DICT_LOADED(dict7)) WAIT(0);
					{
						AI::TASK_PLAY_ANIM(playerPed, dict7, anim7, 1, 1, -1, 47, 0, false, 0, false);
					}
				}
				break;

			case 7:
				if (bPlayerExists)
				{
					Ped playerPed = PLAYER::PLAYER_PED_ID();
					char *dict8 = "mp_suicide";
					char *anim8 = "pistol_fp";
					STREAMING::REQUEST_ANIM_SET(anim8);
					STREAMING::REQUEST_ANIM_DICT(dict8);
					while (!STREAMING::HAS_ANIM_DICT_LOADED(dict8) && !STREAMING::HAS_ANIM_DICT_LOADED(dict8)) WAIT(0);
					{
						AI::TASK_PLAY_ANIM(playerPed, dict8, anim8, 1, 1, -1, 47, 0, false, 0, false);
					}
				}
				break;




			case 8:
				if (bPlayerExists)
				{
					Ped playerPed = PLAYER::PLAYER_PED_ID();
					char *dict9 = "timetable@trevor@on_the_toilet";
					char *anim9 = "trevonlav_struggleloop";
					STREAMING::REQUEST_ANIM_SET(anim9);
					STREAMING::REQUEST_ANIM_DICT(dict9);
					while (!STREAMING::HAS_ANIM_DICT_LOADED(dict9) && !STREAMING::HAS_ANIM_DICT_LOADED(dict9)) WAIT(0);
					{
						AI::TASK_PLAY_ANIM(playerPed, dict9, anim9, 1, 1, -1, 47, 0, false, 0, false);
					}
				}
				break;




			case 9:
				if (bPlayerExists)
				{
					Ped playerPed = PLAYER::PLAYER_PED_ID();
					char *dict10 = "ragdoll@human";
					char *anim10 = "on_fire";
					STREAMING::REQUEST_ANIM_SET(anim10);
					STREAMING::REQUEST_ANIM_DICT(dict10);
					while (!STREAMING::HAS_ANIM_DICT_LOADED(dict10) && !STREAMING::HAS_ANIM_DICT_LOADED(dict10)) WAIT(0);
					{
						AI::TASK_PLAY_ANIM(playerPed, dict10, anim10, 1, 1, -1, 47, 0, false, 0, false);
					}
				}
				break;



			case 10:
				if (bPlayerExists)
				{
					Ped playerPed = PLAYER::PLAYER_PED_ID();
					char *dict11 = "random@arrests";
					char *anim11 = "kneeling_arrest_idle";
					STREAMING::REQUEST_ANIM_SET(anim11);
					STREAMING::REQUEST_ANIM_DICT(dict11);
					while (!STREAMING::HAS_ANIM_DICT_LOADED(dict11) && !STREAMING::HAS_ANIM_DICT_LOADED(dict11)) WAIT(0);
					{
						AI::TASK_PLAY_ANIM(playerPed, dict11, anim11, 1, 1, -1, 47, 0, false, 0, false);
					}
				}
				break;



			case 11:
				if (bPlayerExists)
				{
					Ped playerPed = PLAYER::PLAYER_PED_ID();
					char *dict12 = "mini@strip_club@private_dance@part1";
					char *anim12 = "priv_dance_p1";
					STREAMING::REQUEST_ANIM_SET(anim12);
					STREAMING::REQUEST_ANIM_DICT(dict12);
					while (!STREAMING::HAS_ANIM_DICT_LOADED(dict12) && !STREAMING::HAS_ANIM_DICT_LOADED(dict12)) WAIT(0);
					{
						AI::TASK_PLAY_ANIM(playerPed, dict12, anim12, 1, 1, -1, 47, 0, false, 0, false);
					}
				}
				break;





			case 12:
				if (bPlayerExists)
				{
					Ped playerPed = PLAYER::PLAYER_PED_ID();
					char *dict13 = "mini@prostitutes@sexnorm_veh";
					char *anim13 = "bj_loop_prostitute";
					STREAMING::REQUEST_ANIM_SET(anim13);
					STREAMING::REQUEST_ANIM_DICT(dict13);
					while (!STREAMING::HAS_ANIM_DICT_LOADED(dict13) && !STREAMING::HAS_ANIM_DICT_LOADED(dict13)) WAIT(0);
					{
						AI::TASK_PLAY_ANIM(playerPed, dict13, anim13, 1, 1, -1, 47, 0, false, 0, false);
					}
				}
				break;



			case 13:
				if (bPlayerExists)
				{
					Ped playerPed = PLAYER::PLAYER_PED_ID();
					char *dict14 = "stungun@standing";
					char *anim14 = "Damage";
					STREAMING::REQUEST_ANIM_SET(anim14);
					STREAMING::REQUEST_ANIM_DICT(dict14);
					while (!STREAMING::HAS_ANIM_DICT_LOADED(dict14) && !STREAMING::HAS_ANIM_DICT_LOADED(dict14)) WAIT(0);
					{
						AI::TASK_PLAY_ANIM(playerPed, dict14, anim14, 1, 1, -1, 47, 0, false, 0, false);
					}
				}
				break;

			case 14:
				if (bPlayerExists)
				{
					Ped playerPed = PLAYER::PLAYER_PED_ID();
					char *dict15 = "rcmpaparazzo_2";
					char *anim15 = "shag_loop_a";
					STREAMING::REQUEST_ANIM_SET(anim15);
					STREAMING::REQUEST_ANIM_DICT(dict15);
					while (!STREAMING::HAS_ANIM_DICT_LOADED(dict15) && !STREAMING::HAS_ANIM_DICT_LOADED(dict15)) WAIT(0);
					{
						AI::TASK_PLAY_ANIM(playerPed, dict15, anim15, 1, 1, -1, 47, 0, false, 0, false);
					}
				}
				break;

			case 15:
				if (bPlayerExists)
				{
					Ped playerPed = PLAYER::PLAYER_PED_ID();
					char *dict16 = "rcmpaparazzo_2";
					char *anim16 = "shag_loop_poppy";
					STREAMING::REQUEST_ANIM_SET(anim16);
					STREAMING::REQUEST_ANIM_DICT(dict16);
					while (!STREAMING::HAS_ANIM_DICT_LOADED(dict16) && !STREAMING::HAS_ANIM_DICT_LOADED(dict16)) WAIT(0);
					{
						AI::TASK_PLAY_ANIM(playerPed, dict16, anim16, 1, 1, -1, 47, 0, false, 0, false);
					}
				}
				break;
			case 16:
				if (bPlayerExists)
				{
					Freeze(playerPed);
				}
				break;
				// switchable features
			default:
				if (lines[activeLineIndexAnimation].pState)
					*lines[activeLineIndexAnimation].pState = !(*lines[activeLineIndexAnimation].pState);
				if (lines[activeLineIndexAnimation].pUpdated)
					*lines[activeLineIndexAnimation].pUpdated = true;
			}
			waitTime = 200;
		}
		else
			if (bBack || trainer_switch_pressed())
			{
				menu_beep();
				break;
			}
			else
				if (bUp)
				{
					menu_beep();
					if (activeLineIndexAnimation == 0)
						activeLineIndexAnimation = lineCount;
					activeLineIndexAnimation--;
					waitTime = 150;
				}
				else
					if (bDown)
					{
						menu_beep();
						activeLineIndexAnimation++;
						if (activeLineIndexAnimation == lineCount)
							activeLineIndexAnimation = 0;
						waitTime = 150;
					}
	}
}

int activeLineIndexVis = 0;

void process_vision_menu()
{
	const float lineWidth = 260.0;
	const int lineCount = 6;

	std::string caption = "Vision Options";

	static struct {
		LPCSTR		text;
		bool		*pState;
		bool		*pUpdated;
	} lines[lineCount] = {
		{ "Thermal Vision", &featureVisionThermal, NULL },
		{ "Night Vision", &featureVisionNight, NULL },
		{ "ESP", &featureESP, NULL },
		{ "Name ESP", &featureNameESP, NULL },
		{ "ESP Lines", &featurePlayerESPTrace, NULL },
		{ "Drunk Vision", &featureVisionDrunk, &featureVisionDrunkUpdated }
	};


	DWORD waitTime = 150;
	while (true)
	{
		// timed menu draw, used for pause after active line switch
		DWORD maxTickCount = GetTickCount() + waitTime;
		do
		{
			// draw menu
			draw_menu_line(caption, lineWidth, 7.0, 13.6, 4.0, 4.0, false, true);
			for (int i = 0; i < lineCount; i++)
				if (i != activeLineIndexVis)
					draw_menu_line(line_as_str(lines[i].text, lines[i].pState), lineWidth, 4.0, 60.0 + i * 24.0, 4.0, 9.0, false, false);
			draw_menu_line(line_as_str(lines[activeLineIndexVis].text,
				lines[activeLineIndexVis].pState), lineWidth + 1.0, 2.0, 60.0 + activeLineIndexVis * 24.0, 4.0, 7.0, true, false);

			update_features();
			WAIT(0);
		} while (GetTickCount() < maxTickCount);
		waitTime = 0;

		// process buttons
		bool bSelect, bBack, bUp, bDown;
		get_button_state(&bSelect, &bBack, &bUp, &bDown, NULL, NULL);
		if (bSelect)
		{
			menu_beep();
			switch (activeLineIndexVis)
			{
				// next radio track
			case 0:
				featureVisionThermal = !featureVisionThermal;
				GRAPHICS::SET_SEETHROUGH(featureVisionThermal);
				break;

				// Night Vision 
			case 1:
				featureVisionNight = !featureVisionNight;
				GRAPHICS::SET_NIGHTVISION(featureVisionNight);
				break;
				// switchable features
			default:
				if (lines[activeLineIndexVis].pState)
					*lines[activeLineIndexVis].pState = !(*lines[activeLineIndexVis].pState);
				if (lines[activeLineIndexVis].pUpdated)
					*lines[activeLineIndexVis].pUpdated = true;
			}
			waitTime = 200;
		}
		else
			if (bBack || trainer_switch_pressed())
			{
				menu_beep();
				break;
			}
			else
				if (bUp)
				{
					menu_beep();
					if (activeLineIndexVis == 0)
						activeLineIndexVis = lineCount;
					activeLineIndexVis--;
					waitTime = 150;
				}
				else
					if (bDown)
					{
						menu_beep();
						activeLineIndexVis++;
						if (activeLineIndexVis == lineCount)
							activeLineIndexVis = 0;
						waitTime = 150;
					}
	}
}


int activeLineIndexNorth = 0;


void process_ny_menu()
{
	const float lineWidth = 260.0;
	const int lineCount = 1;

	std::string caption = "North Yankton";

	static struct {
		LPCSTR		text;
		bool		*pState;
		bool		*pUpdated;
	} lines[lineCount] = {
		{ "Load", NULL, NULL },

	};


	DWORD waitTime = 150;
	while (true)
	{
		// timed menu draw, used for pause after active line switch
		DWORD maxTickCount = GetTickCount() + waitTime;
		do
		{
			// draw menu
			draw_menu_line(caption, lineWidth, 7.0, 13.6, 4.0, 4.0, false, true);
			for (int i = 0; i < lineCount; i++)
				if (i != activeLineIndexNorth)
					draw_menu_line(line_as_str(lines[i].text, lines[i].pState), lineWidth, 4.0, 60.0 + i * 24.0, 4.0, 9.0, false, false);
			draw_menu_line(line_as_str(lines[activeLineIndexNorth].text,
				lines[activeLineIndexNorth].pState), lineWidth + 1.0, 2.0, 60.0 + activeLineIndexNorth * 24.0, 4.0, 7.0, true, false);


			update_features();
			WAIT(0);
		} while (GetTickCount() < maxTickCount);
		waitTime = 0;

		// process buttons
		bool bSelect, bBack, bUp, bDown;
		get_button_state(&bSelect, &bBack, &bUp, &bDown, NULL, NULL);
		if (bSelect)
		{
			menu_beep();
			switch (activeLineIndexNorth)
			{
				// Load North Yankton
			case 0:
				if (ENTITY::DOES_ENTITY_EXIST(PLAYER::PLAYER_PED_ID())
					)
					STREAMING::REQUEST_IPL("plg_01");
				STREAMING::REQUEST_IPL("prologue01");
				STREAMING::REQUEST_IPL("prologue01_lod");
				STREAMING::REQUEST_IPL("prologue01c");
				STREAMING::REQUEST_IPL("prologue01c_lod");
				STREAMING::REQUEST_IPL("prologue01d");
				STREAMING::REQUEST_IPL("prologue01d_lod");
				STREAMING::REQUEST_IPL("prologue01e");
				STREAMING::REQUEST_IPL("prologue01e_lod");
				STREAMING::REQUEST_IPL("prologue01f");
				STREAMING::REQUEST_IPL("prologue01f_lod");
				STREAMING::REQUEST_IPL("prologue01g");
				STREAMING::REQUEST_IPL("prologue01h");
				STREAMING::REQUEST_IPL("prologue01h_lod");
				STREAMING::REQUEST_IPL("prologue01i");
				STREAMING::REQUEST_IPL("prologue01i_lod");
				STREAMING::REQUEST_IPL("prologue01j");
				STREAMING::REQUEST_IPL("prologue01j_lod");
				STREAMING::REQUEST_IPL("prologue01k");
				STREAMING::REQUEST_IPL("prologue01k_lod");
				STREAMING::REQUEST_IPL("prologue01z");
				STREAMING::REQUEST_IPL("prologue01z_lod");
				STREAMING::REQUEST_IPL("plg_02");
				STREAMING::REQUEST_IPL("prologue02");
				STREAMING::REQUEST_IPL("prologue02_lod");
				STREAMING::REQUEST_IPL("plg_03");
				STREAMING::REQUEST_IPL("prologue03");
				STREAMING::REQUEST_IPL("prologue03_lod");
				STREAMING::REQUEST_IPL("prologue03b");
				STREAMING::REQUEST_IPL("prologue03b_lod");
				//the commented code disables the 'Prologue' grave and
				//enables the 'Bury the Hatchet' grave
				//STREAMING::REQUEST_IPL("prologue03_grv_cov");
				//STREAMING::REQUEST_IPL("prologue03_grv_cov_lod");
				STREAMING::REQUEST_IPL("prologue03_grv_dug");
				STREAMING::REQUEST_IPL("prologue03_grv_dug_lod");
				//STREAMING::REQUEST_IPL("prologue03_grv_fun");
				STREAMING::REQUEST_IPL("prologue_grv_torch");
				STREAMING::REQUEST_IPL("plg_04");
				STREAMING::REQUEST_IPL("prologue04");
				STREAMING::REQUEST_IPL("prologue04_lod");
				STREAMING::REQUEST_IPL("prologue04b");
				STREAMING::REQUEST_IPL("prologue04b_lod");
				STREAMING::REQUEST_IPL("prologue04_cover");
				STREAMING::REQUEST_IPL("des_protree_end");
				STREAMING::REQUEST_IPL("des_protree_start");
				STREAMING::REQUEST_IPL("des_protree_start_lod");
				STREAMING::REQUEST_IPL("plg_05");
				STREAMING::REQUEST_IPL("prologue05");
				STREAMING::REQUEST_IPL("prologue05_lod");
				STREAMING::REQUEST_IPL("prologue05b");
				STREAMING::REQUEST_IPL("prologue05b_lod");
				STREAMING::REQUEST_IPL("plg_06");
				STREAMING::REQUEST_IPL("prologue06");
				STREAMING::REQUEST_IPL("prologue06_lod");
				STREAMING::REQUEST_IPL("prologue06b");
				STREAMING::REQUEST_IPL("prologue06b_lod");
				STREAMING::REQUEST_IPL("prologue06_int");
				STREAMING::REQUEST_IPL("prologue06_int_lod");
				STREAMING::REQUEST_IPL("prologue06_pannel");
				STREAMING::REQUEST_IPL("prologue06_pannel_lod");
				STREAMING::REQUEST_IPL("prologue_m2_door");
				STREAMING::REQUEST_IPL("prologue_m2_door_lod");
				STREAMING::REQUEST_IPL("plg_occl_00");
				STREAMING::REQUEST_IPL("prologue_occl");
				STREAMING::REQUEST_IPL("plg_rd");
				STREAMING::REQUEST_IPL("prologuerd");
				STREAMING::REQUEST_IPL("prologuerdb");
				STREAMING::REQUEST_IPL("prologuerd_lod");
				set_status_text("North Yankton ~o~Loaded ~w~Teleport using Teleport menu!");
				break;;


				// switchable features
			default:
				if (lines[activeLineIndexNorth].pState)
					*lines[activeLineIndexNorth].pState = !(*lines[activeLineIndexNorth].pState);
				if (lines[activeLineIndexNorth].pUpdated)
					*lines[activeLineIndexNorth].pUpdated = true;
			}
			waitTime = 200;
		}
		else
			if (bBack || trainer_switch_pressed())
			{
				menu_beep();
				break;
			}
			else
				if (bUp)
				{
					menu_beep();
					if (activeLineIndexNorth == 0)
						activeLineIndexNorth = lineCount;
					activeLineIndexNorth--;
					waitTime = 150;
				}
				else
					if (bDown)
					{
						menu_beep();
						activeLineIndexNorth++;
						if (activeLineIndexNorth == lineCount)
							activeLineIndexNorth = 0;
						waitTime = 150;
					}
	}
}

void update_vehicle_Snow()
{
	Player player = PLAYER::PLAYER_ID();
	Ped playerPed = PLAYER::PLAYER_PED_ID();

	if (!ENTITY::DOES_ENTITY_EXIST(playerPed) || !featureWeaponVehSnow) return;

	bool bSelect = IsKeyDown(0x6B); // num plus
	if (bSelect && featureWeaponVehShootLastTime + 150 < GetTickCount() &&
		PLAYER::IS_PLAYER_CONTROL_ON(player) && PED::IS_PED_IN_ANY_VEHICLE(playerPed, 0))
	{
		Vehicle veh = PED::GET_VEHICLE_PED_IS_USING(playerPed);

		Vector3 v0, v1;
		GAMEPLAY::GET_MODEL_DIMENSIONS(ENTITY::GET_ENTITY_MODEL(veh), &v0, &v1);

		Hash weaponAssetRocket = GAMEPLAY::GET_HASH_KEY("WEAPON_SNOWBALL");
		if (!WEAPON::HAS_WEAPON_ASSET_LOADED(weaponAssetRocket))
		{
			WEAPON::REQUEST_WEAPON_ASSET(weaponAssetRocket, 31, 0);
			while (!WEAPON::HAS_WEAPON_ASSET_LOADED(weaponAssetRocket))
				WAIT(0);
		}

		Vector3 coords0from = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(veh, -(v1.x + 0.25f), v1.y + 1.25f, 0.1);
		Vector3 coords1from = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(veh, (v1.x + 0.25f), v1.y + 1.25f, 0.1);
		Vector3 coords0to = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(veh, -v1.x, v1.y + 100.0f, 0.1f);
		Vector3 coords1to = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(veh, v1.x, v1.y + 100.0f, 0.1f);

		GAMEPLAY::SHOOT_SINGLE_BULLET_BETWEEN_COORDS(coords0from.x, coords0from.y, coords0from.z,
			coords0to.x, coords0to.y, coords0to.z,
			250, 1, weaponAssetRocket, playerPed, 1, 0, -1.0);
		GAMEPLAY::SHOOT_SINGLE_BULLET_BETWEEN_COORDS(coords1from.x, coords1from.y, coords1from.z,
			coords1to.x, coords1to.y, coords1to.z,
			250, 1, weaponAssetRocket, playerPed, 1, 0, -1.0);

		featureWeaponVehShootLastTime = GetTickCount();


	}
}


void update_vehicle_Green()
{
	Player player = PLAYER::PLAYER_ID();
	Ped playerPed = PLAYER::PLAYER_PED_ID();

	if (!ENTITY::DOES_ENTITY_EXIST(playerPed) || !featureWeaponVehGreen) return;

	bool bSelect = IsKeyDown(0x6B); // num plus
	if (bSelect && featureWeaponVehShootLastTime + 150 < GetTickCount() &&
		PLAYER::IS_PLAYER_CONTROL_ON(player) && PED::IS_PED_IN_ANY_VEHICLE(playerPed, 0))
	{
		Vehicle veh = PED::GET_VEHICLE_PED_IS_USING(playerPed);

		Vector3 v0, v1;
		GAMEPLAY::GET_MODEL_DIMENSIONS(ENTITY::GET_ENTITY_MODEL(veh), &v0, &v1);

		Hash weaponAssetRocket = GAMEPLAY::GET_HASH_KEY("VEHICLE_WEAPON_PLAYER_LASER");
		if (!WEAPON::HAS_WEAPON_ASSET_LOADED(weaponAssetRocket))
		{
			WEAPON::REQUEST_WEAPON_ASSET(weaponAssetRocket, 31, 0);
			while (!WEAPON::HAS_WEAPON_ASSET_LOADED(weaponAssetRocket))
				WAIT(0);
		}

		Vector3 coords0from = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(veh, -(v1.x + 0.25f), v1.y + 1.25f, 0.1);
		Vector3 coords1from = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(veh, (v1.x + 0.25f), v1.y + 1.25f, 0.1);
		Vector3 coords0to = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(veh, -v1.x, v1.y + 100.0f, 0.1f);
		Vector3 coords1to = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(veh, v1.x, v1.y + 100.0f, 0.1f);

		GAMEPLAY::SHOOT_SINGLE_BULLET_BETWEEN_COORDS(coords0from.x, coords0from.y, coords0from.z,
			coords0to.x, coords0to.y, coords0to.z,
			250, 1, weaponAssetRocket, playerPed, 1, 0, -1.0);
		GAMEPLAY::SHOOT_SINGLE_BULLET_BETWEEN_COORDS(coords1from.x, coords1from.y, coords1from.z,
			coords1to.x, coords1to.y, coords1to.z,
			250, 1, weaponAssetRocket, playerPed, 1, 0, -1.0);

		featureWeaponVehShootLastTime = GetTickCount();


	}
}



void update_vehicle_Flare()
{
	Player player = PLAYER::PLAYER_ID();
	Ped playerPed = PLAYER::PLAYER_PED_ID();

	if (!ENTITY::DOES_ENTITY_EXIST(playerPed) || !featureWeaponVehFlare) return;

	bool bSelect = IsKeyDown(0x6B); // num plus
	if (bSelect && featureWeaponVehShootLastTime + 150 < GetTickCount() &&
		PLAYER::IS_PLAYER_CONTROL_ON(player) && PED::IS_PED_IN_ANY_VEHICLE(playerPed, 0))
	{
		Vehicle veh = PED::GET_VEHICLE_PED_IS_USING(playerPed);

		Vector3 v0, v1;
		GAMEPLAY::GET_MODEL_DIMENSIONS(ENTITY::GET_ENTITY_MODEL(veh), &v0, &v1);

		Hash weaponAssetRocket = GAMEPLAY::GET_HASH_KEY("WEAPON_FLAREGUN");
		if (!WEAPON::HAS_WEAPON_ASSET_LOADED(weaponAssetRocket))
		{
			WEAPON::REQUEST_WEAPON_ASSET(weaponAssetRocket, 31, 0);
			while (!WEAPON::HAS_WEAPON_ASSET_LOADED(weaponAssetRocket))
				WAIT(0);
		}

		Vector3 coords0from = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(veh, -(v1.x + 0.25f), v1.y + 1.25f, 0.1);
		Vector3 coords1from = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(veh, (v1.x + 0.25f), v1.y + 1.25f, 0.1);
		Vector3 coords0to = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(veh, -v1.x, v1.y + 100.0f, 0.1f);
		Vector3 coords1to = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(veh, v1.x, v1.y + 100.0f, 0.1f);

		GAMEPLAY::SHOOT_SINGLE_BULLET_BETWEEN_COORDS(coords0from.x, coords0from.y, coords0from.z,
			coords0to.x, coords0to.y, coords0to.z,
			250, 1, weaponAssetRocket, playerPed, 1, 0, -1.0);
		GAMEPLAY::SHOOT_SINGLE_BULLET_BETWEEN_COORDS(coords1from.x, coords1from.y, coords1from.z,
			coords1to.x, coords1to.y, coords1to.z,
			250, 1, weaponAssetRocket, playerPed, 1, 0, -1.0);

		featureWeaponVehShootLastTime = GetTickCount();


	}
}

bool RequestNetworkControl(uint vehID)
{
	int Tries = 0;
	bool
		hasControl = false,
		giveUp = false;
	do
	{
		hasControl = NETWORK::NETWORK_REQUEST_CONTROL_OF_ENTITY(vehID);
		Tries++;
		if (Tries > 300)
			giveUp = true;
	} while (!hasControl && !giveUp);

	if (giveUp)
		return false;
	else return true;
}







void update_vehicle_Tank()
{
	Player player = PLAYER::PLAYER_ID();
	Ped playerPed = PLAYER::PLAYER_PED_ID();

	if (!ENTITY::DOES_ENTITY_EXIST(playerPed) || !featureWeaponVehTank) return;

	bool bSelect = IsKeyDown(0x6B); // num plus
	if (bSelect && featureWeaponVehShootLastTime + 150 < GetTickCount() &&
		PLAYER::IS_PLAYER_CONTROL_ON(player) && PED::IS_PED_IN_ANY_VEHICLE(playerPed, 0))
	{
		Vehicle veh = PED::GET_VEHICLE_PED_IS_USING(playerPed);

		Vector3 v0, v1;
		GAMEPLAY::GET_MODEL_DIMENSIONS(ENTITY::GET_ENTITY_MODEL(veh), &v0, &v1);

		Hash weaponAssetRocket = GAMEPLAY::GET_HASH_KEY("VEHICLE_WEAPON_TANK");
		if (!WEAPON::HAS_WEAPON_ASSET_LOADED(weaponAssetRocket))
		{
			WEAPON::REQUEST_WEAPON_ASSET(weaponAssetRocket, 31, 0);
			while (!WEAPON::HAS_WEAPON_ASSET_LOADED(weaponAssetRocket))
				WAIT(0);
		}

		Vector3 coords0from = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(veh, -(v1.x + 0.25f), v1.y + 1.25f, 0.1);
		Vector3 coords1from = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(veh, (v1.x + 0.25f), v1.y + 1.25f, 0.1);
		Vector3 coords0to = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(veh, -v1.x, v1.y + 100.0f, 0.1f);
		Vector3 coords1to = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(veh, v1.x, v1.y + 100.0f, 0.1f);

		GAMEPLAY::SHOOT_SINGLE_BULLET_BETWEEN_COORDS(coords0from.x, coords0from.y, coords0from.z,
			coords0to.x, coords0to.y, coords0to.z,
			250, 1, weaponAssetRocket, playerPed, 1, 0, -1.0);
		GAMEPLAY::SHOOT_SINGLE_BULLET_BETWEEN_COORDS(coords1from.x, coords1from.y, coords1from.z,
			coords1to.x, coords1to.y, coords1to.z,
			250, 1, weaponAssetRocket, playerPed, 1, 0, -1.0);

		featureWeaponVehShootLastTime = GetTickCount();


	}
}


void update_vehicle_Red()
{
	Player player = PLAYER::PLAYER_ID();
	Ped playerPed = PLAYER::PLAYER_PED_ID();

	if (!ENTITY::DOES_ENTITY_EXIST(playerPed) || !featureWeaponVehRed) return;

	bool bSelect = IsKeyDown(0x6B); // num plus
	if (bSelect && featureWeaponVehShootLastTime + 150 < GetTickCount() &&
		PLAYER::IS_PLAYER_CONTROL_ON(player) && PED::IS_PED_IN_ANY_VEHICLE(playerPed, 0))
	{
		Vehicle veh = PED::GET_VEHICLE_PED_IS_USING(playerPed);

		Vector3 v0, v1;
		GAMEPLAY::GET_MODEL_DIMENSIONS(ENTITY::GET_ENTITY_MODEL(veh), &v0, &v1);

		Hash weaponAssetRocket = GAMEPLAY::GET_HASH_KEY("VEHICLE_WEAPON_ENEMY_LASER");
		if (!WEAPON::HAS_WEAPON_ASSET_LOADED(weaponAssetRocket))
		{
			WEAPON::REQUEST_WEAPON_ASSET(weaponAssetRocket, 31, 0);
			while (!WEAPON::HAS_WEAPON_ASSET_LOADED(weaponAssetRocket))
				WAIT(0);
		}

		Vector3 coords0from = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(veh, -(v1.x + 0.25f), v1.y + 1.25f, 0.1);
		Vector3 coords1from = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(veh, (v1.x + 0.25f), v1.y + 1.25f, 0.1);
		Vector3 coords0to = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(veh, -v1.x, v1.y + 100.0f, 0.1f);
		Vector3 coords1to = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(veh, v1.x, v1.y + 100.0f, 0.1f);

		GAMEPLAY::SHOOT_SINGLE_BULLET_BETWEEN_COORDS(coords0from.x, coords0from.y, coords0from.z,
			coords0to.x, coords0to.y, coords0to.z,
			250, 1, weaponAssetRocket, playerPed, 1, 0, -1.0);
		GAMEPLAY::SHOOT_SINGLE_BULLET_BETWEEN_COORDS(coords1from.x, coords1from.y, coords1from.z,
			coords1to.x, coords1to.y, coords1to.z,
			250, 1, weaponAssetRocket, playerPed, 1, 0, -1.0);

		featureWeaponVehShootLastTime = GetTickCount();


	}
}


void update_vehicle_guns_fire()
{
	Player player = PLAYER::PLAYER_ID();
	Ped playerPed = PLAYER::PLAYER_PED_ID();

	if (!ENTITY::DOES_ENTITY_EXIST(playerPed) || !featureWeaponVehFire) return;

	bool bSelect = IsKeyDown(0x6B); // num plus
	if (bSelect && featureWeaponVehShootLastTimeFire + 150 < GetTickCount() &&
		PLAYER::IS_PLAYER_CONTROL_ON(player) && PED::IS_PED_IN_ANY_VEHICLE(playerPed, 0))
	{
		Vehicle veh = PED::GET_VEHICLE_PED_IS_USING(playerPed);

		Vector3 v0, v1;
		GAMEPLAY::GET_MODEL_DIMENSIONS(ENTITY::GET_ENTITY_MODEL(veh), &v0, &v1);

		Hash weaponAssetFire = GAMEPLAY::GET_HASH_KEY("WEAPON_FIREWORK");
		if (!WEAPON::HAS_WEAPON_ASSET_LOADED(weaponAssetFire))
		{
			WEAPON::REQUEST_WEAPON_ASSET(weaponAssetFire, 31, 0);
			while (!WEAPON::HAS_WEAPON_ASSET_LOADED(weaponAssetFire))
				WAIT(0);
		}

		Vector3 coords0from = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(veh, -(v1.x + 0.25f), v1.y + 1.25f, 0.1);
		Vector3 coords1from = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(veh, (v1.x + 0.25f), v1.y + 1.25f, 0.1);
		Vector3 coords0to = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(veh, -v1.x, v1.y + 100.0f, 0.1f);
		Vector3 coords1to = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(veh, v1.x, v1.y + 100.0f, 0.1f);

		GAMEPLAY::SHOOT_SINGLE_BULLET_BETWEEN_COORDS(coords0from.x, coords0from.y, coords0from.z,
			coords0to.x, coords0to.y, coords0to.z,
			250, 1, weaponAssetFire, playerPed, 1, 0, -1.0);
		GAMEPLAY::SHOOT_SINGLE_BULLET_BETWEEN_COORDS(coords1from.x, coords1from.y, coords1from.z,
			coords1to.x, coords1to.y, coords1to.z,
			250, 1, weaponAssetFire, playerPed, 1, 0, -1.0);

		featureWeaponVehShootLastTimeFire = GetTickCount();
	}
}


int activeLineIndexwep = 0;


void process_wep_menu()
{
	const float lineWidth = 260.0;
	const int lineCount = 6;

	std::string caption = "Vehicle Weapons";

	static struct {
		LPCSTR		text;
		bool		*pState;
		bool		*pUpdated;
	} lines[lineCount] = {
		{ "Fireworks", &featureWeaponVehFire, NULL },
		{ "Rockets", &featureWeaponVehRockets, NULL },
		{ "Snow Balls", &featureWeaponVehSnow, NULL },
		{ "Green Laser", &featureWeaponVehGreen, NULL },
		{ "Red Laser", &featureWeaponVehRed, NULL },
		{ "Tank", &featureWeaponVehTank, NULL },



	};


	DWORD waitTime = 150;
	while (true)
	{
		// timed menu draw, used for pause after active line switch
		DWORD maxTickCount = GetTickCount() + waitTime;
		do
		{
			// draw menu
			draw_menu_line(caption, lineWidth, 7.0, 13.6, 4.0, 4.0, false, true);
			for (int i = 0; i < lineCount; i++)
				if (i != activeLineIndexwep)
					draw_menu_line(line_as_str(lines[i].text, lines[i].pState), lineWidth, 4.0, 60.0 + i * 24.0, 4.0, 9.0, false, false);
			draw_menu_line(line_as_str(lines[activeLineIndexwep].text,
				lines[activeLineIndexwep].pState), lineWidth + 1.0, 2.0, 60.0 + activeLineIndexwep * 24.0, 4.0, 7.0, true, false);


			update_features();
			WAIT(0);
		} while (GetTickCount() < maxTickCount);
		waitTime = 0;


		// process buttons
		bool bSelect, bBack, bUp, bDown;
		get_button_state(&bSelect, &bBack, &bUp, &bDown, NULL, NULL);
		if (bSelect)
		{
			menu_beep();
			switch (activeLineIndexwep)
			{



				// switchable features
			default:
				if (lines[activeLineIndexwep].pState)
					*lines[activeLineIndexwep].pState = !(*lines[activeLineIndexwep].pState);
				if (lines[activeLineIndexwep].pUpdated)
					*lines[activeLineIndexwep].pUpdated = true;
			}
			waitTime = 200;
		}
		else
			if (bBack || trainer_switch_pressed())
			{
				menu_beep();
				break;
			}
			else
				if (bUp)
				{
					menu_beep();
					if (activeLineIndexwep == 0)
						activeLineIndexwep = lineCount;
					activeLineIndexwep--;
					waitTime = 150;
				}
				else
					if (bDown)
					{
						menu_beep();
						activeLineIndexwep++;
						if (activeLineIndexwep == lineCount)
							activeLineIndexwep = 0;
						waitTime = 150;
					}
	}
}


void bodyguard(Ped ped, Player player)
{
	if (ENTITY::DOES_ENTITY_EXIST(ped))
	{
		GAMEPLAY::DISPLAY_ONSCREEN_KEYBOARD(true, "FMMC_KEY_TIP8", "", "a_f_y_topless_01", "", "", "", 20);
		while (GAMEPLAY::UPDATE_ONSCREEN_KEYBOARD() == 0) WAIT(0);
		if (!STREAMING::IS_MODEL_IN_CDIMAGE(GAMEPLAY::GET_HASH_KEY(GAMEPLAY::GET_ONSCREEN_KEYBOARD_RESULT())))
		{
			return;
		}
		else
		{
			Vector3 get_entity_coords = ENTITY::GET_ENTITY_COORDS(ped, 0);
			Ped get_ped_group_index = PED::GET_PED_GROUP_INDEX(ped);
			char* get_onscreen_keyboard_result = GAMEPLAY::GET_ONSCREEN_KEYBOARD_RESULT();
			Hash get_hash_key = GAMEPLAY::GET_HASH_KEY(get_onscreen_keyboard_result);
			STREAMING::REQUEST_MODEL(get_hash_key);
			while (!STREAMING::HAS_MODEL_LOADED(get_hash_key))
				WAIT(0);
			Ped create_ped = PED::CREATE_PED(1, get_hash_key, get_entity_coords.x, get_entity_coords.y + 5.0f, get_entity_coords.z, ENTITY::GET_ENTITY_HEADING(ped), true, true);
			WAIT(0);
			PED::SET_PED_CAN_RAGDOLL(create_ped, false);
			PED::SET_PED_CAN_RAGDOLL_FROM_PLAYER_IMPACT(create_ped, false);
			PED::SET_PED_AS_GROUP_MEMBER(create_ped, get_ped_group_index);
			PED::SET_PED_COMBAT_RANGE(create_ped, 500);
			PED::SET_PED_ALERTNESS(create_ped, 100);
			PED::SET_PED_ACCURACY(create_ped, 100);
			PED::SET_PED_CAN_SWITCH_WEAPON(create_ped, 1);
			PED::SET_PED_SHOOT_RATE(create_ped, 200);
			PED::SET_PED_KEEP_TASK(create_ped, true);
			AI::TASK_COMBAT_HATED_TARGETS_AROUND_PED(create_ped, 5000.f, 0);
			WAIT(0);
			//for (int a = 0; a < sizeof(weaponNames) / sizeof(weaponNames[0]); a++)
			//{
			////	/WEAPON::GIVE_DELAYED_WEAPON_TO_PED(create_ped, GAMEPLAY::GET_HASH_KEY((char *)weaponNames[a]), 1000, 0);
			//}
			//	WAIT(0);
			PED::SET_PED_CAN_SWITCH_WEAPON(create_ped, true);
			PED::SET_PED_KEEP_TASK(create_ped, true);
			PED::SET_PED_GENERATES_DEAD_BODY_EVENTS(create_ped, true);
			// misc invincible bodyguard
			if (featurePedestrianInvincibleBodyguardUpdate)
			{
				if (!featurePedestrianInvincibleBodyguard)
					ENTITY::SET_ENTITY_INVINCIBLE(create_ped, false);
				featurePedestrianInvincibleBodyguardUpdate = false;
			}
			if (featurePedestrianInvincibleBodyguard)
			{
				ENTITY::SET_ENTITY_INVINCIBLE(create_ped, true);
			}
			WAIT(0);
			STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(get_hash_key);
		}

		std::string playerName = PLAYER::GET_PLAYER_NAME(player);
		std::string text = " A bodyguard was spawned for ~o~ ";
		set_status_text(text + playerName);
	}
}


void spawnChauffer()
{
	int WaypointHandle = UI::GET_FIRST_BLIP_INFO_ID(8);
	if (UI::DOES_BLIP_EXIST(WaypointHandle))
	{
		Vector3 waypoint1 = UI::GET_BLIP_COORDS(WaypointHandle);
		STREAMING::REQUEST_MODEL(GAMEPLAY::GET_HASH_KEY("marshall"));
		while (!STREAMING::HAS_MODEL_LOADED(GAMEPLAY::GET_HASH_KEY("marshall"))) WAIT(0);
		Vector3 pos = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(PLAYER::PLAYER_PED_ID(), 0.0, 5.0, 0);
		Vehicle veh = VEHICLE::CREATE_VEHICLE(GAMEPLAY::GET_HASH_KEY("marshall"), pos.x, pos.y, pos.z, ENTITY::GET_ENTITY_HEADING(PLAYER::PLAYER_PED_ID()), 1, 1);
		if (veh != 0)
		{
			Ped Driver = PED::CREATE_RANDOM_PED_AS_DRIVER(veh, false);
			PED::SET_PED_INTO_VEHICLE(Driver, veh, -1);
			PED::SET_PED_INTO_VEHICLE(PLAYER::PLAYER_PED_ID(), veh, 0);
			AI::TASK_VEHICLE_DRIVE_TO_COORD(Driver, veh, waypoint1.x, waypoint1.y, waypoint1.z, 40, 1, ENTITY::GET_ENTITY_MODEL(veh), 7, 6, -1);
			char* plateText = ("CRUSADER");
			VEHICLE::SET_VEHICLE_NUMBER_PLATE_TEXT(veh, plateText);
			set_status_text("~g~Chauffer Spawned!");
		}
	}
	else
		set_status_text("~r~Please choose a waypoint first.");
}



int activeLineIndexGuard = 0;

void process_guard_menu()
{
	BOOL bPlayerExists = ENTITY::DOES_ENTITY_EXIST(PLAYER::PLAYER_PED_ID());
	Player player = PLAYER::PLAYER_ID();
	Ped playerPed = PLAYER::PLAYER_PED_ID();

	const float lineWidth = 260.0;  //uptohere
	const int lineCount = 10;

	std::string caption = "MISC OPTIONS";

	static struct {
		LPCSTR		text;
		bool		*pState;
		bool		*pUpdated;
	} lines[lineCount] = {
		{ "Skip Radio Track", NULL, NULL },
		{ "Input Bodyguard Model ~h~~o~>>", NULL, NULL },
		{ "Shake Camera", NULL, NULL },
		{ "Spawn Chauffer", NULL, NULL },
		{ "Clone Player", NULL, NULL },
		{ "Hide HUD", &featureMiscHideHud, NULL },
		{ "Mobile Radio", &featureMiscMobile, NULL },
		{ "ESP", &featureESP, NULL }, //trying to see if freeze worksd
		{ "Name ESP", &featureNameESP, NULL },
		{ "Freeze Self", &freezeplayer, NULL },


	};


	DWORD waitTime = 150;
	while (true)
	{
		// timed menu draw, used for pause after active line switch
		DWORD maxTickCount = GetTickCount() + waitTime;
		do
		{
			// draw menu
			draw_menu_line(caption, lineWidth, 7.0, 13.6, 4.0, 4.0, false, true);
			for (int i = 0; i < lineCount; i++)
				if (i != activeLineIndexGuard)
					draw_menu_line(line_as_str(lines[i].text, lines[i].pState), lineWidth, 4.0, 60.0 + i * 24.0, 4.0, 9.0, false, false);
			draw_menu_line(line_as_str(lines[activeLineIndexGuard].text,
				lines[activeLineIndexGuard].pState), lineWidth + 1.0, 2.0, 60.0 + activeLineIndexGuard * 24.0, 4.0, 7.0, true, false);


			update_features();
			WAIT(0);
		} while (GetTickCount() < maxTickCount);
		waitTime = 0;


		// process buttons
		bool bSelect, bBack, bUp, bDown;
		get_button_state(&bSelect, &bBack, &bUp, &bDown, NULL, NULL);
		if (bSelect)
		{
			menu_beep();
			switch (activeLineIndexGuard)
			{
				// next radio track
			case 0:
				if (ENTITY::DOES_ENTITY_EXIST(PLAYER::PLAYER_PED_ID()) &&
					PED::IS_PED_IN_ANY_VEHICLE(PLAYER::PLAYER_PED_ID(), 0))
					AUDIO::SKIP_RADIO_FORWARD();
				break;
			case 1:
				// create bodyguard
				bodyguard(PLAYER::PLAYER_PED_ID(), PLAYER::PLAYER_ID());
				break;
				break;
			case 2:
				if (bPlayerExists)
				{
					CAM::SHAKE_GAMEPLAY_CAM("MEDIUM_EXPLOSION_SHAKE", 5.0f);
				}
				break;
			case 3:
				if (bPlayerExists)
				{
					spawnChauffer();
				}
			case 4:
				if (bPlayerExists)
				{
					int Ped = PED::CLONE_PED(PLAYER::GET_PLAYER_PED(selectedPlayer), ENTITY::GET_ENTITY_HEADING(PLAYER::GET_PLAYER_PED(selectedPlayer)), 1, 1);

				}
				break;
				// switchable features
			default:
				if (lines[activeLineIndexGuard].pState)
					*lines[activeLineIndexGuard].pState = !(*lines[activeLineIndexGuard].pState);
				if (lines[activeLineIndexGuard].pUpdated)
					*lines[activeLineIndexGuard].pUpdated = true;
			}
			waitTime = 200;
		}
		else
			if (bBack || trainer_switch_pressed())
			{
				menu_beep();
				break;
			}
			else
				if (bUp)
				{
					menu_beep();
					if (activeLineIndexGuard == 0)
						activeLineIndexGuard = lineCount;
					activeLineIndexGuard--;
					waitTime = 150;
				}
				else
					if (bDown)
					{
						menu_beep();
						activeLineIndexGuard++;
						if (activeLineIndexGuard == lineCount)
							activeLineIndexGuard = 0;
						waitTime = 150;
					}
	}
}



int activeLineIndexCredits = 0;

void process_credit_menu()
{
	const float lineWidth = 260.0;
	const int lineCount = 5;

	std::string caption = "Credits";

	static struct {
		LPCSTR		text;
		bool		*pState;
		bool		*pUpdated;
	} lines[lineCount] = {
		{ "Anonik ~r~(Developer)",	NULL,					NULL },
		{ "Thanks Too ~r~(For Some Codes):",	NULL,					NULL },
		{ "Alexander Blade",	NULL,					NULL },
		{ "~r~And Thanks To Design",	NULL,					NULL },
		{ "The Crusader",	NULL,					NULL },
	};


	DWORD waitTime = 150;
	while (true)
	{
		// timed menu draw, used for pause after active line switch
		DWORD maxTickCount = GetTickCount() + waitTime;
		do
		{
			// draw menu
			draw_menu_line(caption, lineWidth, 7.0, 13.6, 4.0, 4.0, false, true);
			for (int i = 0; i < lineCount; i++)
				if (i != activeLineIndexCredits)
					draw_menu_line(line_as_str(lines[i].text, lines[i].pState),
						lineWidth, 4.0, 60.0 + i * 24.0, 4.0, 9.0, false, false);
			draw_menu_line(line_as_str(lines[activeLineIndexCredits].text, lines[activeLineIndexCredits].pState),
				lineWidth + 0.0, 2.0, 60.0 + activeLineIndexCredits * 24.0, 4.0, 7.0, true, false);

			update_features();
			WAIT(0);
		} while (GetTickCount() < maxTickCount);
		waitTime = 0;

		// process buttons
		bool bSelect, bBack, bUp, bDown;
		get_button_state(&bSelect, &bBack, &bUp, &bDown, NULL, NULL);
		if (bSelect)
		{
			menu_beep();
			switch (activeLineIndexCredits)
			{
				// next radio track
			case 0:

				break;
				// switchable features
			default:
				if (lines[activeLineIndexCredits].pState)
					*lines[activeLineIndexCredits].pState = !(*lines[activeLineIndexCredits].pState);
				if (lines[activeLineIndexCredits].pUpdated)
					*lines[activeLineIndexCredits].pUpdated = true;
			}
			waitTime = 200;
		}
		else
			if (bBack || trainer_switch_pressed())
			{
				menu_beep();
				break;
			}
			else
				if (bUp)
				{
					menu_beep();
					if (activeLineIndexCredits == 0)
						activeLineIndexCredits = lineCount;
					activeLineIndexCredits--;
					waitTime = 150;
				}
				else
					if (bDown)
					{
						menu_beep();
						activeLineIndexCredits++;
						if (activeLineIndexCredits == lineCount)
							activeLineIndexCredits = 0;
						waitTime = 150;
					}
	}
}


int activeLineIndexMain = 0;

void process_main_menu()
{
	const float lineWidth = 300.0;
	const int lineCount = 14;

	std::string caption = "~r~A~b~n~u~o~o~n~y~i~g~k~p~ ~w~Menu";

	static LPCSTR lineCaption[lineCount] = {
		"Player Menu ~r~>>",
		"Weapon Menu ~r~>>",
		"Vehicle Menu ~r~>>",
		"World Menu ~r~>>",
		"Time Menu ~r~>>",
		"Weather Menu ~r~>>",
		"Misc Menu ~r~>>",
		"Object Menu ~r~>>",
		"Animation Menu ~r~>>",
		"Vision Menu ~r~>>",
		"Nort Yankton ~r~>>",
		"Vehicle Weapons ~r~>>",
		"Misc Options ~r~>>",
		"Credits ~r~>>"
	};

	DWORD waitTime = 150;
	while (true)
	{
		// timed menu draw, used for pause after active line switch
		DWORD maxTickCount = GetTickCount() + waitTime;
		do
		{

			// draw menu
			draw_menu_line(caption, lineWidth, 7.0, 13.6, 4.0, 4.0, false, true);
			for (int i = 0; i < lineCount; i++)
				if (i != activeLineIndexMain)
					draw_menu_line(lineCaption[i], lineWidth, 4.0, 60.0 + i * 24.0, 4.0, 9.0, false, false);

			draw_menu_line(lineCaption[activeLineIndexMain], lineWidth + 0.0, 2.0, 60.0 + activeLineIndexMain * 24.0, 4.0, 7.0, true, false);
			drawNotification("Made By: ~o~Anonik");
			std::string playerName = PLAYER::GET_PLAYER_NAME(PLAYER::PLAYER_ID());
			std::string welcome = "Welcome ~o~";
			set_status_text(welcome + playerName);
			drawNotification("Use The Menu At ~o~1920x1080p ~w~For The Best Experience.");

			update_features();
			WAIT(0);
		} while (GetTickCount() < maxTickCount);
		waitTime = 0;

		// process buttons
		bool bSelect, bBack, bUp, bDown;
		get_button_state(&bSelect, &bBack, &bUp, &bDown, NULL, NULL);
		if (bSelect)
		{
			menu_beep();
			switch (activeLineIndexMain)
			{
			case 0:
				process_player_menu();
				break;
			case 1:
				process_weapon_menu();
				break;
			case 2:
				process_veh_menu();
				break;
			case 3:
				process_world_menu();
				break;
			case 4:
				process_time_menu();
				break;
			case 5:
				process_weather_menu();
				break;
			case 6:
				process_misc_menu();
				break;
			case 7:
				process_obj_menu();
				break;
			case 8:
				process_animation_menu();
				break;
			case 9:
				process_vision_menu();
				break;
			case 10:
				process_ny_menu();
				break;
			case 11:
				process_wep_menu();
				break;
			case 12:
				process_guard_menu();
				break;
			case 13:
				process_credit_menu();
				break;
			}
			waitTime = 200;
		}
		else
			if (bBack || trainer_switch_pressed())
			{
				menu_beep();
				break;
			}
			else
				if (bUp)
				{
					menu_beep();
					if (activeLineIndexMain == 0)
						activeLineIndexMain = lineCount;
					activeLineIndexMain--;
					waitTime = 150;
				}
				else
					if (bDown)
					{
						menu_beep();
						activeLineIndexMain++;
						if (activeLineIndexMain == lineCount)
							activeLineIndexMain = 0;
						waitTime = 150;
					}
	}
}

void reset_globals()
{
	activeLineIndexMain =
		activeLineIndexPlayer =
		skinchangerActiveLineIndex =
		skinchangerActiveItemIndex =
		teleportActiveLineIndex =
		activeLineIndexWeapon =
		activeLineIndexVeh =
		carspawnActiveLineIndex =
		carspawnActiveItemIndex =
		activeLineIndexWorld =
		activeLineIndexWeather = 0;

	featurePlayerInvincible =
		featurePlayerInvincibleUpdated =
		featurePlayerNeverWanted =

		featurePlayerIgnored =
		featurePlayerIgnoredUpdated =
		featurePlayerUnlimitedAbility =
		featurePlayerNoNoise =
		featurePlayerNoNoiseUpdated =
		featurePlayerFastSwim =
		featurePlayerFastSwimUpdated =
		featurePlayerFastRun =
		featurePlayerUltraJump =
		featurePlayerFastRunUpdated =
		featurePlayerSuperJump =
		featureWeaponNoReload =
		featureWeaponFireAmmo =
		featureWeaponExplosiveAmmo =
		featureWeaponExplosiveMelee =
		featureWeaponVehRockets =
		featureVehInvincible =
		featureVehInvincibleUpdated =
		featureVehInvincibleWheels =
		featureVehInvincibleWheelsUpdated =
		featureVehSeatbelt =
		featureVehSeatbeltUpdated =
		featureVehSpeedBoost =
		featureVehWrapInSpawned =
		featureWorldMoonGravity =
		featureTimePaused =
		featureTimePausedUpdated =
		featureTimeSynced =
		featureWeatherWind =
		featureWeatherPers =
		featureMiscLockRadio =
		featureMiscHideHud = false;
		featureMiscMobile = false;
		featurePedestrianInvincibleBodyguard =
		featurePedestrianInvincibleBodyguardUpdate =
		featureWeaponVehFire =
		featureWeaponVehSnow =
		featureWeaponVehGreen =
		featureWeaponVehRed =
		featureWeaponVehTank =
		featureWeaponVehFlare =

	featureWorldRandomCops =
		featureWorldRandomTrains =
		featureWorldRandomBoats =
		featureWorldGarbageTrucks = true;

	skinchanger_used = false;
}

void main()
{
	reset_globals();

	while (true)
	{
		if (trainer_switch_pressed())
		{
			menu_beep();
			process_main_menu();
		}

		update_features();
		WAIT(0);
	}
}

void ScriptMain()
{
	srand(GetTickCount());
	main();
}