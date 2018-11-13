#ifndef MCSFitResult_h
#define MCSFitResult_h

namespace recob {
  /**
   * @file  lardataobj/RecoBase/MCSFitResult.h
   * @class recob::MCSFitResult
   *
   * @brief Class storing the result of the Maximum Likelihood fit of Multiple Coulomb Scattering angles between segments within a Track or Trajectory.
   *
   * Class storing the result of the Maximum Likelihood fit of Multiple Coulomb Scattering angles between segments within a Track or Trajectory.
   * It stores: the resulting momentum, momentum uncertainty, and best likelihood value (both for fwd and bwd fit);
   * the vectors of segment (radiation) lengths and of scattering angles; the PID hypothesis used in the fit.
   *
   * @author  G. Cerati (FNAL, MicroBooNE)
   * @date    2017
   * @version 1.0
   */
  class MCSFitResult {
  public:
    MCSFitResult()=default;
    MCSFitResult(int pid, float momFwd, float momFwdUnc, float llhdFwd, float momBwd, float momBwdUnc, float llhdBwd, const std::vector<float>& radlengths, const std::vector<float>& angles)
      : pid_(pid), momFwd_(momFwd), momFwdUnc_(momFwdUnc), llhdFwd_(llhdFwd), momBwd_(momBwd), momBwdUnc_(momBwdUnc), llhdBwd_(llhdBwd), radlengths_(radlengths), angles_(angles) {}

    /// particle id hypothesis used in the fit
    int    particleIdHyp()     const { return pid_;   }

    /// momentum value from fit assuming a forward track direction
    float fwdMomentum()       const { return momFwd_;   }

    /// momentum uncertainty from fit assuming a forward track direction
    float fwdMomUncertainty() const { return momFwdUnc_;}

    /// minimum negative log likelihood value from fit assuming a forward track direction
    float fwdLogLikelihood()  const { return llhdFwd_;  }

    /// momentum value from fit assuming a backward track direction
    float bwdMomentum()       const { return momBwd_;   }

    /// momentum uncertainty from fit assuming a backward track direction
    float bwdMomUncertainty() const { return momBwdUnc_;}

    /// minimum negative log likelihood value from fit assuming a backward track direction
    float bwdLogLikelihood()  const { return llhdBwd_;  }

    /// vector of radiation lengths of the segments used in the fit
    const std::vector<float>& segmentRadLengths() const { return radlengths_;}

    /// vector of angles between the segments used in the fit
    const std::vector<float>& scatterAngles()  const { return angles_; }

    /// determines best fit direction based on minumum log likelihood between forward and backward fit
    bool   isBestFwd()          const { return llhdFwd_<llhdBwd_;                    }

    /// momentum for best direction fit
    float bestMomentum()       const { return isBestFwd() ? momFwd_    : momBwd_;   }

    /// momentum uncertainty for best direction fit
    float bestMomUncertainty() const { return isBestFwd() ? momFwdUnc_ : momBwdUnc_;}

    /// negative log likelihood for best direction fit
    float bestLogLikelihood()  const { return isBestFwd() ? llhdFwd_   : llhdBwd_;  }

    /// difference in log likelihood between forward and backward fit (absolute value)
    float deltaLogLikelihood() const { return std::abs(llhdFwd_-llhdBwd_); }
    //
  private:
    int   pid_;       ///< particle id hypothesis used in the fit
    float momFwd_;    ///< momentum value from fit assuming a forward track direction
    float momFwdUnc_; ///< momentum uncertainty from fit assuming a forward track direction
    float llhdFwd_;   ///< minimum negative log likelihood value from fit assuming a forward track direction
    float momBwd_;    ///< momentum value from fit assuming a backward track direction
    float momBwdUnc_; ///< momentum uncertainty from fit assuming a backward track direction
    float llhdBwd_;   ///< minimum negative log likelihood value from fit assuming a backward track direction
    std::vector<float> radlengths_; ///< vector of radiation lengths of the segments used in the fit
    std::vector<float> angles_;     ///< vector of angles between the segments used in the fit
  };
}

#endif
