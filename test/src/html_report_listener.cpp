#include "test/inc/html_report_listener.h"

#include <cstdlib>
#include <ctime>
#include <sstream>

#include <sys/utsname.h>

HtmlReportListener::HtmlReportListener(const std::string &executable_name)
    : executable_name_(executable_name), tests_run_(0), tests_passed_(0),
      closed_(false) {
  const char *report_path = std::getenv("GTEST_HTML_REPORT");
  report_.open(report_path == NULL ? "gtest-report.html" : report_path);
}

HtmlReportListener::~HtmlReportListener() {
  if (report_.is_open() && !closed_) {
    report_ << "</tbody></table></body></html>\n";
  }
}

void HtmlReportListener::OnTestProgramStart(const ::testing::UnitTest &) {
  if (!report_.is_open()) {
    return;
  }
  const char *tested_by = std::getenv("TESTED_BY");
  const char *software_dll_name = std::getenv("SOFTWARE_DLL_NAME");
  const char *master_signal_list_name =
      std::getenv("MASTER_SIGNAL_LIST_NAME");
  const char *sw_version = std::getenv("SW_VERSION");
  report_ << "<!doctype html><html><head><meta charset=\"utf-8\"><title>GTest report</title>"
          << "<style>body{font:15px sans-serif;margin:2rem;color:#20252b}"
          << ".banner{background:#17324d;color:#fff;padding:1rem 1.25rem}"
          << ".meta{display:flex;gap:1rem;flex-wrap:wrap;margin:1rem 0}"
          << ".meta div{border:1px solid #ccd3da;padding:.75rem;min-width:12rem}"
          << "table{border-collapse:collapse;width:100%}th,td{border-bottom:1px solid #dfe4e8;padding:.65rem;text-align:left}"
          << ".pass{color:#087f5b;font-weight:bold}.fail{color:#c92a2a;font-weight:bold}"
          << ".summary{margin-top:1rem;font-weight:bold}</style></head><body>"
          << "<header class=\"banner\"><h1>GoogleTest: "
          << EscapeHtml(executable_name_) << "</h1></header><section class=\"meta\">"
          << "<div><strong>Tested By</strong><br>"
          << EscapeHtml(tested_by == NULL ? "unknown" : tested_by)
          << "</div><div><strong>Execution Timestamp</strong><br>"
          << ExecutionTimestamp() << "</div><div><strong>Environment</strong><br>"
          << EscapeHtml(EnvironmentStatus())
          << "</div><div><strong>Software DLL Name</strong><br>"
          << EscapeHtml(software_dll_name == NULL ? "unknown"
                                                   : software_dll_name)
          << "</div><div><strong>Master Signal List Name</strong><br>"
          << EscapeHtml(master_signal_list_name == NULL
                            ? "unknown"
                            : master_signal_list_name)
          << "</div><div><strong>Software Version</strong><br>"
          << EscapeHtml(sw_version == NULL ? "unknown" : sw_version)
          << "</div></section><table><thead><tr><th>Test Suite</th><th>Test Case</th><th>Outcome</th></tr></thead><tbody>\n";
  report_.flush();
}

void HtmlReportListener::OnTestStart(const ::testing::TestInfo &) {
  assertion_failures_.clear();
}

void HtmlReportListener::OnTestPartResult(
    const ::testing::TestPartResult &test_part_result) {
  if (test_part_result.failed()) {
    assertion_failures_.push_back(test_part_result.summary());
  }
}

void HtmlReportListener::OnTestEnd(const ::testing::TestInfo &test_info) {
  ++tests_run_;
  const bool passed = test_info.result()->Passed();
  if (passed) {
    ++tests_passed_;
  }
  if (!report_.is_open()) {
    return;
  }
  report_ << "<tr><td>" << EscapeHtml(test_info.test_suite_name())
          << "</td><td>" << EscapeHtml(test_info.name()) << "</td><td class=\""
      << (passed ? "pass\">PASS" : "fail\">FAIL")
      << "</td></tr>\n";
  report_ << "<tr><td colspan=\"3\"><details><summary>Assertion details</summary>";
  if (assertion_failures_.empty()) {
    report_ << "<p>No failed assertions.</p>";
  } else {
    report_ << "<ul>";
    for (std::vector<std::string>::const_iterator failure =
             assertion_failures_.begin();
         failure != assertion_failures_.end(); ++failure) {
      report_ << "<li><pre>" << EscapeHtml(*failure) << "</pre></li>";
    }
    report_ << "</ul></details></td></tr>\n";
  }
  if (assertion_failures_.empty()) {
    report_ << "</details></td></tr>\n";
  }
  report_.flush();
}

void HtmlReportListener::OnTestProgramEnd(const ::testing::UnitTest &) {
  if (!report_.is_open() || closed_) {
    return;
  }
  report_ << "</tbody></table><div class=\"summary\">Total Count: "
          << tests_run_ << " | PASS Count : " << tests_passed_ << " | FAIL Count : "
          << (tests_run_ - tests_passed_) << "</div></body></html>\n";
  report_.flush();
  closed_ = true;
}

std::string HtmlReportListener::EscapeHtml(const std::string &value) {
  std::string escaped;
  for (std::string::const_iterator character = value.begin();
       character != value.end(); ++character) {
    switch (*character) {
      case '&': escaped += "&amp;"; break;
      case '<': escaped += "&lt;"; break;
      case '>': escaped += "&gt;"; break;
      case '"': escaped += "&quot;"; break;
      case '\'': escaped += "&#39;"; break;
      default: escaped += *character;
    }
  }
  return escaped;
}

std::string HtmlReportListener::ExecutionTimestamp() {
  std::time_t now = std::time(NULL);
  std::tm utc_time;
  gmtime_r(&now, &utc_time);
  char timestamp[32];
  std::strftime(timestamp, sizeof(timestamp), "%Y-%m-%dT%H:%M:%SZ", &utc_time);
  return timestamp;
}

std::string HtmlReportListener::EnvironmentStatus() {
  struct utsname system_info;
  if (uname(&system_info) != 0) {
    return "Linux environment unavailable";
  }
  std::ostringstream status;
  status << system_info.sysname << " " << system_info.release << " ("
         << system_info.machine << ")";
  return status.str();
}