#ifndef TRACKVERTEXMETA_H
#define TRACKVERTEXMETA_H

namespace recob {

  class TrackVertexMeta {
  public:
    enum TrackVertexStatus { Undefined, NotUsedInFit, RejectedByFit, IncludedInFit };
    TrackVertexMeta() { status_ = Undefined; }
    TrackVertexMeta(float pD, float iP, float iPErr, float c2, TrackVertexStatus st)
      : propDist_(pD), impactParam_(iP), impactParamErr_(iPErr), chi2_(c2), status_(st) {}
    float propDist()           const { return propDist_; }
    float impactParam()        const { return impactParam_; }
    float impactParamErr()     const { return impactParamErr_; }
    float impactParamSig()     const { return impactParam_/impactParamErr_; }
    float chi2()               const { return chi2_; }
    TrackVertexStatus status() const { return status_; }
    void updateStatus(const TrackVertexStatus newstatus) { status_ = newstatus; }
  private:
    float propDist_;
    float impactParam_;
    float impactParamErr_;
    float chi2_;
    TrackVertexStatus status_;
  };
  
}

#endif
