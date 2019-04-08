#ifndef __DEBUGQUEUE_H__
#define __DEBUGQUEUE_H__
#include <string>
#include "cocos2d.h"
USING_NS_CC;

class DebugQueue {
public:
  static const int _LIMIT_ = 28;
private:
  std::string* _dq = new std::string[_LIMIT_];
  int _size = 0;
  Label* currentLabel = Label::createWithTTF("!","fonts/Marker Felt.ttf", 5);;
  cocos2d::Scene* scene;
  Vec2 origin;
  Size visibleSize;
public:
  DebugQueue(cocos2d::Scene* scene, Vec2 origin, Size visibleSize);
  ~DebugQueue();
  int size();
  void enqueue(std::string input);
  void empty();
  std::string getNthElement(int n);
  Label* returnLabel();
  void printDebugWithNewMessage(std::string message);
};

#endif
