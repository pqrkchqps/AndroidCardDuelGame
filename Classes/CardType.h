#ifndef __CARDTYPE_H__
#define __CARDTYPE_H__
#include "cocos2d.h"
#include <string>
//#include "CardDeck.h"
USING_NS_CC;

class CardType {
public:
  std::string image_url;
  const char* sound_url;
  std::string name;
  int blue_cost;
  int red_cost;
  int yellow_cost;
  int black_cost;
  int white_cost;

  int damage_amount;
  int healing_amount;

  bool cancel_effect;
  bool destroy_effect;

  CardType() {
    image_url = "!";
    sound_url = "!";
    name = "card";
    blue_cost = 0;
    red_cost = 0;
    yellow_cost = 0;
    black_cost = 0;
    white_cost = 0;

    damage_amount = 0;
    healing_amount = 0;

    cancel_effect = false;
    destroy_effect = false;

  };
  // void SetDeck(CardDeck* deck) {
  //   this->deck = deck;
  //   this->listener = EventListenerTouchOneByOne::create();
  //   this->listener->onTouchEnded = [this](Touch* touch, Event* event){
  //     Vec2 point = touch->getLocation();
  //
  //     int x = (int)std::floor(point.x /  this->deck->sprites[0]->getContentSize().width);
  //     int y = (int)std::floor(point.y /  this->deck->sprites[0]->getContentSize().height);
  //     int index = (int)std::round(y*4+x);
  //
  //     int n = this->deck->playing_deck[index];
  //     this->deck->shuffle1Card(n);
  //     this->deck->drawPlayingDeck();
  //
  //     return true; // if you are consuming it
  //   };
  //   Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(this->listener, this );
  // };
};

#endif
