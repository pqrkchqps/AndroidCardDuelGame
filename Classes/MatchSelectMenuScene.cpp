#include "MatchSelectMenuScene.h"
#include <iostream>
#include "ui/CocosGUI.h"
#include "DelegateSocketIO.h"
#include "MainMenuScene.h"

USING_NS_CC;

Scene* MatchSelectMenu::createScene(std::string username)
{
    // 'scene' is an autorelease object
    auto scene = MatchSelectMenu::create();
    scene->username = username;
    CCLOG("username");
    CCLOG(scene->username.c_str());
    return scene;
}

bool MatchSelectMenu::init()
{
    if ( !Scene::init() )
    {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    this->dq = new DebugQueue(this, origin, visibleSize);

    dq->printDebugWithNewMessage("");

    this->pings = 1;
    DelegateSocketIO* delegate = new DelegateSocketIO();
    delegate->setDebugQueue(dq);

    cocos2d::network::SocketIO* socketIO = cocos2d::network::SocketIO::getInstance();
    cocos2d::network::SIOClient* sio_client = socketIO->connect("http://android-card-game.herokuapp.com:80", * delegate);
    this->sio_client = sio_client;

    auto scheduler = Director::getInstance()->getScheduler();

    scheduler->schedule([this](float dt) {
      this->sio_client->emit("send_username", "{\"name\":\""+this->username+"\"}");
    }, this,10.0f, kRepeatForever, 0.0f, false, "send_username");

    scheduler->schedule([this](float dt) {
      if (this->pings == 0){
        disconnectReset();
      }
      this->pings = 0;
    }, this, 12.0f, kRepeatForever, 0.0f, false, "checkPings");

    //this->sio_client->on("verify_username", CC_CALLBACK_2(MatchSelectMenu::verifyUserName, this));
    this->sio_client->on("ping", CC_CALLBACK_2(MatchSelectMenu::verifyPings, this));
    this->sio_client->on("get_usernames", CC_CALLBACK_2(MatchSelectMenu::getUserNames, this));

    // auto label = Label::createWithSystemFont(this->username, "Arial", 15.0);
    // auto xpos = origin.x + (visibleSize.width - label->getContentSize().width)*0.5 + label->getContentSize().width * 0.5;
    // auto ypos = visibleSize.height*9/10;
    // label->setPosition(Vec2(xpos, ypos));
    // this->addChild(label, 1);

    // touchListener->onTouchBegan = [textField, button, dq, this, this->sio_client](cocos2d::Touch* touch, cocos2d::Event * event) -> bool {
    //     try {
    //         // Show the on screen keyboard
    //         auto bounds = textField->getBoundingBox();
    //         if (bounds.containsPoint(touch->getLocation())) {
    //           auto textKeyboard = dynamic_cast<TextFieldTTF *>(event->getCurrentTarget());
    //           textKeyboard->attachWithIME();
    //         }
    //         auto bounds2 = button->getBoundingBox();
    //         if (bounds2.containsPoint(touch->getLocation())) {
    //           this->username = textField->getString();
    //           this->sendUserName(this->sio_client, textField->getString());
    //         }
    //         return true;
    //     }
    //     catch(std::bad_cast & err){
    //         return true;
    //     }
    // };
    //
    // this->_eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, textField);
    //
    // auto listener = EventListenerKeyboard::create();
    // listener->onKeyReleased = CC_CALLBACK_2(MatchSelectMenu::onKeyReleased, this);
    // Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);

    return true;
}

void MatchSelectMenu::sendUserName(network::SIOClient* client, const std::string& username)
{
    CCLOG("MatchSelectMenu::sendUserName called");

    client->emit("send_username", "{\"name\":\""+username+"\"}");
}

void MatchSelectMenu::sendExitEmit(network::SIOClient* client, const std::string& username)
{
    CCLOG("MatchSelectMenu::sendExitEmit called");

    client->emit("send_exit", "{\"name\":\""+username+"\"}");
}

void MatchSelectMenu::verifyUserName(network::SIOClient* client, const std::string& data)
{
    CCLOG("MatchSelectMenu::verifyUserName called");
    this->dq->printDebugWithNewMessage(data);

    if (data == "User Name Taken"){
      this->dq->printDebugWithNewMessage("User Name Taken");
    } else if (data == "User Name Added"){
      this->dq->printDebugWithNewMessage("User Name Added");
    }
}


void MatchSelectMenu::verifyPings(network::SIOClient* client, const std::string& data)
{
    CCLOG("MatchSelectMenu::verifyPings called");
    this->pings += 1;
}

void MatchSelectMenu::getUserNames(network::SIOClient* client, const std::string& data)
{
    CCLOG("MatchSelectMenu::getUserNames called");

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    Json::Reader reader;
    reader.parse(data, this->usernames);
    this->dq->printDebugWithNewMessage(this->usernames.asString());
    std::string users = this->usernames.asString();
    users.erase( 0, 1 ); // erase the first character
    users.erase( users.size() - 1 );

    int count = 0;
    for(std::string::size_type p0=0,p1=users.find(',');
          (p1!=std::string::npos || p0!=std::string::npos) && users.size() != 0;
          (p0 = ( p1 == std::string::npos) ? p1 : ++p1),  p1=users.find(',',p0) )
    {
      std::string::size_type user_end = (p1 == std::string::npos) ? users.size() : p1;
      std::string user = users.substr(p0+1, user_end-p0-2);

      this->dq->printDebugWithNewMessage(user);
      auto label = Label::createWithSystemFont(user, "Arial", 15.0);
      auto xpos = origin.x + (visibleSize.width - label->getContentSize().width)*0.5 + label->getContentSize().width * 0.5;
      auto ypos = visibleSize.height*(9-count)/10;
      label->setPosition(Vec2(xpos, ypos));
      this->addChild(label, 1);
      count++;
    }
}

void MatchSelectMenu::disconnectReset()
{
    CCLOG("MatchSelectMenu::disconnectReset called");
    Director::getInstance()->replaceScene(MainMenu::createScene());
}

void MatchSelectMenu::onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event)
{
    if(keyCode == cocos2d::EventKeyboard::KeyCode::KEY_BACK)
    {
      this->sendExitEmit(this->sio_client, this->username);
      Director::getInstance()->replaceScene(MainMenu::createScene());
    }
}
