#include "network/SocketIO.h"

class DelegateSocketIO
    : public cocos2d::network::SocketIO::SIODelegate
{
    virtual void onClose(cocos2d::network::SIOClient* client) override {};
    virtual void onError(cocos2d::network::SIOClient* client, const std::string& data) override {};
    virtual void onConnect(cocos2d::network::SIOClient* client) override {
      CCLOG("SIODelegate onConnect fired");
      std::string examplestring = "tom";

      client->emit("emittest", examplestring);
    };
    virtual void onMessage(cocos2d::network::SIOClient* client, const std::string& data) override { CCLOG("SIODelegate onMessage fired with data: %s", data.c_str()); };
    virtual void fireEventToScript(cocos2d::network::SIOClient* client, const std::string& eventName, const std::string& data) override { CCLOG("SIODelegate event '%s' fired with data: %s", eventName.c_str(), data.c_str()); };
};
