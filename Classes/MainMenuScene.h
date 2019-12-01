#pragma once

#include "cocos2d.h"
#include "DebugQueue.h"


class MainMenu : public cocos2d::Scene
{
public:
    DebugQueue* dq;
    Label* label;
    Label* label2;

    virtual bool init();
    static cocos2d::Scene* createScene();
    void onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
    bool onTouchBegan(Touch* touch, Event* event);


    CREATE_FUNC(MainMenu);
};
