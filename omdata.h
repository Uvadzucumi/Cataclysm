#ifndef _OMDATA_H_
#define _OMDATA_H_

#include <libintl.h>
#define _(String) gettext (String)

#include <string>
#include <vector>
#include <bitset>
#include "mtype.h"
#include "itype.h"
#include "output.h"
#include "mongroup.h"
#include "mapdata.h"

#define OMAPX 180
#define OMAPY 180

#define TUTORIAL_Z	10
#define DEFENSE_Z	20
#define NETHER_Z 	30

class overmap;

struct oter_t {
 std::string name;
 long sym;	// This is a long, so we can support curses linedrawing
 nc_color color;
 unsigned char see_cost; // Affects how far the player can see in the overmap
 const map_extras& embellishments;
 bool known_down;
 bool known_up;
};

const map_extras no_extras(0);
const map_extras road_extras(
// %%% HEL MIL SCI STA DRG SUP PRT MIN WLF PUD CRT FUM 1WY
    50, 40, 50,120,200, 30, 10,  5, 80, 20,200, 10,  8,  2);
const map_extras field_extras(
    60, 40, 15, 40, 80, 10, 10,  3, 50, 30,300, 10,  8,  1);
const map_extras subway_extras(
// %%% HEL MIL SCI STA DRG SUP PRT MIN WLF PUD CRT FUM 1WY
    75,  0,  5, 12,  5,  5,  0,  7,  0,  0,120,  0, 20,  1);
const map_extras build_extras(
    90,  0,  5, 12,  0, 10,  0,  5,  5,  0,  0, 60,  8,  1); 

