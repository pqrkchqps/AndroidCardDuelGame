#ifndef __CARDBLIND_H__
#define __CARDBLIND_H__
#include "CardType.h"
#include "cocos2d.h"
#include <string>
USING_NS_CC;

class CardBlind : public CardType{
public:
  CardBlind(): CardType(){
    image_url = "blind.png";
    sound_url = "blind.ogg";
    name = "blind";
    white_cost = -2;

    cancel_effect = true;

    damage_amount = -1;
  }
};

#endif
