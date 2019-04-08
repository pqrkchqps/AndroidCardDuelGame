#ifndef __PLAYER_H__
#define __PLAYER_H__
#include "cocos2d.h"
#include <string>
//#include "CardDeck.h"
USING_NS_CC;

class Player {
public:
  int blue_energy;
  int red_energy;
  int yellow_energy;
  int black_energy;
  int white_energy;

  int health;

  bool cancel_effect;

  DebugQueue* dq;

  Player(DebugQueue* dq) {
    blue_energy = 0;
    red_energy = 0;
    yellow_energy = 0;
    black_energy = 0;
    white_energy = 0;

    cancel_effect = false;

    health = 50;
    this->dq = dq;
  }

  bool isDead() {
    if (health <= 0){
      return true;
    } else {
      return false;
    }
  }

  void printStats(){
    dq->printDebugWithNewMessage("Health:"+std::to_string(health)+" blue:"+std::to_string(blue_energy)
                                  +" red:"+std::to_string(red_energy)+" yellow:"+std::to_string(yellow_energy)
                                  +" black:"+std::to_string(black_energy)+" white:"+std::to_string(white_energy));
  }
  void updateDamage(CardType* card){
    if (!cancel_effect){
      health += card->damage_amount;
    }
    else{
      this->dq->printDebugWithNewMessage("Canceled Attack");
      cancel_effect = false;
    }
  }
  void updateHealing(CardType* card){
    health += card->healing_amount;
  }
  bool updateEnergy(CardType* card){
    if (    blue_energy + card->blue_cost >= 0
          && red_energy + card->red_cost >= 0
          && yellow_energy + card->yellow_cost >= 0
          && black_energy + card->black_cost >= 0
          && white_energy + card->white_cost >= 0){
      blue_energy += card->blue_cost;
      red_energy += card->red_cost;
      yellow_energy += card->yellow_cost;
      black_energy += card->black_cost;
      white_energy += card->white_cost;
      return true;
    }
    else {
      return false;
    }
  }
  void updateCancelEffect(CardType* card){
    this->cancel_effect = card->cancel_effect;
  }
};

#endif
