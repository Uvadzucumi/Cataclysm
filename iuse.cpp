#include "iuse.h"
#include "game.h"
#include "mapdata.h"
#include "keypress.h"
#include "output.h"
#include "rng.h"
#include "line.h"
#include "player.h"
#include <sstream>

#define RADIO_PER_TURN 25 // how many characters per turn of radio

/* To mark an item as "removed from inventory", set its invlet to 0
   This is useful for traps (placed on ground), inactive bots, etc
 */
void iuse::sewage(game *g, player *p, item *it, bool t)
{
 p->vomit(g);
 if (one_in(4))
  p->mutate(g);
}

void iuse::royal_jelly(game *g, player *p, item *it, bool t)
{
// TODO: Add other diseases here; royal jelly is a cure-all!
 p->pkill += 5;
 std::string message;
 if (p->has_disease(DI_FUNGUS)) {
  message = _("You feel cleansed inside!");
  p->rem_disease(DI_FUNGUS);
 }
 if (p->has_disease(DI_BLIND)) {
  message = _("Your sight returns!");
  p->rem_disease(DI_BLIND);
 }
 if (p->has_disease(DI_POISON) || p->has_disease(DI_FOODPOISON) ||
     p->has_disease(DI_BADPOISON)) {
  message = _("You feel much better!");
  p->rem_disease(DI_POISON);
  p->rem_disease(DI_BADPOISON);
  p->rem_disease(DI_FOODPOISON);
 }
 if (p->has_disease(DI_ASTHMA)) {
  message = _("Your breathing clears up!");
  p->rem_disease(DI_ASTHMA);
 }
 if (p->has_disease(DI_COMMON_COLD) || p->has_disease(DI_FLU)) {
  message = _("You feel healther!");
  p->rem_disease(DI_COMMON_COLD);
  p->rem_disease(DI_FLU);
 }
 if (!p->is_npc())
  g->add_msg(message.c_str());
}

void iuse::bandage(game *g, player *p, item *it, bool t) 
{
 int bonus = p->sklevel[sk_firstaid];
 hp_part healed;

 if (p->is_npc()) { // NPCs heal whichever has sustained the most damage
  int highest_damage = 0;
  for (int i = 0; i < num_hp_parts; i++) {
   int damage = p->hp_max[i] - p->hp_cur[i];
   if (i == hp_head)
    damage *= 1.5;
   if (i == hp_torso)
    damage *= 1.2;
   if (damage > highest_damage) {
    highest_damage = damage;
    healed = hp_part(i);
   }
  }
 } else { // Player--present a menu
   
  WINDOW* w = newwin(10, 20, 8, 1);
  wborder(w, LINE_XOXO, LINE_XOXO, LINE_OXOX, LINE_OXOX,
             LINE_OXXO, LINE_OOXX, LINE_XXOO, LINE_XOOX );
  mvwprintz(w, 1, 1, c_ltred,  _("Bandage where?"));
  mvwprintz(w, 2, 1, c_ltgray, _("1: Head"));
  mvwprintz(w, 3, 1, c_ltgray, _("2: Torso"));
  mvwprintz(w, 4, 1, c_ltgray, _("3: Left Arm"));
  mvwprintz(w, 5, 1, c_ltgray, _("4: Right Arm"));
  mvwprintz(w, 6, 1, c_ltgray, _("5: Left Leg"));
  mvwprintz(w, 7, 1, c_ltgray, _("6: Right Leg"));
  mvwprintz(w, 8, 1, c_ltgray, _("7: Exit"));
  nc_color col;
  int curhp;
  for (int i = 0; i < num_hp_parts; i++) {
   curhp = p->hp_cur[i];
   int tmpbonus = bonus;
   if (curhp != 0) {
    switch (hp_part(i)) {
     case hp_head:  curhp += 1;	tmpbonus *=  .8;	break;
     case hp_torso: curhp += 4;	tmpbonus *= 1.5;	break;
     default:       curhp += 3;				break;
    }
    curhp += tmpbonus;
    if (curhp > p->hp_max[i])
     curhp = p->hp_max[i];
    if (curhp == p->hp_max[i])
     col = c_green;
    else if (curhp > p->hp_max[i] * .8)
     col = c_ltgreen;
    else if (curhp > p->hp_max[i] * .5)
     col = c_yellow;
    else if (curhp > p->hp_max[i] * .3)
     col = c_ltred;
    else
     col = c_red;
    if (p->has_trait(PF_HPIGNORANT))
     mvwprintz(w, i + 2, 15, col, "***");
    else {
     if (curhp >= 100)
      mvwprintz(w, i + 2, 15, col, "%d", curhp);
     else if (curhp >= 10)
      mvwprintz(w, i + 2, 16, col, "%d", curhp);
     else
      mvwprintz(w, i + 2, 17, col, "%d", curhp);
    }
   } else	// curhp is 0; requires surgical attention
    mvwprintz(w, i + 2, 15, c_dkgray, "---");
  }
  wrefresh(w);
  char ch;
  do {
   ch = getch();
   if (ch == '1')
    healed = hp_head;
   else if (ch == '2')
    healed = hp_torso;
   else if (ch == '3') {
    if (p->hp_cur[hp_arm_l] == 0) {
     g->add_msg(_("That arm is broken.  It needs surgical attention."));
     it->charges++;
     return;
    } else
     healed = hp_arm_l;
   } else if (ch == '4') {
    if (p->hp_cur[hp_arm_r] == 0) {
     g->add_msg(_("That arm is broken.  It needs surgical attention."));
     it->charges++;
     return;
    } else
     healed = hp_arm_r;
   } else if (ch == '5') {
    if (p->hp_cur[hp_leg_l] == 0) {
     g->add_msg(_("That leg is broken.  It needs surgical attention."));
     it->charges++;
     return;
    } else
     healed = hp_leg_l;
   } else if (ch == '6') {
    if (p->hp_cur[hp_leg_r] == 0) {
     g->add_msg(_("That leg is broken.  It needs surgical attention."));
     it->charges++;
     return;
    } else
     healed = hp_leg_r;
   } else if (ch == '7') {
    g->add_msg(_("Never mind."));
    it->charges++;
    return;
   }
  } while (ch < '1' || ch > '7');
  werase(w);
  wrefresh(w);
  delwin(w);
  refresh();
 }

 p->practice(sk_firstaid, 8);
 int dam = 0;
 if (healed == hp_head)
  dam = 1 + bonus * .8;
 else if (healed == hp_torso)
  dam = 4 + bonus * 1.5;
 else
  dam = 3 + bonus;
 p->heal(healed, dam);
}

void iuse::firstaid(game *g, player *p, item *it, bool t) 
{
 int bonus = p->sklevel[sk_firstaid];
 hp_part healed;

 if (p->is_npc()) { // NPCs heal whichever has sustained the most damage
  int highest_damage = 0;
  for (int i = 0; i < num_hp_parts; i++) {
   int damage = p->hp_max[i] - p->hp_cur[i];
   if (i == hp_head)
    damage *= 1.5;
   if (i == hp_torso)
    damage *= 1.2;
   if (damage > highest_damage) {
    highest_damage = damage;
    healed = hp_part(i);
   }
  }
 } else { // Player--present a menu
   
  WINDOW* w = newwin(10, 20, 8, 1);
  wborder(w, LINE_XOXO, LINE_XOXO, LINE_OXOX, LINE_OXOX,
             LINE_OXXO, LINE_OOXX, LINE_XXOO, LINE_XOOX );
  mvwprintz(w, 1, 1, c_ltred,  _("Bandage where?"));
  mvwprintz(w, 2, 1, c_ltgray, _("1: Head"));
  mvwprintz(w, 3, 1, c_ltgray, _("2: Torso"));
  mvwprintz(w, 4, 1, c_ltgray, _("3: Left Arm"));
  mvwprintz(w, 5, 1, c_ltgray, _("4: Right Arm"));
  mvwprintz(w, 6, 1, c_ltgray, _("5: Left Leg"));
  mvwprintz(w, 7, 1, c_ltgray, _("6: Right Leg"));
  mvwprintz(w, 8, 1, c_ltgray, _("7: Exit"));
  nc_color col;
  int curhp;
  for (int i = 0; i < num_hp_parts; i++) {
   curhp = p->hp_cur[i];
   int tmpbonus = bonus;
   if (curhp != 0) {
    switch (hp_part(i)) {
     case hp_head:  curhp += 10; tmpbonus *=  .8;	break;
     case hp_torso: curhp += 18; tmpbonus *= 1.5;	break;
     default:       curhp += 14;			break;
    }
    curhp += tmpbonus;
    if (curhp > p->hp_max[i])
     curhp = p->hp_max[i];
    if (curhp == p->hp_max[i])
     col = c_green;
    else if (curhp > p->hp_max[i] * .8)
     col = c_ltgreen;
    else if (curhp > p->hp_max[i] * .5)
     col = c_yellow;
    else if (curhp > p->hp_max[i] * .3)
     col = c_ltred;
    else
     col = c_red;
    if (p->has_trait(PF_HPIGNORANT))
     mvwprintz(w, i + 2, 15, col, "***");
    else {
     if (curhp >= 100)
      mvwprintz(w, i + 2, 15, col, "%d", curhp);
     else if (curhp >= 10)
      mvwprintz(w, i + 2, 16, col, "%d", curhp);
     else
      mvwprintz(w, i + 2, 17, col, "%d", curhp);
    }
   } else	// curhp is 0; requires surgical attention
    mvwprintz(w, i + 2, 15, c_dkgray, "---");
  }
  wrefresh(w);
  char ch;
  do {
   ch = getch();
   if (ch == '1')
    healed = hp_head;
   else if (ch == '2')
    healed = hp_torso;
   else if (ch == '3') {
    if (p->hp_cur[hp_arm_l] == 0) {
     g->add_msg(_("That arm is broken.  It needs surgical attention."));
     it->charges++;
     return;
    } else
     healed = hp_arm_l;
   } else if (ch == '4') {
    if (p->hp_cur[hp_arm_r] == 0) {
     g->add_msg(_("That arm is broken.  It needs surgical attention."));
     it->charges++;
     return;
    } else
     healed = hp_arm_r;
   } else if (ch == '5') {
    if (p->hp_cur[hp_leg_l] == 0) {
     g->add_msg(_("That leg is broken.  It needs surgical attention."));
     it->charges++;
     return;
    } else
     healed = hp_leg_l;
   } else if (ch == '6') {
    if (p->hp_cur[hp_leg_r] == 0) {
     g->add_msg(_("That leg is broken.  It needs surgical attention."));
     it->charges++;
     return;
    } else
     healed = hp_leg_r;
   } else if (ch == '7') {
    g->add_msg(_("Never mind."));
    it->charges++;
    return;
   }
  } while (ch < '1' || ch > '7');
  werase(w);
  wrefresh(w);
  delwin(w);
  refresh();
 }

 p->practice(sk_firstaid, 8);
 int dam = 0;
 if (healed == hp_head)
  dam = 10 + bonus * .8;
 else if (healed == hp_torso)
  dam = 18 + bonus * 1.5;
 else
  dam = 14 + bonus;
 p->heal(healed, dam);
}

