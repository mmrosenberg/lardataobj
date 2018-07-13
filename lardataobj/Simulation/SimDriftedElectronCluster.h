//vim: set sw=2 expandtab:
////////////////////////////////////////////////////////////////////////
/// \file  SimDriftedElectronCluster.h
/// \brief contains objects relating to SimDriftedElectronCluster
////////////////////////////////////////////////////////////////////////

#ifndef SimDriftedElectronCluster_h
#define SimDriftedElectronCluster_h

// LArSoft includes
// Define the LArSoft standard geometry types and methods.
#include "larcoreobj/SimpleTypesAndConstants/geo_vectors.h"

// ROOT includes
#include "Math/GenVector/Cartesian3D.h"
#include "Math/GenVector/PositionVector3D.h"
#include "Math/GenVector/PxPyPzE4D.h"
#include "Math/GenVector/LorentzVector.h"

// C++ includes
#include <iostream>

namespace sim
{

  // This structure contains all the information per simulated cluster of drifted electrons towards the anode.

  class SimDriftedElectronCluster
  {
  public:
    using Length_t = double;
    using Point_t = ROOT::Math::PositionVector3D< ROOT::Math::Cartesian3D<Length_t> >;

    SimDriftedElectronCluster(int ne = 0,
		     double t = 0.,
		     Point_t start = {0.,0.,0.},
		     Point_t end = {0.,0.,0.},
		     Point_t width = {0.,0.,0.},
		     double e = 0.,
		     int id = 0)
      : NumberOfElectrons(ne)
      , Time(t)
      , InitialPosition(start)
      , FinalPosition(end)
      , ClusterWidth(width)
      , Energy(e)
      , MotherTrackID(id)
    {}


#ifndef __GCCXML__
    // Accessors, hidden from the ROOT dictionary generation.
    // Note that even if we store a value as float, we return
    // it as double so the user doesn't have to think about
    // precision issues.

    int getNumberOfElectrons() const { return NumberOfElectrons; }
    double getTime() const { return Time; }
    geo::Point_t getInitialPosition() const { return { InitialPosition.X(), InitialPosition.Y(), InitialPosition.Z() }; }
    geo::Point_t getFinalPosition() const { return { FinalPosition.X(), FinalPosition.Y(), FinalPosition.Z() }; }
    geo::Point_t getClusterWidth() const { return { ClusterWidth.X(), ClusterWidth.Y(), ClusterWidth.Z() }; }
    double getEnergy() const { return Energy; }
    int getTrackID() const { return MotherTrackID; }

    geo::Length_t FinalPositionX() const { return FinalPosition.X(); }
    geo::Length_t FinalPositionY() const { return FinalPosition.Y(); }
    geo::Length_t FinalPositionZ() const { return FinalPosition.Z(); }


    bool operator<(const SimDriftedElectronCluster& rhs) const
    {
      return NumberOfElectrons < rhs.NumberOfElectrons
	&& Time < rhs.Time
	&& InitialPosition.Z() < rhs.InitialPosition.Z()
	&& InitialPosition.Y() < rhs.InitialPosition.Y()
	&& InitialPosition.X() < rhs.InitialPosition.X()
	&& FinalPosition.Z() < rhs.FinalPosition.Z()
	&& FinalPosition.Y() < rhs.FinalPosition.Y()
	&& FinalPosition.X() < rhs.FinalPosition.X()
	&& Energy > rhs.Energy
	&& MotherTrackID > rhs.MotherTrackID;
    }

#endif // __GCCXML__

  private:
    // While the accessors above return all values in double
    // precision, store whatever we can in single precision to save
    // memory and disk space.

    // There are roughly 7 digits of decimal precision in a float.
    // This will suffice for energy. A float (as opposed to a double)
    // can hold a little more than 7 digits of decimal precision. The
    // smallest position resolution in the simulation is about 0.1mm,
    // or 10^-4m. With seven digits of precision, that means a float
    // can be accurate to up to the range of 10^3m. That's why the
    // definition of our local Point_t (see above) is based on float,
    // while geo::Point_t is based on double.

    // If the above reasoning is wrong, just change the definition of
    // Length_t near the top of this file. Of course, also edit these
    // comments if you do, because you're a good and responsible
    // programmer.

    // For time, it's possible for long-lived particles like neutrons
    // to deposit energy after billions of ns. Chances are time cuts
    // will take care of that, but let's make sure that any overlay studies
    // won't suffer due to lack of precision.

    int           NumberOfElectrons; //< of ionization electrons
    double        Time;    //< (ns)
    Point_t       InitialPosition;     //< positions in (cm)
    Point_t       FinalPosition;
    Point_t       ClusterWidth;
    float         Energy;         //< energy deposition (MeV)
    int           MotherTrackID;      //< simulation track id
   // int           MotherTrackID2;      //< simulation track id
  };

} // namespace sim
#endif // SimDriftedElectronCluster_n
