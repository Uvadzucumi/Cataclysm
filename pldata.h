#ifndef _PLDATA_H_
#define _PLDATA_H_

#include <libintl.h>
#define _(String) gettext (String)

enum character_type {
 PLTYPE_CUSTOM,
 PLTYPE_RANDOM,
 PLTYPE_TEMPLATE,
 PLTYPE_MAX
};

const std::string pltype_name[PLTYPE_MAX] = {
_("Custom Character"), _("Random Character"), _("Template Character")};

const std::string pltype_desc[PLTYPE_MAX] = { _("\
A custom character you design yourself.  A pool of points is used for\n\
statistics, traits, and skills."),
_("\
A character with stats, traits, and skills chosen randomly."),
_("\
A character loaded from a template file."),
};

enum dis_type {
 DI_NULL,
// Temperature and weather
 DI_GLARE, DI_WET,
 DI_COLD, DI_COLD_FACE, DI_COLD_HANDS, DI_COLD_LEGS, DI_COLD_FEET,
 DI_HOT,	// Can lead to heatstroke
 DI_HEATSTROKE, DI_FBFACE, DI_FBHANDS, DI_FBFEET,
 DI_INFECTION,
 DI_COMMON_COLD, DI_FLU,
// Fields
 DI_SMOKE, DI_ONFIRE, DI_TEARGAS,
// Monsters
 DI_BOOMERED, DI_SAP, DI_SPORES, DI_FUNGUS, DI_SLIMED,
 DI_DEAF, DI_BLIND,
 DI_LYING_DOWN, DI_SLEEP,
 DI_POISON, DI_BADPOISON, DI_FOODPOISON, DI_SHAKES,
 DI_DERMATIK, DI_FORMICATION,
 DI_WEBBED,
 DI_RAT,
// Food & Drugs
 DI_PKILL1, DI_PKILL2, DI_PKILL3, DI_PKILL_L, DI_DRUNK, DI_CIG, DI_HIGH,
  DI_HALLU, DI_VISUALS, DI_IODINE, DI_TOOK_XANAX, DI_TOOK_PROZAC,
  DI_TOOK_FLUMED, DI_ADRENALINE, DI_ASTHMA, DI_METH,
// Traps
 DI_BEARTRAP, DI_IN_PIT,
// Other
 DI_AMIGARA, DI_TELEGLOW, DI_ATTENTION, DI_EVIL, DI_ASKED_TO_FOLLOW,
};

enum add_type {
 ADD_NULL,
 ADD_CAFFEINE, ADD_ALCOHOL, ADD_SLEEP, ADD_PKILLER, ADD_SPEED, ADD_CIG,
 ADD_COKE
};

struct disease
{
 dis_type type;
 int duration;
 disease() { type = DI_NULL; duration = 0; }
 disease(dis_type t, int d) { type = t; duration = d;}
};

struct addiction
{
 add_type type;
 int intensity;
 int sated;
 addiction() { type = ADD_NULL; intensity = 0; sated = 600; }
 addiction(add_type t) { type = t; intensity = 1; sated = 600; }
 addiction(add_type t, int i) { type = t; intensity = i; sated = 600; }
};

enum activity_type {
 ACT_NULL = 0,
 ACT_RELOAD, ACT_READ, ACT_WAIT, ACT_CRAFT, ACT_BUTCHER, ACT_BUILD,
 NUM_ACTIVITIES
};

struct player_activity
{
 activity_type type;
 int moves_left;
 int index;
 std::vector<int> values;
 point placement;
 player_activity() { type = ACT_NULL; moves_left = 0; index = -1;
                     placement = point(-1, -1); }
 player_activity(activity_type t, int turns, int Index)
 {
  type = t;
  moves_left = turns;
  index = Index;
  placement = point(-1, -1);
 }
};
 
enum pl_flag {
 PF_NULL = 0,
 PF_FLEET,	// -15% terrain movement cost
 PF_PARKOUR,	// Terrain movement cost of 3 or 4 are both 2
 PF_QUICK,	// +10% movement points
 PF_OPTIMISTIC,	// Morale boost
 PF_FASTHEALER,	// Heals faster
 PF_LIGHTEATER,	// Less hungry
 PF_PAINRESIST,	// Effects of pain are reduced
 PF_NIGHTVISION,// Increased sight during the night
 PF_POISRESIST,	// Resists poison, etc
 PF_FASTREADER,	// Reads books faster
 PF_TOUGH,	// Bonus to HP
 PF_THICKSKIN,	// Built-in armor of 1
 PF_PACKMULE,	// Bonus to carried volume
 PF_FASTLEARNER,// Better chance of skills leveling up
 PF_DEFT,	// Less movement penalty for melee miss
 PF_DRUNKEN,	// Having a drunk status improves melee combat
 PF_GOURMAND,	// Faster eating, higher level of max satiated
 PF_ANIMALEMPATH,// Animals attack less
 PF_TERRIFYING,	// All creatures run away more
 PF_DISRESISTANT,// Less likely to succumb to low health; TODO: Implement this
 PF_ADRENALINE,	// Big bonuses when low on HP
 PF_INCONSPICUOUS,// Less spawns due to timeouts
 PF_MASOCHIST,	// Morale boost from pain
 PF_LIGHTSTEP,	// Less noise from movement
 PF_HEARTLESS,	// No morale penalty for murder &c
 PF_ANDROID,	// Start with two bionics (occasionally one)
 PF_ROBUST,	// Mutations tend to be good (usually they tend to be bad)

