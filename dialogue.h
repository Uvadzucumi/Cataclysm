#ifndef _DIALOGUE_H_
#define _DIALOGUE_H_

#include "player.h"
#include "output.h"
#include "npc.h"
#include <vector>
#include <string>

struct dialogue {
 player *alpha;
 npc *beta;
 WINDOW *win;
 bool done;
 std::vector<std::string> history;
 std::vector<talk_topic> topic_stack;

 int opt(std::string challenge, ...);
 talk_topic opt(talk_topic topic, game *g);

 dialogue()
 {
  alpha = NULL;
  beta = NULL;
  win = NULL;
  done = false;
 }
};

struct talk_function
{
 void nothing			(game *g, npc *p) {};
 void assign_mission		(game *g, npc *p);
 void mission_success		(game *g, npc *p);
 void mission_failure		(game *g, npc *p);
 void clear_mission		(game *g, npc *p);
 void mission_reward		(game *g, npc *p);
 void give_equipment		(game *g, npc *p);
 void start_trade		(game *g, npc *p);
 void follow			(game *g, npc *p); // p follows u
 void deny_follow		(game *g, npc *p); // p gets DI_ASKED_TO_FOLLOW
 void enslave			(game *g, npc *p) {}; // p becomes slave of u
 void hostile			(game *g, npc *p) {}; // p turns hostile to u

 void toggle_use_guns		(game *g, npc *p);
 void toggle_use_grenades	(game *g, npc *p);
 void set_engagement_none	(game *g, npc *p);
 void set_engagement_close	(game *g, npc *p);
 void set_engagement_weak	(game *g, npc *p);
 void set_engagement_hit	(game *g, npc *p);
 void set_engagement_all	(game *g, npc *p);
};

enum talk_trial
{
 TALK_TRIAL_NONE, // No challenge here!
 TALK_TRIAL_LIE, // Straight up lying
 TALK_TRIAL_PERSUADE, // Convince them
 TALK_TRIAL_INTIMIDATE, // Physical intimidation
 NUM_TALK_TRIALS
};

std::string talk_trial_text[NUM_TALK_TRIALS] = {
"", _("LIE"), _("PERSUADE"), _("INTIMIDATE")
};

struct talk_response
{
 std::string text;
 talk_trial trial;
 int difficulty;
 int mission_index;
 npc_opinion opinion_success;
 npc_opinion opinion_failure;
 void (talk_function::*effect_success)(game *, npc *);
 void (talk_function::*effect_failure)(game *, npc *);
 talk_topic success;
 talk_topic failure;

 talk_response()
 {
  text = "";
  trial = TALK_TRIAL_NONE;
  difficulty = 0;
  mission_index = -1;
  effect_success = &talk_function::nothing;
  effect_failure = &talk_function::nothing;
  success = TALK_NONE;
  failure = TALK_NONE;
 }

 talk_response(const talk_response &rhs)
 {
  text = rhs.text;
  trial = rhs.trial;
  difficulty = rhs.difficulty;
  mission_index = rhs.mission_index;
  effect_success = rhs.effect_success;
  effect_failure = rhs.effect_failure;
  success = rhs.success;
  failure = rhs.failure;
 }
};

struct talk_response_list
{
 std::vector<talk_response> none(game *g, npc *p);
 std::vector<talk_response> shelter(game *g, npc *p);
 std::vector<talk_response> shopkeep(game *g, npc *p);
};

/* There is a array of tag_data, "tags", at the bottom of this file.
 * It maps tags to the array of string replacements;
 * e.g. "<name_g>" => talk_good_names
 * Other tags, like "<yrwp>", are mapped to dynamic things
 *  (like the player's weapon), and are defined in parse_tags() (npctalk.cpp)
 */
struct tag_data
{
 std::string tag;
 std::string (*replacement)[10];
};

std::string talk_needs[num_needs][5] = {
{"", "", "", "", ""},
{_("Hey<punc> You got any <ammo>?"), _("I'll need some <ammo> soon, got any?"),
 _("I really need some <ammo><punc>"), _("I need <ammo> for my <mywp>, got any?"),
 _("I need some <ammo> <very> bad<punc>")},
{_("Got anything I can use as a weapon?"),
 _("<ill_die> without a good weapon<punc>"),
 _("I'm sick of fighting with my <swear> <mywp>, got something better?"),
 _("Hey <name_g>, care to sell me a weapon?"),
 _("My <mywp> just won't cut it, I need a real weapon...")},
{_("Hey <name_g>, I could really use a gun."),
 _("Hey, you got a spare gun?  It'd be better than my <swear> <mywp><punc>"),
 _("<ill_die> if I don't find a gun soon!"),
 _("<name_g><punc> Feel like selling me a gun?"),
 _("I need a gun, any kind will do!")},
{_("I could use some food, here."), _("I need some food, <very> bad!"),
 _("Man, am I <happy> to see you!  Got any food to trade?"),
 _("<ill_die> unless I get some food in me<punc> <okay>?"),
 _("Please tell me you have some food to trade!")},
{_("Got anything to drink?"), _("I need some water or something."),
 _("<name_g>, I need some water... got any?"),
 _("<ill_die> without something to drink."), _("You got anything to drink?")}
/*
{"<ill_die> unless I get healed<punc>", "You gotta heal me up, <name_g><punc>",
 "Help me<punc> <ill_die> if you don't heal me<punc>",
 "Please... I need medical help<punc>", "
*/
};

