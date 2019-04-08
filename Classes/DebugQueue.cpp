#include "DebugQueue.h"

DebugQueue::DebugQueue(cocos2d::Scene* scene, Vec2 origin, Size visibleSize){
  this->scene = scene;
  this-> origin = origin;
  this->visibleSize = visibleSize;
}

DebugQueue::~DebugQueue(){
  delete[] this->_dq;
}

void DebugQueue::printDebugWithNewMessage(std::string message){
  this->enqueue(message);
  this->currentLabel->removeFromParentAndCleanup(false);
  this->currentLabel = this->returnLabel();
  this->currentLabel->setPosition(Vec2(this->origin.x + this->currentLabel->getContentSize().width*0.5,
                          this->visibleSize.height - this->currentLabel->getContentSize().height*0.5));
  this->scene->addChild(this->currentLabel, 1);
}


int DebugQueue::size(){
  return this->_size;
}

void DebugQueue::enqueue(std::string input){
  int loop_size = this->_size;
  if (loop_size == this->_LIMIT_){
    loop_size -= 1;
  }

  for (int i = loop_size; i > 0; i--) {
    this->_dq[i] = this->_dq[i-1];
  }

  this->_dq[0] = input;

  if (this->_size < this->_LIMIT_) {
    this->_size += 1;
  }
}

void DebugQueue::empty(){
  this->_size = 0;
  delete []this->_dq;
  this->_dq = new std::string[this->_LIMIT_];
}

Label* DebugQueue::returnLabel(){
  std::string retval;
  for (int i = this->_size-1; i >= 0; i--){
    retval += this->_dq[i];
    retval += "\n";
  }
  Label* label = Label::createWithTTF(retval,"fonts/Marker Felt.ttf", 5);
  return label;
}

std::string DebugQueue::getNthElement(int n) {
  std::string retval;
  if (n <= _size) {
    retval = this->_dq[n-1];
  }
  else{
    retval = "DebugQueue n > size";
  }
  return retval;
}
