#include "MainMenuScene.h"
#include "DuelComputerScene.h"
#include "LoginMenuScene.h"



USING_NS_CC;

Scene* MainMenu::createScene()
{
    // 'scene' is an autorelease object
    auto scene = MainMenu::create();

    return scene;
}

bool MainMenu::init()
{
    if ( !Scene::init() )
    {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    this->dq = new DebugQueue(this, origin, visibleSize);


    auto label = Label::createWithSystemFont("Duel Computer", "Arial", 15);
    auto xpos = origin.x + (visibleSize.width - label->getContentSize().width)*0.5 + label->getContentSize().width * 0.5;
    auto ypos = visibleSize.height*2/3;
    label->setPosition(Vec2(xpos, ypos));
    this->addChild(label, 1);
    this->label = label;

    auto label2 = Label::createWithSystemFont("Duel Human", "Arial", 15);
    auto xpos2 = origin.x + (visibleSize.width - label2->getContentSize().width)*0.5 + label2->getContentSize().width * 0.5;
    auto ypos2 = visibleSize.height*1/3;
    label2->setPosition(Vec2(xpos2, ypos2));
    this->addChild(label2, 2);
    this->label2 = label2;


    auto listener = EventListenerKeyboard::create();
    listener->onKeyReleased = CC_CALLBACK_2(MainMenu::onKeyReleased, this);
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);


    auto touchListener = EventListenerTouchOneByOne::create();
    touchListener->onTouchBegan = CC_CALLBACK_2(MainMenu::onTouchBegan, this);
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener, this);

    return true;
}

bool MainMenu::onTouchBegan(Touch* touch, Event* event)
{
  auto bounds = this->label->getBoundingBox();
  auto bounds2 = this->label2->getBoundingBox();

   if (bounds.containsPoint(touch->getLocation())){
     Director::getInstance()->replaceScene(DuelComputer::createScene());
   }
   if (bounds2.containsPoint(touch->getLocation())){
     Director::getInstance()->replaceScene(LoginMenu::createScene());
   }
   return true;
}

void MainMenu::onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event)
{
    if(keyCode == cocos2d::EventKeyboard::KeyCode::KEY_BACK)
    {
      Director::getInstance()->end();
    }
}
