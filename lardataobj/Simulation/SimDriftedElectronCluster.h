//vim: set sw=2 expandtab:
////////////////////////////////////////////////////////////////////////
/// \file  lardataobj/Simulation/SimDriftedElectronCluster.h
/// \brief contains objects relating to SimDriftedElectronCluster
////////////////////////////////////////////////////////////////////////

#ifndef SimDriftedElectronCluster_h
#define SimDriftedElectronCluster_h

// LArSoft includes
// Define the LArSoft standard geometry types and methods.
#include "larcoreobj/SimpleTypesAndConstants/geo_vectors.h"

// C++ includes
#include <iostream>

namespace sim
{

  // This structure contains all the information per simulated cluster of drifted electrons towards the anode.

  class SimDriftedElectronCluster
  {
  private:
    using Length_t = double;
    using StoredPoint_t = geo::GenPoint3DBase_t<double>;
    using StoredVector_t = geo::GenVector3DBase_t<double>;

  public:

    SimDriftedElectronCluster() = default;

    SimDriftedElectronCluster(int ne,
          double t,
          StoredPoint_t start,
          StoredPoint_t end,
          StoredPoint_t width,
          float e,
          int id)
      : fNumberOfElectrons(ne)
      , fTime(t)
      , fInitialPosition(start)
      , fFinalPosition(end)
      , fClusterWidth(width)
      , fEnergy(e)
      , fMotherTrackID(id)
    {}

    int NumberOfElectrons() const { return fNumberOfElectrons; }
    double Time() const { return fTime; }
    geo::Point_t InitialPosition() const { return fInitialPosition; }
    geo::Point_t FinalPosition() const { return fFinalPosition; }
    geo::Vector_t ClusterWidth() const { return fClusterWidth; }
    float Energy() const { return fEnergy; }
    int TrackID() const { return fMotherTrackID; }

    geo::Length_t FinalPositionX() const { return fFinalPosition.X(); }
    geo::Length_t FinalPositionY() const { return fFinalPosition.Y(); }
    geo::Length_t FinalPositionZ() const { return fFinalPosition.Z(); }

  private:

    int            fNumberOfElectrons; //< of ionization electrons
    double         fTime;    //< (ns)
    StoredPoint_t  fInitialPosition;     //< positions in (cm)
    StoredPoint_t  fFinalPosition;
    StoredVector_t  fClusterWidth;
    float          fEnergy;         //< energy deposition (MeV)
    int            fMotherTrackID;      //< simulation track id
  };

} // namespace sim
#endif // SimDriftedElectronCluster_n
