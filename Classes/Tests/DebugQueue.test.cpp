#include <gtest/gtest.h>
#include "../DebugQueue.h"

class DebugQueueTest : public::testing::Test {
  void SetUp() {}
  void TearDown() {}

public:
  DebugQueue dq;
};

TEST_F(DebugQueueTest, IsEmptyInitially) {
  EXPECT_EQ(dq.size(), 0);
}

TEST_F(DebugQueueTest, LoadOneElementIntoEmpty) {
  dq.enqueue("test string");
  ASSERT_EQ(dq.size(),1);
}

TEST_F(DebugQueueTest, OverLoadQueue) {
  for(int i=0; i < 128; i++) {
    dq.enqueue("test string");
  }
  ASSERT_EQ(dq.size(),128);
}

TEST_F(DebugQueueTest, IsEmptyAfter) {
  dq.enqueue("test string");
  dq.empty();
  EXPECT_EQ(dq.size(), 0);
}

TEST_F(DebugQueueTest, ReturnsNthElement) {
  std::string test_string = "test string";
  dq.enqueue(test_string);
  EXPECT_EQ(dq.getNthElement(1), test_string);
}

TEST_F(DebugQueueTest, EnqueueFillsElementsInOrder) {
  std::string test_string1 = "test string1";
  std::string test_string2 = "test string2";
  dq.enqueue(test_string1);
  dq.enqueue(test_string2);
  EXPECT_EQ(dq.getNthElement(2), test_string1);
  EXPECT_EQ(dq.getNthElement(1), test_string2);
}

TEST_F(DebugQueueTest, EnqueueOverFillsElementsInOrder) {
  for (int i = 1; i <= dq._LIMIT_*2; i++) {
    dq.enqueue(std::string("test string") + std::to_string(i));
  }
  int k = 1+dq._LIMIT_;
  for (int i = dq._LIMIT_; i > 0; i--) {
    std::string test_value = std::string("test string")+std::to_string(k);
    std::string real_value = dq.getNthElement(i);
    EXPECT_EQ(real_value, test_value);
    k++;
  }
}

TEST_F(DebugQueueTest, EmptyDoesNotReturnNthElement) {
  for (int i = 1; i <= dq._LIMIT_*2; i++) {
    dq.enqueue(std::string("test string") + std::to_string(i));
  }
  dq.empty();
  EXPECT_EQ(dq.getNthElement(1), "DebugQueue n > size");
}
