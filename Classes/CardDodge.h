#ifndef __CARDDODGE_H__
#define __CARDDODGE_H__
#include "CardType.h"
#include "cocos2d.h"
#include <string>
USING_NS_CC;

class CardDodge : public CardType{
public:
  CardDodge(): CardType(){
    image_url = "dodge.png";
    sound_url = "dodge.wav";
    name = "dodge";
    white_cost = -1;

    cancel_effect = true;
  }
};

#endif
