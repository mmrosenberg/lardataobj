#ifndef TRACKFITHITINFO_H
#define TRACKFITHITINFO_H

#include "larcoreobj/SimpleTypesAndConstants/geo_types.h"
#include "lardataobj/RecoBase/TrackingTypes.h"

namespace recob {

  using SVector5     = tracking::SVector5;
  using SMatrixSym55 = tracking::SMatrixSym55;

  /// \file  lardataobj/RecoBase/TrackFitHitInfo.h
  /// \class recob::TrackFitHitInfo
  ///
  /// \brief Object storing per-hit information from a track fit.
  ///
  /// \author  G. Cerati (FNAL, MicroBooNE)
  /// \date    2017
  /// \version 1.0
  ///
  /// This object stores the information related to each hit from a track fit.
  /// In particular it stores the 1D hit position measurement (TrackFitHitInfo#fHitMeas) and its squared uncertainty (TrackFitHitInfo#fHitMeasErr2),
  /// the ids of the wire where the hit is found (TrackFitHitInfo#fCryostatId,TrackFitHitInfo#fTpcId,TrackFitHitInfo#fPlaneId,TrackFitHitInfo#fWireId),
  /// the track parameters (TrackFitHitInfo#fTrackStatePar) and correspoding covariance matrix (TrackFitHitInfo#fTrackStateCov).
  /// The track parameters need to be defined at the wire location, and, in order to make and unbiased comparison of the hit and track parameters,
  /// they should be obtained without using this hit in the fit.
  ///

  class TrackFitHitInfo {
  public:
  TrackFitHitInfo(double aHitMeas, double aHitMeasErr2, const SVector5& aTrackStatePar, const SMatrixSym55& aTrackStateCov, const geo::WireID& aWireId)
    : fHitMeas(aHitMeas), fHitMeasErr2(aHitMeasErr2),
      fTrackStatePar(aTrackStatePar), fTrackStateCov(aTrackStateCov),
      fWireId(aWireId.Wire), fPlaneId(aWireId.Plane), fTpcId(aWireId.TPC), fCryostatId(aWireId.Cryostat) { }

    TrackFitHitInfo() {}
    
    /// hit position measurement
    double hitMeas()     const { return fHitMeas; }
    /// squared uncertainty of the hit position measurement
    double hitMeasErr2() const { return fHitMeasErr2; }
    
    /// track parameters
    const SVector5&     trackStatePar() const { return fTrackStatePar; }
    /// covariance matrix
    const SMatrixSym55& trackStateCov() const { return fTrackStateCov; }

    /// wire id where the hit is located
    geo::WireID WireId() const { return geo::WireID(fCryostatId,fTpcId,fPlaneId,fWireId); }
    
  private:
    float             fHitMeas;       ///< hit position measurement
    float             fHitMeasErr2;   ///< squared uncertainty of the hit position measurement
    SVector5          fTrackStatePar; ///< track parameters
    SMatrixSym55      fTrackStateCov; ///< covariance matrix
    unsigned int      fWireId;        ///< wire id where the hit is located
    unsigned int      fPlaneId;       ///< plane id where the hit is located
    unsigned int      fTpcId;         ///< tpc id where the hit is located
    unsigned int      fCryostatId;    ///< cryostat id where the hit is located
  };
  
}

#endif