// Aspirin
void iuse::pkill_1(game *g, player *p, item *it, bool t)
{
 if (!p->is_npc())
  g->add_msg(_("You take some %s."), it->tname().c_str());

 if (!p->has_disease(DI_PKILL1))
  p->add_disease(DI_PKILL1, 120, g);
 else {
  for (int i = 0; i < p->illness.size(); i++) {
   if (p->illness[i].type == DI_PKILL1) {
    p->illness[i].duration = 120;
    i = p->illness.size();
   }
  }
 }
}

// Codeine
void iuse::pkill_2(game *g, player *p, item *it, bool t)
{
 if (!p->is_npc())
  g->add_msg(_("You take some %s."), it->tname().c_str());

 p->add_disease(DI_PKILL2, 180, g);
}

void iuse::pkill_3(game *g, player *p, item *it, bool t)
{
 if (!p->is_npc())
  g->add_msg(_("You take some %s."), it->tname().c_str());

 p->add_disease(DI_PKILL3, 20, g);
 p->add_disease(DI_PKILL2, 200, g);
}

void iuse::pkill_4(game *g, player *p, item *it, bool t)
{
 if (!p->is_npc())
  g->add_msg(_("You shoot up."));

 p->add_disease(DI_PKILL3, 80, g);
 p->add_disease(DI_PKILL2, 200, g);
}

void iuse::pkill_l(game *g, player *p, item *it, bool t)
{
 if (!p->is_npc())
  g->add_msg(_("You take some %s."), it->tname().c_str());

 p->add_disease(DI_PKILL_L, rng(12, 18) * 300, g);
}

void iuse::xanax(game *g, player *p, item *it, bool t)
{
 if (!p->is_npc())
  g->add_msg(_("You take some %s."), it->tname().c_str());

 if (!p->has_disease(DI_TOOK_XANAX))
  p->add_disease(DI_TOOK_XANAX, 900, g);
 else
  p->add_disease(DI_TOOK_XANAX, 200, g);
}

void iuse::caff(game *g, player *p, item *it, bool t)
{
 it_comest *food = dynamic_cast<it_comest*> (it->type);
 p->fatigue -= food->stim * 3;
}

void iuse::alcohol(game *g, player *p, item *it, bool t)
{
 int duration = 680 - (10 * p->str_max); // Weaker characters are cheap drunks
 if (p->has_trait(PF_LIGHTWEIGHT))
  duration += 300;
 p->pkill += 8;
 p->add_disease(DI_DRUNK, duration, g);
}

void iuse::cig(game *g, player *p, item *it, bool t)
{
 if (!p->is_npc())
  g->add_msg(_("You light a cigarette and smoke it."));
 p->add_disease(DI_CIG, 200, g);
 for (int i = 0; i < p->illness.size(); i++) {
  if (p->illness[i].type == DI_CIG && p->illness[i].duration > 600 &&
      !p->is_npc())
   g->add_msg(_("Ugh, too much smoke... you feel gross."));
 }
}

void iuse::weed(game *g, player *p, item *it, bool t)
{
 if (!p->is_npc())
  g->add_msg(_("Good stuff, man!"));

 int duration = 60;
 if (p->has_trait(PF_LIGHTWEIGHT))
  duration = 90;
 p->hunger += 8;
 if (p->pkill < 15)
  p->pkill += 5;
 p->add_disease(DI_HIGH, duration, g);
}

void iuse::coke(game *g, player *p, item *it, bool t)
{
 if (!p->is_npc())
  g->add_msg(_("You snort a bump."));

 int duration = 21 - p->str_cur;
 if (p->has_trait(PF_LIGHTWEIGHT))
  duration += 20;
 p->hunger -= 8;
 p->add_disease(DI_HIGH, duration, g);
}

void iuse::meth(game *g, player *p, item *it, bool t)
{
 int duration = 10 * (40 - p->str_cur);
 if (p->has_charges(itm_lighter, 1)) {
  if (!p->is_npc())
   g->add_msg(_("You smoke some crystals."));
  duration *= 1.5;
 } else if (!p->is_npc())
  g->add_msg(_("You snort some crystals."));
 if (!p->has_disease(DI_METH))
  duration += 600;
 int hungerpen = (p->str_cur < 10 ? 20 : 30 - p->str_cur);
 p->hunger -= hungerpen;
 p->add_disease(DI_METH, duration, g);
}

void iuse::poison(game *g, player *p, item *it, bool t)
{
 p->add_disease(DI_POISON, 100, g);
}

void iuse::hallu(game *g, player *p, item *it, bool t)
{
 p->add_disease(DI_HALLU, 2400, g);
}

void iuse::thorazine(game *g, player *p, item *it, bool t)
{
 p->fatigue += 15;
 p->rem_disease(DI_HALLU);
 p->rem_disease(DI_VISUALS);
 p->rem_disease(DI_HIGH);
 if (!p->has_disease(DI_DERMATIK))
  p->rem_disease(DI_FORMICATION);
 if (!p->is_npc())
  g->add_msg(_("You feel somewhat sedated."));
}

void iuse::prozac(game *g, player *p, item *it, bool t)
{
 if (!p->has_disease(DI_TOOK_PROZAC) && p->morale_level() < 0)
  p->add_disease(DI_TOOK_PROZAC, 7200, g);
 else
  p->stim += 3;
}

void iuse::sleep(game *g, player *p, item *it, bool t)
{
 p->fatigue += 40;
 if (!p->is_npc())
  g->add_msg(_("You feel very sleepy..."));
}

void iuse::iodine(game *g, player *p, item *it, bool t)
{
 p->add_disease(DI_IODINE, 1200, g);
 if (!p->is_npc())
  g->add_msg(_("You take an iodine tablet."));
}

void iuse::flumed(game *g, player *p, item *it, bool t)
{
 p->add_disease(DI_TOOK_FLUMED, 6000, g);
 if (!p->is_npc())
  g->add_msg(_("You take some %s"), it->tname().c_str());
}

void iuse::flusleep(game *g, player *p, item *it, bool t)
{
 p->add_disease(DI_TOOK_FLUMED, 7200, g);
 p->fatigue += 30;
 if (!p->is_npc())
  g->add_msg(_("You feel very sleepy..."));
}

void iuse::inhaler(game *g, player *p, item *it, bool t)
{
 p->rem_disease(DI_ASTHMA);
 if (!p->is_npc())
  g->add_msg(_("You take a puff from your inhaler."));
}

void iuse::blech(game *g, player *p, item *it, bool t)
{
// TODO: Add more effects?
 if (!p->is_npc())
  g->add_msg(_("Blech, that burns your throat!"));
 p->vomit(g);
}

void iuse::mutagen(game *g, player *p, item *it, bool t)
{
 p->mutate(g);
}

void iuse::mutagen_3(game *g, player *p, item *it, bool t)
{
 p->mutate(g);
 p->mutate(g);
 p->mutate(g);
}

void iuse::purifier(game *g, player *p, item *it, bool t)
{
 std::vector<int> valid;	// Which flags the player has
 for (int i = 1; i < PF_MAX2; i++) {
  if (p->has_trait(pl_flag(i)) && p->has_mutation(pl_flag(i)))
   valid.push_back(i);
 }
 if (valid.size() == 0) {
  g->add_msg(_("You feel cleansed."));
  return;
 }
 int num_cured = rng(1, valid.size());
 if (num_cured > 4)
  num_cured = 4;
 for (int i = 0; i < num_cured && valid.size() > 0; i++) {
  int index = rng(0, valid.size() - 1);
  p->remove_mutation(g, pl_flag(valid[index]) );
  valid.erase(valid.begin() + index);
 }
}

void iuse::marloss(game *g, player *p, item *it, bool t)
{
 if (p->is_npc())
  return;
// If we have the marloss in our veins, we are a "breeder" and will spread
// alien lifeforms.
 if (p->has_trait(PF_MARLOSS)) {
  g->add_msg(_("As you eat the berry, you have a near-religious experience, feeling at one with your surroundings..."));
  p->add_morale(MORALE_MARLOSS, 100, 1000);
  p->hunger = -100;
  monster goo(g->mtypes[mon_blob]);
  goo.friendly = -1;
  int goo_spawned = 0;
  for (int x = p->posx - 4; x <= p->posx + 4; x++) {
   for (int y = p->posy - 4; y <= p->posy + 4; y++) {
    if (rng(0, 10) > trig_dist(x, y, p->posx, p->posy) &&
        rng(0, 10) > trig_dist(x, y, p->posx, p->posy)   )
     g->m.marlossify(x, y);
    if (one_in(10 + 5 * trig_dist(x, y, p->posx, p->posy)) &&
        (goo_spawned == 0 || one_in(goo_spawned * 2))) {
     goo.spawn(x, y);
     g->z.push_back(goo);
     goo_spawned++;
    }
   }
  }
  return;
 }
/* If we're not already carriers of Marloss, roll for a random effect:
 * 1 - Mutate
 * 2 - Mutate
 * 3 - Mutate
 * 4 - Purify
 * 5 - Purify
 * 6 - Cleanse radiation + Purify
 * 7 - Fully satiate
 * 8 - Vomit
 * 9 - Give Marloss mutation
 */
 int effect = rng(1, 9);
 if (effect <= 3) {
  g->add_msg(_("This berry tastes extremely strange!"));
  p->mutate(g);
 } else if (effect <= 6) { // Radiation cleanse is below
  g->add_msg(_("This berry makes you feel better all over."));
  p->pkill += 30;
  this->purifier(g, p, it, t);
 } else if (effect == 7) {
  g->add_msg(_("This berry is delicious, and very filling!"));
  p->hunger = -100;
 } else if (effect == 8) {
  g->add_msg(_("You take one bite, and immediately vomit!"));
  p->vomit(g);
 } else if (!p->has_trait(PF_MARLOSS)) {
  g->add_msg(_("You feel a strange warmth spreading throughout your body..."));
  p->toggle_trait(PF_MARLOSS);
 }
 if (effect == 6)
  p->radiation = 0;
}

