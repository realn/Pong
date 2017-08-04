#include "stdafx.h"
#include "App.h"

#include <iostream>
#include <CBLog/Logger.h>

using utf8strvec = std::vector<cb::utf8string>;

int main(int argc, char* argv[]) {
  auto log = std::make_shared<cb::CLogger>();
  log->AddStream({std::shared_ptr<cb::ostream>{}, &std::wcout});
  cb::CLogger::SetInstance(log);

  auto cmdvec = cb::strvector();
  {
    auto utf8cmdvec = utf8strvec(argv, argv + argc);
    auto func = [](cb::utf8string const& text) -> auto {return cb::fromUtf8(text); };
    std::transform(utf8cmdvec.begin(), utf8cmdvec.end(), std::back_inserter(cmdvec), func);
  }

  auto app = pong::CApp(cmdvec);

  return app.Execute();
}
 