enum oter_id {
 ot_null = 0,
 ot_crater,
// Wild terrain
 ot_field, ot_forest, ot_forest_thick, ot_forest_water, ot_hive, ot_spider_pit,
  ot_fungal_bloom,
// Roads
 ot_hiway_ns, ot_hiway_ew,
 ot_road_null,
 ot_road_ns, ot_road_ew,
 ot_road_ne, ot_road_es, ot_road_sw, ot_road_wn,
 ot_road_nes, ot_road_new, ot_road_nsw, ot_road_esw, ot_road_nesw,
 ot_road_nesw_manhole,
 ot_bridge_ns, ot_bridge_ew,
 ot_river_center,
 ot_river_c_not_ne, ot_river_c_not_nw, ot_river_c_not_se, ot_river_c_not_sw,
 ot_river_north, ot_river_east, ot_river_south, ot_river_west,
 ot_river_ne, ot_river_se, ot_river_sw, ot_river_nw,
// City buildings
 ot_house_north, ot_house_east, ot_house_south, ot_house_west,
 ot_house_base_north, ot_house_base_east, ot_house_base_south,
  ot_house_base_west,
 ot_s_lot, ot_park,
 ot_s_gas_north, ot_s_gas_east, ot_s_gas_south, ot_s_gas_west,
 ot_s_pharm_north, ot_s_pharm_east, ot_s_pharm_south, ot_s_pharm_west,
 ot_s_grocery_north, ot_s_grocery_east, ot_s_grocery_south, ot_s_grocery_west,
 ot_s_hardware_north, ot_s_hardware_east, ot_s_hardware_south,
  ot_s_hardware_west,
 ot_s_electronics_north, ot_s_electronics_east, ot_s_electronics_south,
  ot_s_electronics_west,
 ot_s_sports_north, ot_s_sports_east, ot_s_sports_south, ot_s_sports_west,
 ot_s_liquor_north, ot_s_liquor_east, ot_s_liquor_south, ot_s_liquor_west,
 ot_s_gun_north, ot_s_gun_east, ot_s_gun_south, ot_s_gun_west,
 ot_s_clothes_north, ot_s_clothes_east, ot_s_clothes_south, ot_s_clothes_west,
 ot_s_library_north, ot_s_library_east, ot_s_library_south, ot_s_library_west,
 ot_sub_station_north, ot_sub_station_east, ot_sub_station_south,
  ot_sub_station_west,
 ot_police_north, ot_police_east, ot_police_south, ot_police_west,
 ot_bank_north, ot_bank_east, ot_bank_south, ot_bank_west,
 ot_bar_north, ot_bar_east, ot_bar_south, ot_bar_west,
 ot_pawn_north, ot_pawn_east, ot_pawn_south, ot_pawn_west,
 ot_mil_surplus_north, ot_mil_surplus_east, ot_mil_surplus_south,
  ot_mil_surplus_west,
 ot_megastore_entrance, ot_megastore,
 ot_hospital_entrance, ot_hospital,
 ot_mansion_entrance, ot_mansion,
// Goodies/dungeons
 ot_shelter, ot_shelter_under,
 ot_lab, ot_lab_stairs, ot_lab_core, ot_lab_finale,
 ot_nuke_plant_entrance, ot_nuke_plant, // TODO
 ot_bunker, ot_outpost,
 ot_silo, ot_silo_finale,
 ot_temple, ot_temple_stairs, ot_temple_core, ot_temple_finale, // TODO
 ot_sewage_treatment, ot_sewage_treatment_hub, ot_sewage_treatment_under,
 ot_mine_entrance, ot_mine_shaft, ot_mine, ot_mine_down, ot_mine_finale,
 ot_spiral_hub, ot_spiral,
 ot_radio_tower,
 ot_toxic_dump,
 ot_cave, ot_cave_rat,
// Underground terrain
 ot_spider_pit_under,
 ot_anthill,
 ot_rock, ot_rift, ot_hellmouth,
 ot_slimepit, ot_slimepit_down,
 ot_triffid_grove, ot_triffid_roots, ot_triffid_finale,
 ot_basement,
 ot_subway_station,
 ot_subway_ns, ot_subway_ew,
 ot_subway_ne, ot_subway_es, ot_subway_sw, ot_subway_wn,
 ot_subway_nes, ot_subway_new, ot_subway_nsw, ot_subway_esw, ot_subway_nesw,
 ot_sewer_ns, ot_sewer_ew,
 ot_sewer_ne, ot_sewer_es, ot_sewer_sw, ot_sewer_wn,
 ot_sewer_nes, ot_sewer_new, ot_sewer_nsw, ot_sewer_esw, ot_sewer_nesw,
 ot_ants_ns, ot_ants_ew,
 ot_ants_ne, ot_ants_es, ot_ants_sw, ot_ants_wn,
 ot_ants_nes, ot_ants_new, ot_ants_nsw, ot_ants_esw, ot_ants_nesw,
 ot_ants_food, ot_ants_larvae, ot_ants_queen,
 ot_cavern,

 ot_tutorial,
 num_ter_types
};

// LINE_**** corresponds to the ACS_**** macros in ncurses, and are patterned
// the same way; LINE_NESW, where X indicates a line and O indicates no line
// (thus, LINE_OXXX looks like 'T'). LINE_ is defined in output.h.  The ACS_
// macros can't be used here, since ncurses hasn't been initialized yet.

// Order MUST match enum oter_id above!