 PF_SPLIT,	// Null trait, splits between bad & good

 PF_MYOPIC,	// Smaller sight radius UNLESS wearing glasses
 PF_HEAVYSLEEPER, // Sleeps in, won't wake up to sounds as easily
 PF_ASTHMA,	// Occasionally needs medicine or suffers effects
 PF_BADBACK,	// Carries less
 PF_ILLITERATE,	// Can not read books
 PF_BADHEARING,	// Max distance for heard sounds is halved
 PF_INSOMNIA,	// Sleep may not happen
 PF_VEGETARIAN,	// Morale penalty for eating meat
 PF_GLASSJAW,	// Head HP is 15% lower
 PF_FORGETFUL,	// Skills decrement faster
 PF_LIGHTWEIGHT,// Longer DI_DRUNK and DI_HIGH
 PF_ADDICTIVE,	// Better chance of addiction / longer addictive effects
 PF_TRIGGERHAPPY,// Possible chance of unintentional burst fire
 PF_SMELLY,	// Default scent is higher
 PF_CHEMIMBALANCE,// Random tweaks to some values
 PF_SCHIZOPHRENIC,// Random bad effects, variety
 PF_JITTERY,	// Get DI_SHAKES under some circumstances
 PF_HOARDER,	// Morale penalty when volume is less than max
 PF_SAVANT,	// All skills except our best are trained more slowly
 PF_MOODSWINGS,	// Big random shifts in morale
 PF_WEAKSTOMACH,// More likely to throw up in all circumstances
 PF_WOOLALLERGY,// Can't wear wool
 PF_HPIGNORANT,	// Don't get to see exact HP numbers, just colors & symbols
 PF_TRUTHTELLER, // Worse at telling lies
 PF_UGLY, // +1 grotesqueness

 PF_MAX,
// Below this point is mutations and other mid-game perks.
// They are NOT available during character creation.
 PF_SKIN_ROUGH,//
 PF_NIGHTVISION2,//
 PF_NIGHTVISION3,//
 PF_INFRARED,//
 PF_FASTHEALER2,//
 PF_REGEN,//
 PF_FANGS,//
 PF_MEMBRANE,//
 PF_GILLS,//
 PF_SCALES,//
 PF_THICK_SCALES,//
 PF_SLEEK_SCALES,//
 PF_LIGHT_BONES,//
 PF_FEATHERS,//
 PF_LIGHTFUR,// TODO: Warmth effects
 PF_FUR,// TODO: Warmth effects
 PF_CHITIN,//
 PF_CHITIN2,//
 PF_CHITIN3,//
 PF_SPINES,//
 PF_QUILLS,//
 PF_PLANTSKIN,//
 PF_BARK,//
 PF_THORNS,
 PF_LEAVES,//
 PF_NAILS,
 PF_CLAWS,
 PF_TALONS,//
 PF_RADIOGENIC,//
 PF_MARLOSS,//
 PF_PHEROMONE_INSECT,//
 PF_PHEROMONE_MAMMAL,//
 PF_DISIMMUNE,
 PF_POISONOUS,//
 PF_SLIME_HANDS,
 PF_COMPOUND_EYES,//
 PF_PADDED_FEET,//
 PF_HOOVES,//
 PF_SAPROVORE,//
 PF_RUMINANT,//
 PF_HORNS,//
 PF_HORNS_CURLED,//
 PF_HORNS_POINTED,//
 PF_ANTENNAE,//
 PF_FLEET2,//
 PF_TAIL_STUB,//
 PF_TAIL_FIN,//
 PF_TAIL_LONG,//
 PF_TAIL_FLUFFY,//
 PF_TAIL_STING,//
 PF_TAIL_CLUB,//
 PF_PAINREC1,//
 PF_PAINREC2,//
 PF_PAINREC3,//
 PF_WINGS_BIRD,//
 PF_WINGS_INSECT,//
 PF_MOUTH_TENTACLES,//
 PF_MANDIBLES,//
 PF_CANINE_EARS,
 PF_WEB_WALKER,
 PF_WEB_WEAVER,
 PF_WHISKERS,

