// Fixtured test cases
#ifndef TEST_INC_FIXTURE_TESTS_H_
#define TEST_INC_FIXTURE_TESTS_H_

#include "test/inc/common.h"

using ::testing::Test;

class MathsTest : public Test {
 protected:
  int counter;
  float a, b;
  EasyMaths* maths;

  void SetUp() override {
    counter = 0;
    a = 5.0f;
    b = 2.0f;
    maths = new EasyMaths(a, b, &counter);
  }

  void TearDown() override {
    delete maths;
    maths = nullptr;
  }
};

#endif  // TEST_INC_FIXTURE_TESTS_H_
