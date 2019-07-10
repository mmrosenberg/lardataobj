#include "TriggerData.h"

#include <stdexcept>

namespace raw {

  //****************************************************
  bool Trigger::Triggered(const unsigned char bit) const
  //****************************************************
  {

    if(bit>32) {
      throw std::invalid_argument("\n\nCannot access bit higher than 32!\n");
    }

    return ( (fTriggerBits >> bit) & 0x1);

  }

}
