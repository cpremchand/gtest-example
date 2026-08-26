#include <string>

#include <gtest/gtest.h>

#include "test/inc/html_report_listener.h"

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  ::testing::TestEventListeners &listeners =
      ::testing::UnitTest::GetInstance()->listeners();
  listeners.Append(new HtmlReportListener(
      argc > 0 ? std::string(argv[0]) : std::string("GTest")));
  return RUN_ALL_TESTS();
}