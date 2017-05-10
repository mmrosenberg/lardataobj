////////////////////////////////////////////////////////////////////////////
//
// Definition of SpacePoint class for LArSoft
//
// SpacePoints are 3D objects that contain pointers to Hits from multiple
// wireplanes that have been identified as matching.
//
// msoderbe@syr.edu
//
////////////////////////////////////////////////////////////////////////////

#ifndef SPACEPOINT_H
#define SPACEPOINT_H

#include <iosfwd>

#include "larcoreobj/SimpleTypesAndConstants/PhysicalConstants.h"

namespace recob {

  class SpacePoint {

  public:
    using ID_t = int; ///< type of spacepoint ID
    
    /// Special value for an invalid ID.
    static constexpr ID_t InvalidID = util::kBogusI;
    

    SpacePoint();  ///Default constructor

  private:
    ID_t                       fID;        ///< SpacePoint ID
    double                     fXYZ[3];    ///< position of SpacePoint in xyz
    double                     fErrXYZ[6]; ///< Error matrix (triangular).
    double                     fChisq;     ///< Chisquare.

  public:
    SpacePoint(double const*xyz,
	       double const*err,
	       double  chisq,
	       int     id=InvalidID);

    ID_t                       ID()      const;
    const double*              XYZ()     const;
    const double*              ErrXYZ()  const;
    double                     Chisq()   const;

    friend std::ostream& operator << (std::ostream& o, const SpacePoint & a);
    friend bool          operator <  (const SpacePoint & a, const SpacePoint & b);


  };

  /// Comparison of a space point with an ID, for sorting and lookup.
  inline bool operator< (SpacePoint const& s, SpacePoint::ID_t id) { return s.ID() < id; }
  /// Comparison of a space point with an ID, for sorting and lookup.
  inline bool operator< (SpacePoint::ID_t id, SpacePoint const& s) { return id < s.ID(); }

}


inline recob::SpacePoint::ID_t recob::SpacePoint::ID() const { return fID; }
inline const double* recob::SpacePoint::XYZ()     const { return fXYZ;    }
inline const double* recob::SpacePoint::ErrXYZ()  const { return fErrXYZ; }
inline double        recob::SpacePoint::Chisq()   const { return fChisq;  }

#endif //SPACEPOINT_H
