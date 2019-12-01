#pragma once

#include "cocos2d.h"
#include "DebugQueue.h"
#include "ui/CocosGUI.h"
#include "network/SocketIO.h"
#include "/usr/include/nlohmann/json.hpp"

using json = nlohmann::json;


class LoginMenu : public cocos2d::Scene
{
public:
    DebugQueue* dq;
    cocos2d::network::SIOClient* sio_client;
    std::string username;
    json usernames;

    int pings;

    virtual bool init();
    static cocos2d::Scene* createScene();
    void onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
    void sendUserName(network::SIOClient* client, const std::string& username);
    void verifyUserName(network::SIOClient* client, const std::string& data);
    void verifyPings(network::SIOClient* client, const std::string& data);
    void getUserNames(network::SIOClient* client, const std::string& data);
    void disconnectReset();
    void sendExitEmit(network::SIOClient* client, const std::string& username);

    CREATE_FUNC(LoginMenu);
};
