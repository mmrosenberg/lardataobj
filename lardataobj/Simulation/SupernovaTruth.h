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
      SupernovaTruth();
      SupernovaTruth(double time, double weight,
       double xs_avg, SupernovaSamplingMode_t sampling_mode);

      /// @brief Arrival time of the supernova neutrino (seconds)
      /// @details The arrival time is measured relative to the supernova
      /// core bounce (defined to be t = 0), not the start of the detector
      /// readout window.
      double SupernovaTime;

      /// @brief Statistical weight for this neutrino vertex.
      /// @details The statistical weight will always be unity unless
      /// a biased sampling method was used. In general, the weight is
      /// given by the likelihood ratio @f$f(E_{\nu}, t_{SN})
      /// / g(E_{\nu}, t_{SN})@f$ where @f$ f(E_nu, t_SN) @f$ is the unbiased
      /// probability density, and @f$ g(E_{\nu}, t_{SN})@f$ is the actual
      /// probability density that was used for sampling.
      double Weight;

      /// @brief Flux-averaged total cross section (fm<sup>2</sup>,
      /// average is taken over all energies and times for all defined
      /// MARLEY reactions) used by MARLEY to generate this neutrino vertex
      double FluxAveragedCrossSection;

      /// @brief Method used to sample the supernova neutrino's energy
      /// and arrival time
      SupernovaSamplingMode_t SamplingMode;
  };

}
#endif
