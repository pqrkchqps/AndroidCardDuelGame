#ifndef __CARDBLUE_H__
#define __CARDBLUE_H__
#include "CardType.h"
#include "cocos2d.h"
#include <string>
USING_NS_CC;

class CardBlue : public CardType{
public:
  CardBlue(): CardType(){
    image_url = "blue.png";
    sound_url = "blue.ogg";
    name = "blue";
    blue_cost = 1;

  }
};

#endif