void iuse::dogfood(game *g, player *p, item *it, bool t)
{
 int dirx, diry;
 g->draw();
 mvprintw(0, 0, _("Which direction?"));
 get_direction(g, dirx, diry, input());
 if (dirx == -2) {
  g->add_msg(_("Invalid direction."));
  return;
 }
 p->moves -= 15;
 dirx += p->posx;
 diry += p->posy;
 int mon_dex = g->mon_at(dirx,diry);
 if (mon_dex != -1) {
  if (g->z[mon_dex].type->id == mon_dog) {
   g->add_msg(_("The dog seems to like you!"));
   g->z[mon_dex].friendly = -1;
  } else
   g->add_msg(_("The %s seems quit unimpressed!"),g->z[mon_dex].type->name.c_str());
 } else
  g->add_msg(_("You spill the dogfood all over the ground."));

}
  
 

// TOOLS below this point!

void iuse::lighter(game *g, player *p, item *it, bool t)
{
 int dirx, diry;
 g->draw();
 mvprintw(0, 0, _("Light where?"));
 get_direction(g, dirx, diry, input());
 if (dirx == -2) {
  g->add_msg(_("Invalid direction."));
  it->charges++;
  return;
 }
 p->moves -= 15;
 dirx += p->posx;
 diry += p->posy;
 if (g->m.flammable_items_at(dirx, diry)) {
  if (g->m.add_field(g, dirx, diry, fd_fire, 1))
   g->m.field_at(dirx, diry).age = 30;
 } else {
  g->add_msg(_("There's nothing to light there."));
  it->charges++;
 }
}

void iuse::sew(game *g, player *p, item *it, bool t)
{
 char ch = g->inv(_("Repair what?"));
 item* fix = &(p->i_at(ch));
 if (fix == NULL || fix->is_null()) {
  g->add_msg(_("You do not have that item!"));
  it->charges++;
  return;
 }
 if (!fix->is_armor()) {
  g->add_msg(_("That isn't clothing!"));
  it->charges++;
  return;
 }
 if (!fix->made_of(COTTON) && !fix->made_of(WOOL)) {
  g->add_msg(_("Your %s is not made of cotton or wool."), fix->tname().c_str());
  it->charges++;
  return;
 }
 if (fix->damage < 0) {
  g->add_msg(_("Your %s is already enhanced."), fix->tname().c_str());
  it->charges++;
  return;
 } else if (fix->damage == 0) {
  p->moves -= 500;
  p->practice(sk_tailor, 10);
  int rn = dice(4, 2 + p->sklevel[sk_tailor]);
  if (p->dex_cur < 8 && one_in(p->dex_cur))
   rn -= rng(2, 6);
  if (p->dex_cur >= 16 || (p->dex_cur > 8 && one_in(16 - p->dex_cur)))
   rn += rng(2, 6);
  if (p->dex_cur > 16)
   rn += rng(0, p->dex_cur - 16);
  if (rn <= 4) {
   g->add_msg(_("You damage your %s!"), fix->tname().c_str());
   fix->damage++;
  } else if (rn >= 12) {
   g->add_msg(_("You make your %s extra-sturdy."), fix->tname().c_str());
   fix->damage--;
  } else
   g->add_msg(_("You practice your sewing."));
 } else {
  p->moves -= 500;
  p->practice(sk_tailor, 8);
  int rn = dice(4, 2 + p->sklevel[sk_tailor]);
  rn -= rng(fix->damage, fix->damage * 2);
  if (p->dex_cur < 8 && one_in(p->dex_cur))
   rn -= rng(2, 6);
  if (p->dex_cur >= 8 && (p->dex_cur >= 16 || one_in(16 - p->dex_cur)))
   rn += rng(2, 6);
  if (p->dex_cur > 16)
   rn += rng(0, p->dex_cur - 16);

  if (rn <= 4) {
   g->add_msg(_("You damage your %s further!"), fix->tname().c_str());
   fix->damage++;
   if (fix->damage >= 5) {
    g->add_msg(_("You destroy it!"));
    p->i_rem(ch);
   }
  } else if (rn <= 6) {
   g->add_msg(_("You don't repair your %s, but you waste lots of thread."),
              fix->tname().c_str());
   int waste = rng(1, 8);
   if (waste > it->charges)
    it->charges = 0;
   else
    it->charges -= waste;
  } else if (rn <= 8) {
   g->add_msg(_("You repair your %s, but waste lots of thread."),
              fix->tname().c_str());
   fix->damage--;
   int waste = rng(1, 8);
   if (waste > it->charges)
    it->charges = 0;
   else
    it->charges -= waste;
  } else if (rn <= 16) {
   g->add_msg(_("You repair your %s!"), fix->tname().c_str());
   fix->damage--;
  } else {
   g->add_msg(_("You repair your %s completely!"), fix->tname().c_str());
   fix->damage = 0;
  }
 }
}

void iuse::scissors(game *g, player *p, item *it, bool t)
{
 char ch = g->inv(_("Chop up what?"));
 item* cut = &(p->i_at(ch));
 if (cut->type->id == 0) {
  g->add_msg(_("You do not have that item!"));
  return;
 }
 if (cut->type->id == itm_rag) {
  g->add_msg(_("There's no point in cutting a rag."));
  return;
 }
 if (cut->type->id == itm_string_36) {
  p->moves -= 150;
  g->add_msg(_("You cut the string into 6 smaller pieces."));
  item string(g->itypes[itm_string_6], int(g->turn), g->nextinv);
  p->i_rem(ch);
  bool drop = false;
  for (int i = 0; i < 6; i++) {
   int iter = 0;
   while (p->has_item(string.invlet)) {
    string.invlet = g->nextinv;
    g->advance_nextinv();
    iter++;
   }
   if (!drop && (iter == 52 || p->volume_carried() >= p->volume_capacity()))
    drop = true;
   if (drop)
    g->m.add_item(p->posx, p->posy, string);
   else
    p->i_add(string);
  }
  return;
 }
 if (!cut->made_of(COTTON)) {
  g->add_msg(_("You can only slice items made of cotton."));
  return;
 }
 p->moves -= 25 * cut->volume();
 int count = cut->volume();
 if (p->sklevel[sk_tailor] == 0)
  count = rng(0, count);
 else if (p->sklevel[sk_tailor] == 1 && count >= 2)
  count -= rng(0, 2);
 if (dice(3, 3) > p->dex_cur)
  count -= rng(1, 3);

 if (count <= 0) {
  g->add_msg(_("You clumsily cut the %s into useless ribbons."),
             cut->tname().c_str());
  p->i_rem(ch);
  return;
 }
 g->add_msg(_("You slice the %s into %d rag%s."), cut->tname().c_str(), count,
            (count == 1 ? "" : "s"));
 item rag(g->itypes[itm_rag], int(g->turn), g->nextinv);
 p->i_rem(ch);
 bool drop = false;
 for (int i = 0; i < count; i++) {
  int iter = 0;
  while (p->has_item(rag.invlet) && iter < 52) {
   rag.invlet = g->nextinv;
   g->advance_nextinv();
   iter++;
  }
  if (!drop && (iter == 52 || p->volume_carried() >= p->volume_capacity()))
   drop = true;
  if (drop)
   g->m.add_item(p->posx, p->posy, rag);
  else
   p->i_add(rag);
 }
}

void iuse::extinguisher(game *g, player *p, item *it, bool t)
{
 g->draw();
 mvprintz(0, 0, c_red, _("Pick a direction to spray:"));
 int dirx, diry;
 get_direction(g, dirx, diry, input());
 if (dirx == -2) {
  g->add_msg(_("Invalid direction!"));
  it->charges++;
  return;
 }
 p->moves -= 140;
 int x = dirx + p->posx;
 int y = diry + p->posy;
 if (g->m.field_at(x, y).type == fd_fire) {
  g->m.field_at(x, y).density -= rng(2, 3);
  if (g->m.field_at(x, y).density <= 0) {
   g->m.field_at(x, y).density = 1;
   g->m.remove_field(x, y);
  }
 }
 int mondex = g->mon_at(x, y);
 if (mondex != -1) {
  int linet;
  g->z[mondex].moves -= 150;
  if (g->u_see(&(g->z[mondex]), linet))
   g->add_msg(_("The %s is sprayed!"), g->z[mondex].name().c_str());
  if (g->z[mondex].made_of(LIQUID)) {
   if (g->u_see(&(g->z[mondex]), linet))
    g->add_msg(_("The %s is frozen!"), g->z[mondex].name().c_str());
   if (g->z[mondex].hurt(rng(20, 60)))
    g->kill_mon(mondex);
   else
    g->z[mondex].speed /= 2;
  }
 }
 if (g->m.move_cost(x, y) != 0) {
  x += dirx;
  y += diry;
  if (g->m.field_at(x, y).type == fd_fire) {
   g->m.field_at(x, y).density -= rng(0, 1) + rng(0, 1);
   if (g->m.field_at(x, y).density <= 0) {
    g->m.field_at(x, y).density = 1;
    g->m.remove_field(x, y);
   }
  }
 }
}

void iuse::hammer(game *g, player *p, item *it, bool t)
{
 g->draw();
 mvprintz(0, 0, c_red, _("Pick a direction in which to pry:"));
 int dirx, diry;
 get_direction(g, dirx, diry, input());
 if (dirx == -2) {
  g->add_msg(_("Invalid direction!"));
  return;
 }
 dirx += p->posx;
 diry += p->posy;
 int nails = 0, boards = 0;
 ter_id newter;
 switch (g->m.ter(dirx, diry)) {
 case t_window_boarded:
  nails =  8;
  boards = 3;
  newter = t_window_empty;
  break;
 case t_door_boarded:
  nails = 12;
  boards = 3;
  newter = t_door_b;
  break;
 default:
  g->add_msg(_("Hammers can only remove boards from windows and doors."));
  g->add_msg(_("To board up a window or door, press *"));
  return;
 }
 p->moves -= 500;
 item it_nails(g->itypes[itm_nail], 0, g->nextinv);
 it_nails.charges = nails;
 g->m.add_item(p->posx, p->posy, it_nails);
 item board(g->itypes[itm_2x4], 0, g->nextinv);
 for (int i = 0; i < boards; i++)
  g->m.add_item(p->posx, p->posy, board);
 g->m.ter(dirx, diry) = newter;
}
 
