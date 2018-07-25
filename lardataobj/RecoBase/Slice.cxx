#include "lardataobj/RecoBase/Slice.h"
#include "larcoreobj/SimpleTypesAndConstants/PhysicalConstants.h"
#include "messagefacility/MessageLogger/MessageLogger.h"

#include <iomanip>
#include <iostream>

namespace recob{

  //----------------------------------------------------------------------
  Slice::Slice()
    :
    Slice(util::kBogusI, util::kBogusF)
  {
  }

  //----------------------------------------------------------------------
  Slice::Slice(int id, float aspectratio)
    : fID(id)
    , fAspectRatio(aspectratio)
  {
  }

}
