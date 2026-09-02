#include "test/inc/signal_access_reporter.h"

namespace {
std::vector<std::string> signal_access_log;
std::vector<std::pair<std::string, bool>> assertion_log;
}

void ClearSignalAccessLog() {
  signal_access_log.clear();
}

void RecordSignalRead(const std::string &signal_name) {
  signal_access_log.push_back("READ: " + signal_name);
}

void RecordSignalWrite(const std::string &signal_name,
                       const std::string &value) {
  signal_access_log.push_back("WRITE: " + signal_name + " = " + value);
}

void ClearAssertionLog() {
  assertion_log.clear();
}

void RecordAssertionResult(const std::string &description, bool passed) {
  assertion_log.push_back(std::make_pair(description, passed));
}

const std::vector<std::pair<std::string, bool>> &AssertionLog() {
  return assertion_log;
}

const std::vector<std::string> &SignalAccessLog() {
  return signal_access_log;
}
