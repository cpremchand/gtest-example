#include "test/inc/html_report_listener.h"

#include <chrono>
#include <cstdlib>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

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
    return "Reading the " + signal_name;
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

  std::string assertion_kind = "ASSERTION";
  if (text.find("Expected equality of these values") != std::string::npos ||
      text.find("Expected equality") != std::string::npos) {
    assertion_kind = "EXPECT_EQ";
  } else if (text.find("Expected: true") != std::string::npos ||
             text.find("Value of:") != std::string::npos) {
    assertion_kind = "EXPECT_TRUE";
  } else if (text.find("Expected: false") != std::string::npos) {
    assertion_kind = "EXPECT_FALSE";
  } else if (text.find("Which is:") != std::string::npos &&
             text.find("Expected: ") != std::string::npos) {
    assertion_kind = "EXPECT_FLOAT_EQ";
  } else if (text.find("Expected: ") != std::string::npos &&
             text.find("Actual: ") != std::string::npos) {
    assertion_kind = "EXPECT_EQ";
  }

  if (lines.size() < 4) {
    return assertion_kind + ": " + text;
  }

  std::string variable_name;
  std::string actual_value;
  std::string expected_value;

  for (std::size_t index = 0; index < lines.size(); ++index) {
    const std::string &current = lines[index];
    if (current.find("Which is:") != std::string::npos) {
      const std::string value = Trim(current.substr(current.find(":") + 1));
      if (index > 0) {
        const std::string &previous = lines[index - 1];
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
    return assertion_kind + ": Actual value of " + variable_name + " is " +
           actual_value + ", expecting it to be " + expected_value;
  }

  if (!actual_value.empty() && !expected_value.empty()) {
    return assertion_kind + ": Actual value is " + actual_value +
           ", expecting it to be " + expected_value;
  }

  return assertion_kind + ": " + text;
}

}  // namespace