const oter_t oterlist[num_ter_types] = {
{_("nothing"),		'%',	c_white,	0, no_extras, false, false},
{_("crater"),		'O',	c_red,		2, field_extras, false, false},
{_("field"),		'.',	c_brown,	2, field_extras, false, false},
{_("forest"),		'F',	c_green,	3, field_extras, false, false},
{_("forest"),		'F',	c_green,	4, field_extras, false, false},
{_("swamp"),		'F',	c_cyan,		4, field_extras, false, false},
{_("bee hive"),		'8',	c_yellow,	3, field_extras, false, false},
{_("forest"),		'F',	c_green,	3, field_extras, false, false},
/* The tile above is a spider pit. */
{_("fungal bloom"),	'T',	c_ltgray,	2, field_extras, false, false},
{_("highway"),		'H',	c_dkgray,	2, road_extras, false, false},
{_("highway"),		'=',	c_dkgray,	2, road_extras, false, false},
{_("BUG"),			'%',	c_magenta,	0, no_extras, false, false},
{_("road"),          LINE_XOXO,	c_dkgray,	2, road_extras, false, false},
{_("road"),          LINE_OXOX,	c_dkgray,	2, road_extras, false, false},
{_("road"),          LINE_XXOO,	c_dkgray,	2, road_extras, false, false},
{_("road"),          LINE_OXXO,	c_dkgray,	2, road_extras, false, false},
{_("road"),          LINE_OOXX,	c_dkgray,	2, road_extras, false, false},
{_("road"),          LINE_XOOX,	c_dkgray,	2, road_extras, false, false},
{_("road"),          LINE_XXXO,	c_dkgray,	2, road_extras, false, false},
{_("road"),          LINE_XXOX,	c_dkgray,	2, road_extras, false, false},
{_("road"),          LINE_XOXX,	c_dkgray,	2, road_extras, false, false},
{_("road"),          LINE_OXXX,	c_dkgray,	2, road_extras, false, false},
{_("road"),          LINE_XXXX,	c_dkgray,	2, road_extras, false, false},
{_("road, manhole"), LINE_XXXX,	c_yellow,	2, road_extras, true, false},
{_("bridge"),		'|',	c_dkgray,	2, road_extras, false, false},
{_("bridge"),		'-',	c_dkgray,	2, road_extras, false, false},
{_("river"),		'R',	c_blue,		1, no_extras, false, false},
{_("river bank"),		'R',	c_ltblue,	1, no_extras, false, false},
{_("river bank"),		'R',	c_ltblue,	1, no_extras, false, false},
{_("river bank"),		'R',	c_ltblue,	1, no_extras, false, false},
{_("river bank"),		'R',	c_ltblue,	1, no_extras, false, false},
{_("river bank"),		'R',	c_ltblue,	1, no_extras, false, false},
{_("river bank"),		'R',	c_ltblue,	1, no_extras, false, false},
{_("river bank"),		'R',	c_ltblue,	1, no_extras, false, false},
{_("river bank"),		'R',	c_ltblue,	1, no_extras, false, false},
{_("river bank"),		'R',	c_ltblue,	1, no_extras, false, false},
{_("river bank"),		'R',	c_ltblue,	1, no_extras, false, false},
{_("river bank"),		'R',	c_ltblue,	1, no_extras, false, false},
{_("river bank"),		'R',	c_ltblue,	1, no_extras, false, false},
{_("house"),		'^',	c_ltgreen,	5, build_extras, false, false},
{_("house"),		'>',	c_ltgreen,	5, build_extras, false, false},
{_("house"),		'v',	c_ltgreen,	5, build_extras, false, false},
{_("house"),		'<',	c_ltgreen,	5, build_extras, false, false},
{_("house"),		'^',	c_ltgreen,	5, build_extras, false, false},
{_("house"),		'>',	c_ltgreen,	5, build_extras, false, false},
{_("house"),		'v',	c_ltgreen,	5, build_extras, false, false},
{_("house"),		'<',	c_ltgreen,	5, build_extras, false, false},
{_("parking lot"),		'O',	c_dkgray,	1, build_extras, false, false},
{_("park"),		'O',	c_green,	2, build_extras, false, false},
{_("gas station"),		'^',	c_ltblue,	5, build_extras, false, false},
{_("gas station"),		'>',	c_ltblue,	5, build_extras, false, false},
{_("gas station"),		'v',	c_ltblue,	5, build_extras, false, false},
{_("gas station"),		'<',	c_ltblue,	5, build_extras, false, false},
{_("pharmacy"),		'^',	c_ltred,	5, build_extras, false, false},
{_("pharmacy"),		'>',	c_ltred,	5, build_extras, false, false},
{_("pharmacy"),		'v',	c_ltred,	5, build_extras, false, false},
{_("pharmacy"),		'<',	c_ltred,	5, build_extras, false, false},
{_("grocery store"),	'^',	c_green,	5, build_extras, false, false},
{_("grocery store"),	'>',	c_green,	5, build_extras, false, false},
{_("grocery store"),	'v',	c_green,	5, build_extras, false, false},
{_("grocery store"),	'<',	c_green,	5, build_extras, false, false},
{_("hardware store"),	'^',	c_cyan,		5, build_extras, false, false},
{_("hardware store"),	'>',	c_cyan,		5, build_extras, false, false},
{_("hardware store"),	'v',	c_cyan,		5, build_extras, false, false},
{_("hardware store"),	'<',	c_cyan,		5, build_extras, false, false},
{_("electronics store"),   '^',	c_yellow,	5, build_extras, false, false},
{_("electronics store"),   '>',	c_yellow,	5, build_extras, false, false},
{_("electronics store"),   'v',	c_yellow,	5, build_extras, false, false},
{_("electronics store"),   '<',	c_yellow,	5, build_extras, false, false},
{_("sporting goods store"),'^',	c_ltcyan,	5, build_extras, false, false},
{_("sporting goods store"),'>',	c_ltcyan,	5, build_extras, false, false},
{_("sporting goods store"),'v',	c_ltcyan,	5, build_extras, false, false},
{_("sporting goods store"),'<',	c_ltcyan,	5, build_extras, false, false},
{_("liquor store"),	'^',	c_magenta,	5, build_extras, false, false},
{_("liquor store"),	'>',	c_magenta,	5, build_extras, false, false},
{_("liquor store"),	'v',	c_magenta,	5, build_extras, false, false},
{_("liquor store"),	'<',	c_magenta,	5, build_extras, false, false},
{_("gun store"),		'^',	c_red,		5, build_extras, false, false},
{_("gun store"),		'>',	c_red,		5, build_extras, false, false},
{_("gun store"),		'v',	c_red,		5, build_extras, false, false},
{_("gun store"),		'<',	c_red,		5, build_extras, false, false},
{_("clothing store"),	'^',	c_blue,		5, build_extras, false, false},
{_("clothing store"),	'>',	c_blue,		5, build_extras, false, false},
{_("clothing store"),	'v',	c_blue,		5, build_extras, false, false},
{_("clothing store"),	'<',	c_blue,		5, build_extras, false, false},
{_("library"),		'^',	c_brown,	5, build_extras, false, false},
{_("library"),		'>',	c_brown,	5, build_extras, false, false},
{_("library"),		'v',	c_brown,	5, build_extras, false, false},
{_("library"),		'<',	c_brown,	5, build_extras, false, false},
{_("subway station"),	'S',	c_yellow,	5, build_extras, true, false},
{_("subway station"),	'S',	c_yellow,	5, build_extras, true, false},
{_("subway station"),	'S',	c_yellow,	5, build_extras, true, false},
{_("subway station"),	'S',	c_yellow,	5, build_extras, true, false},
{_("police station"),	'^',	c_dkgray,	5, build_extras, false, false},
{_("police station"),	'>',	c_dkgray,	5, build_extras, false, false},
{_("police station"),	'v',	c_dkgray,	5, build_extras, false, false},
{_("police station"),	'<',	c_dkgray,	5, build_extras, false, false},
{_("bank"),		'^',	c_ltgray,	5, no_extras, false, false},
{_("bank"),		'>',	c_ltgray,	5, no_extras, false, false},
{_("bank"),		'v',	c_ltgray,	5, no_extras, false, false},
{_("bank"),		'<',	c_ltgray,	5, no_extras, false, false},
{_("bar"),			'^',	c_pink,		5, build_extras, false, false},
{_("bar"),			'>',	c_pink,		5, build_extras, false, false},
{_("bar"),			'v',	c_pink,		5, build_extras, false, false},
{_("bar"),			'<',	c_pink,		5, build_extras, false, false},
{_("pawn shop"),		'^',	c_white,	5, build_extras, false, false},
{_("pawn shop"),		'>',	c_white,	5, build_extras, false, false},
{_("pawn shop"),		'v',	c_white,	5, build_extras, false, false},
{_("pawn shop"),		'<',	c_white,	5, build_extras, false, false},
{_("mil. surplus"),	'^',	c_white,	5, build_extras, false, false},
{_("mil. surplus"),	'>',	c_white,	5, build_extras, false, false},
{_("mil. surplus"),	'v',	c_white,	5, build_extras, false, false},
{_("mil. surplus"),	'<',	c_white,	5, build_extras, false, false},
{_("megastore"),		'M',	c_ltblue,	5, build_extras, false, false},
{_("megastore"),		'M',	c_blue,		5, build_extras, false, false},
{_("hospital"),		'H',	c_ltred,	5, build_extras, false, false},
{_("hospital"),		'H',	c_red,		5, build_extras, false, false},
{_("mansion"),		'M',	c_ltgreen,	5, build_extras, false, false},
{_("mansion"),		'M',	c_green,	5, build_extras, false, false},
{_("evac shelter"),	'+',	c_white,	2, no_extras, true, false},
{_("evac shelter"),	'+',	c_white,	2, no_extras, false, true},
{_("science lab"),		'L',	c_ltblue,	5, no_extras, false, false},
{_("science lab"),		'L',	c_blue,		5, no_extras, true, false},
{_("science lab"),		'L',	c_ltblue,	5, no_extras, false, false},
{_("science lab"),		'L',	c_cyan,		5, no_extras, false, false},
{_("nuclear plant"),	'P',	c_ltgreen,	5, no_extras, false, false},
{_("nuclear plant"),	'P',	c_ltgreen,	5, no_extras, false, false},
{_("military bunker"),	'B',	c_dkgray,	2, no_extras, true, true},
{_("military outpost"),	'M',	c_dkgray,	2, build_extras, false, false},
{_("missile silo"),	'0',	c_ltgray,	2, no_extras, false, false},
{_("missile silo"),	'0',	c_ltgray,	2, no_extras, false, false},
{_("strange temple"),	'T',	c_magenta,	5, no_extras, true, false},
{_("strange temple"),	'T',	c_pink,		5, no_extras, true, false},
{_("strange temple"),	'T',	c_pink,		5, no_extras, false, false},
{_("strange temple"),	'T',	c_yellow,	5, no_extras, false, false},
{_("sewage treatment"),	'P',	c_red,		5, no_extras, true, false},
{_("sewage treatment"),	'P',	c_green,	5, no_extras, false, true},
{_("sewage treatment"),	'P',	c_green,	5, no_extras, false, false},
{_("mine entrance"),	'M',	c_ltgray,	5, no_extras, true, false},
{_("mine shaft"),		'O',	c_dkgray,	5, no_extras, true, true},
{_("mine"),		'M',	c_brown,	2, no_extras, false, false},
{_("mine"),		'M',	c_brown,	2, no_extras, false, false},
{_("mine"),		'M',	c_brown,	2, no_extras, false, false},
{_("spiral cavern"),	'@',	c_pink,		2, no_extras, false, false},
{_("spiral cavern"),	'@',	c_pink,		2, no_extras, false, false},
{_("radio tower"),         'X',    c_ltgray,       2, no_extras, false, false},
{_("toxic waste dump"),	'D',	c_pink,		2, no_extras, false, false},
{_("cave"),		'C',	c_brown,	2, field_extras, false, false},
{_("rat cave"),		'C',	c_dkgray,	2, no_extras, true, false},
{_("cavern"),		'0',	c_ltgray,	2, no_extras, false, false},
{_("anthill"),		'%',	c_brown,	2, no_extras, true, false},
{_("solid rock"),		'%',	c_dkgray,	5, no_extras, false, false},
{_("rift"),		'^',	c_red,		2, no_extras, false, false},
{_("hellmouth"),		'^',	c_ltred,	2, no_extras, true, false},
{_("slime pit"),		'~',	c_ltgreen,	2, no_extras, true, false},
{_("slime pit"),		'~',	c_ltgreen,	2, no_extras, false, false},
{_("triffid grove"),	'T',	c_ltred,	5, no_extras, true, false},
{_("triffid roots"),	'T',	c_ltred,	5, no_extras, true, true},
{_("triffid heart"),	'T',	c_red,		5, no_extras, false, true},
{_("basement"),		'O',	c_dkgray,	5, no_extras, false, true},
{_("subway station"),	'S',	c_yellow,	5, subway_extras, false, true},
{_("subway"),        LINE_XOXO,	c_dkgray,	5, subway_extras, false, false},
{_("subway"),        LINE_OXOX,	c_dkgray,	5, subway_extras, false, false},
{_("subway"),        LINE_XXOO,	c_dkgray,	5, subway_extras, false, false},
{_("subway"),        LINE_OXXO,	c_dkgray,	5, subway_extras, false, false},
{_("subway"),        LINE_OOXX,	c_dkgray,	5, subway_extras, false, false},
{_("subway"),        LINE_XOOX,	c_dkgray,	5, subway_extras, false, false},
{_("subway"),        LINE_XXXO,	c_dkgray,	5, subway_extras, false, false},
{_("subway"),        LINE_XXOX,	c_dkgray,	5, subway_extras, false, false},
{_("subway"),        LINE_XOXX,	c_dkgray,	5, subway_extras, false, false},
{_("subway"),        LINE_OXXX,	c_dkgray,	5, subway_extras, false, false},
{_("subway"),        LINE_XXXX,	c_dkgray,	5, subway_extras, false, false},
{_("sewer"),         LINE_XOXO,	c_green,	5, no_extras, false, false},
{_("sewer"),         LINE_OXOX,	c_green,	5, no_extras, false, false},
{_("sewer"),         LINE_XXOO,	c_green,	5, no_extras, false, false},
{_("sewer"),         LINE_OXXO,	c_green,	5, no_extras, false, false},
{_("sewer"),         LINE_OOXX,	c_green,	5, no_extras, false, false},
{_("sewer"),         LINE_XOOX,	c_green,	5, no_extras, false, false},
{_("sewer"),         LINE_XXXO,	c_green,	5, no_extras, false, false},
{_("sewer"),         LINE_XXOX,	c_green,	5, no_extras, false, false},
{_("sewer"),         LINE_XOXX,	c_green,	5, no_extras, false, false},
{_("sewer"),         LINE_OXXX,	c_green,	5, no_extras, false, false},
{_("sewer"),         LINE_XXXX,	c_green,	5, no_extras, false, false},
{_("ant tunnel"),    LINE_XOXO,	c_brown,	5, no_extras, false, false},
{_("ant tunnel"),    LINE_OXOX,	c_brown,	5, no_extras, false, false},
{_("ant tunnel"),    LINE_XXOO,	c_brown,	5, no_extras, false, false},
{_("ant tunnel"),    LINE_OXXO,	c_brown,	5, no_extras, false, false},
{_("ant tunnel"),    LINE_OOXX,	c_brown,	5, no_extras, false, false},
{_("ant tunnel"),    LINE_XOOX,	c_brown,	5, no_extras, false, false},
{_("ant tunnel"),    LINE_XXXO,	c_brown,	5, no_extras, false, false},
{_("ant tunnel"),    LINE_XXOX,	c_brown,	5, no_extras, false, false},
{_("ant tunnel"),    LINE_XOXX,	c_brown,	5, no_extras, false, false},
{_("ant tunnel"),    LINE_OXXX,	c_brown,	5, no_extras, false, false},
{_("ant tunnel"),    LINE_XXXX,	c_brown,	5, no_extras, false, false},
{_("ant food storage"),	'O',	c_green,	5, no_extras, false, false},
{_("ant larva chamber"),	'O',	c_white,	5, no_extras, false, false},
{_("ant queen chamber"),	'O',	c_red,		5, no_extras, false, false},
{_("cavern"),		'0',	c_ltgray,	5, no_extras, false, false},
{_("tutorial room"),	'O',	c_cyan,		5, no_extras, false, false}
};