void iuse::light_off(game *g, player *p, item *it, bool t)
{
 if (it->charges == 0)
  g->add_msg(_("The flaslight's batteries are dead."));
 else {
  g->add_msg(_("You turn the flashlight on."));
  it->make(g->itypes[itm_flashlight_on]);
  it->active = true;
 }
}
 
void iuse::light_on(game *g, player *p, item *it, bool t)
{
 if (t) {	// Normal use
// Do nothing... game::light_level() handles this
 } else {	// Turning it off
  g->add_msg(_("The flashlight flicks off."));
  it->make(g->itypes[itm_flashlight]);
  it->active = false;
 }
}

void iuse::water_purifier(game *g, player *p, item *it, bool t)
{
 char ch = g->inv(_("Purify what?"));
 if (!p->has_item(ch)) {
  g->add_msg(_("You do not have that idea!"));
  return;
 }
 if (p->i_at(ch).contents.size() == 0) {
  g->add_msg(_("You can only purify water."));
  return;
 }
 item *pure = &(p->i_at(ch).contents[0]);
 if (pure->type->id != itm_water && pure->type->id != itm_salt_water) {
  g->add_msg(_("You can only purify water."));
  return;
 }
 p->moves -= 150;
 pure->make(g->itypes[itm_water]);
 pure->poison = 0;
}

void iuse::two_way_radio(game *g, player *p, item *it, bool t)
{
 WINDOW* w = newwin(6, 36, 9, 5);
 wborder(w, LINE_XOXO, LINE_XOXO, LINE_OXOX, LINE_OXOX,
            LINE_OXXO, LINE_OOXX, LINE_XXOO, LINE_XOOX );
// TODO: More options here.  Thoughts...
//       > Respond to the SOS of an NPC
//       > Report something to a faction
//       > Call another player
 mvwprintz(w, 1, 1, c_white, _("1: Radio a faction for help..."));
 mvwprintz(w, 2, 1, c_white, _("2: Call Acquaitance..."));
 mvwprintz(w, 3, 1, c_white, _("3: General S.O.S."));
 mvwprintz(w, 4, 1, c_white, _("0: Cancel"));
 wrefresh(w);
 char ch = getch();
 if (ch == '1') {
  p->moves -= 300;
  faction* fac = g->list_factions(_("Call for help..."));
  if (fac == NULL) {
   it->charges++;
   return;
  }
  int bonus = 0;
  if (fac->goal == FACGOAL_CIVILIZATION)
   bonus += 2;
  if (fac->has_job(FACJOB_MERCENARIES))
   bonus += 4;
  if (fac->has_job(FACJOB_DOCTORS))
   bonus += 2;
  if (fac->has_value(FACVAL_CHARITABLE))
   bonus += 3;
  if (fac->has_value(FACVAL_LONERS))
   bonus -= 3;
  if (fac->has_value(FACVAL_TREACHERY))
   bonus -= rng(0, 8);
  bonus += fac->respects_u + 3 * fac->likes_u;
  if (bonus >= 25) {
   popup(_("They reply, \"Help is on the way!\""));
   g->add_event(EVENT_HELP, int(g->turn) + fac->response_time(g), fac->id, -1, -1);
   fac->respects_u -= rng(0, 8);
   fac->likes_u -= rng(3, 5);
  } else if (bonus >= -5) {
   popup(_("They reply, \"Sorry, you're on your own!\""));
   fac->respects_u -= rng(0, 5);
  } else {
   popup(_("They reply, \"Hah!  We hope you die!\""));
   fac->respects_u -= rng(1, 8);
  }

 } else if (ch == '2') {	// Call Acquaitance
// TODO: Implement me!
 } else if (ch == '3') {	// General S.O.S.
  p->moves -= 150;
  std::vector<npc*> in_range;
  for (int i = 0; i < g->cur_om.npcs.size(); i++) {
   if (g->cur_om.npcs[i].op_of_u.value >= 4 &&
       trig_dist(g->levx, g->levy, g->cur_om.npcs[i].mapx,
                                   g->cur_om.npcs[i].mapy) <= 30)
    in_range.push_back(&(g->cur_om.npcs[i]));
  }
  if (in_range.size() > 0) {
   npc* coming = in_range[rng(0, in_range.size() - 1)];
   popup(_("A reply!  %s says, \"I'm on my way; give me %d minutes!\""),
         coming->name.c_str(), coming->minutes_to_u(g));
   coming->mission = NPC_MISSION_RESCUE_U;
  } else
   popup(_("No-one seems to reply..."));
 } else
  it->charges++;	// Canceled the call, get our charge back
 werase(w);
 wrefresh(w);
 delwin(w);
 refresh();
}
 
void iuse::radio_off(game *g, player *p, item *it, bool t)
{
 if (it->charges == 0)
  g->add_msg(_("It's dead."));
 else {
  g->add_msg(_("You turn the radio on."));
  it->make(g->itypes[itm_radio_on]);
  it->active = true;
 }
}

void iuse::radio_on(game *g, player *p, item *it, bool t)
{
 if (t) {	// Normal use
  int best_signal = 0;
  std::string message = _("Radio: Kssssssssssssh.");
  for (int k = 0; k < g->cur_om.radios.size(); k++) {
   int signal = g->cur_om.radios[k].strength -
                trig_dist(g->cur_om.radios[k].x, g->cur_om.radios[k].y,
                          g->levx, g->levy);
   if (signal > best_signal) {
    best_signal = signal;
    message = g->cur_om.radios[k].message;
   }
  }
  if (best_signal > 0) {
   for (int j = 0; j < message.length(); j++) {
    if (dice(10, 100) > dice(10, best_signal * 3)) {
     if (!one_in(10))
      message[j] = '#';
     else
      message[j] = char(rng('a', 'z'));
    }
   }

   std::vector<std::string> segments;
   while (message.length() > RADIO_PER_TURN) {
    int spot = message.find_last_of(' ', RADIO_PER_TURN);
    if (spot == std::string::npos)
     spot = RADIO_PER_TURN;
    segments.push_back( message.substr(0, spot) );
    message = message.substr(spot);
   }
   segments.push_back(message);
   int index = g->turn % (segments.size());
   std::stringstream messtream;
   messtream << _("radio: ") << segments[index];
   message = messtream.str();
  }
  point p = g->find_item(it);
  g->sound(p.x, p.y, 6, message.c_str());
 } else {	// Turning it off
  g->add_msg(_("The radio dies."));
  it->make(g->itypes[itm_radio]);
  it->active = false;
 }
}

void iuse::crowbar(game *g, player *p, item *it, bool t)
{
 int dirx, diry;
 g->draw();
 mvprintw(0, 0, _("Pry where?"));
 get_direction(g, dirx, diry, input());
 if (dirx == -2) {
  g->add_msg(_("Invalid direction."));
  return;
 }
 dirx += p->posx;
 diry += p->posy;
 ter_id type = g->m.ter(dirx, diry);
 if (type == t_door_c || type == t_door_locked || type == t_door_locked_alarm) {
  if (dice(4, 6) < dice(4, p->str_cur)) {
   g->add_msg(_("You pry the door open."));
   p->moves -= (150 - (p->str_cur * 5));
   g->m.ter(dirx, diry) = t_door_o;
  } else {
   g->add_msg(_("You pry, but cannot open the door."));
   p->moves -= 100;
  }
 } else if (g->m.ter(dirx, diry) == t_manhole_cover) {
  if (dice(8, 8) < dice(8, p->str_cur)) {
   g->add_msg(_("You lift the manhole cover."));
   p->moves -= (500 - (p->str_cur * 5));
   g->m.ter(dirx, diry) = t_manhole;
   g->m.add_item(p->posx, p->posy, g->itypes[itm_manhole_cover], 0);
  } else {
   g->add_msg(_("You pry, but cannot lift the manhole cover."));
   p->moves -= 100;
  }
 } else if (g->m.ter(dirx, diry) == t_crate_c) {
  if (p->str_cur >= rng(3, 30)) {
   g->add_msg(_("You pop the crate open."));
   p->moves -= (150 - (p->str_cur * 5));
   g->m.ter(dirx, diry) = t_crate_o;
  } else {
   g->add_msg(_("You pry, but cannot open the crate."));
   p->moves -= 100;
  } 
 } else {
  int nails = 0, boards = 0;
  ter_id newter;
  switch (g->m.ter(dirx, diry)) {
  case t_window_boarded:
   nails =  8;
   boards = 3;
   newter = t_window_empty;
   break;
  case t_door_boarded:
   nails = 12;
   boards = 3;
   newter = t_door_b;
   break;
  default:
   g->add_msg(_("There's nothing to pry there."));
   return;
  }
  p->moves -= 500;
  item it_nails(g->itypes[itm_nail], 0, g->nextinv);
  it_nails.charges = nails;
  g->m.add_item(p->posx, p->posy, it_nails);
  item board(g->itypes[itm_2x4], 0, g->nextinv);
  for (int i = 0; i < boards; i++)
   g->m.add_item(p->posx, p->posy, board);
  g->m.ter(dirx, diry) = newter;
 }
}

void iuse::makemound(game *g, player *p, item *it, bool t)
{
 if (g->m.has_flag(diggable, p->posx, p->posy)) {
  g->add_msg(_("You churn up the earth here."));
  p->moves = -300;
  g->m.ter(p->posx, p->posy) = t_dirtmound;
 } else
  g->add_msg(_("You can't churn up this ground."));
}

