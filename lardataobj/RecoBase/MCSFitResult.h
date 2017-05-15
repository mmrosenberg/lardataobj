#ifndef MCSFitResult_h
#define MCSFitResult_h

namespace recob {
  class MCSFitResult {
  public:
    MCSFitResult() {}
  MCSFitResult(int pid, double momFwd, double momFwdUnc, double llhdFwd, double momBwd, double momBwdUnc, double llhdBwd, std::vector<double>& radlengths, std::vector<double>& angles)
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
    double deltaLogLikelihood() const { return llhdFwd_-llhdBwd_; }
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
