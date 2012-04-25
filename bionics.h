#ifndef _BIONICS_H_
#define _BIONICS_H_
#include <string>

/* Thought: Perhaps a HUD bionic that changes the display of the game?
 * Showing more information or something. */

enum bionic_id {
bio_null,
// Power sources
bio_batteries, bio_metabolics, bio_solar, bio_furnace, bio_ethanol,
// Automatic / Always-On
bio_memory, bio_ears,
bio_eye_enhancer, bio_membrane, bio_targeting,
bio_gills, bio_purifier,
bio_climate, bio_storage, bio_recycler, bio_digestion,	// TODO: Ynnn
bio_tools, bio_shock, bio_heat_absorb,
bio_carbon, bio_armor_head, bio_armor_torso, bio_armor_arms, bio_armor_legs,
// Player Activated
bio_flashlight, bio_night_vision, bio_infrared, 
bio_face_mask,	// TODO
bio_ads, bio_ods, bio_scent_mask,bio_scent_vision, bio_cloak, bio_painkiller, bio_nanobots,
 bio_heatsink, bio_resonator, bio_time_freeze, bio_teleport, bio_blood_anal,
 bio_blood_filter, bio_alarm,
bio_evap, bio_lighter, bio_claws, bio_blaster, bio_laser, bio_emp,
// TODO: fingerhack
 bio_hydraulics, bio_water_extractor, bio_magnet, bio_fingerhack, bio_lockpick,
bio_ground_sonar,
max_bio_start,
// EVERYTHING below this point is not available to brand-new players
// TODO: all these suckers
bio_banish, bio_gate_out, bio_gate_in, bio_nightfall, bio_drilldown,
bio_heatwave, bio_lightning, bio_tremor, bio_flashflood,
max_bio_good,
// Everything below THIS point is bad bionics, meant to be punishments
bio_dis_shock, bio_dis_acid, bio_drain, bio_noise, bio_power_weakness,
 bio_stiff,
max_bio
};

struct bionic_data {
 std::string name;
 bool power_source;
 bool activated;	// If true, then the below function only happens when
			// activated; otherwise, it happens every turn
 int power_cost;
 int charge_time;	// How long, when activated, between drawing power_cost
			// If 0, it draws power once
 std::string description;
};

struct bionic {
 bionic_id id;
 char invlet;
 bool powered;
 int charge;
 bionic() {
  id = bio_batteries;
  invlet = 'a';
  powered = false;
  charge = 0;
 }
 bionic(bionic_id pid, char pinvlet) {
  id = pid;
  invlet = pinvlet;
  powered = false;
  charge = 0;
 };
};
 