void iuse::dig(game *g, player *p, item *it, bool t)
{
 g->add_msg(_("You can dig a pit via the construction menu--hit *"));
/*
 int dirx, diry;
 g->draw();
 mvprintw(0, 0, "Dig where?");
 get_direction(g, dirx, diry, input());
 if (dirx == -2) {
  g->add_msg("Invalid direction.");
  return;
 }
 if (g->m.has_flag(diggable, p->posx + dirx, p->posy + diry)) {
  p->moves -= 300;
  g->add_msg("You dig a pit.");
  g->m.ter     (p->posx + dirx, p->posy + diry) = t_pit;
  g->m.add_trap(p->posx + dirx, p->posy + diry, tr_pit);
  p->practice(sk_traps, 1);
 } else
  g->add_msg("You can't dig through %s!",
             g->m.tername(p->posx + dirx, p->posy + diry).c_str());
*/
}

void iuse::chainsaw_off(game *g, player *p, item *it, bool t)
{
 p->moves -= 80;
 if (rng(0, 10) - it->damage > 5 && it->charges > 0) {
  g->sound(p->posx, p->posy, 20,
           _("With a roar, the chainsaw leaps to life!"));
  it->make(g->itypes[itm_chainsaw_on]);
  it->active = true;
 } else
  g->add_msg(_("You yank the cord, but nothing happens."));
}

void iuse::chainsaw_on(game *g, player *p, item *it, bool t)
{
 if (t) {	// Effects while simply on
  if (one_in(15))
   g->sound(p->posx, p->posy, 12, _("Your chainsaw rumbles."));
 } else {	// Toggling
  g->add_msg(_("Your chainsaw dies."));
  it->make(g->itypes[itm_chainsaw_off]);
  it->active = false;
 }
}

void iuse::jackhammer(game *g, player *p, item *it, bool t)
{
 int dirx, diry;
 g->draw();
 mvprintw(0, 0, _("Drill in which direction?"));
 get_direction(g, dirx, diry, input());
 if (dirx == -2) {
  g->add_msg(_("Invalid direction."));
  return;
 }
 dirx += p->posx;
 diry += p->posy;
 if (g->m.is_destructable(dirx, diry)) {
  g->m.destroy(g, dirx, diry, false);
  p->moves -= 500;
  g->sound(dirx, diry, 45, _("TATATATATATATAT!"));
 } else {
  g->add_msg(_("You can't drill there."));
  it->charges += (dynamic_cast<it_tool*>(it->type))->charges_per_use;
 }
}

void iuse::set_trap(game *g, player *p, item *it, bool t)
{
 int dirx, diry;
 g->draw();
 mvprintw(0, 0, _("Place where?"));
 get_direction(g, dirx, diry, input());
 if (dirx == -2) {
  g->add_msg(_("Invalid direction."));
  return;
 }
 int posx = dirx + p->posx;
 int posy = diry + p->posy;
 if (g->m.move_cost(posx, posy) != 2) {
  g->add_msg(_("You can't place a %s there."), it->tname().c_str());
  return;
 }

 trap_id type = tr_null;
 bool buried = false;
 std::stringstream message;
 int practice;

 switch (it->type->id) {
 case itm_boobytrap:
  message << _("You set the boobytrap up and activate the grenade.");
  type = tr_boobytrap;
  practice = 4;
  break;
 case itm_bubblewrap:
  message << _("You set the bubblewrap on the ground, ready to be popped.");
  type = tr_bubblewrap;
  practice = 2;
  break;
 case itm_beartrap:
  buried = (p->has_amount(itm_shovel, 1) &&
            g->m.has_flag(diggable, posx, posy) &&
            query_yn(_("Bury the beartrap?")));
  type = (buried ? tr_beartrap_buried : tr_beartrap);
  message << _("You ") << (buried ? _("bury") : _("set")) << _(" the beartrap.");
  practice = (buried ? 7 : 4); 
  break;
 case itm_board_trap:
  message << _("You set the board trap on the ") << g->m.tername(posx, posy) <<
             _(", nails facing up.");
  type = tr_nailboard;
  practice = 2;
  break;
 case itm_tripwire:
// Must have a connection between solid squares.
  if ((g->m.move_cost(posx    , posy - 1) != 2 &&
       g->m.move_cost(posx    , posy + 1) != 2   ) ||
      (g->m.move_cost(posx + 1, posy    ) != 2 &&
       g->m.move_cost(posx - 1, posy    ) != 2   ) ||
      (g->m.move_cost(posx - 1, posy - 1) != 2 &&
       g->m.move_cost(posx + 1, posy + 1) != 2   ) ||
      (g->m.move_cost(posx + 1, posy - 1) != 2 &&
       g->m.move_cost(posx - 1, posy + 1) != 2   )) {
   message << _("You string up the tripwire.");
   type= tr_tripwire;
   practice = 3;
  } else {
   g->add_msg(_("You must place the tripwire between two solid tiles."));
   return;
  }
  break;
 case itm_crossbow_trap:
  message << _("You set the crossbow trap.");
  type = tr_crossbow;
  practice = 4;
  break;
 case itm_shotgun_trap:
  message << _("You set the shotgun trap.");
  type = tr_shotgun_2;
  practice = 5;
  break;
 case itm_blade_trap:
  posx += dirx;
  posy += diry;
  for (int i = -1; i <= 1; i++) {
   for (int j = -1; j <= 1; j++) {
    if (g->m.move_cost(posx + i, posy + j) != 2) {
     g->add_msg(_("\
That trap needs a 3x3 space to be clear, centered two tiles from you."));
     return;
    }
   }
  }
  message << _("You set the blade trap two squares away.");
  type = tr_engine;
  practice = 12;
  break;
 case itm_landmine:
  buried = true;
  message << _("You bury the landmine.");
  type = tr_landmine;
  practice = 7;
  break;
 default:
  g->add_msg(_("Tried to set a trap.  But got confused! %s"), it->tname().c_str());
  return;
 }

 if (buried) {
  if (!p->has_amount(itm_shovel, 1)) {
   g->add_msg(_("You need a shovel."));
   return;
  } else if (!g->m.has_flag(diggable, posx, posy)) {
   g->add_msg(_("You can't dig in that %s"), g->m.tername(posx, posy).c_str());
   return;
  }
 }

 g->add_msg(message.str().c_str());
 p->practice(sk_traps, practice);
 g->m.add_trap(posx, posy, type);
 p->moves -= practice * 25;
 if (type == tr_engine) {
  for (int i = -1; i <= 1; i++) {
   for (int j = -1; j <= 1; j++) {
    if (i != 0 || j != 0)
     g->m.add_trap(posx + i, posy + j, tr_blade);
   }
  }
 }
 it->invlet = 0; // Remove the trap from the player's inv
}

void iuse::geiger(game *g, player *p, item *it, bool t)
{
 if (t) { // Every-turn use when it's on
  int rads = g->m.radiation(p->posx, p->posy);
  if (rads == 0)
   return;
  g->sound(p->posx, p->posy, 6, "");
  if (rads > 50)
   g->add_msg(_("The geiger counter buzzes intensely."));
  else if (rads > 35)
   g->add_msg(_("The geiger counter clicks wildly."));
  else if (rads > 25)
   g->add_msg(_("The geiger counter clicks rapidly."));
  else if (rads > 15)
   g->add_msg(_("The geiger counter clicks steadily."));
  else if (rads > 8)
   g->add_msg(_("The geiger counter clicks slowly."));
  else if (rads > 4)
   g->add_msg(_("The geiger counter clicks intermittantly."));
  else
   g->add_msg(_("The geiger counter clicks once."));
  return;
 }
// Otherwise, we're activating the geiger counter
 it_tool *type = dynamic_cast<it_tool*>(it->type);
 bool is_on = (type->id == itm_geiger_on);
 if (is_on) {
  g->add_msg(_("The geiger counter's SCANNING LED flicks off."));
  it->make(g->itypes[itm_geiger_off]);
  it->active = false;
  return;
 }
 std::string toggle_text = _("Turn continuous scan ");
 toggle_text += (is_on ? _("off") : _("on"));
 int ch = menu(_("Geiger counter:"), _("Scan yourself"), _("Scan the ground"),
               toggle_text.c_str(), _("Cancel"), NULL);
 switch (ch) {
  case 1: g->add_msg(_("Your radiation level: %d"), p->radiation); break;
  case 2: g->add_msg(_("The ground's radiation level: %d"),
                     g->m.radiation(p->posx, p->posy));		break;
  case 3:
   g->add_msg(_("The geiger counter's scan LED flicks on."));
   it->make(g->itypes[itm_geiger_on]);
   it->active = true;
   break;
  case 4:
   it->charges++;
   break;
 }
}

void iuse::teleport(game *g, player *p, item *it, bool t)
{
 p->moves -= 100;
 g->teleport(p);
}

void iuse::can_goo(game *g, player *p, item *it, bool t)
{
 int tries = 0, goox, gooy, junk;
 do {
  goox = p->posx + rng(-2, 2);
  gooy = p->posy + rng(-2, 2);
  tries++;
 } while (g->m.move_cost(goox, gooy) == 0 && tries < 10);
 if (tries == 10)
  return;
 int mondex = g->mon_at(goox, gooy);
 if (mondex != -1) {
  if (g->u_see(goox, gooy, junk))
   g->add_msg(_("Black goo emerges from the canister and envelopes a %s!"),
              g->z[mondex].name().c_str());
  g->z[mondex].poly(g->mtypes[mon_blob]);
  g->z[mondex].speed -= rng(5, 25);
  g->z[mondex].hp = g->z[mondex].speed;
 } else {
  if (g->u_see(goox, gooy, junk))
   g->add_msg(_("Living black goo emerges from the canister!"));
  monster goo(g->mtypes[mon_blob]);
  goo.friendly = -1;
  goo.spawn(goox, gooy);
  g->z.push_back(goo);
 }
 tries = 0;
 while (!one_in(4) && tries < 10) {
  tries = 0;
  do {
   goox = p->posx + rng(-2, 2);
   gooy = p->posy + rng(-2, 2);
   tries++;
  } while (g->m.move_cost(goox, gooy) == 0 &&
           g->m.tr_at(goox, gooy) == tr_null && tries < 10);
  if (tries < 10) {
   if (g->u_see(goox, gooy, junk))
    g->add_msg(_("A nearby splatter of goo forms into a goo pit."));
   g->m.tr_at(goox, gooy) = tr_goo;
  }
 }
}

void iuse::pipebomb(game *g, player *p, item *it, bool t)
{
 if (!p->has_charges(itm_lighter, 1)) {
  g->add_msg(_("You need a lighter!"));
  return;
 }
 p->use_charges(itm_lighter, 1);
 g->add_msg(_("You light the fuse on the pipe bomb."));
 it->make(g->itypes[itm_pipebomb_act]);
 it->charges = 3;
 it->active = true;
}