HtmlReportListener::HtmlReportListener(const std::string &executable_name)
    : executable_name_(executable_name),
      tests_run_(0),
      tests_passed_(0),
      assertion_total_(0),
      assertions_passed_(0),
      next_step_number_(1),
      start_time_point_(std::chrono::steady_clock::now()),
      last_step_time_point_(start_time_point_),
      closed_(false) {
  const char *report_path = std::getenv("GTEST_HTML_REPORT");
  report_.open(report_path == nullptr ? "gtest-report.html" : report_path);
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
  const char *master_signal_list_name = std::getenv("MASTER_SIGNAL_LIST_NAME");
  const char *sw_version = std::getenv("SW_VERSION");
  const char *product_id = std::getenv("PRODUCT_ID");

  report_ << "<!doctype html><html><head><meta charset=\"utf-8\"><title>GTest report</title>"
          << "<style>"
          << "body{font-family:Arial,sans-serif;margin:2rem 5%;color:#1f2933;background:#f8fafc}"
          << "h1,h2,h3,h4,h5,h6{color:#1d4ed8;margin-top:1.25rem;margin-bottom:0.5rem;line-height:1.3}"
          << "h1,h2,h3{border-bottom:1px solid #dbe3ee}"
          << ".report-header{display:flex;align-items:center;gap:1rem;margin:1rem 0 1.5rem}"
          << ".logo{width:72px;height:auto;border:none;border-radius:12px;box-shadow:0 2px 8px rgba(31,41,55,0.12)}"
          << ".tableblock{margin-top:1rem}"
          << ".summary-table,.test-table{width:100%;border-collapse:collapse;margin:1rem 0;border:1px solid #d8dee7;background:#fff}"
          << ".summary-table th,.summary-table td,.test-table th,.test-table td{border:1px solid #d8dee7;padding:0.7rem;text-align:left;vertical-align:top}"
          << ".test-table th{background:#eef3ff}"
          << ".pass{color:#047857;font-weight:bold}"
          << ".fail{color:#b42318;font-weight:bold}"
          << "details{margin:0.45rem 0}"
          << "pre{white-space:pre-wrap;word-break:break-word;background:#f3f4f6;border:1px solid #e5e7eb;border-radius:4px;padding:0.75rem;margin:0.5rem 0 0;font-family:\"SFMono-Regular\",Consolas,monospace;line-height:1.4}"
          << "code{font-family:\"SFMono-Regular\",Consolas,monospace;background:#f3f4f6;padding:0.12rem 0.3rem;border-radius:3px}"
          << "</style></head><body>"
          << "<div id=\"content\"><div class=\"sect1\"><div class=\"report-header\">"
          << "<img class=\"logo\" src=\"" << kLogoDataUri << "\" alt=\"Project logo\" />"
          << "<h1>" << EscapeHtml(executable_name_) << "</h1></div>"
          << "<div class=\"tableblock\" style=\"text-align:left;\"><table rules=\"all\" width=\"100%\" frame=\"border\" cellspacing=\"0\" cellpadding=\"4\"><caption class=\"title\" style=\"text-align:left;\">Test Environment</caption>"
          << "<tbody><tr><td align=\"left\" valign=\"top\"><p class=\"table\">Parameter</p></td><td align=\"left\" valign=\"top\"><p class=\"table\">Value</p></td></tr>"
          << "<tr><td align=\"left\" valign=\"top\"><p class=\"table\">Environment</p></td><td align=\"left\" valign=\"top\"><p class=\"table\">" << EscapeHtml(EnvironmentStatus()) << "</p></td></tr>"
          << "<tr><td align=\"left\" valign=\"top\"><p class=\"table\">Start Time</p></td><td align=\"left\" valign=\"top\"><p class=\"table\">" << EscapeHtml(start_time_) << "</p></td></tr>"
          << "<tr><td align=\"left\" valign=\"top\"><p class=\"table\">Stop Time</p></td><td align=\"left\" valign=\"top\"><p class=\"table\">" << EscapeHtml(end_time_.empty() ? "not yet completed" : end_time_) << "</p></td></tr>"
          << "<tr><td align=\"left\" valign=\"top\"><p class=\"table\">Software Version</p></td><td align=\"left\" valign=\"top\"><p class=\"table\">" << EscapeHtml(sw_version == nullptr ? "unknown" : sw_version) << "</p></td></tr>"
          << "<tr><td align=\"left\" valign=\"top\"><p class=\"table\">Software DLL Name</p></td><td align=\"left\" valign=\"top\"><p class=\"table\">" << EscapeHtml(software_dll_name == nullptr ? "unknown" : software_dll_name) << "</p></td></tr>"
          << "<tr><td align=\"left\" valign=\"top\"><p class=\"table\">MSL Signal List Name</p></td><td align=\"left\" valign=\"top\"><p class=\"table\">" << EscapeHtml(master_signal_list_name == nullptr ? "unknown" : master_signal_list_name) << "</p></td></tr>"
          << "<tr><td align=\"left\" valign=\"top\"><p class=\"table\">Product ID</p></td><td align=\"left\" valign=\"top\"><p class=\"table\">" << EscapeHtml(product_id == nullptr ? "" : product_id) << "</p></td></tr>"
          << "<tr><td align=\"left\" valign=\"top\"><p class=\"table\">Tested By</p></td><td align=\"left\" valign=\"top\"><p class=\"table\">" << EscapeHtml(tested_by == nullptr ? "unknown" : tested_by) << "</p></td></tr>"
          << "</tbody></table></div></div>\n";
  report_.flush();
}

void HtmlReportListener::OnTestStart(const ::testing::TestInfo &) {
  assertion_results_.clear();
  ClearSignalAccessLog();
  ClearAssertionLog();
  last_step_time_point_ = std::chrono::steady_clock::now();
}

void HtmlReportListener::OnTestPartResult(
    const ::testing::TestPartResult &test_part_result) {
  const char *summary = test_part_result.summary();
  if (summary != nullptr && summary[0] != '\0') {
    assertion_results_.push_back(
        std::make_pair(std::string(summary), test_part_result.passed()));
  }
}

