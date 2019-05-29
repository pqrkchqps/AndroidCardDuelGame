/****************************************************************************
 Copyright (c) 2017-2018 Xiamen Yaji Software Co., Ltd.

 http://www.cocos2d-x.org

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"
#include "CardBlind.h"
#include "CardBlue.h"
#include "CardDodge.h"
#include "CardHealingPotion.h"
#include "CardPoison.h"
#include "CardWhite.h"
#include "CardOnGameBoard.h"
#include "RandomCardTypes.h"
#include "Player.h"
#include "network/SocketIO.h"
#include "DelegateSocketIO.h"

USING_NS_CC;

Scene* HelloWorld::createScene()
{
    return HelloWorld::create();
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
  auto visibleSize = Director::getInstance()->getVisibleSize();
  Vec2 origin = Director::getInstance()->getVisibleOrigin();
  this->dq = new DebugQueue(this, origin, visibleSize);
  this->dq->printDebugWithNewMessage("HelloWorld Init Called");
  this->dq->printDebugWithNewMessage("Got Origin and VisibleSize From Director");

    //////////////////////////////
    // 1. super init first
    if ( !Scene::init() )
    {
        return false;
    }

    this->dq->printDebugWithNewMessage("Set Display Stats To False");
    Director::getInstance()->setDisplayStats(false);

    this->dq->printDebugWithNewMessage("Listen For Keyboard Messages (back button)");
    auto listener = EventListenerKeyboard::create();
    listener->onKeyReleased = CC_CALLBACK_2(HelloWorld::onKeyReleased, this);
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);


    Player* player = new Player(dq);
    Player* computer = new Player(dq);

    CardType* card1 = new CardDodge();
    CardType* card2 = new CardBlind();
    CardType* card3 = new CardHealingPotion();
    CardType* card4 = new CardPoison();
    CardType* card_color1 = new CardWhite();
    CardType* card_color2 = new CardBlue();
    RandomCardTypes* random_card_types = new RandomCardTypes(card1, card2, card3, card4, card_color1, card_color2);

    for (int i = 0; i < 16; i++){
      this->cards_on_game_board[i] = new CardOnGameBoard(i, random_card_types, this, dq, player, computer);
      this->cards_on_game_board[i]->randomizeCardType();
      this->cards_on_game_board[i]->createAndDrawCardSprite();
    }

    this->cards_on_game_board[16] = new CardOnGameBoard(999, random_card_types, this, dq, computer, player);
    this->cards_on_game_board[16]->randomizeCardType();

    this->current_time =0;
    this->game_over = false;
    this->scheduleUpdate();

    DelegateSocketIO* delegate = new DelegateSocketIO();

    cocos2d::network::SocketIO* socketIO = cocos2d::network::SocketIO::getInstance();
    cocos2d::network::SIOClient* sio_client = socketIO->connect("192.168.0.27:9000/", * delegate);
    //std::string examplestring = "tom";

    //sio_client->send(examplestring);
    sio_client->on("connect", CC_CALLBACK_2(HelloWorld::MyConnect, this));

    this->dq->printDebugWithNewMessage("Return From Init... Init Complete");
    return true;
}


void HelloWorld::MyConnect(network::SIOClient* client, const std::string& data)
{
    CCLOGINFO("SocketIOTest::connect called");

    client->emit("emittest", "{\"name\":\"myname\",\"type\":\"mytype\"}");
}

void HelloWorld::update(float delta_time){
  this->current_time += delta_time;
  if (this->current_time > this->TURNS_TIME){
    if (this->cards_on_game_board[16]->isSelfDead()){
      if (!game_over){
        dq->printDebugWithNewMessage("******************Opponent Defeated****************************");
        game_over = true;
      }
    } else if (this->cards_on_game_board[16]->isOpponentDead()){
      if (!game_over){
        dq->printDebugWithNewMessage("*******************You Are Defeated****************************");
        game_over = true;
      }
    } else {
      this->cards_on_game_board[16]->executeCardOnPlayers();
      this->cards_on_game_board[16]->randomizeCardType();
      this->current_time = 0;
      dq->printDebugWithNewMessage("***************************************************************");
      this->cards_on_game_board[16]->printPlayerStats();
    }
  }
}

void HelloWorld::onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event)
{
    if(keyCode == cocos2d::EventKeyboard::KeyCode::KEY_BACK)
    {
      Director::getInstance()->end();
    }
}
