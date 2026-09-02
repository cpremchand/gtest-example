cmake_minimum_required(VERSION 3.10)
project(gtest_example LANGUAGES CXX)

find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

link_libraries(${GTEST_LIBRARIES} gtest pthread)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_INCLUDE_CURRENT_DIR True)
set(CMAKE_CXX_FLAGS "-fprofile-arcs -ftest-coverage")
set(CMAKE_EXE_LINKER_FLAGS "-lgcov")

set(SIMPLE "runSimpleTests")
add_executable(${SIMPLE}
    "test/src/simple_tests.cpp"
    "test/src/test_main.cpp"
    "test/src/html_report_listener.cpp"
    "test/src/signal_access_reporter.cpp"
    "code/src/easy_math.cpp")

set(FAILING "runFailingTests")
add_executable(${FAILING}
    "test/src/failing_tests.cpp"
    "test/src/test_main.cpp"
    "test/src/html_report_listener.cpp"
    "test/src/signal_access_reporter.cpp"
    "code/src/easy_math.cpp")

set(FIXTURE "runFixtureTests")
add_executable(${FIXTURE}
    "test/src/fixture_tests.cpp"
    "test/src/test_main.cpp"
    "test/src/html_report_listener.cpp"
    "test/src/signal_access_reporter.cpp"
    "code/src/easy_math.cpp")

set(PARAM "runParamTests")
add_executable(${PARAM}
    "test/src/parametric_tests.cpp"
    "test/src/test_main.cpp"
    "test/src/html_report_listener.cpp"
    "test/src/signal_access_reporter.cpp"
    "code/src/easy_math.cpp")

set(SIGNAL_ACCESS "runSignalAccessTests")
add_executable(${SIGNAL_ACCESS}
    "test/src/signal_access_tests.cpp"
    "test/src/test_main.cpp"
    "test/src/html_report_listener.cpp"
    "test/src/signal_access_reporter.cpp"
    "code/src/easy_math.cpp")
