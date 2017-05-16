#ifndef MCSFitResult_h
#define MCSFitResult_h

namespace recob {
  /**
   * @brief Class storing the result of the Maximum Likelihood fit of Multiple Coulomb Scattering angles between segments within a Track or Trajectory.
   *
   * Class storing the result of the Maximum Likelihood fit of Multiple Coulomb Scattering angles between segments within a Track or Trajectory.
   * It stores: the resulting momentum, momentum uncertainty, and best likelihood value (both for fwd and bwd fit);
   * the vectors of segment (radiation) lengths and of scattering angles; the PID hypothesis used in the fit.
   */
  class MCSFitResult {
  public:
    MCSFitResult()=default;
    MCSFitResult(int pid, double momFwd, double momFwdUnc, double llhdFwd, double momBwd, double momBwdUnc, double llhdBwd, const std::vector<double>& radlengths, const std::vector<double>& angles)
      : pid_(pid), momFwd_(momFwd), momFwdUnc_(momFwdUnc), llhdFwd_(llhdFwd), momBwd_(momBwd), momBwdUnc_(momBwdUnc), llhdBwd_(llhdBwd), radlengths_(radlengths), angles_(angles) {}
    //
    int    particleIdHyp()     const { return pid_;   }
    double fwdMomentum()       const { return momFwd_;   }
    double fwdMomUncertainty() const { return momFwdUnc_;}
    double fwdLogLikelihood()  const { return llhdFwd_;  }
    double bwdMomentum()       const { return momBwd_;   }
    double bwdMomUncertainty() const { return momBwdUnc_;}
    double bwdLogLikelihood()  const { return llhdBwd_;  }
    const std::vector<double>& segmentRadLengths() const { return radlengths_;}
    const std::vector<double>& scatterAngles()  const { return angles_; }
    //
    bool   isBestFwd()          const { return llhdFwd_<llhdBwd_;                    }
    double bestMomentum()       const { return isBestFwd() ? momFwd_    : momBwd_;   }
    double bestMomUncertainty() const { return isBestFwd() ? momFwdUnc_ : momBwdUnc_;}
    double bestLogLikelihood()  const { return isBestFwd() ? llhdFwd_   : llhdBwd_;  }
    double deltaLogLikelihood() const { return std::abs(llhdFwd_-llhdBwd_); }
    //
  private:
    double pid_;
    double momFwd_;
    double momFwdUnc_;
    double llhdFwd_;
    double momBwd_;
    double momBwdUnc_;
    double llhdBwd_;
    std::vector<double> radlengths_;
    std::vector<double> angles_;
  };
}

#endif
