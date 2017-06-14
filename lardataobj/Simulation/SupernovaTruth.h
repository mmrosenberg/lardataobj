////////////////////////////////////////////////////////////////////////
/// \file  SupernovaTruth.h
/// \brief Stores extra MC truth information that is recorded when
/// generating events using a time-dependent supernova neutrino spectrum
///
/// \author Steven Gardiner <sjgardiner@ucdavis.edu>
////////////////////////////////////////////////////////////////////////

#ifndef SupernovaTruth_h
#define SupernovaTruth_h

namespace sim {

  enum SupernovaSamplingMode_t {
    kUnknownSupernovaSamplingMode = 0,  ///< Unknown sampling mode
    kUnbiased,   ///< Sample directly from cross-section weighted spectrum
    kUniformTime, ///< Arrival times were sampled uniformly
    kUniformEnergy, ///< Neutrino energies were sampled uniformly
    kSupernovaSamplingModeMax ///< Max value of enum for iteration
  };

  class SupernovaTruth {
    public:
      //SupernovaTruth();
      //SupernovaTruth(double time, double weight,
      //  SupernovaSamplingMode_t sampling_mode);
      inline SupernovaTruth() : SNTime(0.), Weight(0.),
        SamplingMode(kUnknownSupernovaSamplingMode) {}
      inline SupernovaTruth(double time, double weight,
        SupernovaSamplingMode_t sampling_mode) : SNTime(time), Weight(weight),
        SamplingMode(sampling_mode) {}

      /// @brief Arrival time of the supernova neutrino (seconds)
      /// @details The arrival time is measured relative to the start of the
      /// supernova, not the start of the detector readout window.
      double SNTime;

      /// @brief Event weight for the supernova neutrino
      /// @details The weight will be one unless the event was generated
      /// using a biased sampling method. In general, the weight is
      /// given by @f$cross section(E_{\nu}) * flux(E_{\nu}, t_{SN})
      /// / g(E_{\nu}, t_{SN})@f$ where @f$ g(E_nu, t_SN) @f$ is the true
      /// distribution that was sampled.
      double Weight;

      /// @brief Method used to sample the supernova neutrino's energy
      /// and arrival time
      SupernovaSamplingMode_t SamplingMode; 
  };

}
#endif