// Overmap specials--these are "special encounters," dungeons, nests, etc.
// This specifies how often and where they may be placed.

// OMSPEC_FREQ determines the length of the side of the square in which each
// overmap special will be placed.  At OMSPEC_FREQ 6, the overmap is divided
// into 900 squares; lots of space for interesting stuff!
#define OMSPEC_FREQ 7

// Flags that determine special behavior for placement
enum omspec_flag {
OMS_FLAG_NULL = 0,
OMS_FLAG_ROTATE_ROAD,	// Rotate to face road--assumes 3 following rotations
OMS_FLAG_ROTATE_RANDOM, // Rotate randomly--assumes 3 following rotations
OMS_FLAG_3X3,		// 3x3 square, e.g. bee hive
OMS_FLAG_BLOB,		// Randomly shaped blob
OMS_FLAG_3X3_SECOND,	// 3x3 square, made of the tile AFTER the main one
OMS_FLAG_BIG,		// As big as possible
OMS_FLAG_ROAD,		// Add a road_point here; connect to towns etc.
OMS_FLAG_PARKING_LOT,	// Add a road_point to the north of here
NUM_OMS_FLAGS
};

struct omspec_place
{
// Able functions - true if p is valid
 bool never      (overmap *om, point p) { return false; }
 bool always     (overmap *om, point p) { return true;  }
 bool water      (overmap *om, point p); // Only on rivers
 bool land       (overmap *om, point p); // Only on land (no rivers)
 bool forest     (overmap *om, point p); // Forest
 bool wilderness (overmap *om, point p); // Forest or fields
 bool by_highway (overmap *om, point p); // Next to existing highways
};

