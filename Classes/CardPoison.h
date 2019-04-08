#ifndef __CARDPOISON_H__
#define __CARDPOISON_H__
#include "CardType.h"
#include "cocos2d.h"
#include <string>
USING_NS_CC;

class CardPoison : public CardType{
public:
  CardPoison(): CardType(){
    image_url = "poison.png";
    sound_url = "poison.wav";
    name = "posion";
    blue_cost = -3;

    damage_amount = -5;
  }
};

#endif