const bionic_data bionics[] = {
{_("NULL bionics"), false, false, 0, 0, _("\
If you're seeing this, it's a bug.")},
// NAME          ,PW_SRC, ACT ,COST, TIME,
{_("Battery System"), true, false, 0, 0, _("\
You have a battery draining attachment, and thus can make use of the energy\n\
contained in normal, everyday batteries.  Use 'E' to consume batteries.")},
{_("Metabolic Interchange"), true, false, 0, 0, _("\
Your digestive system and power supply are interconnected.  Any drain on\n\
energy instead increases your hunger.")},
{_("Solar Panels"), true, false, 0, 0, _("\
You have a few solar panels installed.  While in direct sunlight, your power\n\
level will slowly recharge.")},
{_("Internal Furnace"), true, false, 0, 0, _("\
You can burn nearly any organic material as fuel (use 'E'), recharging your\n\
power level.  Some materials will burn better than others.")},
{_("Ethanol Burner"), true, false, 0, 0, _("\
You burn alcohol as fuel in an extremely efficient reaction.  However, you\n\
will still suffer the inebriating effects of the substance.")},

{_("Enhanced Memory Banks"), false, false, 1, 0, _("\
Your memory has been enhanced with small quantum storage drives.  Any time\n\
you start to forget a skill, you have a chance at retaining all knowledge, at\n\
the cost of a small amount of power.")},
{_("Enhanced Hearing"), false, false, 0, 0, _("\
Your hearing has been drastically improved, allowing you to hear ten times\n\
better than the average person.  Additionally, high-intensity sounds will be\n\
automatically dampened before they can damage your hearing.")},
{_("Diamond Cornea"), false, false, 0, 0, _("\
Your vision is greatly enhanced, giving you a +2 bonus to perception.")},
{_("Nictating Membrane"), false, false, 0, 0, _("\
Your eyes have a thin membrane that closes over your eyes while underwater,\n\
negating any vision penalties.")},
{_("Targeting System"), false, false, 0, 0, _("\
Your eyes are equipped with range finders, and their movement is synced with\n\
that of your arms, to a degree.  Shots you fire will be much more accurate,\n\
particularly at long range.")},
{_("Membrane Oxygenator"), false, false, 1, 0, _("\
An oxygen interchange system automatically switches on while underwater,\n\
slowly draining your energy reserves but providing oxygen.")},
{_("Air Filtration System"), false, false, 1, 0, _("\
Implanted in your trachea is an advanced filtration system.  If toxins find\n\
their way into your windpipe, the filter will attempt to remove them, at the\n\
cost of a small amount of power.")},
{_("Internal Climate Control"), false, false, 0, 0, _("\
Throughout your body lies a network of thermal piping which eases the effects\n\
of high and low ambient temperatures.  It has an operating range of 0 to 140\n\
degrees Fahrenheit.")},
{_("Internal Storage"), false, false, 0, 0, _("\
Space inside your chest cavity has been converted into a storage area.  You\n\
may carry an extra 8 units of volume.")},
{_("Recycler Unit"), false, false, 0, 0, _("\
Your digestive system has been outfitted with a series of filters and\n\
processors, allowing you to reclaim waste liquid and, to a lesser degree,\n\
nutrients.  The net effect is a greatly reduced need to eat and drink.")},
{_("Expanded Digestive System"), false, false, 0, 0, _("\
You have been outfitted with three synthetic stomachs and industrial-grade\n\
intestines.  Not only can you extract much more nutrition from food, but you\n\
are highly resistant to foodborne illness, and can sometimes eat rotten food.")},
{_("Integrated Toolset"), false, false, 0, 0, _("\
Implanted in your hands and fingers is a complete tool set - screwdriver,\n\
hammer, wrench, and heating elements.  You can use this in place of many\n\
tools when crafting.")},
{_("Electroshock Unit"), false, false, 1, 0, _("\
While fighting unarmed, or with a weapon that conducts electricity, there is\n\
a chance that a successful hit will shock your opponent, inflicting extra\n\
damage and disabling them temporarily at the cost of some energy.")},
{_("Heat Drain"), false, false, 1, 0, _("\
While fighting unarmed against a warm-blooded opponent, there is a chance\n\
that a successful hit will drain body heat, inflicting a small amount of\n\
extra damage, and increasing your power reserves slightly.")},
{_("Subdermal Carbon Filament"), false, false, 0, 0, _("\
Lying just beneath your skin is a thin armor made of carbon nanotubes. This\n\
reduces bashing damage by 2 and cutting damage by 4, but also reduces your\n\
dexterity by 2.")},
{_("Alloy Plating - Head"), false, false, 0, 0, _("\
The flesh on your head has been replaced by a strong armor, protecting both\n\
your head and jaw regions, but increasing encumberance by 2 and decreasing\n\
perception by 1.")},
{_("Alloy Plating - Torso"), false, false, 0, 0, _("\
The flesh on your torso has been replaced by a strong armor, protecting you\n\
greatly, but increasing your encumberance by 2.")},
{_("Alloy Plating - Arms"), false, false, 0, 0, _("\
The flesh on your arms has been replaced by a strong armor, protecting you\n\
greatly, but decreasing your dexterity by 1.")},
{_("Alloy Plating - Legs"), false, false, 0, 0, _("\
The flesh on your legs has been replaced by a strong armor, protecting you\n\
greatly, but decreasing your speed by 15.")},

{_("Cranial Flashlight"), false, true, 1, 30, _("\
Mounted between your eyes is a small but powerful LED flashlight.")},
{_("Implanted Night Vision"), false, true, 1, 20, _("\
Your eyes have been modified to amplify existing light, allowing you to see\n\
in the dark.")},
{_("Infrared Vision"), false, true, 1, 4, _("\
Your range of vision extends into the infrared, allowing you to see warm-\n\
blooded creatures in the dark, and even through walls.")},
{_("Facial Distortion"), false, true, 1, 10, _("\
Your face is actually made of a compound which may be molded by electrical\n\
impulses, making you impossible to recognize.  While not powered, however,\n\
the compound reverts to its default shape.")},
{_("Active Defense System"), false, true, 1, 7, _("\
A thin forcefield surrounds your body, continually draining power.  Anything\n\
attempting to penetrate this field has a chance of being deflected at the\n\
cost of more energy.  Melee attacks will be stopped more often than bullets.")},
{_("Offensive Defense System"), false, true, 1, 6, _("\
A thin forcefield surrounds your body, continually draining power.  This\n\
field does not deflect penetration, but rather delivers a very strong shock,\n\
damaging unarmed attackers and those with a conductive weapon.")},
{_("Olfactory Mask"), false, true, 1, 8, _("\
While this system is powered, your body will produce very little odor, making\n\
it nearly impossible for creatures to track you by scent.")},
{_("Scent Vision"), false, true, 1, 30, _("\
While this system is powered, you're able to visually sense your own scent,\n\
making it possible for you to recognize your surroundings even if you can't\n\
see it.")},
{_("Cloaking System"), false, true, 2, 1, _("\
This high-power system uses a set of cameras and LEDs to make you blend into\n\
your background, rendering you fully invisible to normal vision.  However,\n\
you may be detected by infrared, sonar, etc.")},
{_("Sensory Dulling"), false, true, 2, 0, _("\
Your nervous system is wired to allow you to inhibit the signals of pain,\n\
allowing you to dull your senses at will.  However, the use of this system\n\
may cause delayed reaction time and drowsiness.")},
{_("Repair Nanobots"), false, true, 5, 0, _("\
Inside your body is a fleet of tiny dormant robots.  Once charged from your\n\
energy banks, they will flit about your body, repairing any damage.")},
{_("Thermal Dissapation"), false, true, 1, 6, _("\
Powerful heatsinks supermaterials are woven into your flesh.  While powered,\n\
this system will prevent heat damage up to 2000 degrees fahrenheit.  Note\n\
that this does not affect your internal temperature.")},
{_("Sonic Resonator"), false, true, 4, 0, _("\
Your entire body may resonate at very high power, creating a short-range\n\
shockwave.  While it will not to much damage to flexible creatures, stiff\n\
items such as walls, doors, and even robots will be severely damaged.")},
{_("Time Dilation"), false, true, 3, 0, _("\
At an immense cost of power, you may increase your body speed and reactions\n\
dramatically, essentially freezing time.  You are still delicate, however,\n\
and violent or rapid movements may damage you due to friction.")},
{_("Teleportation Unit"), false, true, 10, 0, _("\
This highly experimental unit folds space over short distances, instantly\n\
transporting your body up to 25 feet at the cost of much power.  Note that\n\
prolonged or frequent use may have dangerous side effects.")},
{_("Blood Analysis"), false, true, 1, 0, _("\
Small sensors have been implanted in your heart, allowing you to analyse your\n\
blood.  This will detect many illnesses, drugs, and other conditions.")},
{_("Blood Filter"), false, true, 3, 0, _("\
A filtration system in your heart allows you to actively filter out chemical\n\
impurities, primarily drugs.  It will have limited impact on viruses.  Note\n\
that it is not a targeted filter; ALL drugs in your system will be affected.")},
{_("Alarm System"), false, true, 1, 400, _("\
A motion-detecting alarm system will notice almost all movement within a\n\
fifteen-foot radius, and will silently alert you.  This is very useful during\n\
sleep, or if you suspect a cloaked pursuer.")},
{_("Aero-Evaporator"), false, true, 8, 0, _("\
This unit draws moisture from the surrounding air, which then is poured from\n\
a fingertip in the form of water.  It may fail in very dry environments.")},
{_("Mini-Flamethrower"), false, true, 3, 0, _("\
The index fingers of both hands have powerful fire starters which extend from\n\
the tip.")},
{_("Adamantium Claws"), false, true, 3, 0, _("\
Your fingers can withdraw into your hands, allowing a set of vicious claws to\n\
extend.  These do considerable cutting damage, but prevent you from holding\n\
anything else.")},
{_("Fusion Blaster Arm"), false, true, 2, 0, _("\
Your left arm has been replaced by a heavy-duty fusion blaster!  You may use\n\
your energy banks to fire a damaging heat ray; however, you are unable to use\n\
or carry two-handed items, and may only fire handguns.")},
{_("Finger-Mounter Laser"), false, true, 2, 0, _("\
One of your fingers has a small high-powered laser embedded in it.  This long\n\
range weapon is not incredibly damaging, but is very accurate, and has the\n\
potential to start fires.")},
{_("Directional EMP"), false, true, 4, 0, _("\
Mounted in the palms of your hand are small parabolic EMP field generators.\n\
You may use power to fire a short-ranged blast which will disable electronics\n\
and robots.")},
{_("Hydraulic Muscles"), false, true, 1, 3, _("\
While activated, the muscles in your arms will be greatly enchanced,\n\
increasing your strength by 20.")},
{_("Water Extraction Unit"), false, true, 2, 0, _("\
Nanotubs embedded in the palm of your hand will pump any available fluid out\n\
of a dead body, cleanse it of impurities and convert it into drinkable water.\n\
You must, however, have a container to store the water in.")},
{_("Electromagnetic Unit"), false, true, 2, 0, _("\
Embedded in your hand is a powerful electromagnet, allowing you to pull items\n\
made of iron over short distances.")},
{_("Fingerhack"), false, true, 1, 0, _("\
One of your fingers has an electrohack embedded in it; an all-purpose hacking\n\
unit used to override control panels and the like (but not computers).  Skill\n\
in computers is important, and a failed use may damage your circuits.")},
{_("Fingerpick"), false, true, 1, 0, _("\
One of your fingers has an electronic lockpick embedded in it.  This auto-\n\
matic system will quickly unlock all but the most advanced key locks without\n\
any skill required on the part of the user.")},
{_("Terranian Sonar"), false, true, 1, 5, _("\
Your feet are equipped with precision sonar equipment, allowing you to detect\n\
the movements of creatures below the ground.")},

{_("max_bio_start - BUG"), false, false, 0, 0, _("\
This is a placeholder bionic meant to demarkate those which a new character\n\
can start with.  If you are reading this, you have found a bug!")},

{_("Banishment"), false, true, 40, 0, _("\
You can briefly open a one-way gate to the nether realm, banishing a single\n\
target there permanently.  This is not without its dangers, however, as the\n\
inhabitants of the nether world may take notice.")},
{_("Gate Out"), false, true, 55, 0, _("\
You can temporarily open a two-way gate to the nether realm, accessible only\n\
by you.  This will remove you from immediate danger, but may attract the\n\
attention of the nether world's inhabitants...")},
{_("Gate In"), false, true, 35, 0, _("\
You can temporarily open a one-way gate from the nether realm.  This will\n\
attract the attention of its horrifying inhabitants, who may begin to pour\n\
into reality.")},
{_("Artificial Night"), false, true, 5, 1, _("\
Photon absorbtion panels will attract and obliterate photons within a 100'\n\
radius, casting an area around you into pitch darkness.")},
{_("Borehole Drill"), false, true, 30, 0, _("\
Your legs can transform into a powerful drill which will bury you 50 feet\n\
into the earth.  Be careful to only drill down when you know you will be able\n\
to get out, or you'll simply dig your own tomb.")},
{_("Heatwave"), false, true, 45, 0, _("\
At the cost of immense power, you can cause dramatic spikes in the ambient\n\
temperature around you.  These spikes are very short-lived, but last long\n\
enough to ignite wood, paper, gunpowder, and other substances.")},
{_("Chain Lightning"), false, true, 48, 0, _("\
Powerful capacitors unleash an electrical storm at your command, sending a\n\
chain of highly-damaging lightning through the air.  It has the power to\n\
injure several opponents before grounding itself.")},
{_("Tremor Pads"), false, true, 40, 0, _("\
Using tremor pads in your feet, you can cause a miniature earthquake.  The\n\
shockwave will damage enemies (particularly those digging underground), tear\n\
down walls, and churn the earth.")},
{_("Flashflood"), false, true, 35, 0, _("\
By drawing the moisture from the air, and synthesizing water from in-air\n\
elements, you can create a massive puddle around you.  The effects are more\n\
powerful when used near a body of water.")},

{_("max_bio_good - BUG"), false, false, 0, 0, _("\
This is a placeholder bionic.  If you are reading this, you have found a bug!")},

{_("Electrical Discharge"), false, false, 0, 0, _("\
A malfunctioning bionic which occasionally discharges electricity through\n\
your body, causing pain and brief paralysis but no damage.")},
{_("Acidic Discharge"), false, false, 0, 0, _("\
A malfunctioning bionic which occasionally discharges acid into your muscles,\n\
causing sharp pain and minor damage.")},
{_("Electrical Drain"), false, false, 0, 0, _("\
A malfunctioning bionic.  It doesn't perform any useful function, but will\n\
occasionally draw power from your batteries.")},
{_("Noisemaker"), false, false, 0, 0, _("\
A malfunctioning bionic.  It will occasionally emit a loud burst of noise.")},
{_("Power Overload"), false, false, 0, 0, _("\
Damaged power circuits cause short-circuiting inside your muscles when your\n\
batteries are above 75%%%% capacity, causing greatly reduced strength.  This\n\
has no effect if you have no internal batteries.")},
{_("Wire-induced Stiffness"), false, false, 0, 0, _("\
Improperly installed wires cause a physical stiffness in most of your body,\n\
causing increased encumberance.")}

};

#endif