struct overmap_special
{
 oter_id ter;           // Terrain placed
 int min_appearances;	// Min number in an overmap
 int max_appearances;   // Max number in an overmap
 int min_dist_from_city;// Min distance from city limits
 int max_dist_from_city;// Max distance from city limits

 moncat_id monsters;    // Type of monsters that appear here
 int monster_pop_min;   // Minimum monster population
 int monster_pop_max;   // Maximum monster population
 int monster_rad_min;   // Minimum monster radius
 int monster_rad_max;   // Maximum monster radius

 bool (omspec_place::*able) (overmap *om, point p); // See above
 unsigned flags : NUM_OMS_FLAGS; // See above
};

enum omspec_id
{
 OMSPEC_CRATER,
 OMSPEC_HIVE,
 OMSPEC_HOUSE,
 OMSPEC_GAS,
 OMSPEC_CABIN,
 OMSPEC_TEMPLE,
 OMSPEC_LAB,
 OMSPEC_BUNKER,
 OMSPEC_OUTPOST,
 OMSPEC_SILO,
 OMSPEC_RADIO,
 OMSPEC_MEGASTORE,
 OMSPEC_HOSPITAL,
 OMSPEC_SEWAGE,
 OMSPEC_MINE,
 OMSPEC_ANTHILL,
 OMSPEC_SPIDER,
 OMSPEC_SLIME,
 OMSPEC_FUNGUS,
 OMSPEC_TRIFFID,
 OMSPEC_LAKE,
 OMSPEC_SHELTER,
 OMSPEC_CAVE,
 OMSPEC_TOXIC_DUMP,
 NUM_OMSPECS
};

