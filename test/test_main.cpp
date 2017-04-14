#include <vector>
#include <string>
#include <sstream>

#include "test_util.hpp"

int main(int argc, const char *argv[]) {
  std::vector<std::string> test_set;

  const std::string run_test_opt = "--run_test=";
  for (int i = 1; i < argc; ++i) {
    if (strncmp(argv[i], run_test_opt.c_str(), run_test_opt.size()) == 0) {
      std::stringstream opt(argv[i] + run_test_opt.size());
      while (opt.good()) {
        std::string testname;
        getline(opt, testname, ',');
        test_set.push_back(testname);
      }
    }
  }
  kaguya_test_util::TestRunner::instance().set_test_set(test_set);

  bool test_result = kaguya_test_util::TestRunner::instance().execute();
  return test_result ? 0 : -1;
}