void iuse::pipebomb_act(game *g, player *p, item *it, bool t)
{
 int linet;
 point pos = g->find_item(it);
 if (pos.x == -999 || pos.y == -999)
  return;
 if (t) // Simple timer effects
  g->sound(pos.x, pos.y, 0, _("Ssssss"));	// Vol 0 = only heard if you hold it
 else {	// The timer has run down
  if (one_in(10) && g->u_see(pos.x, pos.y, linet))
   g->add_msg(_("The pipe bomb fizzles out."));
  else
   g->explosion(pos.x, pos.y, rng(6, 14), rng(0, 4), false);
 }
}
 
void iuse::grenade(game *g, player *p, item *it, bool t)
{
 g->add_msg(_("You pull the pin on the grenade."));
 it->make(g->itypes[itm_grenade_act]);
 it->charges = 5;
 it->active = true;
}

void iuse::grenade_act(game *g, player *p, item *it, bool t)
{
 point pos = g->find_item(it);
 if (pos.x == -999 || pos.y == -999)
  return;
 if (t) // Simple timer effects
  g->sound(pos.x, pos.y, 0, _("Tick."));	// Vol 0 = only heard if you hold it
 else	// When that timer runs down...
  g->explosion(pos.x, pos.y, 12, 28, false);
}

void iuse::flashbang(game *g, player *p, item *it, bool t)
{
 g->add_msg(_("You pull the pin on the flashbang."));
 it->make(g->itypes[itm_flashbang_act]);
 it->charges = 5;
 it->active = true;
}

void iuse::flashbang_act(game *g, player *p, item *it, bool t)
{
 point pos = g->find_item(it);
 if (pos.x == -999 || pos.y == -999)
  return;
 if (t) // Simple timer effects
  g->sound(pos.x, pos.y, 0, _("Tick."));	// Vol 0 = only heard if you hold it
 else	// When that timer runs down...
  g->flashbang(pos.x, pos.y);
}

void iuse::c4(game *g, player *p, item *it, bool t)
{
 int time = query_int(_("Set the timer to (0 to cancel)?"));
 if (time == 0) {
  g->add_msg(_("Never mind."));
  return;
 }
 g->add_msg(_("You set the timer to %d."), time);
 it->make(g->itypes[itm_c4armed]);
 it->charges = time;
 it->active = true;
}

void iuse::c4armed(game *g, player *p, item *it, bool t)
{
 point pos = g->find_item(it);
 if (pos.x == -999 || pos.y == -999)
  return;
 if (t) // Simple timer effects
  g->sound(pos.x, pos.y, 0, _("Tick."));	// Vol 0 = only heard if you hold it
 else	// When that timer runs down...
  g->explosion(pos.x, pos.y, 40, 3, false);
}

void iuse::EMPbomb(game *g, player *p, item *it, bool t)
{
 g->add_msg(_("You pull the pin on the EMP grenade."));
 it->make(g->itypes[itm_EMPbomb_act]);
 it->charges = 3;
 it->active = true;
}

void iuse::EMPbomb_act(game *g, player *p, item *it, bool t)
{
 point pos = g->find_item(it);
 if (pos.x == -999 || pos.y == -999)
  return;
 if (t)	// Simple timer effects
  g->sound(pos.x, pos.y, 0, _("Tick."));	// Vol 0 = only heard if you hold it
 else {	// When that timer runs down...
  for (int x = pos.x - 4; x <= pos.x + 4; x++) {
   for (int y = pos.y - 4; y <= pos.y + 4; y++)
    g->emp_blast(x, y);
  }
 }
}

void iuse::gasbomb(game *g, player *p, item *it, bool t)
{
 g->add_msg(_("You pull the pin on the teargas canister."));
 it->make(g->itypes[itm_gasbomb_act]);
 it->charges = 20;
 it->active = true;
}

void iuse::gasbomb_act(game *g, player *p, item *it, bool t)
{
 point pos = g->find_item(it);
 if (pos.x == -999 || pos.y == -999)
  return;
 if (t) {
  if (it->charges > 15)
   g->sound(pos.x, pos.y, 0, _("Tick."));	// Vol 0 = only heard if you hold it
  else {
   int junk;
   for (int i = -2; i <= 2; i++) {
    for (int j = -2; j <= 2; j++) {
     if (g->m.sees(pos.x, pos.y, pos.x + i, pos.y + j, 3, junk) &&
         g->m.move_cost(pos.x + i, pos.y + j) > 0)
      g->m.add_field(g, pos.x + i, pos.y + j, fd_tear_gas, 3);
    }
   }
  }
 } else
  it->make(g->itypes[itm_canister_empty]);
}

void iuse::smokebomb(game *g, player *p, item *it, bool t)
{
 g->add_msg(_("You pull the pin on the smoke bomb."));
 it->make(g->itypes[itm_smokebomb_act]);
 it->charges = 20;
 it->active = true;
}

void iuse::smokebomb_act(game *g, player *p, item *it, bool t)
{
 point pos = g->find_item(it);
 if (pos.x == -999 || pos.y == -999)
  return;
 if (t) {
  if (it->charges > 17)
   g->sound(pos.x, pos.y, 0, _("Tick."));	// Vol 0 = only heard if you hold it
  else {
   int junk;
   for (int i = -2; i <= 2; i++) {
    for (int j = -2; j <= 2; j++) {
     if (g->m.sees(pos.x, pos.y, pos.x + i, pos.y + j, 3, junk) &&
         g->m.move_cost(pos.x + i, pos.y + j) > 0)
      g->m.add_field(g, pos.x + i, pos.y + j, fd_smoke, rng(1, 2) + rng(0, 1));
    }
   }
  }
 } else
  it->make(g->itypes[itm_canister_empty]);
}

void iuse::acidbomb(game *g, player *p, item *it, bool t)
{
 g->add_msg(_("You remove the divider, and the chemicals mix."));
 p->moves -= 150;
 it->make(g->itypes[itm_acidbomb_act]);
 it->charges = 1;
 it->bday = int(g->turn);
 it->active = true;
}
 
void iuse::acidbomb_act(game *g, player *p, item *it, bool t)
{
 if (!p->has_item(it)) {
  point pos = g->find_item(it);
  if (pos.x == -999)
   pos = point(p->posx, p->posy);
  it->charges = 0;
  for (int x = pos.x - 1; x <= pos.x + 1; x++) {
   for (int y = pos.y - 1; y <= pos.y + 1; y++)
    g->m.add_field(g, x, y, fd_acid, 3);
  }
 }
}

void iuse::molotov(game *g, player *p, item *it, bool t)
{
 if (!p->has_charges(itm_lighter, 1)) {
  g->add_msg(_("You need a lighter!"));
  return;
 }
 p->use_charges(itm_lighter, 1);
 g->add_msg(_("You light the molotov cocktail."));
 p->moves -= 150;
 it->make(g->itypes[itm_molotov_lit]);
 it->charges = 1;
 it->bday = int(g->turn);
 it->active = true;
}
 
void iuse::molotov_lit(game *g, player *p, item *it, bool t)
{
 int age = int(g->turn) - it->bday;
 if (!p->has_item(it)) {
  point pos = g->find_item(it);
  it->charges = -1;
  g->explosion(pos.x, pos.y, 8, 0, true);
 } else if (age >= 5) { // More than 5 turns old = chance of going out
  if (rng(1, 50) < age) {
   g->add_msg(_("Your lit molotov goes out."));
   it->make(g->itypes[itm_molotov]);
   it->charges = 0;
   it->active = false;
  }
 }
}

void iuse::dynamite(game *g, player *p, item *it, bool t)
{
 if (!p->has_charges(itm_lighter, 1)) {
  g->add_msg(_("You need a lighter!"));
  return;
 }
 p->use_charges(itm_lighter, 1);
 g->add_msg(_("You light the dynamite."));
 it->make(g->itypes[itm_dynamite_act]);
 it->charges = 20;
 it->active = true;
}

void iuse::dynamite_act(game *g, player *p, item *it, bool t)
{
 point pos = g->find_item(it);
 if (pos.x == -999 || pos.y == -999)
  return;
 if (t) // Simple timer effects
  g->sound(pos.x, pos.y, 0, _("ssss..."));
 else	// When that timer runs down...
  g->explosion(pos.x, pos.y, 60, 0, false);
}

void iuse::mininuke(game *g, player *p, item *it, bool t)
{
 g->add_msg(_("You activate the mininuke."));
 it->make(g->itypes[itm_mininuke_act]);
 it->charges = 10;
 it->active = true;
}

void iuse::mininuke_act(game *g, player *p, item *it, bool t)
{
 point pos = g->find_item(it);
 if (pos.x == -999 || pos.y == -999)
  return;
 if (t) 	// Simple timer effects
  g->sound(pos.x, pos.y, 2, _("Tick."));
 else {	// When that timer runs down...
  g->explosion(pos.x, pos.y, 200, 0, false);
  int junk;
  for (int i = -4; i <= 4; i++) {
   for (int j = -4; j <= 4; j++) {
    if (g->m.sees(pos.x, pos.y, pos.x + i, pos.y + j, 3, junk) &&
        g->m.move_cost(pos.x + i, pos.y + j) > 0)
     g->m.add_field(g, pos.x + i, pos.y + j, fd_nuke_gas, 3);
   }
  }
 }
}

void iuse::pheromone(game *g, player *p, item *it, bool t)
{
 point pos(p->posx, p->posy);

 int junk;
 bool is_u = !p->is_npc(), can_see = (is_u || g->u_see(p->posx, p->posy, junk));
 if (pos.x == -999 || pos.y == -999)
  return;

 if (is_u)
  g->add_msg(_("You squeeze the pheromone ball..."));
 else if (can_see)
  g->add_msg(_("%s squeezes a pheromone ball..."), p->name.c_str());
 p->moves -= 15;

 int converts = 0;
 for (int x = pos.x - 4; x <= pos.x + 4; x++) {
  for (int y = pos.y - 4; y <= pos.y + 4; y++) {
   int mondex = g->mon_at(x, y);
   if (mondex != -1 && g->z[mondex].symbol() == 'Z' &&
       g->z[mondex].friendly == 0 && rng(0, 500) > g->z[mondex].hp) {
    converts++;
    g->z[mondex].make_friendly();
   }
  }
 }

 if (can_see) {
  if (converts == 0)
   g->add_msg(_("...but nothing happens."));
  else if (converts == 1)
   g->add_msg(_("...and a nearby zombie turns friendly!"));
  else
   g->add_msg(_("...and several nearby zombies turn friendly!"));
 }
}
 

