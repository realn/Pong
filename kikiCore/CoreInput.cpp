#include "stdafx.h"
#include "CoreInput.h"

namespace core {
  CInput::CInput() {}

  void CInput::AddDevice(std::shared_ptr<IInputDevice> device) {
    mDevices.push_back(device);
  }

  bool CInput::SendEvent(CInputDeviceEvent && event) {
    mEvents.push_back(event);
    return true;
  }

}
