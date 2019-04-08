#ifndef __CARDDECK_H__
#define __CARDDECK_H__
#include "cocos2d.h"
#include "CardType.h"
#include <vector>
#include <random>
#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include <cmath>
#include "DebugQueue.h"

USING_NS_CC;

class CardDeck {
public:
  DebugQueue* dq;
  Card* cards[6];
  Sprite* sprites[16];
  int playing_deck[16];
  int drawing_deck[16];
  cocos2d::Scene* scene;
  Vec2 origin;
  Size visibleSize;
  EventListenerTouchOneByOne* listener;
  EventDispatcher* event_dispatcher;


  void printDeck16(int deck[]){
    std::string ret_string = ":";
    for (int k =0; k < 16; k++){
      int val = deck[k];
      std::ostringstream ss;
      ss << std::setw( 2 ) << std::setfill( ' ' ) << std::to_string(val);
      ret_string += ss.str() + ":";
    }
    this->dq->printDebugWithNewMessage(ret_string);
  }

  int getCardAtN(int n){
    switch (n) {
    case 0:
    case 1:
    case 2:
    case 3:
      return 0;
    case 4:
    case 5:
    case 6:
    case 7:
      return 1;
    case 8:
    case 9:
    case 10:
    case 11:
      return 2;
    case 12:
    case 13:
    case 14:
    case 15:
      return 3;
    case 16:
    case 17:
    case 18:
    case 19:
    case 20:
    case 21:
    case 22:
    case 23:
      return 4;
    case 24:
    case 25:
    case 26:
    case 27:
    case 28:
    case 29:
    case 30:
    case 31:
      return 5;
    }
  }
public:
  CardDeck(Card* card1, Card* card2, Card* card3, Card* card4, Card* color_card1, Card* color_card2, cocos2d::Scene* scene, Vec2 origin, Size visibleSize){
    this->cards[0] = card1;
    this->cards[1] = card2;
    this->cards[2] = card3;
    this->cards[3] = card4;
    this->cards[4] = color_card1;
    this->cards[5] = color_card2;
    this->scene = scene;
    this->origin = origin;
    this->visibleSize = visibleSize;
  }

  void SetupGameDeck(DebugQueue* dq){
    std::vector<int> deck1;
    std::vector<int> deck2;
    for (int j = 0; j < 32; j++){
      deck1.push_back(j);
    }
    std::random_device dev;
    std::mt19937 rng(dev());
    for (int i = 31; i >=0; i--) {
      std::uniform_int_distribution<std::mt19937::result_type> distn(0,i); // distribution in range [1, 6]
      int n = distn(rng);
      deck2.push_back(deck1[n]);
      deck1.erase(deck1.begin() + n);
    }
    std::string ret_string = ":";
    for (int k =0; k < 16; k++){
      int val = deck2[k];
      std::ostringstream ss;
      ss << std::setw( 2 ) << std::setfill( ' ' ) << std::to_string(val);
      ret_string += ss.str() + ":";
    }
    dq->printDebugWithNewMessage(ret_string);
    dq->printDebugWithNewMessage("Deck Shuffled");
    for (int l = 0; l < 16; l++){
      this->playing_deck[l] = deck2[l];
    }
    for (int m = 16; m < 31; m++){
      this->drawing_deck[m-16] = deck2[m];
    }

    this->dq = dq;
  }

  void shuffle1Card(int return_card_position){
    int return_card_n = this->playing_deck[return_card_position];
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> distn(0,15); // distribution in range [1, 6]
    int random_n = distn(rng);
    this->playing_deck[return_card_position] = this->drawing_deck[random_n];
    this->drawing_deck[random_n] = return_card_n;
    printDeck16(this->playing_deck);
    printDeck16(this->drawing_deck);
  }

  void drawPlayingDeck() {
    for (double i = 0; i < 4; i++) {
        for (double j = 0; j < 4; j++) {
          int index = (int)std::round(i*4+j);
          int n = this->playing_deck[(int)std::round(i*4+j)];
          int card_num = this->getCardAtN(n);
          Card* card = this->cards[card_num];
          std::string str = card->image_url;
          this->sprites[index] = Sprite::create(str);
          this->sprites[index]->setPosition(Vec2(origin.x + this->sprites[index]->getContentSize().width*(0.5+j),
                                                  origin.y + this->sprites[index]->getContentSize().height*(0.5+i)));
          this->scene->addChild(this->sprites[index], 1);
        }
    }
  }

};

#endif
