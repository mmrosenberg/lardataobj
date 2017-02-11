#ifndef TRACKFITHITINFO_H
#define TRACKFITHITINFO_H

#include "larcoreobj/SimpleTypesAndConstants/geo_types.h"
#include "lardataobj/RecoBase/TrackingTypes.h"

namespace recob {

  using SVector5     = tracking::SVector5;
  using SMatrixSym55 = tracking::SMatrixSym55;
  
  class TrackFitHitInfo {
  public:
  TrackFitHitInfo(double aHitMeas, double aHitMeasErr2, const SVector5& aTrackStatePar, const SMatrixSym55& aTrackStateCov, const geo::WireID& aWireId)
    : fHitMeas(aHitMeas), fHitMeasErr2(aHitMeasErr2),
      fTrackStatePar(aTrackStatePar), fTrackStateCov(aTrackStateCov),
      fWireId(aWireId.Wire), fPlaneId(aWireId.Plane), fTpcId(aWireId.TPC), fCryostatId(aWireId.Cryostat) { }

    TrackFitHitInfo() {}
    
    double hitMeas()     const { return fHitMeas; }
    double hitMeasErr2() const { return fHitMeasErr2; }
    
    const SVector5&     trackStatePar() const { return fTrackStatePar; }
    const SMatrixSym55& trackStateCov() const { return fTrackStateCov; }

    geo::WireID WireId() const { return geo::WireID(fCryostatId,fTpcId,fPlaneId,fWireId); }
    
  private:
    double            fHitMeas;
    double            fHitMeasErr2;
    SVector5          fTrackStatePar;
    SMatrixSym55      fTrackStateCov;
    unsigned int      fWireId;
    unsigned int      fPlaneId;
    unsigned int      fTpcId;
    unsigned int      fCryostatId;
  };
  
}

#endif
