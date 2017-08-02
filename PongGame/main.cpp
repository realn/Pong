#include <vector>
#include <algorithm>
#include <iostream>

#include "App.h"

#include <CBStr/StringEx.h>
#include <CBSDL/Window.h>

using utf8strvec = std::vector<cb::utf8string>;

class Test {
  int mX;
public:
  Test(int x) : mX(x) {
    std::cout << "Test: " << mX << std::endl;
  }
  Test(Test const& other) : mX(other.mX) {
    std::cout << "Copy test " << mX << std::endl;
  }
  Test(Test&& other) : mX(0) { 
    std::swap(mX, other.mX);
    std::cout << "Move test " << mX << std::endl;
  }
  ~Test() {
    std::cout << "Delete test: " << mX << std::endl;
  }
};

int main(int argc, char* argv[]) {
  auto cmdvec = cb::strvector();
  {
    auto utf8cmdvec = utf8strvec(argv, argv + argc);
    auto func = [](cb::utf8string const& text) -> auto {return cb::fromUtf8(text); };
    std::transform(utf8cmdvec.begin(), utf8cmdvec.end(), std::back_inserter(cmdvec), func);
  }

  auto test = Test(1);

  return 0;
}
 