void HtmlReportListener::OnTestEnd(const ::testing::TestInfo &test_info) {
  const ::testing::TestResult *const result = test_info.result();
  assertion_results_.clear();

  if (!AssertionLog().empty()) {
    assertion_results_.assign(AssertionLog().begin(), AssertionLog().end());
  } else if (result != nullptr) {
    for (int index = 0; index < result->total_part_count(); ++index) {
      const ::testing::TestPartResult &part = result->GetTestPartResult(index);
      const char *summary = part.summary();
      if (summary != nullptr && summary[0] != '\0') {
        assertion_results_.push_back(
            std::make_pair(std::string(summary), part.passed()));
      }
    }
  }

  ++tests_run_;
  const bool passed = result != nullptr && result->Passed();
  if (passed) {
    ++tests_passed_;
  }

  for (std::vector<std::pair<std::string, bool>>::const_iterator result_entry =
           assertion_results_.begin();
       result_entry != assertion_results_.end(); ++result_entry) {
    ++assertion_total_;
    if (result_entry->second) {
      ++assertions_passed_;
    }
  }

  if (!report_.is_open()) {
    return;
  }

  std::ostringstream row_html;
  const std::string description = std::string("Suite: ") +
                                 test_info.test_suite_name() +
                                 " | Case: " + test_info.name();

  const std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
  const double suite_seconds =
      std::chrono::duration<double>(now - last_step_time_point_).count();
  row_html << "<tr><td>" << next_step_number_++ << "</td><td>"
           << FormatElapsedSeconds(suite_seconds) << "</td><td>"
           << EscapeHtml(description) << "</td><td></td></tr>\n";
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

  if (passed && assertion_results_.empty()) {
    const std::chrono::steady_clock::time_point result_now =
        std::chrono::steady_clock::now();
    const double result_seconds =
        std::chrono::duration<double>(result_now - last_step_time_point_).count();
    row_html << "<tr><td>" << next_step_number_++ << "</td><td>"
             << FormatElapsedSeconds(result_seconds) << "</td><td>"
             << EscapeHtml("All assertions passed")
             << "</td><td class=\"pass\">(Passed)</td></tr>\n";
    last_step_time_point_ = result_now;
  }

  if (!assertion_results_.empty()) {
    for (std::vector<std::pair<std::string, bool>>::const_iterator result =
             assertion_results_.begin();
         result != assertion_results_.end(); ++result) {
      const std::chrono::steady_clock::time_point result_now =
          std::chrono::steady_clock::now();
      const double result_seconds =
          std::chrono::duration<double>(result_now - last_step_time_point_).count();
      row_html << "<tr><td>" << next_step_number_++ << "</td><td>"
               << FormatElapsedSeconds(result_seconds) << "</td><td>"
               << EscapeHtml(FormatAssertionStep(result->first))
               << "</td><td class=\"" << (result->second ? "pass\">(Passed)" : "fail\">(Failed)")
               << "</td></tr>\n";
      last_step_time_point_ = result_now;
    }
  }

  test_rows_.push_back(row_html.str());
}

void HtmlReportListener::OnTestProgramEnd(const ::testing::UnitTest &) {
  if (!report_.is_open() || closed_) {
    return;
  }

  end_time_ = ExecutionTimestamp();
  const unsigned int assertion_failed = assertion_total_ - assertions_passed_;
  const double pass_percent =
      assertion_total_ == 0 ? 0.0 : (100.0 * assertions_passed_) / assertion_total_;
  const double fail_percent =
      assertion_total_ == 0 ? 0.0 : (100.0 * assertion_failed) / assertion_total_;

  std::ostringstream pass_summary_stream;
  pass_summary_stream << std::fixed << std::setprecision(2) << pass_percent;
  std::ostringstream fail_summary_stream;
  fail_summary_stream << std::fixed << std::setprecision(2) << fail_percent;

  report_ << "<div class=\"sect1\"><h2 id=\"_test_results\">Test Results</h2>"
          << "<div class=\"sectionbody\"><p>Pass: <span class=\"pass\">" << assertions_passed_
          << "/" << assertion_total_ << " (" << pass_summary_stream.str()
          << "%)</span></p>"
          << "<p>Fail: <span class=\"fail\">" << assertion_failed << "/" << assertion_total_
          << " (" << fail_summary_stream.str() << "%)</span></p>"
          << "<div class=\"tableblock\" style=\"text-align:left;\"><table class=\"test-table\" style=\"text-align:left;\"><caption class=\"title\" style=\"text-align:left;\">Table 2. Test Steps</caption><thead><tr><th>Step Num</th><th>Time Taken (secs)</th><th>Description</th><th>Status</th></tr></thead><tbody>\n";

  for (std::vector<std::string>::const_iterator row = test_rows_.begin();
       row != test_rows_.end(); ++row) {
    report_ << *row;
  }
  report_ << "</tbody></table></div></div></div>";

  const auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(
      std::chrono::steady_clock::now() - start_time_point_);
  const int hours = static_cast<int>(elapsed.count() / 3600);
  const int minutes = static_cast<int>((elapsed.count() % 3600) / 60);
  const int seconds = static_cast<int>(elapsed.count() % 60);
  std::ostringstream duration;
  duration << std::setw(2) << std::setfill('0') << hours << ":"
           << std::setw(2) << std::setfill('0') << minutes << ":"
           << std::setw(2) << std::setfill('0') << seconds;
  (void)duration;

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
  std::time_t now = std::time(nullptr);
  std::tm utc_time{};
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
