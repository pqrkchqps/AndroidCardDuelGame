#ifndef __RANDOMCARDTYPES_H__
#define __RANDOMCARDTYPES_H__

#include <vector>
#include <random>
#include "CardType.h"

class RandomCardTypes{
private:
  std::vector<CardType*> cards;
public:
  RandomCardTypes(CardType* card1, CardType* card2, CardType* card3, CardType* card4, CardType* card_color1, CardType* card_color2){
    cards.push_back(card1);
    cards.push_back(card2);
    cards.push_back(card3);
    cards.push_back(card4);
    cards.push_back(card_color1);
    cards.push_back(card_color2);
  }
  int getRandomIndex(){
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> dist10(0,9);
    int n = dist10(rng);
    if (n == 6 || n ==7) n = 4;
    if (n == 8 || n == 9) n = 5;
    return n;
  }
  CardType* getNthCardType(int n){
    return cards[n];
  }
};
#endif
