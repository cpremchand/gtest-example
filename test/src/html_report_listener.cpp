#include "test/inc/html_report_listener.h"

#include <cstdlib>
#include <ctime>
#include <regex>
#include <sstream>
#include <string>

#include <sys/utsname.h>

namespace {
const char kLogoDataUri[] =
    "data:image/gif;base64,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";
std::string Trim(const std::string &value) {
  const std::string whitespace = " \t\r\n";
  const std::size_t start = value.find_first_not_of(whitespace);
  if (start == std::string::npos) {
    return "";
  }
  const std::size_t end = value.find_last_not_of(whitespace);
  return value.substr(start, end - start + 1);
}

std::string FormatSignalLogStep(const std::string &signal_entry) {
  const std::string write_prefix = "WRITE: ";
  const std::string read_prefix = "READ: ";

  if (signal_entry.compare(0, write_prefix.size(), write_prefix) == 0) {
    const std::string body = signal_entry.substr(write_prefix.size());
    const std::size_t split = body.find(" = ");
    if (split == std::string::npos) {
      return "Setting the signal to the requested value";
    }
    const std::string signal_name = Trim(body.substr(0, split));
    const std::string signal_value = Trim(body.substr(split + 3));
    return "Setting the " + signal_name + " to " + signal_value;
  }

  if (signal_entry.compare(0, read_prefix.size(), read_prefix) == 0) {
    const std::string signal_name = Trim(signal_entry.substr(read_prefix.size()));
    return "Reading the " + signal_name + " from signal";
  }

  return signal_entry;
}

std::string FormatAssertionStep(const std::string &summary) {
  const std::string text = Trim(summary);
  std::vector<std::string> lines;
  std::istringstream stream(text);
  std::string line;
  while (std::getline(stream, line)) {
    const std::string trimmed = Trim(line);
    if (!trimmed.empty()) {
      lines.push_back(trimmed);
    }
  }

  if (lines.size() < 4) {
    return text;
  }

  std::string variable_name;
  std::string actual_value;
  std::string expected_value;

  for (std::size_t index = 0; index < lines.size(); ++index) {
    const std::string &current = lines[index];
    if (current.find("Which is:") != std::string::npos) {
      const std::string value = Trim(current.substr(current.find(":") + 1));
      const std::size_t previous_index = index == 0 ? 0 : index - 1;
      if (previous_index < lines.size()) {
        const std::string &previous = lines[previous_index];
        if (previous.find("Which is:") == std::string::npos &&
            previous.find("Expected equality") == std::string::npos &&
            previous.find("Value of:") == std::string::npos &&
            previous.find("Actual") == std::string::npos &&
            previous.find("Expected") == std::string::npos &&
            previous.find("input_") != std::string::npos) {
          variable_name = previous;
          actual_value = value;
        }
      }

      if (index >= 2) {
        const std::string &previous_candidate = lines[index - 1];
        if (previous_candidate.find("Which is:") == std::string::npos &&
            previous_candidate.find("input_") == std::string::npos &&
            previous_candidate.find("Actual") == std::string::npos &&
            previous_candidate.find("Expected") == std::string::npos) {
          expected_value = value;
        }
      }
    }
  }

  if (variable_name.empty()) {
    for (std::size_t index = 0; index < lines.size(); ++index) {
      const std::string &current = lines[index];
      if (current.find("input_") != std::string::npos ||
          current.find("value") != std::string::npos) {
        variable_name = current;
      }
      if (current.find("Which is:") != std::string::npos) {
        const std::string value = Trim(current.substr(current.find(":") + 1));
        if (actual_value.empty()) {
          actual_value = value;
        } else if (expected_value.empty()) {
          expected_value = value;
        }
      }
    }
  }

  if (!variable_name.empty() && !actual_value.empty() && !expected_value.empty()) {
    return "Actual value of " + variable_name + " is " + actual_value +
           ", expecting it to be " + expected_value;
  }

  if (!actual_value.empty() && !expected_value.empty()) {
    return "Actual value is " + actual_value + ", expecting it to be " + expected_value;
  }

  return text;
}

}  // namespace

HtmlReportListener::HtmlReportListener(const std::string &executable_name)
    : executable_name_(executable_name), tests_run_(0), tests_passed_(0),
      next_step_number_(1), start_time_point_(std::chrono::steady_clock::now()),
      last_step_time_point_(start_time_point_), closed_(false) {
  const char *report_path = std::getenv("GTEST_HTML_REPORT");
  report_.open(report_path == NULL ? "gtest-report.html" : report_path);
  start_time_ = ExecutionTimestamp();
}

