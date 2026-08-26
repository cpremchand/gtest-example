#ifndef TEST_INC_SIGNAL_ACCESS_REPORTER_H_
#define TEST_INC_SIGNAL_ACCESS_REPORTER_H_

#include <sstream>
#include <string>
#include <vector>

void ClearSignalAccessLog();
void RecordSignalRead(const std::string &signal_name);
void RecordSignalWrite(const std::string &signal_name,
                       const std::string &value);

template <typename ValueType>
void RecordSignalWrite(const std::string &signal_name, const ValueType &value) {
  std::ostringstream value_text;
  value_text << value;
  RecordSignalWrite(signal_name, value_text.str());
}

const std::vector<std::string> &SignalAccessLog();

#endif  // TEST_INC_SIGNAL_ACCESS_REPORTER_H_
