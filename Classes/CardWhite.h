#ifndef __CARDWHITE_H__
#define __CARDWHITE_H__
#include "CardType.h"
#include "cocos2d.h"
#include <string>
USING_NS_CC;

class CardWhite : public CardType{
public:
  CardWhite(): CardType(){
    image_url = "white.png";
    sound_url = "white.wav";
    name = "white";
    white_cost = 1;
  }
};

#endif
