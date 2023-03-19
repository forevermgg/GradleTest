//
// Created by centforever on 2023/3/19.
//

#ifndef GRADLE_TEST_TESTDATA_H
#define GRADLE_TEST_TESTDATA_H

#include <map>
#include <mutex>
#include <unordered_map>
#include <vector>

class TestData : public std::enable_shared_from_this<TestData> {
 public:
  TestData();
  ~TestData();
  void TestExec();
  int test = 0;
};

#endif  // GRADLE_TEST_TESTDATA_H
