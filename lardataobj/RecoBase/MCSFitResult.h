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
    MCSFitResult(int pid, double momFwd, double momFwdUnc, double llhdFwd, double momBwd, double momBwdUnc, double llhdBwd, const std::vector<double>& radlengths, const std::vector<double>& angles)
      : pid_(pid), momFwd_(momFwd), momFwdUnc_(momFwdUnc), llhdFwd_(llhdFwd), momBwd_(momBwd), momBwdUnc_(momBwdUnc), llhdBwd_(llhdBwd), radlengths_(radlengths), angles_(angles) {}

    /// particle id hypothesis used in the fit
    int    particleIdHyp()     const { return pid_;   }

    /// momentum value from fit assuming a forward track direction
    double fwdMomentum()       const { return momFwd_;   }

    /// momentum uncertainty from fit assuming a forward track direction
    double fwdMomUncertainty() const { return momFwdUnc_;}

    /// minimum negative log likelihood value from fit assuming a forward track direction
    double fwdLogLikelihood()  const { return llhdFwd_;  }

    /// momentum value from fit assuming a backward track direction
    double bwdMomentum()       const { return momBwd_;   }

    /// momentum uncertainty from fit assuming a backward track direction
    double bwdMomUncertainty() const { return momBwdUnc_;}

    /// minimum negative log likelihood value from fit assuming a backward track direction
    double bwdLogLikelihood()  const { return llhdBwd_;  }

    /// vector of radiation lengths of the segments used in the fit
    const std::vector<double>& segmentRadLengths() const { return radlengths_;}

    /// vector of angles between the segments used in the fit
    const std::vector<double>& scatterAngles()  const { return angles_; }

    /// determines best fit direction based on minumum log likelihood between forward and backward fit
    bool   isBestFwd()          const { return llhdFwd_<llhdBwd_;                    }

    /// momentum for best direction fit
    double bestMomentum()       const { return isBestFwd() ? momFwd_    : momBwd_;   }

    /// momentum uncertainty for best direction fit
    double bestMomUncertainty() const { return isBestFwd() ? momFwdUnc_ : momBwdUnc_;}

    /// negative log likelihood for best direction fit
    double bestLogLikelihood()  const { return isBestFwd() ? llhdFwd_   : llhdBwd_;  }

    /// difference in log likelihood between forward and backward fit (absolute value)
    double deltaLogLikelihood() const { return std::abs(llhdFwd_-llhdBwd_); }
    //
  private:
    double pid_;       ///< particle id hypothesis used in the fit
    double momFwd_;    ///< momentum value from fit assuming a forward track direction
    double momFwdUnc_; ///< momentum uncertainty from fit assuming a forward track direction
    double llhdFwd_;   ///< minimum negative log likelihood value from fit assuming a forward track direction
    double momBwd_;    ///< momentum value from fit assuming a backward track direction
    double momBwdUnc_; ///< momentum uncertainty from fit assuming a backward track direction
    double llhdBwd_;   ///< minimum negative log likelihood value from fit assuming a backward track direction
    std::vector<double> radlengths_; ///< vector of radiation lengths of the segments used in the fit
    std::vector<double> angles_;     ///< vector of angles between the segments used in the fit
  };
}

#endif
