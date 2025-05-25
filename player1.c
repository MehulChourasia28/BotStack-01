#include "header.h"
#include <stdio.h>

/* ----------------------------------------
The terminal commands for testing the bot can be found in the "cmds.txt" file.
  Ensure the directory you are running the terminal in is correct or things will
  not load correctly at runtime
---------------------------------------- */

typedef enum PlayMode {
  DEFENSIVE,
  AGGRESSIVE
} PlayMode;

PlayMode playmode = AGGRESSIVE;
Item prev_item = ITEM_Empty;
int next_fire = ACTION_ShootOther;

int player1_abstain(Player self, Player other, Gun state, int opponentRequest) {
  /* ----------------------------------------
  This is a technically optional piece of logic. The abstention mechanic is
    detailed in the game mechanics, but you have the option to not interact
    at all with this system - in that case, simply leave this function
    untouched.

  If you do want to utilise this mechanic, return 1 to request an abstain,
    or return 0 to deny.
  If the "opponentRequest" value is 1, then your opponent has also requested
    an abstain, and you are accepting or denying this. Otherwise, it is your
    initiative to request an abstention.
  ---------------------------------------- */
  if (self.lives == 1 && other.lives > 1 && opponentRequest == 1) return 1;
  else return 0;
}
ItemAction player1_item(Player self, Player other, Gun state) {
  /* ----------------------------------------
  This is the first major function you will be modifying in order to create
    your bot

  It is responsible for using your available items if you wish to do so - you
   can return one of [ACTION_NoItem, ACTION_Item1, ACTION_Item2].
  Your available items are visible at [self.item1] and [self.item2] (as well
   as all other data for both yourself and opponent)
  ---------------------------------------- */

  Item a = self.item1, b = self.item2;

  // Set playmode depending on lives left
  if (self.lives == 1) playmode = DEFENSIVE;
  else playmode = AGGRESSIVE;

  // Compute probability of live bullet next
  float probability_of_live = (float)state.currentLiveBullets / (float)state.currentBullets;

  // Compute round winning scenarios 
  // 1 live bullet in total and opponent has 1 life
  if (state.currentLiveBullets == state.currentBullets && other.lives == 1){
    next_fire = ACTION_ShootOther;
    return ACTION_NoItem;
  }
  // 2 live bullets, 2 opponent lives, and skip enemy item
  else if (state.currentLiveBullets == state.currentBullets && state.currentLiveBullets == 2 && other.lives == 2 && (a == ITEM_SkipEnemy || b == ITEM_SkipEnemy)){
    next_fire = ACTION_ShootOther;
    if (a == ITEM_SkipEnemy) return ACTION_Item1;
    else return ACTION_Item2;
  }
  // 2 bullets total, 1 live bullet, opponent has 1 life, peek current item
  else if (state.currentBullets = 2 && state.currentLiveBullets == 1 && other.lives == 1 && (a == ITEM_PeekCurrent || b == ITEM_PeekCurrent)){
    prev_item = ITEM_PeekCurrent;
    if (a == ITEM_PeekCurrent) return ACTION_Item1;
    else return ACTION_Item2;
  }

  // Compute urgent defensive scenarios
  if (playmode == DEFENSIVE){
    // With no live bullets
    if (state.currentLiveBullets == 0){
      next_fire = ACTION_ShootSelf;
      return ACTION_NoItem;
    }
    // With healthkit
    if (a == ITEM_HealthKit || b == ITEM_HealthKit){
      if (state.currentLiveBullets == 0) next_fire = ACTION_ShootSelf;
      else next_fire = ACTION_ShootOther;
      if (a == ITEM_HealthKit) return ACTION_Item1;
      else return ACTION_Item2;
    }
    // With peek current
    else if (a == ITEM_PeekCurrent || b == ITEM_PeekCurrent){
      prev_item = ITEM_PeekCurrent;
      if (a == ITEM_PeekCurrent) return ACTION_Item1;
      else return ACTION_Item2;
    }
    // With eject bullet and probability of live <= 0.5
    else if (probability_of_live <= 0.5 && (a == ITEM_EjectCurrent || b == ITEM_EjectCurrent)){
      next_fire = ACTION_ShootOther;
      if (a == ITEM_EjectCurrent) return ACTION_Item1;
      else return ACTION_Item2;
    }
    // With skip enemy
    else if (a == ITEM_SkipEnemy || b == ITEM_SkipEnemy){
      next_fire = ACTION_ShootOther;
      if (a == ITEM_SkipEnemy) return ACTION_Item1;
      else return ACTION_Item2;
    }
    // With reload gun
    else if (a == ITEM_ResetGun || b == ITEM_ResetGun){
      next_fire = ACTION_ShootOther;
      if (a == ITEM_ResetGun) return ACTION_Item1;
      else return ACTION_Item2;
    }
  }

  // Compute cases to improve situation with items
  if (playmode == AGGRESSIVE){
    // Double shoot
    if (prev_item == ITEM_SkipEnemy){
      prev_item = ITEM_Empty;
      next_fire = ACTION_ShootOther;
      return ACTION_NoItem;
    }
    // No live bullets
    if (state.currentLiveBullets == 0){
      next_fire = ACTION_ShootSelf;
      return ACTION_NoItem;
    }
    // With gun reload
    if (probability_of_live < 0.3 && (a == ITEM_ResetGun || b == ITEM_ResetGun)){
      next_fire = ACTION_ShootOther;
      if (a == ITEM_ResetGun) return ACTION_Item1;
      else return ACTION_Item2;
    }
    // Initiate double shoot
    else if (state.currentBullets - state.currentLiveBullets <= 1 && (a == ITEM_SkipEnemy || b == ITEM_SkipEnemy)){
      next_fire = ACTION_ShootOther;
      prev_item = ITEM_SkipEnemy;
      if (a == ITEM_SkipEnemy) return ACTION_Item1;
      else return ACTION_Item2;
    } 
    // With peek
    else if (probability_of_live > 0.7 && (a == ITEM_PeekCurrent || b == ITEM_PeekCurrent)){
      // peek to confirm you really have blanks, else hold off
      if (a == ITEM_PeekCurrent) return ACTION_Item1;
      else return ACTION_Item2;
    }
    else if (probability_of_live > 0.6 && (a == ITEM_EjectCurrent || b == ITEM_EjectCurrent)){
      // small chance it'll be live — eject it to improve odds
      next_fire = ACTION_ShootOther;
      if (a == ITEM_EjectCurrent) return ACTION_Item1;
      else return ACTION_Item2;
    }
    else if (probability_of_live > 0.5){
      next_fire = ACTION_ShootOther;
      return ACTION_NoItem;
    }
    else{
      next_fire = ACTION_ShootSelf;
      return ACTION_NoItem;
    }
  }
  
  // Compute cases for no items
  if (a == ITEM_Empty && b == ITEM_Empty){
    // 1 life left or opponent has both items
    if (playmode == DEFENSIVE || (other.item1 != ITEM_Empty && other.item2 != ITEM_Empty)){
      next_fire = ACTION_ShootOther;
      return ACTION_NoItem;
    }
    // >1 life left
    else if (probability_of_live < 0.5){
      next_fire = ACTION_ShootSelf;
      return ACTION_NoItem;
    } 
  }
}
TurnAction player1_action(Player self, Player other, Gun state, Bullet nextBullet) {
  /* ----------------------------------------
  This is the second major function you will be modifying in order to create
    your bot

  It is responsible for deciding who to shoot. You can see your own and your
   opponent's data, and the current total bullets as well as the total live
   bullets in the [state] struct.

  If your previous action was [ITEM_PeekCurrent], the value of nextBullet will
   be either [BULLET_Live | BULLET_Blank] depending on the observed bullet.
   Otherwise, this value will be BULLET_UNKNOWN
  ---------------------------------------- */

  // Handle item peek current
  if (prev_item == ITEM_PeekCurrent){
    prev_item = ITEM_Empty;
    if (nextBullet == BULLET_Live) return ACTION_ShootOther;
    else return ACTION_ShootSelf;

  }

  return next_fire;
}