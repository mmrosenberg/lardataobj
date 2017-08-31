////////////////////////////////////////////////////////////////////////
/// \file  Simulation/SupernovaTruth.cxx
/// \brief Stores extra MC truth information that is recorded when
/// generating events using a time-dependent supernova neutrino spectrum
///
/// \author Steven Gardiner <sjgardiner@ucdavis.edu>
////////////////////////////////////////////////////////////////////////

#include "lardataobj/Simulation/SupernovaTruth.h"

namespace sim {

  SupernovaTruth::SupernovaTruth() : SupernovaTime(0.), Weight(0.),
    FluxAveragedCrossSection(0.), SamplingMode(kUnknownSupernovaSamplingMode) {}

  SupernovaTruth::SupernovaTruth(double time, double weight,
    double xs_avg, SupernovaSamplingMode_t sampling_mode)
    : SupernovaTime(time), Weight(weight), FluxAveragedCrossSection(xs_avg),
    SamplingMode(sampling_mode) {}

}