std::string talk_okay[10] = {
_("okay"), _("get it"), _("you dig"), _("dig"), _("got it"), _("you see"), _("see, <name_g>"),
_("alright"), _("that clear")};

std::string talk_no[10] = {
_("no"), _("fuck no"), _("hell no"), _("no way"), _("not a chance"),
_("I don't think so"), _("no way in hell"), _("nuh uh"), _("nope"), _("fat chance")};

std::string talk_bad_names[10] = {
_("punk"),		_("bitch"),	_("dickhead"),	_("asshole"),	_("fucker"),
_("sucker"),	_("fuckwad"),	_("cocksucker"),	_("motherfucker"),	_("shithead")};

std::string talk_good_names[10] = {
_("stranger"),	_("friend"),	_("pilgrim"),	_("traveler"),	_("pal"),
_("fella"),	_("you"),		_("dude"),		_("buddy"),	_("man")};

std::string talk_swear[10] = { // e.g. "drop the <swear> weapon"
_("fucking"), _("goddamn"), _("motherfucking"), _("freaking"), _("damn"), _("<swear> <swear>"),
_("fucking"), _("fuckin'"), _("god damn"), _("mafuckin'")};

std::string talk_swear_interjection[10] = {
_("fuck"), _("damn"), _("damnit"), _("shit"), _("cocksucker"), _("crap"),
_("motherfucker"), _("<swear><punc> <swear!>"), _("<very> <swear!>"), _("son of a bitch")};

std::string talk_fuck_you[10] = {
_("fuck you"), _("fuck off"), _("go fuck yourself"), _("<fuck_you>, <name_b>"),
_("<fuck_you>, <swear> <name_b>"), _("<name_b>"), _("<swear> <name_b>"),
_("fuck you"), _("fuck off"), _("go fuck yourself")};

std::string talk_very[10] = { // Synonyms for "very" -- applied to adjectives
_("really"), _("fucking"), _("super"), _("wicked"), _("very"), _("mega"), _("uber"), _("ultra"),
_("so <very>"), _("<very> <very>")};

std::string talk_really[10] = { // Synonyms for "really" -- applied to verbs
_("really"), _("fucking"), _("absolutely"), _("definitely"), _("for real"), _("honestly"),
_("<really> <really>"), _("most <really>"), _("urgently"), _("REALLY")};

std::string talk_happy[10] = {
_("glad"), _("happy"), _("overjoyed"), _("ecstatic"), _("thrilled"), _("stoked"),
_("<very> <happy>"), _("tickled pink"), _("delighted"), _("pumped")};

std::string talk_sad[10] = {
_("sad"), _("bummed"), _("depressed"), _("pissed"), _("unhappy"), _("<very> <sad>"), _("dejected"),
_("down"), _("blue"), _("glum")};

std::string talk_greeting_gen[10] = {
_("Hey <name_g>."), _("Greetings <name_g>."), _("Hi <name_g><punc> You okay?"),
_("<name_g><punc>  Let's talk."), _("Well hey there."),
_("<name_g><punc>  Hello."), _("What's up, <name_g>?"), _("You okay, <name_g>?"),
_("Hello, <name_g>."), _("Hi <name_g>")};

std::string talk_ill_die[10] = {
_("I'm not gonna last much longer"), _("I'll be dead soon"), _("I'll be a goner"),
_("I'm dead, <name_g>,"), _("I'm dead meat"), _("I'm in <very> serious trouble"),
_("I'm <very> doomed"), _("I'm done for"), _("I won't last much longer"),
_("my days are <really> numbered")};

std::string talk_ill_kill_you[10] = {
_("I'll kill you"), _("you're dead"), _("I'll <swear> kill you"), _("you're dead meat"),
_("<ill_kill_you>, <name_b>"), _("you're a dead <man>"), _("you'll taste my <mywp>"),
_("you're <swear> dead"), _("<name_b>, <ill_kill_you>")};