void iuse::portal(game *g, player *p, item *it, bool t)
{
 g->m.add_trap(p->posx + rng(-2, 2), p->posy + rng(-2, 2), tr_portal);
}

void iuse::manhack(game *g, player *p, item *it, bool t)
{
 std::vector<point> valid;	// Valid spawn locations
 for (int x = p->posx - 1; x <= p->posx + 1; x++) {
  for (int y = p->posy - 1; y <= p->posy + 1; y++) {
   if (g->is_empty(x, y))
    valid.push_back(point(x, y));
  }
 }
 if (valid.size() == 0) {	// No valid points!
  g->add_msg(_("There is no adjacent square to release the manhack in!"));
  return;
 }
 int index = rng(0, valid.size() - 1);
 p->moves -= 60;
 it->invlet = 0; // Remove the manhack from the player's inv
 monster manhack(g->mtypes[mon_manhack], valid[index].x, valid[index].y);
 if (rng(0, p->int_cur / 2) + p->sklevel[sk_electronics] / 2 +
     p->sklevel[sk_computer] < rng(0, 4))
  g->add_msg(_("You misprogram the manhack; it's hostile!"));
 else
  manhack.friendly = -1;
 g->z.push_back(manhack);
}

void iuse::turret(game *g, player *p, item *it, bool t)
{
 int dirx, diry;
 g->draw();
 mvprintw(0, 0, _("Place where?"));
 get_direction(g, dirx, diry, input());
 if (dirx == -2) {
  g->add_msg(_("Invalid direction."));
  return;
 }
 p->moves -= 100;
 dirx += p->posx;
 diry += p->posy;
 if (!g->is_empty(dirx, diry)) {
  g->add_msg(_("You cannot place a turret there."));
  return;
 }
 it->invlet = 0; // Remove the turret from the player's inv
 monster turret(g->mtypes[mon_turret], dirx, diry);
 if (rng(0, p->int_cur / 2) + p->sklevel[sk_electronics] / 2 +
     p->sklevel[sk_computer] < rng(0, 6))
  g->add_msg(_("You misprogram the turret; it's hostile!"));
 else
  turret.friendly = -1;
 g->z.push_back(turret);
}

void iuse::UPS_off(game *g, player *p, item *it, bool t)
{
 if (it->charges == 0)
  g->add_msg(_("The power supply's batteries are dead."));
 else {
  g->add_msg(_("You turn the power supply on."));
  if (p->is_wearing(itm_goggles_nv))
   g->add_msg(_("Your light amp goggles power on."));
  it->make(g->itypes[itm_UPS_on]);
  it->active = true;
 }
}
 
void iuse::UPS_on(game *g, player *p, item *it, bool t)
{
 if (t) {	// Normal use
	// Does nothing
 } else {	// Turning it off
  g->add_msg(_("The UPS powers off with a soft hum."));
  it->make(g->itypes[itm_UPS_off]);
  it->active = false;
 }
}

void iuse::tazer(game *g, player *p, item *it, bool t)
{
 int dirx, diry;
 g->draw();
 mvprintw(0, 0, _("Shock in which direction?"));
 get_direction(g, dirx, diry, input());
 if (dirx == -2) {
  g->add_msg(_("Invalid direction."));
  it->charges += (dynamic_cast<it_tool*>(it->type))->charges_per_use;
  return;
 }
 int sx = dirx + p->posx, sy = diry + p->posy;
 int mondex = g->mon_at(sx, sy);
 int npcdex = g->npc_at(sx, sy);
 if (mondex == -1 && npcdex == -1) {
  g->add_msg(_("Your tazer crackles in the air."));
  return;
 }

 int numdice = 3 + (p->dex_cur / 2.5) + p->sklevel[sk_melee] * 2;
 p->moves -= 100;

 if (mondex != -1) {
  monster *z = &(g->z[mondex]);
  switch (z->type->size) {
   case MS_TINY:  numdice -= 2; break;
   case MS_SMALL: numdice -= 1; break;
   case MS_LARGE: numdice += 2; break;
   case MS_HUGE:  numdice += 4; break;
  }
  int mondice = z->dodge();
  if (dice(numdice, 10) < dice(mondice, 10)) {	// A miss!
   g->add_msg(_("You attempt to shock the %s, but miss."), z->name().c_str());
   return;
  }
  g->add_msg(_("You shock the %s!"), z->name().c_str());
  int shock = rng(5, 25);
  z->moves -= shock * 100;
  if (z->hurt(shock))
   g->kill_mon(mondex);
  return;
 }
 
 if (npcdex != -1) {
  npc *foe = dynamic_cast<npc*>(&g->active_npc[npcdex]);
  if (foe->attitude != NPCATT_FLEE)
   foe->attitude = NPCATT_KILL;
  if (foe->str_max >= 17)
    numdice++;	// Minor bonus against huge people
  else if (foe->str_max <= 5)
   numdice--;	// Minor penalty against tiny people
  if (dice(numdice, 10) <= dice(foe->dodge(g), 6)) {
   g->add_msg(_("You attempt to shock %s, but miss."), foe->name.c_str());
   return;
  }
  g->add_msg(_("You shock %s!"), foe->name.c_str());
  int shock = rng(5, 20);
  foe->moves -= shock * 100;
  foe->hurtall(shock);
  if (foe->hp_cur[hp_head]  <= 0 || foe->hp_cur[hp_torso] <= 0) {
   foe->die(g, true);
   g->active_npc.erase(g->active_npc.begin() + npcdex);
  }
 }

}

void iuse::mp3(game *g, player *p, item *it, bool t)
{
 if (it->charges == 0)
  g->add_msg(_("The mp3 player's batteries are dead."));
 else if (p->has_active_item(itm_mp3_on))
  g->add_msg(_("You are already listening to an mp3 player!"));
 else {
  g->add_msg(_("You put in the earbuds and start listening to music."));
  it->make(g->itypes[itm_mp3_on]);
  it->active = true;
 }
}

void iuse::mp3_on(game *g, player *p, item *it, bool t)
{
 if (t) {	// Normal use
  if (!p->has_item(it))
   return;	// We're not carrying it!
  p->add_morale(MORALE_MUSIC, 1, 50);

  if (int(g->turn) % 10 == 0) {	// Every 10 turns, describe the music
   std::string sound = "";
   switch (rng(1, 10)) {
    case 1: sound = _("a sweet guitar solo!");	p->stim++;	break;
    case 2: sound = _("a funky bassline.");			break;
    case 3: sound = _("some amazing vocals.");			break;
    case 4: sound = _("some pumping bass.");			break;
    case 5: sound = _("dramatic classical music.");
            if (p->int_cur >= 10)
             p->add_morale(MORALE_MUSIC, 1, 100);		break;
   }
   if (sound.length() > 0)
    g->add_msg(_("You listen to %s"), sound.c_str());
  }
 } else {	// Turning it off
  g->add_msg(_("The mp3 player turns off."));
  it->make(g->itypes[itm_mp3]);
  it->active = false;
 }
}

void iuse::vortex(game *g, player *p, item *it, bool t)
{
 std::vector<point> spawn;
 for (int i = -3; i <= 3; i++) {
  if (g->is_empty(p->posx - 3, p->posy + i))
   spawn.push_back( point(p->posx - 3, p->posy + i) );
  if (g->is_empty(p->posx + 3, p->posy + i))
   spawn.push_back( point(p->posx + 3, p->posy + i) );
  if (g->is_empty(p->posx + i, p->posy - 3))
   spawn.push_back( point(p->posx + i, p->posy - 3) );
  if (g->is_empty(p->posx + i, p->posy + 3))
   spawn.push_back( point(p->posx + i, p->posy + 3) );
 }
 if (spawn.empty()) {
  if (!p->is_npc())
   g->add_msg(_("Air swirls around you for a moment."));
  it->make(g->itypes[itm_spiral_stone]);
  return;
 }

 g->add_msg(_("Air swirls all over..."));
 int index = rng(0, spawn.size() - 1);
 p->moves -= 100;
 it->make(g->itypes[itm_spiral_stone]);
 monster vortex(g->mtypes[mon_vortex], spawn[index].x, spawn[index].y);
 vortex.friendly = -1;
 g->z.push_back(vortex);
}

void iuse::dog_whistle(game *g, player *p, item *it, bool t)
{
 if (!p->is_npc())
  g->add_msg(_("You blow your dog whistle."));
 for (int i = 0; i < g->z.size(); i++) {
  if (g->z[i].friendly != 0 && g->z[i].type->id == mon_dog) {
   int linet;
   bool u_see = g->u_see(&(g->z[i]), linet);
   if (g->z[i].has_effect(ME_DOCILE)) {
    if (u_see)
     g->add_msg(_("Your %s looks ready to attack."), g->z[i].name().c_str());
    g->z[i].rem_effect(ME_DOCILE);
   } else {
    if (u_see)
     g->add_msg(_("Your %s goes docile."), g->z[i].name().c_str());
    g->z[i].add_effect(ME_DOCILE, -1);
   }
  }
 }
}

void iuse::vacutainer(game *g, player *p, item *it, bool t)
{
 if (p->is_npc())
  return; // No NPCs for now!

 if (!it->contents.empty()) {
  g->add_msg(_("That %s is full!"), it->tname().c_str());
  return;
 }

 item blood(g->itypes[itm_blood], g->turn);
 bool drew_blood = false;
 for (int i = 0; i < g->m.i_at(p->posx, p->posy).size() && !drew_blood; i++) {
  item *it = &(g->m.i_at(p->posx, p->posy)[i]);
  if (it->type->id == itm_corpse &&
      query_yn(_("Draw blood from %s?"), it->tname().c_str())) {
   blood.corpse = it->corpse;
   drew_blood = true;
  }
 }

 if (!drew_blood && query_yn(_("Draw your own blood?")))
  drew_blood = true;

 if (!drew_blood)
  return;

 it->put_in(blood);
}
 

