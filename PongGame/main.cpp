#include "stdafx.h"

#include <iostream>
#include <CBLog/Logger.h>

#include "Game.h"

// "Run on Nvidia Graphic Processor" hack
extern "C" {
  _declspec(dllexport) unsigned int NvOptimusEnablement = 0x00000001;
}

using utf8strvec = std::vector<cb::utf8string>;

int WinMain(void*, void*, char*, int) {
  auto log = std::make_shared<cb::CLogger>();
  {
    auto fslog = std::wofstream(L"main.log", std::ios::out | std::ios::trunc);
    log->AddStream(std::make_shared<std::wofstream>(std::move(fslog)));
  }
  //log->AddStream({std::shared_ptr<cb::ostream>{}, &std::wcout});
  cb::CLogger::SetInstance(log);

  auto cmdvec = cb::strvector();
  {
    //auto utf8cmdvec = utf8strvec(argv, argv + argc);
    //auto func = [](cb::utf8string const& text) -> auto {return cb::fromUtf8(text); };
    //std::transform(utf8cmdvec.begin(), utf8cmdvec.end(), std::back_inserter(cmdvec), func);
  }

  auto app = pong::CGame(cmdvec);

  return app.Execute();
}
 