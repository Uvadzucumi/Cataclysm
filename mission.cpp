#include "mission.h"
#include "game.h"

mission mission_type::create(game *g, int npc_id)
{
 mission ret;
 ret.uid = g->assign_mission_id();
 ret.type = this;
 ret.npc_id = npc_id;
 ret.item_id = item_id;
 ret.value = value;
 ret.follow_up = follow_up;

 if (deadline_low != 0 || deadline_high != 0)
  ret.deadline = int(g->turn) + rng(deadline_low, deadline_high);
 else
  ret.deadline = 0;

 return ret;
}

std::string mission::name()
{
 if (type == NULL)
  return "NULL";
 return type->name;
}

std::string mission_dialogue (mission_id id, talk_topic state)
{
 switch (id) {

 case MISSION_GET_ANTIBIOTICS:
  switch (state) {
   case TALK_MISSION_DESCRIBE:
    switch (rng(1, 4)) {
     case 1: return _("Hey <name_g>... I really need your help...");
     case 2: return _("<swear!><punc> I'm hurting...");
     case 3: return _("This infection is bad, <very> bad...");
     case 4: return _("Oh god, it <swear> hurts...");
    }
    break;
   case TALK_MISSION_OFFER:
    return _("\
I'm infected.  Badly.  I need you to get some antibiotics for me...");
   case TALK_MISSION_ACCEPTED:
    return _("\
Oh, thank god, thank you so much!  I won't last more than a couple of days, \
so hurry...");
   case TALK_MISSION_REJECTED:
    return _("\
What?!  Please, <ill_die> without your help!");
   case TALK_MISSION_ADVICE:
    return _("\
There's a town nearby.  Check pharmacies; it'll be behind the counter.");
   case TALK_MISSION_INQUIRE:
    return _("Find any antibiotics yet?");
   case TALK_MISSION_SUCCESS:
    return _("Oh thank god!  I'll be right as rain in no time.");
   case TALK_MISSION_SUCCESS_LIE:
    return _("What?!  You're lying, I can tell!  Ugh, forget it!");
   case TALK_MISSION_FAILURE:
    return _("How am I not dead already?!");
  }
  break;

 case MISSION_GET_SOFTWARE:
  switch (state) {
   case TALK_MISSION_DESCRIBE:
    return _("Oh man, I can't believe I forgot to download it...");
   case TALK_MISSION_OFFER:
    return _("There's some important software on my computer that I need on USB.");
   case TALK_MISSION_ACCEPTED:
    return _("\
Thanks!  Just pull the data onto this USB drive and bring it to me.");
   case TALK_MISSION_REJECTED:
    return _("Seriously?  It's an easy job...");
   case TALK_MISSION_ADVICE:
    return _("Take this USB drive.  Use the console, and download the software.");
   case TALK_MISSION_INQUIRE:
    return _("So, do you have my software yet?");
   case TALK_MISSION_SUCCESS:
    return _("Excellent, thank you!");
   case TALK_MISSION_SUCCESS_LIE:
    return _("What?!  You liar!");
   case TALK_MISSION_FAILURE:
    return _("Wow, you failed?  All that work, down the drain...");
  }
  break;

 case MISSION_GET_ZOMBIE_BLOOD_ANAL:
  switch (state) {
   case TALK_MISSION_DESCRIBE:
    return _("\
It could be very informative to perform an analysis of zombie blood...");
   case TALK_MISSION_OFFER:
    return _("\
I need someone to get a sample of zombie blood, take it to a hospital, and \
perform a centrifuge analysis of it.");
   case TALK_MISSION_ACCEPTED:
    return _("\
Excellent.  Take this vacutainer; once you've produced a zombie corpse, use it \
to extrace blood from the body, then take it to a hospital for analysis.");
   case TALK_MISSION_REJECTED:
    return _("\
Are you sure?  The scientific value of that blood data could be priceless...");
   case TALK_MISSION_ADVICE:
    return _("\
The centrifuge is a bit technical; you might want to study up on the usage of \
computers before completing that part.");
   case TALK_MISSION_INQUIRE:
    return _("Well, do you have the data yet?");
   case TALK_MISSION_SUCCESS:
    return _("Excellent!  This may be the key to removing the infection.");
   case TALK_MISSION_SUCCESS_LIE:
    return _("Wait, you couldn't possibly have the data!  Liar!");
   case TALK_MISSION_FAILURE:
    return _("What a shame, that data could have proved invaluable...");
  }
  break;
  
 case MISSION_RESCUE_DOG:
  switch (state) {
   case TALK_MISSION_DESCRIBE:
    return _("Oh, my poor puppy...");

   case TALK_MISSION_OFFER:
    return _("\
I left my poor dog in a house, not far from here.  Can you retrieve it?");

   case TALK_MISSION_ACCEPTED:
    return _("\
Thank you!  Please hurry back!");

   case TALK_MISSION_REJECTED:
    return _("\
Please, think of my poor little puppy!");

   case TALK_MISSION_ADVICE:
    return _("\
Take my dog whistle; if the dog starts running off, blow it and he'll return \
to your side.");

   case TALK_MISSION_INQUIRE:
    return _("\
Have you found my dog yet?");

   case TALK_MISSION_SUCCESS:
    return _("\
Thank you so much for finding him!");

   case TALK_MISSION_SUCCESS_LIE:
    return _("What?!  You're lying, I can tell!  Ugh, forget it!");

   case TALK_MISSION_FAILURE:
    return _("Oh no!  My poor puppy...");

  }
  break;

 case MISSION_KILL_ZOMBIE_MOM:
  switch (state) {
   case TALK_MISSION_DESCRIBE:
    return _("Oh god, I can't believe it happened...");
   case TALK_MISSION_OFFER:
    return _("\
My mom... she's... she was killed, but then she just got back up... she's one \
of those things now.  Can you put her out of her misery for me?");
   case TALK_MISSION_ACCEPTED:
    return _("Thank you... she would've wanted it this way.");
   case TALK_MISSION_REJECTED:
    return _("Please reconsider, I know she's suffering...");
   case TALK_MISSION_ADVICE:
    return _("Find a gun if you can, make it quick...");
   case TALK_MISSION_INQUIRE:
    return _("Well...?  Did you... finish things for my mom?");
   case TALK_MISSION_SUCCESS:
    return _("Thank you.  I couldn't rest until I knew that was finished.");
   case TALK_MISSION_SUCCESS_LIE:
    return _("What?!  You're lying, I can tell!  Ugh, forget it!");
   case TALK_MISSION_FAILURE:
    return _("Really... that's too bad.");
  }
  break;
 default:
  switch (state) {
   case TALK_MISSION_DESCRIBE:
   case TALK_MISSION_OFFER:
   case TALK_MISSION_ACCEPTED:
   case TALK_MISSION_REJECTED:
   case TALK_MISSION_ADVICE:
   case TALK_MISSION_INQUIRE:
   case TALK_MISSION_SUCCESS:
   case TALK_MISSION_SUCCESS_LIE:
   case TALK_MISSION_FAILURE:
    return _("Someone forgot to code this message!");
  }
  break;
 }
 return _("Someone forgot to code this message!");
}