/* MACGUFFIN FUNCTIONS
 * These functions should refer to it->associated_mission for the particulars
 */
void iuse::mcg_note(game *g, player *p, item *it, bool t)
{
 std::stringstream message;
 message << _("Dear ") << it->name << ":\n";
/*
 faction* fac = NULL;
 direction dir = NORTH;
// Pick an associated faction
 switch (it->associated_mission) {
 case MISSION_FIND_FAMILY_FACTION:
  fac = &(g->factions[rng(0, g->factions.size() - 1)]);
  break;
 case MISSION_FIND_FAMILY_KIDNAPPER:
  fac = g->random_evil_faction();
  break;
 }
// Calculate where that faction is
 if (fac != NULL) {
  int omx = g->cur_om.posx, omy = g->cur_om.posy;
  if (fac->omx != g->cur_om.posx || fac->omx != g->cur_om.posy)
   dir = direction_from(omx, omy, fac->omx, fac->omy);
  else
   dir = direction_from(g->levx, g->levy, fac->mapx, fac->mapy);
 }
// Produce the note and generate the next mission
 switch (it->associated_mission) {
 case MISSION_FIND_FAMILY_FACTION:
  if (fac->name == "The army")
   message << "\
I've been rescued by an army patrol.  They're taking me\n\
to their outpost to the " << direction_name(dir) << ".\n\
Please meet me there.  I need to know you're alright.";
  else
   message << "\
This group came through, looking for survivors.  They\n\
said they were members of this group calling itself\n" << fac->name << ".\n\
They've got a settlement to the " << direction_name(dir) << ", so\n\
I guess I'm heading there.  Meet me there as soon as\n\
you can, I need to know you're alright.";
  break;


  popup(message.str().c_str());
*/
}

void iuse::artifact(game *g, player *p, item *it, bool t)
{
 if (!it->is_artifact()) {
  debugmsg("iuse::artifact called on a non-artifact item! %s",
           it->tname().c_str());
  return;
 } else if (!it->is_tool()) {
  debugmsg("iuse::artifact called on a non-tool artifact! %s",
           it->tname().c_str());
  return;
 }
 it_artifact_tool *art = dynamic_cast<it_artifact_tool*>(it->type);
 int num_used = rng(1, art->effects_activated.size());
 if (num_used < art->effects_activated.size())
  num_used += rng(1, art->effects_activated.size() - num_used);

 std::vector<art_effect_active> effects = art->effects_activated;
 for (int i = 0; i < num_used; i++) {
  int index = rng(0, effects.size() - 1);
  art_effect_active used = effects[index];
  effects.erase(effects.begin() + index);

  switch (used) {
  case AEA_STORM: {
   g->sound(p->posx, p->posy, 10, _("Ka-BOOM!"));
   int num_bolts = rng(2, 4);
   for (int j = 0; j < num_bolts; j++) {
    int xdir = 0, ydir = 0;
    while (xdir == 0 && ydir == 0) {
     xdir = rng(-1, 1);
     ydir = rng(-1, 1);
    }
    int dist = rng(4, 12);
    int boltx = p->posx, bolty = p->posy;
    for (int n = 0; n < dist; n++) {
     boltx += xdir;
     bolty += ydir;
     g->m.add_field(g, boltx, bolty, fd_electricity, rng(2, 3));
     if (one_in(4)) {
      if (xdir == 0)
       xdir = rng(0, 1) * 2 - 1;
      else
       xdir = 0;
     }
     if (one_in(4)) {
      if (ydir == 0)
       ydir = rng(0, 1) * 2 - 1;
      else
       ydir = 0;
     }
    }
   }
  } break;

  case AEA_FIREBALL: {
   point fireball = g->look_around();
   if (fireball.x != -1 && fireball.y != -1)
    g->explosion(fireball.x, fireball.y, 8, 0, true);
  } break;

  case AEA_ADRENALINE:
   g->add_msg(_("You're filled with a roaring energy!"));
   p->add_disease(DI_ADRENALINE, rng(200, 250), g);
   break;

  case AEA_MAP: {
   bool new_map = false;
   for (int x = int(g->levx / 2) - 20; x <= int(g->levx / 2) + 20; x++) {
    for (int y = int(g->levy / 2) - 20; y <= int(g->levy / 2) + 20; y++) {
     if (!g->cur_om.seen(x, y)) {
      new_map = true;
      g->cur_om.seen(x, y) = true;
     }
    }
   }
   if (new_map) {
    g->add_msg(_("You have a vision of the surrounding area..."));
    p->moves -= 100;
   }
  } break;

  case AEA_BLOOD: {
   bool blood = false;
   int j;
   for (int x = p->posx - 4; x <= p->posx + 4; x++) {
    for (int y = p->posy - 4; y <= p->posy + 4; y++) {
     if (!one_in(4) && g->m.add_field(g, x, y, fd_blood, 3) &&
         (blood || g->u_see(x, y, j)))
      blood = true;
    }
   }
   if (blood)
    g->add_msg(_("Blood soaks out of the ground and walls."));
  } break;

  case AEA_FATIGUE: {
   g->add_msg(_("The fabric of space seems to decay."));
   int x = rng(p->posx - 3, p->posx + 3), y = rng(p->posy - 3, p->posy + 3);
   if (g->m.field_at(x, y).type == fd_fatigue &&
       g->m.field_at(x, y).density < 3)
    g->m.field_at(x, y).density++;
   else
    g->m.add_field(g, x, y, fd_fatigue, rng(1, 2));
  } break;

  case AEA_ACIDBALL: {
   point acidball = g->look_around();
   if (acidball.x != -1 && acidball.y != -1) {
    for (int x = acidball.x - 1; x <= acidball.x + 1; x++) {
     for (int y = acidball.y - 1; y <= acidball.y + 1; y++) {
      if (g->m.field_at(x, y).type == fd_acid &&
          g->m.field_at(x, y).density < 3)
       g->m.field_at(x, y).density++;
      else
       g->m.add_field(g, x, y, fd_acid, rng(2, 3));
     }
    }
   }
  } break;

  case AEA_PULSE:
   g->sound(p->posx, p->posy, 30, _("The earth shakes!"));
   for (int x = p->posx - 2; x <= p->posx + 2; x++) {
    for (int y = p->posy - 2; y <= p->posy + 2; y++) {
     std::string junk;
     g->m.bash(x, y, 40, junk);
     g->m.bash(x, y, 40, junk);  // Multibash effect, so that doors &c will fall
     g->m.bash(x, y, 40, junk);
     if (g->m.is_destructable(x, y) && rng(1, 10) >= 3)
      g->m.ter(x, y) = t_rubble;
    }
   }
   break;

  case AEA_HEAL:
   g->add_msg(_("You feel healed."));
   p->healall(2);
   break;

  case AEA_CONFUSED:
   for (int x = p->posx - 8; x <= p->posx + 8; x++) {
    for (int y = p->posy - 8; y <= p->posy + 8; y++) {
     int mondex = g->mon_at(x, y);
     if (mondex != -1)
      g->z[mondex].add_effect(ME_STUNNED, rng(5, 15));
    }
   }

  case AEA_ENTRANCE:
   for (int x = p->posx - 8; x <= p->posx + 8; x++) {
    for (int y = p->posy - 8; y <= p->posy + 8; y++) {
     int mondex = g->mon_at(x, y);
     if (mondex != -1 &&  g->z[mondex].friendly == 0 &&
         rng(0, 600) > g->z[mondex].hp)
      g->z[mondex].make_friendly();
    }
   }
   break;

  case AEA_BUGS: {
   int roll = rng(1, 10);
   mon_id bug = mon_null;
   int num = 0;
   std::vector<point> empty;
   for (int x = p->posx - 1; x <= p->posx + 1; x++) {
    for (int y = p->posy - 1; y <= p->posy + 1; y++) {
     if (g->is_empty(x, y))
      empty.push_back( point(x, y) );
    }
   }
   if (empty.empty() || roll <= 4)
    g->add_msg(_("Flies buzz around you."));
   else if (roll <= 7) {
    g->add_msg(_("Giant flies appear!"));
    bug = mon_fly;
    num = rng(2, 4);
   } else if (roll <= 9) {
    g->add_msg(_("Giant bees appear!"));
    bug = mon_bee;
    num = rng(1, 3);
   } else {
    g->add_msg(_("Giant wasps appear!"));
    bug = mon_wasp;
    num = rng(1, 2);
   }
   if (bug != mon_null) {
    monster spawned(g->mtypes[bug]);
    spawned.friendly = -1;
    for (int i = 0; i < num && !empty.empty(); i++) {
     int index = rng(0, empty.size() - 1);
     point spawnp = empty[index];
     empty.erase(empty.begin() + index);
     spawned.spawn(spawnp.x, spawnp.y);
     g->z.push_back(spawned);
    }
   }
  } break;
    

  case AEA_RADIATION:
   g->add_msg(_("Horrible gasses are emitted!"));
   for (int x = p->posx - 1; x <= p->posx + 1; x++) {
    for (int y = p->posy - 1; y <= p->posy + 1; y++)
     g->m.add_field(g, x, y, fd_nuke_gas, rng(2, 3));
   }
   break;

  case AEA_PAIN:
   g->add_msg(_("You're wracked with pain!"));
   p->pain += rng(5, 15);
   break;

  case AEA_MUTATE:
   if (!one_in(3))
    p->mutate(g);
   break;

  case AEA_PARALYZE:
   g->add_msg(_("You're paralyzed!"));
   p->moves -= rng(50, 200);
   break;

  case AEA_FIRESTORM:
   g->add_msg(_("Fire rains down around you!"));
   for (int x = p->posx - 3; x <= p->posx + 3; x++) {
    for (int y = p->posy - 3; y <= p->posy + 3; y++) {
     if (!one_in(3)) {
      if (g->m.add_field(g, x, y, fd_fire, 1 + rng(0, 1) * rng(0, 1)))
       g->m.field_at(x, y).age = 30;
     }
    }
   }
   break;

  case AEA_ATTENTION:
   g->add_msg(_("You feel like your action has attracted attention."));
   p->add_disease(DI_ATTENTION, 600 * rng(3, 6), g);
   break;

  case AEA_TELEGLOW:
   g->add_msg(_("You feel unhinged."));
   p->add_disease(DI_TELEGLOW, 100 * rng(3, 12), g);
   break;

  }
 }
}
