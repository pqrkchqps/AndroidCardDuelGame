#ifndef __CARDHEALINGPOTION_H__
#define __CARDHEALINGPOTION_H__
#include "CardType.h"
#include "cocos2d.h"
#include <string>
USING_NS_CC;

class CardHealingPotion : public CardType{
public:
  CardHealingPotion(): CardType(){
    image_url = "healing_potion.png";
    sound_url = "healing_potion.wav";
    name = "healing_potion";
    blue_cost = -2;

    healing_amount = 2;
  }
};

#endif