 PF_HEADBUMPS,//
 PF_ANTLERS,//
 PF_SLIT_NOSTRILS,//
 PF_FORKED_TONGUE,//
 PF_EYEBULGE,//
 PF_MOUTH_FLAPS,//
 PF_WINGS_STUB,//
 PF_WINGS_BAT,//
 PF_PALE,//
 PF_SPOTS,//
 PF_SMELLY2,//TODO: NPC reaction
 PF_DEFORMED,
 PF_DEFORMED2,
 PF_DEFORMED3,
 PF_HOLLOW_BONES,//
 PF_NAUSEA,//
 PF_VOMITOUS,//
 PF_HUNGER,//
 PF_THIRST,//
 PF_ROT1,//
 PF_ROT2,//
 PF_ROT3,//
 PF_ALBINO,//
 PF_SORES,//
 PF_TROGLO,//
 PF_TROGLO2,//
 PF_TROGLO3,//
 PF_WEBBED,//
 PF_BEAK,//
 PF_UNSTABLE,//
 PF_RADIOACTIVE1,//
 PF_RADIOACTIVE2,//
 PF_RADIOACTIVE3,//
 PF_SLIMY,//
 PF_HERBIVORE,//
 PF_CARNIVORE,//
 PF_PONDEROUS1,	// 10% movement penalty
 PF_PONDEROUS2, // 20%
 PF_PONDEROUS3, // 30%
 PF_SUNLIGHT_DEPENDANT,//
 PF_COLDBLOOD,//
 PF_COLDBLOOD2,//
 PF_COLDBLOOD3,//
 PF_GROWL,//
 PF_SNARL,//
 PF_SHOUT1,//
 PF_SHOUT2,//
 PF_SHOUT3,//
 PF_ARM_TENTACLES,
 PF_ARM_TENTACLES_4,
 PF_ARM_TENTACLES_8,
 PF_SHELL,
 PF_LEG_TENTACLES,

 PF_MAX2
};

struct trait {
 std::string name;
 int points;		// How many points it costs in character creation
 int visiblity;		// How visible it is--see below, at PF_MAX
 int ugliness;		// How ugly it is--see below, at PF_MAX
 std::string description;
};

