#ifndef __CARDONBOARD_H__
#define __CARDONBOARD_H__
#include "RandomCardTypes.h"
#include "Player.h"
#include <SimpleAudioEngine.h>
#include "cocos2d.h"

class CardOnGameBoard :Sprite {
private:
  int position;
  RandomCardTypes* card_types;
  CardType* current_card;
  cocos2d::Scene* scene;
  Vec2 origin;
  EventListenerTouchOneByOne* listener;
  Sprite* current_sprite = NULL;
  CocosDenshion::SimpleAudioEngine* current_audio = NULL;
  DebugQueue* dq;
  Player* self;
  Player* opponent;

public:
  CardOnGameBoard(int position, RandomCardTypes* card_types, cocos2d::Scene* scene, DebugQueue* dq, Player* self, Player* opponent){
    this->position = position;
    this->card_types = card_types;
    this->scene = scene;
    this->dq = dq;
    this->self = self;
    this->opponent = opponent;

    this->origin = Director::getInstance()->getVisibleOrigin();

    this->current_audio = CocosDenshion::SimpleAudioEngine::getInstance();

    this->listener = EventListenerTouchOneByOne::create();
    this->listener->setSwallowTouches(true);
    this->listener->onTouchBegan = [this](cocos2d::Touch* touch, cocos2d::Event* event) {
      cocos2d::Vec2 p = touch->getLocation();
      cocos2d::Rect rect = this->current_sprite->getBoundingBox();
      if(rect.containsPoint(p)) {
        this->executeCardOnPlayers();
        this->randomizeCardType();
        this->createAndDrawCardSprite();
        return true;
      }

      return false;
    };

    _eventDispatcher->addEventListenerWithSceneGraphPriority( this->listener, this->scene );
  }

  void createAndDrawCardSprite(){
    this->scene->removeChild(this->current_sprite);
    this->current_sprite = Sprite::create(current_card->image_url);
    Sprite* sprite = current_sprite;
    int pos_x = position % 4;
    int pos_y = position / 4;
    sprite->setPosition(Vec2(this->origin.x +sprite->getContentSize().width*(0.5+pos_x),
                              this->origin.y + sprite->getContentSize().height*(0.5+pos_y)));
    this->scene->addChild(sprite, 1);
  }

  void randomizeCardType(){
    int n = card_types->getRandomIndex();
    this->current_card = card_types->getNthCardType(n);
  }

  void executeCardOnPlayers() {
    bool enough_energy = self->updateEnergy(this->current_card);
    if (enough_energy) {
      this->current_audio->playEffect(this->current_card->sound_url, false, 1.0f, 0.0f, 1.0f);
      self->updateHealing(this->current_card);
      opponent->updateDamage(this->current_card);
      self->updateCancelEffect(this->current_card);
    }
  }
  void printPlayerStats(){
    self->printStats();
    opponent->printStats();
  }

  bool isOpponentDead(){
    if (opponent->isDead()) {
      return true;
    } else {
      return false;
    }
  }
  bool isSelfDead(){
    if (self->isDead()) {
      return true;
    } else {
      return false;
    }
  }
};

#endif
