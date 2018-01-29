#ifndef VERTEXASSNMETA_H
#define VERTEXASSNMETA_H

namespace recob {

  /**
   * @file  lardataobj/RecoBase/VertexAssnMeta.h
   * @class recob::VertexAssnMeta
   *
   * @brief Class storing the meta-data for track-vertex association: status, propagation distance, impact parameter, impact parameter error, chi2.
   *
   * The status is meant to describe if and how the track was used in the vertex fit.
   * The values of the parameters (propagation distance, impact parameter, impact parameter error, chi2) should be computed in an unbiased way,
   * i.e. with respect to the vertex re-fitted without using the specific associated track.
   * A method is also provided to retrieve the impact parameter significance.
   *
   * @author  G. Cerati (FNAL, MicroBooNE)
   * @date    2017
   * @version 1.0
   */

  class VertexAssnMeta {
  public:
    enum VertexAssnStatus { Undefined, NotUsedInFit, RejectedByFit, IncludedInFit };
    VertexAssnMeta() { status_ = Undefined; }
    VertexAssnMeta(float pD, float iP, float iPErr, float c2, VertexAssnStatus st)
      : propDist_(pD), impactParam_(iP), impactParamErr_(iPErr), chi2_(c2), status_(st) {}
    float propDist()           const { return propDist_; }
    float impactParam()        const { return impactParam_; }
    float impactParamErr()     const { return impactParamErr_; }
    float impactParamSig()     const { return impactParam_/impactParamErr_; }
    float chi2()               const { return chi2_; }
    VertexAssnStatus status() const { return status_; }
    void updateStatus(const VertexAssnStatus& newstatus) { status_ = newstatus; }
  private:
    float propDist_;
    float impactParam_;
    float impactParamErr_;
    float chi2_;
    VertexAssnStatus status_;
  };

}

#endif
