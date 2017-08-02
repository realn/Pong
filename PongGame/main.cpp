#include <vector>
#include <algorithm>
#include <iostream>

#include "App.h"

#include <CBStr/StringEx.h>
#include <CBSDL/Window.h>

using utf8strvec = std::vector<cb::utf8string>;

int main(int argc, char* argv[]) {
  auto cmdvec = cb::strvector();
  {
    auto utf8cmdvec = utf8strvec(argv, argv + argc);
    auto func = [](cb::utf8string const& text) -> auto {return cb::fromUtf8(text); };
    std::transform(utf8cmdvec.begin(), utf8cmdvec.end(), std::back_inserter(cmdvec), func);
  }

  auto app = pong::CApp(cmdvec);

  return app.Execute();
}
 