// Set min or max to -1 to ignore them

const overmap_special overmap_specials[NUM_OMSPECS] = {

// Terrain	 MIN MAX DISTANCE
{ot_crater,	   0, 10,  0, -1, mcat_null, 0, 0, 0, 0,
 &omspec_place::land, mfb(OMS_FLAG_BLOB)},

{ot_hive, 	   0, 50, 10, -1, mcat_bee, 20, 60, 2, 4,
 &omspec_place::forest, mfb(OMS_FLAG_3X3)},

{ot_house_north,   0,100,  0, -1, mcat_null, 0, 0, 0, 0,
 &omspec_place::by_highway, mfb(OMS_FLAG_ROTATE_ROAD)},

{ot_s_gas_north,   0,100,  0, -1, mcat_null, 0, 0, 0, 0,
 &omspec_place::by_highway, mfb(OMS_FLAG_ROTATE_ROAD)},

{ot_house_north,   0, 50, 20, -1, mcat_null, 0, 0, 0, 0,  // Woods cabin
 &omspec_place::forest, mfb(OMS_FLAG_ROTATE_RANDOM)|mfb(OMS_FLAG_ROTATE_ROAD)},

{ot_temple_stairs, 0,  3, 20, -1, mcat_null, 0, 0, 0, 0,
 &omspec_place::forest, 0},

{ot_lab_stairs,	   0, 30,  8, -1, mcat_null, 0, 0, 0, 0,
 &omspec_place::land, mfb(OMS_FLAG_ROAD)},

// Terrain	 MIN MAX DISTANCE
{ot_bunker,	   2, 10,  4, -1, mcat_null, 0, 0, 0, 0,
 &omspec_place::land, mfb(OMS_FLAG_ROAD)},

{ot_outpost,	   0, 10,  4, -1, mcat_null, 0, 0, 0, 0,
 &omspec_place::wilderness, 0},

{ot_silo,	   0,  1, 30, -1, mcat_null, 0, 0, 0, 0,
 &omspec_place::wilderness, mfb(OMS_FLAG_ROAD)},

{ot_radio_tower,   1,  5,  0, 20, mcat_null, 0, 0, 0, 0,
 &omspec_place::by_highway, 0},

{ot_megastore_entrance, 0, 5, 0, 10, mcat_null, 0, 0, 0, 0,
 &omspec_place::by_highway, mfb(OMS_FLAG_3X3_SECOND)},

{ot_hospital_entrance, 1, 5, 3, 15, mcat_null, 0, 0, 0, 0,
 &omspec_place::by_highway, mfb(OMS_FLAG_3X3_SECOND)},

{ot_sewage_treatment, 1,  5, 10, 20, mcat_null, 0, 0, 0, 0,
 &omspec_place::land, mfb(OMS_FLAG_PARKING_LOT)},

{ot_mine_entrance,  0,  5,  15, -1, mcat_null, 0, 0, 0, 0,
 &omspec_place::wilderness, mfb(OMS_FLAG_PARKING_LOT)},

// Terrain	 MIN MAX DISTANCE
{ot_anthill,	   0, 30,  10, -1, mcat_ant, 10, 30, 1000, 2000,
 &omspec_place::wilderness, 0},

{ot_spider_pit,	   0,500,  0, -1, mcat_null, 0, 0, 0, 0,
 &omspec_place::forest, 0},

{ot_slimepit,	   0,  4,  0, -1, mcat_goo, 2, 10, 100, 200,
 &omspec_place::land, 0},

{ot_fungal_bloom,  0,  3,  5, -1, mcat_fungi, 600, 1200, 30, 50,
 &omspec_place::wilderness, 0},

{ot_triffid_grove, 0,  4,  0, -1, mcat_triffid, 800, 1300, 12, 20,
 &omspec_place::forest, 0},

{ot_river_center,  0, 10, 10, -1, mcat_null, 0, 0, 0, 0,
 &omspec_place::always, mfb(OMS_FLAG_BLOB)},

// Terrain	 MIN MAX DISTANCE
{ot_shelter,       5, 10,  5, 10, mcat_null, 0, 0, 0, 0,
 &omspec_place::wilderness, mfb(OMS_FLAG_ROAD)},

{ot_cave,	   0, 30,  0, -1, mcat_null, 0, 0, 0, 0,
 &omspec_place::wilderness, 0},

{ot_toxic_dump,	   0,  5, 15, -1, mcat_null, 0, 0, 0, 0,
 &omspec_place::wilderness,0}

};
 

#endif
