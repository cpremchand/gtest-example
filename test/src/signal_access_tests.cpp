// Example signal wrappers and tests for the HTML signal access trace.
#include <map>
#include <string>

#include "test/inc/common.h"
#include "test/inc/signal_access_reporter.h"

namespace {
std::map<std::string, float> signal_values;

float read_sim_signal(const std::string &signal_name) {
  RecordSignalRead(signal_name);
  return signal_values[signal_name];
}

void write_sim_signal(const std::string &signal_name, float value) {
  RecordSignalWrite(signal_name, value);
  signal_values[signal_name] = value;
}
}  // namespace

TEST(SignalAccessTests, ReportsReadWriteOrder) {
  write_sim_signal("TargetSpeed", 1500.0F);
  float target_speed = read_sim_signal("TargetSpeed");

  EXPECT_FLOAT_EQ_REPORT(1500.0F, target_speed);
}

TEST(SignalAccessTests, ReportsMultipleSignalsInCodeOrder) {
  write_sim_signal("InputA", 10.0F);
  write_sim_signal("InputB", 20.0F);
  float input_a = read_sim_signal("InputA");
  float input_b = read_sim_signal("InputB");

  // The signal access log should preserve the execution order of each read/write
  // call, and the value read back should match the value previously written.
  EXPECT_FLOAT_EQ_REPORT(15.0F, input_a);
  EXPECT_FLOAT_EQ_REPORT(20.0F, input_b);
}
