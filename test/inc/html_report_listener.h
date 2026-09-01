#ifndef TEST_INC_HTML_REPORT_LISTENER_H_
#define TEST_INC_HTML_REPORT_LISTENER_H_

#include <fstream>
#include <string>
#include <vector>

#include <gtest/gtest.h>

#include "test/inc/signal_access_reporter.h"

class HtmlReportListener : public ::testing::EmptyTestEventListener {
 public:
  explicit HtmlReportListener(const std::string &executable_name);
  ~HtmlReportListener() override;

  void OnTestProgramStart(const ::testing::UnitTest &unit_test) override;
  void OnTestStart(const ::testing::TestInfo &test_info) override;
  void OnTestPartResult(
      const ::testing::TestPartResult &test_part_result) override;
  void OnTestEnd(const ::testing::TestInfo &test_info) override;
  void OnTestProgramEnd(const ::testing::UnitTest &unit_test) override;

 private:
  static std::string EscapeHtml(const std::string &value);
  static std::string EnvironmentStatus();
  static std::string ExecutionTimestamp();

  std::ofstream report_;
  std::string executable_name_;
  std::vector<std::string> assertion_failures_;
  std::vector<std::string> test_rows_;
  std::string start_time_;
  std::string end_time_;
  unsigned int tests_run_;
  unsigned int tests_passed_;
  bool closed_;
};

#endif  // TEST_INC_HTML_REPORT_LISTENER_H_