std::string talk_drop_weapon[10] = {
_("Drop your <swear> weapon!"),
_("Okay <name_b>, drop your weapon!"),
_("Put your <swear> weapon down!"),
_("Drop the <yrwp>, <name_b>!"),
_("Drop the <swear> <yrwp>!"),
_("Drop your <yrwp>!"),
_("Put down the <yrwp>!"),
_("Drop your <swear> weapon, <name_b>!"),
_("Put down your <yrwp>!"),
_("Alright, drop the <yrwp>!")
};

std::string talk_hands_up[10] = {
_("Put your <swear> hands up!"),
_("Put your hands up, <name_b>!"),
_("Reach for the sky!"),
_("Hands up!"),
_("Hands in the air!"),
_("Hands up, <name_b>!"),
_("Hands where I can see them!"),
_("Okay <name_b>, hands up!"),
_("Okay <name_b><punc> hands up!"),
_("Hands in the air, <name_b>!")
};

std::string talk_no_faction[10] = {
_("I'm unaffiliated."),
_("I don't run with a crew."),
_("I'm a solo artist, <okay>?"),
_("I don't kowtow to any group, <okay>?"),
_("I'm a freelancer."),
_("I work alone, <name_g>."),
_("I'm a free agent, more money that way."),
_("I prefer to work uninhibited by that kind of connection."),
_("I haven't found one that's good enough for me."),
_("I don't belong to a faction, <name_g>.")
};

std::string talk_come_here[10] = {
_("Wait up, let's talk!"),
_("Hey, I <really> want to talk to you!"),
_("Come on, talk to me!"),
_("Hey <name_g>, let's talk!"),
_("<name_g>, we <really> need to talk!"),
_("Hey, we should talk, <okay>?"),
_("<name_g>!  Wait up!"),
_("Wait up, <okay>?"),
_("Let's talk, <name_g>!"),
_("Look, <name_g><punc> let's talk!")
};

std::string talk_wait[10] = {
_("Hey, where are you?"),
_("Wait up, <name_g>!"),
_("<name_g>, wait for me!"),
_("Hey, wait up, <okay>?"),
_("You <really> need to wait for me!"),
_("You <swear> need to wait!"),
_("<name_g>, where are you?"),
_("Hey <name_g><punc> Wait for me!"),
_("Where are you?!"),
_("Hey, I'm over here!")
};

std::string talk_let_me_pass[10] = {
_("Excuse me, let me pass."),
_("Hey <name_g>, can I get through?"),
_("Let me get past you, <name_g>."),
_("Let me through, <okay>?"),
_("Can I get past you, <name_g>?"),
_("I need to get past you, <name_g>."),
_("Move your <swear> ass, <name_b>!"),
_("Out of my way, <name_b>!"),
_("Move it, <name_g>!"),
_("You need to move, <name_g>, <okay>?")
};

// Used to tell player to move to avoid friendly fire
std::string talk_move[10] = {
_("Move"),
_("Move your ass"),
_("Get out of the way"),
_("You need to move"),
_("Hey <name_g>, move"),
_("<swear> move it"),
_("Move your <swear> ass"),
_("Get out of my way, <name_b>,"),
_("Move to the side"),
_("Get out of my line of fire")
};

std::string talk_done_mugging[10] = {
_("Thanks for the cash, <name_b>!"),
_("So long, <name_b>!"),
_("Thanks a lot, <name_g>!"),
_("Catch you later, <name_g>!"),
_("See you later, <name_b>!"),
_("See you in hell, <name_b>!"),
_("Hasta luego, <name_g>!"),
_("I'm outta here! <done_mugging>"),
_("Bye bye, <name_b>!"),
_("Thanks, <name_g>!")
};

#define NUM_STATIC_TAGS 23


tag_data talk_tags[NUM_STATIC_TAGS] = {
{"<okay>",		&talk_okay},
{"<no>",		&talk_no},
{"<name_b>",		&talk_bad_names},
{"<name_g>",		&talk_good_names},
{"<swear>",		&talk_swear},
{"<swear!>",		&talk_swear_interjection},
{"<fuck_you>",		&talk_fuck_you},
{"<very>",		&talk_very},
{"<really>",		&talk_really},
{"<happy>",		&talk_happy},
{"<sad>",		&talk_sad},
{"<greet>",		&talk_greeting_gen},
{"<ill_die>",		&talk_ill_die},
{"<ill_kill_you>",	&talk_ill_kill_you},
{"<drop_it>",		&talk_drop_weapon},
{"<hands_up>",		&talk_hands_up},
{"<no_faction>",	&talk_no_faction},
{"<come_here>",		&talk_come_here},
{"<lets_talk>",		&talk_come_here},
{"<wait>",		&talk_wait},
{"<let_me_pass>",	&talk_let_me_pass},
{"<move>",		&talk_move},
{"<done_mugging>",	&talk_done_mugging}
};

#endif