HtmlReportListener::~HtmlReportListener() {
  if (report_.is_open() && !closed_) {
    report_ << "</tbody></table></div></body></html>\n";
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
  const char *product_id = std::getenv("PRODUCT_ID");
  report_ << "<!doctype html><html><head><meta charset=\"utf-8\"><title>GTest report</title>"
          << "<style>"
          << "body{font-family:Georgia,serif;margin:1em 5% 1em 5%;color:#20252b;background:#f8f9fb}"
          << "h1,h2,h3,h4,h5,h6{color:#527bbd;margin-top:1.2em;margin-bottom:0.5em;line-height:1.3}"
          << "h1,h2,h3{border-bottom:2px solid silver}"
          << ".report-header{display:flex;align-items:center;gap:1.2rem;margin:1.5rem 0}"
          << ".logo{width:100px;height:auto;border:none}"
          << ".tableblock{margin-top:1rem}"
          << ".summary-table,.test-table{width:100%;border-collapse:collapse;margin:1rem 0;border:1px solid #dfe4e8;background:#fff}"
          << ".summary-table th,.summary-table td,.test-table th,.test-table td{border:1px solid #dfe4e8;padding:0.7rem;text-align:left;vertical-align:top}"
          << ".test-table th{background:#eef3f8}"
          << ".pass{color:#087f5b;font-weight:bold}"
          << ".fail{color:#c92a2a;font-weight:bold}"
          << ".paragraph{margin:0.75rem 0}"
          << ".assertion-list,.signal-list{margin:0.5rem 0 0 1.25rem;padding:0}"
          << "details{margin:0.45rem 0}"
          << "pre{white-space:pre-wrap;word-break:break-word;background:#f6f8fa;border:1px solid #e1e5eb;border-radius:4px;padding:0.75rem;margin:0.5rem 0 0;font-family:\"Courier New\",Courier,monospace;line-height:1.4}"
          << "code{font-family:\"Courier New\",Courier,monospace;background:#f3f4f6;padding:0.12rem 0.3rem;border-radius:3px}"
          << "</style></head><body class=\"article\">"
          << "<div id=\"content\"><div class=\"sect1\"><div class=\"report-header\">"
          << "<img class=\"logo\" src=\"" << kLogoDataUri << "\" alt=\"Logo\" />"
          << "<div><h1>" << EscapeHtml(executable_name_) << "</h1></div></div>"
          << "<div class=\"tableblock\" style=\"text-align:left;\"><table rules=\"all\" width=\"100%\" frame=\"border\" cellspacing=\"0\" cellpadding=\"4\"><caption class=\"title\" style=\"text-align:left;\">Table 1. Test Environment</caption>"
          << "<tbody><tr><td align=\"left\" valign=\"top\"><p class=\"table\">Parameter</p></td><td align=\"left\" valign=\"top\"><p class=\"table\">Value</p></td></tr>"
          << "<tr><td align=\"left\" valign=\"top\"><p class=\"table\">Environment</p></td><td align=\"left\" valign=\"top\"><p class=\"table\">" << EscapeHtml(EnvironmentStatus()) << "</p></td></tr>"
          << "<tr><td align=\"left\" valign=\"top\"><p class=\"table\">Start Time</p></td><td align=\"left\" valign=\"top\"><p class=\"table\">" << EscapeHtml(start_time_) << "</p></td></tr>"
          << "<tr><td align=\"left\" valign=\"top\"><p class=\"table\">Stop Time</p></td><td align=\"left\" valign=\"top\"><p class=\"table\">" << EscapeHtml(start_time_) << "</p></td></tr>"
          << "<tr><td align=\"left\" valign=\"top\"><p class=\"table\">Software Version</p></td><td align=\"left\" valign=\"top\"><p class=\"table\">" << EscapeHtml(sw_version == NULL ? "unknown" : sw_version) << "</p></td></tr>"
          << "<tr><td align=\"left\" valign=\"top\"><p class=\"table\">Software DLL Name</p></td><td align=\"left\" valign=\"top\"><p class=\"table\">" << EscapeHtml(software_dll_name == NULL ? "unknown" : software_dll_name) << "</p></td></tr>"
          << "<tr><td align=\"left\" valign=\"top\"><p class=\"table\">MSL Signal List Name</p></td><td align=\"left\" valign=\"top\"><p class=\"table\">" << EscapeHtml(master_signal_list_name == NULL ? "unknown" : master_signal_list_name) << "</p></td></tr>"
          << "<tr><td align=\"left\" valign=\"top\"><p class=\"table\">Product ID</p></td><td align=\"left\" valign=\"top\"><p class=\"table\">" << EscapeHtml(product_id == NULL ? "" : product_id) << "</p></td></tr>"
          << "<tr><td align=\"left\" valign=\"top\"><p class=\"table\">Duration (Hours:Mins:Secs)</p></td><td align=\"left\" valign=\"top\"><p class=\"table\">00:00:00</p></td></tr>"
          << "<tr><td align=\"left\" valign=\"top\"><p class=\"table\">Tested By</p></td><td align=\"left\" valign=\"top\"><p class=\"table\">" << EscapeHtml(tested_by == NULL ? "unknown" : tested_by) << "</p></td></tr>"
          << "</tbody></table></div></div>\n";
  report_.flush();
}

void HtmlReportListener::OnTestStart(const ::testing::TestInfo &) {
  assertion_failures_.clear();
  ClearSignalAccessLog();
  last_step_time_point_ = std::chrono::steady_clock::now();
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

  std::ostringstream row_html;
  const std::string description = std::string("Suite: ") +
                                 test_info.test_suite_name() +
                                 " | Case: " + test_info.name();

  const std::chrono::steady_clock::time_point now =
      std::chrono::steady_clock::now();
  const double suite_seconds =
      std::chrono::duration<double>(now - last_step_time_point_).count();
  row_html << "<tr><td>" << next_step_number_++ << "</td><td>"
           << FormatElapsedSeconds(suite_seconds) << "</td><td>"
           << EscapeHtml(description) << "</td><td class=\""
           << (passed ? "pass\">(Passed)" : "fail\">(Failed)")
           << "</td></tr>\n";
  last_step_time_point_ = now;

  if (!SignalAccessLog().empty()) {
    for (std::vector<std::string>::const_iterator access =
             SignalAccessLog().begin();
         access != SignalAccessLog().end(); ++access) {
      const std::chrono::steady_clock::time_point access_now =
          std::chrono::steady_clock::now();
      const double access_seconds =
          std::chrono::duration<double>(access_now - last_step_time_point_).count();
      row_html << "<tr><td>" << next_step_number_++ << "</td><td>"
               << FormatElapsedSeconds(access_seconds) << "</td><td>"
               << EscapeHtml(FormatSignalLogStep(*access)) << "</td><td></td></tr>\n";
      last_step_time_point_ = access_now;
    }
  }

  if (!assertion_failures_.empty()) {
    for (std::vector<std::string>::const_iterator failure =
             assertion_failures_.begin();
         failure != assertion_failures_.end(); ++failure) {
      const std::chrono::steady_clock::time_point failure_now =
          std::chrono::steady_clock::now();
      const double failure_seconds =
          std::chrono::duration<double>(failure_now - last_step_time_point_).count();
      row_html << "<tr><td>" << next_step_number_++ << "</td><td>"
               << FormatElapsedSeconds(failure_seconds) << "</td><td>"
               << EscapeHtml(FormatAssertionStep(*failure))
               << "</td><td class=\"fail\">(Failed)</td></tr>\n";
      last_step_time_point_ = failure_now;
    }
  }

  test_rows_.push_back(row_html.str());
}

void HtmlReportListener::OnTestProgramEnd(const ::testing::UnitTest &) {
  if (!report_.is_open() || closed_) {
    return;
  }

  end_time_ = ExecutionTimestamp();
  const unsigned int tests_failed = tests_run_ - tests_passed_;
  const double pass_percent =
      tests_run_ == 0 ? 0.0 : (100.0 * tests_passed_) / tests_run_;
  const double fail_percent =
      tests_run_ == 0 ? 0.0 : (100.0 * tests_failed) / tests_run_;

  report_ << "<div class=\"sect1\"><h2 id=\"_test_results\">Test Results</h2>"
          << "<div class=\"sectionbody\"><p>Pass: <span class=\"pass\">" << tests_passed_
          << "/" << tests_run_ << " (" << pass_percent << "%)</span></p>"
          << "<p>Fail: <span class=\"fail\">" << tests_failed << "/" << tests_run_
          << " (" << fail_percent << "%)</span></p>"
          << "<div class=\"tableblock\" style=\"text-align:left;\"><table class=\"test-table\" style=\"text-align:left;\"><caption class=\"title\" style=\"text-align:left;\">Table 2. Test Steps</caption><thead><tr><th>Step Num</th><th>Time Taken (secs)</th><th>Description</th><th>Status</th></tr></thead><tbody>\n";
  for (std::vector<std::string>::const_iterator row = test_rows_.begin();
       row != test_rows_.end(); ++row) {
    report_ << *row;
  }
  report_ << "</tbody></table></div></div></div>";

  const std::time_t start_epoch = std::time(NULL) - 1;
  const std::time_t end_epoch = std::time(NULL);
  const std::time_t duration_seconds = std::max<long long>(0, end_epoch - start_epoch);
  const int hours = duration_seconds / 3600;
  const int minutes = (duration_seconds % 3600) / 60;
  const int seconds = duration_seconds % 60;
  std::ostringstream duration;
  duration << std::setw(2) << std::setfill('0') << hours << ":"
           << std::setw(2) << std::setfill('0') << minutes << ":"
           << std::setw(2) << std::setfill('0') << seconds;

  report_.seekp(0, std::ios::end);
  report_.flush();
  closed_ = true;
  report_ << "</div></body></html>\n";
  report_.flush();
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

std::string HtmlReportListener::FormatElapsedSeconds(double seconds) {
  std::ostringstream value;
  if (seconds < 0.0) {
    seconds = 0.0;
  }
  value << std::fixed << std::setprecision(3) << seconds;
  return value.str();
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