const trait traits[] = {
{_("NULL trait!"), 0, 0, 0, _("\
This is a bug.  Weird.")},
{_("Fleet-Footed"), 3, 0, 0, _("\
You can run more quickly than most, resulting in a 15%% speed bonus on sure\n\
footing.")},
{_("Parkour Expert"), 2, 0, 0, _("\
You're skilled at clearing obstacles; terrain like railings or counters are\n\
as easy for you to move on as solid ground.")},
{_("Quick"), 3, 0, 0, _("\
You're just generally quick!  You get a 10%% bonus to action points.")},
{_("Optimist"), 2, 0, 0, _("\
Nothing gets you down!  Your morale will generally be higher than average.")},
{_("Fast Healer"), 2, 0, 0, _("\
You heal a little faster than most; sleeping will heal more lost HP.")},
{_("Light Eater"), 3, 0, 0, _("\
Your metabolism is a little slower, and you require less food than most.")},
{_("Pain Resistant"), 2, 0, 0, _("\
You have a high tolerance for pain.")},
{_("Night Vision"), 1, 0, 0, _("\
You possess natural night vision, and can see two squares instead of one in\n\
pitch blackness.")},
{_("Poison Resistant"), 1, 0, 0, _("\
Your system is rather tolerant of poisons and toxins, and most will affect\n\
you less.")},
{_("Fast Reader"), 1, 0, 0, _("\
You're a quick reader, and can get through books a lot faster than most.")},
{_("Tough"), 5, 0, 0, _("\
It takes a lot to bring you down!  You get a 20%% bonus to all hit points.")},
{_("Thick-Skinned"), 2, 0, 0, _("\
Your skin is tough.  Cutting damage is slightly reduced for you.")},
{_("Packmule"), 3, 0, 0, _("\
You can manage to find space for anything!  You can carry 40%% more volume.")},
{_("Fast Learner"), 3, 0, 0, _("\
Your skill comprehension is 50%% higher, allowing you to learn skills much\n\
faster than others.  Note that this only applies to real-world experience,\n\
not to skill gain from other sources like books.")},
{_("Deft"), 2, 0, 0, _("\
While you're not any better at melee combat, you are better at recovering\n\
from a miss, and will be able to attempt another strike faster.")},
{_("Drunken Master"), 2, 0, 0, _("\
The martial art technique of Zui Quan, or Drunken Fist, comes naturally to\n\
you.  While under the influence of alcohol, your melee skill will rise\n\
considerably, especially unarmed combat.")},
{_("Gourmand"), 2, 0, 0, _("\
You eat faster, and can eat and drink more, than anyone else!  You also enjoy\n\
food more; delicious food is better for your morale, and you don't mind some\n\
unsavory meals.")},
{_("Animal Empathy"), 1, 0, 0, _("\
Peaceful animals will not run away from you, and even aggressive animals are\n\
less likely to attack.  This only applies to natural animals such as woodland\n\
creatures.")},
{_("Terrifying"), 2, 0, 0, _("\
There's something about you that creatures find frightening, and they are\n\
more likely to try to flee.")},
{_("Disease Resistant"), 1, 0, 0, _("\
It's very unlikely that you will catch ambient diseases like a cold or the\n\
flu.")},
{_("High Adrenaline"), 3, 0, 0, _("\
If you are in a very dangerous situation, you may experience a temporary rush\n\
which increases your speed and strength significantly.")},
{_("Inconspicuous"), 2, 0, 0, _("\
While sleeping or staying still, it is less likely that monsters will wander\n\
close to you.")},
{_("Masochist"), 2, 0, 0, _("\
Although you still suffer the negative effects of pain, it also brings a\n\
unique pleasure to you.")},
{_("Light Step"), 1, 0, 0, _("\
You make less noise while walking.  You're also less likely to set off traps.")},
{_("Heartless"), 2, 0, 0, _("\
You have few qualms, and no capacity for pity. Killing the helpless, the\n\
young, and your friends will not affect your morale at all.")},
{_("Android"), 4, 0, 0, _("\
At some point in the past you had a bionic upgrade installed in your body.\n\
You start the game with a power system, and one random bionic enhancement.")},
{_("Robust Genetics"), 2, 0, 0, _("\
You have a very strong genetic base.  If you mutate, the odds that the\n\
mutation will be beneficial are greatly increased.")},

{"NULL", 0, 0, 0, " -------------------------------------------------- "},

{_("Near-Sighted"), -2, 0, 0, _("\
Without your glasses, your seeing radius is severely reduced!  However, while\n\
wearing glasses this trait has no effect, and you are guaranteed to start\n\
with a pair.")},
{_("Heavy Sleeper"), -1, 0, 0, _("\
You're quite the heavy sleeper.  Noises are unlikely to wake you up.")},
{_("Asthmatic"), -4, 0, 0, _("\
You will occasionally need to use an inhaler, or else suffer severe physical\n\
limitations.  However, you are guaranteed to start with an inhaler.")},
{_("Bad Back"), -3, 0, 0, _("\
You simply can not carry as much as people with a similar strength could.\n\
Your maximum weight carried is reduced by 35%%.")},
{_("Illiterate"), -5, 0, 0, _("\
You never learned to read!  Books and computers are off-limits to you.")},
{_("Poor Hearing"), -2, 0, 0, _("\
Your hearing is poor, and you may not hear quiet or far-off noises.")},
{_("Insomniac"), -2, 0, 0, _("\
You have a hard time falling asleep, even under the best circumstances!")},
{_("Vegetarian"), -3, 0, 0, _("\
You have moral objections to eating meat.  You may consume it, but doing so\n\
will hurt your morale.")},
{_("Glass Jaw"), -3, 0, 0, _("\
Your head can't take much abuse.  Its maximum HP is 15%% lower than usual.")},
{_("Forgetful"), -3, 0, 0, _("\
You have a hard time remembering things.  Your skills will erode slightly\n\
faster than usual.")},
{_("Lightweight"), -1, 0, 0, _("\
Alcohol and drugs go straight to your head.  You suffer the negative effects\n\
of these for longer.")},
{_("Addictive Personality"), -3, 0, 0, _("\
It's easier for you to become addicted to substances, and harder to rid\n\
yourself of these addictions.")},
{_("Trigger Happy"), -2, 0, 0, _("\
On rare occasion, you will go full-auto when you intended to fire a single\n\
shot.  This has no effect when firing handguns or other semi-automatic\n\
firearms.")},
{_("Smelly"), -1, 0, 0, _("\
Your scent is particularly strong.  It's not offensive to humans, but animals\n\
that track your scent will do so more easily.")},
{_("Chemical Imbalance"), -2, 0, 0, _("\
You suffer from a minor chemical imbalance, whether mental or physical. Minor\n\
changes to your internal chemistry will manifest themselves on occasion,\n\
such as hunger, sleepiness, narcotic effects, etc.")},
{_("Schizophrenic"), -5, 0, 0, _("\
You will periodically suffer from delusions, ranging from minor effects to\n\
full visual hallucinations.  Some of these effects may be controlled through\n\
the use of Thorazine.")},
{_("Jittery"), -3, 0, 0, _("\
During moments of great stress or under the effects of stimulants, you may\n\
find your hands shaking uncontrollably, severely reducing your dexterity.")},
{_("Hoarder"), -4, 0, 0, _("\
You don't feel right unless you're carrying as much as you can.  You suffer\n\
morale penalties for carrying less than maximum volume (weight is ignored).\n\
Xanax can help control this anxiety.")},
{_("Savant"), -4, 0, 0, _("\
You tend to specialize in one skill and be poor at all others.  You advance\n\
at half speed in all skills except your best one. Note that combining this\n\
with Fast Learner will come out to a slower rate of learning for all skills.")},
{_("Mood Swings"), -1, 0, 0, _("\
Your morale will shift up and down at random, often dramatically.")},
{_("Weak Stomach"), -1, 0, 0, _("\
You are more likely to throw up from food poisoning, alcohol, etc.")},
{_("Wool Allergy"), -1, 0, 0, _("\
You are badly allergic to wool, and can not wear any clothing made of the\n\
substance.")},
{_("HP Ignorant"), -2, 0, 0, _("\
You do not get to see your exact amount of HP remaining, but do have a vague\n\
idea of whether you're in good condition or not.")},
{_("Truth Teller"), -2, 0, 0, _("\
When you try to tell a lie, you blush, stammer, and get all shifty-eyed.\n\
Telling lies and otherwise bluffing will be much more difficult for you.")},
{_("Ugly"), -1, 0, 2, _("\
You're not much to look at.  NPCs who care about such things will react\n\
poorly to you.")},

{_("Bug - PF_MAX"), 0, 0, 0, _("\
This shouldn't be here!  You have the trait PF_MAX toggled.  Weird.")},

/* From here down are mutations.
 * In addition to a points value, mutations have a visibility value and an
 *  ugliness value.
 * Positive visibility means that the mutation is prominent.  This will visibly
 *  identify the player as a mutant, resulting in discrimination from mutant-
 *  haters and trust with mutants/mutant-lovers.
 * Poistive ugliness means that the mutation is grotesque.  This will result in
 *  a negative reaction from NPCs, even those who are themselves mutated, unless
 *  the NPC is a mutant-lover.
 */

{_("Rough Skin"), 0, 2, 1, _("\
Your skin is slightly rough.  This has no gameplay effect.")},
{_("High Night Vision"), 3, 0, 0, _("\
You can see incredibly well in the dark!")},
{_("Full Night Vision"), 5, 0, 0, _("\
You can see in pitch blackness as if you were wearing night-vision goggles.")},
{_("Infrared Vision"), 5, 0, 0, _("\
Your eyes have mutated to pick up radiation in the infrared spectrum.")},
{_("Very Fast Healer"), 5, 0, 0, _("\
Your flesh regenerates slowly, and you will regain HP even when not sleeping.")},
{_("Regeneration"), 10, 0, 0, _("\
Your flesh regenerates from wounds incredibly quickly.")},
{_("Fangs"), 2, 2, 2, _("\
Your teeth have grown into two-inch-long fangs, allowing you to make an extra\n\
attack when conditions favor it.")},
{_("Nictitating Membrane"), 1, 1, 2, _("\
You have a second set of clear eyelids which lower while underwater, allowing\n\
you to see as though you were wearing goggles.")},
{_("Gills"), 3, 5, 3, _("\
You've grown a set of gills in your neck, allowing you to breathe underwater.")},
{_("Scales"), 6, 10, 3, _("\
A set of flexible green scales have grown to cover your body, acting as a\n\
natural armor.")},
{_("Thick Scales"), 6, 10, 4, _("\
A set of heavy green scales have grown to cover your body, acting as a\n\
natural armor.  It is very difficult to penetrate, but also limits your\n\
flexibility, resulting in a -2 penalty to Dexterity.")},
{_("Sleek Scales"), 6, 10, 4, _("\
A set of very flexible and slick scales have grown to cover your body.  These\n\
act as a weak set of armor, improve your ability to swim, and make you\n\
difficult to grab.")},
{_("Light Bones"), 2, 0, 0, _("\
Your bones are very light.  This enables you to run and attack 10%% faster,\n\
but also reduces your carrying weight by 20%% and makes bashing attacks hurt\n\
a little more.")},
{_("Feathers"), 2, 10, 3, _("\
Iridescent feathers have grown to cover your entire body, providing a\n\
marginal protection against attacks and minor protection from cold. They\n\
also provide a natural waterproofing.")},
{_("Lightly Furred"), 1, 6, 2, _("\
Light fur has grown to coveryour entire body, providing slight protection\n\
from cold.")},
{_("Furry"), 2, 10, 3, _("\
Thick black fur has grown to cover your entire body, providing a marginal\n\
protection against attacks, and considerable protection from cold.")},
{_("Chitinous Skin"), 2, 3, 2, _("\
Your epidermis has turned into a thin, flexible layer of chitin.  It provides\n\
minor protection from cutting wounds.")},
{_("Chitinous Armor"), 2, 6, 3, _("\
You've grown a chitin exoskeleton, much like that of an insect.  It provides\n\
considerable physical protection, but reduces your dexterity by 1.")},
{_("Chitinous Plate"), 2, 8, 5, _("\
You've grown a chitin exoskeleton made of thick, stiff plates, like that of\n\
a beetle.  It provides excellent physical protection, but reduces your\n\
dexterity by 1 and encumbers all body parts but your eyes and mouth.")},
{_("Spines"), 1, 0, 0, _("\
Your skin is covered with fine spines.  Whenever an unarmed opponent strikes\n\
a part of your body that is not covered by clothing, they will receive\n\
moderate damage.")},
{_("Quills"), 3, 0, 0, _("\
Your body is covered with large quills.  Whenever an unarmed opponent strikes\n\
a part of your body that is not covered by clothing, they will receive\n\
significant damage.")},
{_("Phelloderm"), 3, 3, 2, _("\
Your skin is light green and has a slightly woody quality to it.  This\n\
provides a weak armor, and helps you retain moisture, resulting in less\n\
thirst.")},
{_("Bark"), 5, 10, 3, _("\
Your skin is coated in a light bark, like that of a tree.  This provides\n\
resistance to bashing and cutting damage and minor protection from fire.")},
{_("Thorns"), 6, 8, 4, _("\
Your skin is covered in small, woody thorns.  Whenever an unarmed opponent\n\
strikes a part of your body that is not covered by clothing, they will\n\
receive minor damage.  Your punches may also deal extra damage.")},
{_("Leaves"), 6, 8, 3, _("\
All the hair on your body has turned to long, grass-like leaves.  Apart from\n\
being physically striking, these provide you with a minor amount of nutrition\n\
while in sunlight.")},
{_("Long Fingernails"), 1, 1, 0, _("\
Your fingernails are long and sharp.  If you aren't wearing gloves, your\n\
unarmed attacks deal a minor amount of cutting damage.")},
{_("Claws"), 2, 3, 2, _("\
You have claws on the ends of your fingers.  If you aren't wearing gloves,\n\
your unarmed attacks deal a minor amount of cutting damage.")},
{_("Large Talons"), 2, 4, 3, _("\
Your index fingers have grown into huge talons.  After a bit of practice, you\n\
find that this does not affect your dexterity, but allows for a deadly\n\
unarmed attack.  They also prevent you from wearing gloves.")},
{_("Radiogenic"), 3, 0, 0, _("\
Your system has adapted to radiation.  While irradiated, you will actually\n\
heal slowly, converting the radiation into hit points.")},
{_("Marloss Carrier"), 4, 0, 0, _("\
Ever since you ate that Marloss berry, you can't get its scent out of your\n\
nose, and you have a strong desire to eat more.")},
{_("Insect Pheromones"), 8, 0, 0, _("\
Your body produces low-level pheromones, identifying you as a friend to many\n\
species of insects.  Insects will attack you much less.")},
{_("Mammal Pheromones"), 8, 0, 0, _("\
Your body produces low-level pheromones which puts mammals at ease.  They\n\
will be less likely to attack or flee from you.")},
{_("Disease Immune"), 6, 0, 0, _("\
Your body is simply immune to diseases.  You will never catch an ambient\n\
disease.")},
{_("Poisonous"), 8, 0, 0, _("\
Your body produces a potent venom.  Any special attacks from mutatations\n\
have a chance to poison your target.")},
{_("Slime Hands"), 4, 5, 4, _("\
The skin on your hands is a mucous membrane and produces a thick, acrid\n\
slime.  Attacks using your hand will cause minor acid damage.")},
{_("Compound Eyes"), 2, 9, 5, _("\
Your eyes are compound, like those of an insect.  This increases your\n\
perception by 2 so long as you aren't wearing eyewear.")},
{_("Padded Feet"), 1, 1, 0, _("\
The bottoms of your feet are strongly padded.  You receive no movement\n\
penalty for not wearing shoes, and even receive a 10%% bonus when running\n\
barefoot.")},
{_("Hooves"), -4, 2, 2, _("\
Your feet have fused into hooves.  This allows kicking attacks to do much\n\
more damage, provides natural armor, and removes the need to wear shoes;\n\
however, you can not wear shoes of any kind.")},
{_("Saprovore"), 4, 0, 0, _("\
Your digestive system is specialized to allow you to consume decaying\n\
material.  You can eat rotten food, albeit for less nutrition than\n\
usual.")},
{_("Ruminant"), 5, 0, 0, _("\
Your digestive system is capable of digesting cellulose and other rough\n\
plant material.  You can eat underbrush by standing over it and pressing\n\
E.")},
{_("Horns"), 2, 3, 1, _("\
You have a pair of small horns on your head.  They allow you to make a weak\n\
piercing headbutt attack.")},
{_("Curled Horns"), 1, 8, 2, _("\
You have a pair of large curled horns, like those of a ram.  They allow you\n\
to make a strong bashing headbutt attack, but prevent you from wearing any\n\
headwear.")},
{_("Pointed Horns"), 2, 8, 2, _("\
You have a pair of long, pointed horns, like those of an antelope.  They\n\
allow you to make a strong piercing headbutt attack, but prevent you from\n\
wearing any headwear the is not made of fabric.")},
{_("Antennae"), 1, 9, 4, _("\
You have a pair of antennae.  They allow you to detect the presence of\n\
monsters up to a few tiles away, even if you can't see or hear them, but\n\
prevent you from wearing headwear that is not made of fabric.")},
{_("Road-Runner"), 4, 0, 0, _("\
Your legs are extremely limber and fast-moving.  You run 30%% faster on\n\
flat surfaces.")},
{_("Stubby Tail"), 0, 1, 2, _("\
You have a short, stubby tail, like a rabbit's.  It serves no purpose.")},
{_("Tail Fin"), 1, 4, 2, _("\
You have a fin-like tail.  It allows you to swim more quickly.")},
{_("Long Tail"), 2, 6, 2, _("\
You have a long, graceful tail, like that of a big cat.  It improves your\n\
balance, making your ability to dodge higher.")},
{_("Fluffy Tail"), 2, 7, 0, _("\
You have a long, fluffy-furred tail.  It greatly improves your balance,\n\
making your ability to dodge much higher.")},
{_("Spiked Tail"), 2, 6, 3, _("\
You have a long tail that ends in a vicious stinger, like that of a\n\
scorpion.  It does not improve your balance at all, but allows for a\n\
powerful piercing attack.")},
{_("Club Tail"), 2, 7, 2, _("\
You have a long tail that ends in a heavy, bony club.  It does not improve\n\
your balance at all, but alows for a powerful bashing attack.")},
{_("Pain Recovery"), 3, 0, 0, _("\
You recover from pain slightly faster than normal.")},
{_("Quick Pain Recovery"), 5, 0, 0, _("\
You recover from pain faster than normal.")},
{_("Very Quick Pain Reovery"), 8, 0, 0, _("\
You recover from pain much faster than normal.")},
{_("Bird Wings"), 2, 4, 2, _("\
You have a pair of large, feathered wings.  Your body is too heavy to be able\n\
to fly, but you can use them to slow your descent during a fall, and will not\n\
take falling damage under any circumstances.")},
{_("Insect Wings"), 3, 4, 4, _("\
You have a pair of large, translucent wings.  You buzz them as you run,\n\
enabling you to run faster.")},
{_("Mouth Tentacles"), 1, 8, 5, _("\
A set of tentacles surrounds your mouth.  They allow you to eat twice as\n\
fast.")},
{_("Mandibles"), 2, 8, 6, _("\
A set of insect-like mandibles have grown around your mouth.  They allow you\n\
to eat faster and provide a slicing unarmed attack, but prevent you from\n\
wearing mouthwear.")},
{_("Canine Ears"), 2, 4, 1, _("\
Your ears have extended into long, pointed ones, like those of a canine.\n\
They enhance your hearing, allowing you to hear at greater distances.")},
{_("Web Walker"), 3, 0, 0, _("\
Your body excretes very fine amounts of a chemcial which prevents you from\n\
sticking to webs.  Walking through webs does not affect you at all.")},
{_("Web Weaver"), 3, 0, 0, _("\
Your body produces webs.  As you move, there is a chance that you will\n\
leave webs in your wake.")},
{_("Whiskers"), 1, 3, 1, _("\
You have a set of prominent rodent-like whiskers around your mouth.  These\n\
make you more aware of vibrations in the air, and improve your ability to\n\
dodge very slightly.")},

{_("Head Bumps"), 0, 3, 3, _("\
You have a pair of bumps on your skull.")},
{_("Antlers"), -2, 10, 3, _("\
You have a huge rack of antlers, like those of a moose.  They prevent you\n\
from hearing headwear that is not made of fabric, but provide a weak\n\
headbutt attack.")},
{_("Slit Nostrils"), -2, 7, 4, _("\
You have a flattened nose and thin slits for nostrils, giving you a lizard-\n\
like appearance.  This makes breathing slightly difficult and increases\n\
mouth encumbrance by 1.")},
{_("Forked Tongue"), 0, 1, 3, _("\
Your tongue is forked, like that of a reptile.  This has no effect.")},
{_("Bulging Eyes"), 0, 8, 4, _("\
Your eyes bulge out several inches from your skull.  This does not affect\n\
your vision in any way.")},
{_("Mouth Flaps"), -1, 7, 6, _("\
Skin tabs and odd flaps of skin surround your mouth.  They don't affect your\n\
eating, but are unpleasant to look at.")},
{_("Wing Stubs"), 0, 2, 2, _("\
You have a pair of stubby little wings projecting from your shoulderblades.\n\
They can be wiggled at will, but are useless.")},
{_("Bat Wings"), -1, 9, 4, _("\
You have a pair of large, leathery wings.  You can move them a little, but\n\
they are useless, and in fact put you off balance, reducing your ability to\n\
dodge slightly.")},
{_("Pale Skin"), 0, 3, 1, _("\
Your skin is rather pale.")},
{_("Spots"), 0, 6, 2, _("\
Your skin is covered in a pattern of red spots.")},
{_("Very Smelly"), -4, 4, 5, _("\
You smell awful.  Monsters that track scent will find you very easily, and\n\
humans will react poorly.")},
{_("Deformed"), -2, 4, 4, _("\
You're minorly deformed.  Some people will react badly to your appearance.")},
{_("Badly Deformed"), -4, 7, 7, _("\
You're hideously deformed.  Some people will have a strong negative reaction\n\
to your appearance.")},
{_("Grotesque"), -7, 10, 10, _("\
Your visage is disgusting and liable to induce vomiting.  People will not\n\
want to interact with you unless they have a very good reason to.")},
{_("Hollow Bones"), -6, 0, 0, _("\
You have Avian Bone Syndrome--your bones are nearly hollow.  Your body is\n\
very light as a result, enabling you to run and attack 20%% faster, but\n\
also frail; you can carry 40%% less, and bashing attacks injure you more.")},
{_("Nausea"), -3, 0, 0, _("\
You feel nauseous almost constantly, and are more liable to throw up from\n\
food poisoning, alcohol, etc.")},
{_("Vomitous"), -8, 0, 0, _("\
You have a major digestive disorder, which causes you to vomit frequently.")},
{_("Fast Metabolism"), -2, 0, 0, _("\
You require more food than most people.")},
{_("High Thirst"), -3, 0, 0, _("\
Your body dries out easily; you need to drink a lot more water.")},
{_("Weakening"), -6, 0, 0, _("\
You feel as though you are slowly weakening, but it's so slight a feeling\n\
that it does not affect you at all.")},
{_("Deterioration"), -8, 0, 0, _("\
Your body is very slowly wasting away.")},
{_("Disintegration"), -10, 0, 0, _("\
Your body is slowly wasting away!")},
{_("Albino"), -2, 0, 0, _("\
Your skin lacks pigment, and is nearly transparent.  You suffer serious burns\n\
in direct sunlight.")},
{_("Sores"), -2, 5, 6, _("\
Your body is covered in painful sores.  The pain is worse when they are\n\
covered in clothing.")},
{_("Light Sensitive"), -2, 0, 0, _("\
Sunlight makes you uncomfortable.  If you are outdoors and the weather is\n\
Sunny, you suffer -1 to all stats.")},
{_("Very Light Sensitive"), -3, 0, 0, _("\
Sunlight makes you very uncomfortable.  If you are outdoors during the day,\n\
you suffer -1 to all stats; -2 if the weather is Sunny.")},
{_("Troglobite"), -5, 0, 0, _("\
Sunlight makes you extremely uncomfortable, resulting in large penalties to\n\
all stats.")},
{_("Webbed Hands"), -1, 3, 2, _("\
Your hands and feet are heavily webbed, reducing your dexterity by 1 and\n\
preventing you from wearing gloves.  However, you can swim much faster.")},
{_("Beak"), -1, 8, 4, _("\
You have a beak for a mouth.  You can occasionally use it to peck at your\n\
enemies, but it is impossible for you to wear mouthgear.")},
{_("Genetically Unstable"), -4, 0, 0, _("\
Your DNA has been damaged in a way that causes you to continually develop\n\
more mutations.")},
{_("Minor Radioactivity"), -4, 0, 0, _("\
Your body has become radioactive!  You continuously emit low levels of\n\
radiation, some of which will be absorbed by you, and some of which will\n\
contaminate the world around you.")},
{_("Radioactivity"), -4, 0, 0, _("\
Your body has become radioactive!  You continuously emit moderate levels of\n\
radiation, some of which will be absorbed by you, and some of which will\n\
contaminate the world around you.")},
{_("Severe Radioactivity"), -4, 0, 0, _("\
Your body has become radioactive!  You continuously emit heavy levels of\n\
radiation, some of which will be absorbed by you, and some of which will\n\
contaminate the world around you.")},
{_("Slimy"), -1, 7, 6, _("\
Your body is coated with a fine slime, which oozes off of you, leaving a\n\
trail.")},
{_("Herbivore"), -3, 0, 0, _("\
Your body's ability to digest meat is severely hampered.  Eating meat has a\n\
good chance of making you vomit it back up; even if you manage to keep it\n\
down, its nutritional value is greatly reduced.")},
{_("Carnivore"), -3, 0, 0, _("\
Your body's ability to digest fruits, vegetables and grains is severely\n\
hampered.  You cannot eat anything besides meat.")},
{_("Ponderous"), -3, 0, 0, _("\
Your muscles are generally slow to move.  You run 10%% slower.")},
{_("Very Ponderous"), -5, 0, 0, _("\
Your muscles are quite slow to move.  You run 20%% slower.")},
{_("Extremely Ponderous"), -8, 0, 0, _("\
Your muscles are very slow to move.  You run 30%% slower.")},
{_("Sunlight Dependant"), -5, 0, 0, _("\
You feel very sluggish when not in direct sunlight.  You suffer a 5%% drop in\n\
speed when in shade, and a 10%% drop in speed when in the dark.")},
{_("Heat Dependant"), -2, 0, 0, _("\
Your muscle response is dependant on ambient temperatures.  You lose 1%% of\n\
your speed for every 5 degrees below 65 F.")},
{_("Very Heat Dependant"), -3, 0, 0, _("\
Your muscle response is highly dependant on ambient temperatures.  You lose\n\
1%% of your speed for every 3 degrees below 65 F.")},
{_("Cold Blooded"), -5, 0, 0, _("\
You are cold-blooded and rely on heat to keep moving.  Your lose 1%% of your\n\
speed for every 2 degrees below 65 F.")},
{_("Growling Voice"), -1, 0, 0, _("\
You have a growling, rough voice.  Persuading NPCs will be more difficult,\n\
but threatening them will be easier.")},
{_("Snarling Voice"), -2, 0, 0, _("\
You have a threatening snarl in your voice.  Persuading NPCs will be near\n\
impossible, but threatening them will be much easier.")},
{_("Shouter"), -2, 0, 0, _("\
You occasionally shout uncontrollably.")},
{_("Screamer"), -3, 0, 0, _("\
You sometimes scream uncontrollably.")},
{_("Howler"), -5, 0, 0, _("\
You frequently let out a piercing howl.")},
{_("Tentacle Arms"), -5, 7, 4, _("\
Your arms have transformed into tentacles.  Though they are flexible and\n\
increase your dexterity by 1, the lack of fingers results in a permanent\n\
hand encumbrance of 3, and prevents the wearing of gloves.")},
{_("4 Tentacles"), -3, 8, 5, _("\
Your arms have transformed into four tentacles, resulting in a bonus of 1 to\n\
dexterity, permanent hand encumbrance of 3, and preventing you from wearing\n\
gloves.  You can make up to 3 extra attacks with them.")},
{_("8 Tentacles"), -2, 9, 6, _("\
Your arms have transformed into eight tentacles, resulting in a bonus of 1 to\n\
dexterity, permanent hand encumbrance of 3, and preventing you from wearing\n\
gloves.  You can make up to 7 extra attacks with them.")},
{_("Shell"), -6, 8, 3, _("\
You have grown a thick shell over your torso, providing excellent armor.  You\n\
find you can use the empty space as 16 storage space, but cannot wear\n\
anything on your torso.")},
{_("Leg Tentacles"), -3, 8, 4, _("\
Your legs have transformed into six tentacles.  This decreases your speed on\n\
land by 20%, but makes your movement silent.  However, they also increase\n\
your swimming speed.")}
};

enum hp_part {
 hp_head = 0,
 hp_torso,
 hp_arm_l,
 hp_arm_r,
 hp_leg_l,
 hp_leg_r,
 num_hp_parts
};
#endif
