#include "LoginMenuScene.h"
#include <iostream>
#include "ui/CocosGUI.h"
#include "DelegateSocketIO.h"
#include "MainMenuScene.h"
#include "MatchSelectMenuScene.h"


USING_NS_CC;

Scene* LoginMenu::createScene()
{
    // 'scene' is an autorelease object
    auto scene = LoginMenu::create();

    return scene;
}

bool LoginMenu::init()
{
    if ( !Scene::init() )
    {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    this->dq = new DebugQueue(this, origin, visibleSize);


    auto label = Label::createWithSystemFont("Enter User Name", "Arial", 15.0);
    auto xpos = origin.x + (visibleSize.width - label->getContentSize().width)*0.5 + label->getContentSize().width * 0.5;
    auto ypos = visibleSize.height*9/10;
    label->setPosition(Vec2(xpos, ypos));
    this->addChild(label, 1);

    // Create a text field
    TextFieldTTF* textField = cocos2d::TextFieldTTF::textFieldWithPlaceHolder("Click here to type",
            cocos2d::Size(120,35),TextHAlignment::LEFT , "Arial", 15.0);

    auto xposTextField = origin.x + (visibleSize.width - 120)*0.5 + 63;
    auto yposTextField = ypos - label->getContentSize().height*2;
    textField->setPosition(xposTextField, yposTextField);
    textField->setColorSpaceHolder(Color3B::GREEN);
    //textField->setDelegate(this);

    this->addChild(textField);

    auto button = Label::createWithSystemFont("Submit", "Arial", 18);
    button->setPosition(Vec2(origin.x + visibleSize.width*0.5, yposTextField-40));

    //button->addTouchEventListener(CC_CALLBACK_2(LoginMenu::buttonEventHandler, this));
    this->addChild(button);


    // Add a touch handler to our textfield that will show a keyboard when touched
    auto touchListener = EventListenerTouchOneByOne::create();
    auto dq = this->dq;
    dq->printDebugWithNewMessage("");

    this->pings = 1;
    DelegateSocketIO* delegate = new DelegateSocketIO();
    delegate->setDebugQueue(dq);

    cocos2d::network::SocketIO* socketIO = cocos2d::network::SocketIO::getInstance();
    cocos2d::network::SIOClient* sio_client = socketIO->connect("http://android-card-game.herokuapp.com:80", * delegate);
    this->sio_client = sio_client;

    auto scheduler = Director::getInstance()->getScheduler();

    scheduler->schedule([sio_client](float dt) {
      sio_client->emit("ping", "ping");
    }, this,10.0f, kRepeatForever, 0.0f, false, "sendPings");

    scheduler->schedule([sio_client, this](float dt) {
      if (this->pings == 0){
        disconnectReset();
      }
      this->pings = 0;
    }, this, 12.0f, kRepeatForever, 0.0f, false, "checkPings");

    sio_client->on("verify_username", CC_CALLBACK_2(LoginMenu::verifyUserName, this));
    sio_client->on("ping", CC_CALLBACK_2(LoginMenu::verifyPings, this));
    sio_client->on("get_usernames", CC_CALLBACK_2(LoginMenu::getUserNames, this));


    touchListener->onTouchBegan = [textField, button, dq, this, sio_client](cocos2d::Touch* touch, cocos2d::Event * event) -> bool {
        try {
            // Show the on screen keyboard
            auto bounds = textField->getBoundingBox();
            if (bounds.containsPoint(touch->getLocation())) {
              auto textKeyboard = dynamic_cast<TextFieldTTF *>(event->getCurrentTarget());
              textKeyboard->attachWithIME();
            }
            auto bounds2 = button->getBoundingBox();
            if (bounds2.containsPoint(touch->getLocation())) {
              this->username = textField->getString();
              this->sendUserName(sio_client, textField->getString());
            }
            return true;
        }
        catch(std::bad_cast & err){
            return true;
        }
    };

    this->_eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, textField);

    auto listener = EventListenerKeyboard::create();
    listener->onKeyReleased = CC_CALLBACK_2(LoginMenu::onKeyReleased, this);
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);

    return true;
}

void LoginMenu::sendUserName(network::SIOClient* client, const std::string& username)
{
    CCLOG("LoginMenu::sendUserName called");

    client->emit("send_username", "{\"name\":\""+username+"\"}");
}

void LoginMenu::sendExitEmit(network::SIOClient* client, const std::string& username)
{
    CCLOG("LoginMenu::sendExitEmit called");

    client->emit("send_exit", "{\"name\":\""+username+"\"}");
}

void LoginMenu::verifyUserName(network::SIOClient* client, const std::string& data)
{
    CCLOG("LoginMenu::verifyUserName called");
    //this->dq->printDebugWithNewMessage(data);

    if (data == "\"User Name Taken\""){
      this->dq->printDebugWithNewMessage("User Name Taken");
    } else if (data == "\"User Name Added\""){
      this->dq->printDebugWithNewMessage("Replace Scene");
      this->dq->printDebugWithNewMessage(this->username);
      std::string usernameValue = this->username;
      Director::getInstance()->replaceScene(MatchSelectMenu::createScene(usernameValue));
    }
}


void LoginMenu::verifyPings(network::SIOClient* client, const std::string& data)
{
    CCLOG("LoginMenu::verifyPings called");
    this->pings += 1;
}

void LoginMenu::getUserNames(network::SIOClient* client, const std::string& data)
{
    CCLOG("LoginMenu::getUserNames called");
    this->dq->printDebugWithNewMessage(data);
    this->usernames = json::parse(data);
    if (this->usernames.type_name() != "string") {
      for (json::iterator it = this->usernames.begin(); it != this->usernames.end(); ++it) {
        this->dq->printDebugWithNewMessage(it.key());
      }
    }
}

void LoginMenu::disconnectReset()
{
    CCLOG("LoginMenu::disconnectReset called");
    Director::getInstance()->replaceScene(MainMenu::createScene());
}

void LoginMenu::onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event)
{
    if(keyCode == cocos2d::EventKeyboard::KeyCode::KEY_BACK)
    {
      this->sendExitEmit(this->sio_client, this->username);
      Director::getInstance()->replaceScene(MainMenu::createScene());
    }
}
