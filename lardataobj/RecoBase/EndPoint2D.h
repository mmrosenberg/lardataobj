////////////////////////////////////////////////////////////////////////////
//
// \brief Definition of vertex object for LArSoft
//
// \author joshua.spitz@yale.edu
//
////////////////////////////////////////////////////////////////////////////

#ifndef RECOB_ENDPOINT2D_H
#define RECOB_ENDPOINT2D_H

#include "larcoreobj/SimpleTypesAndConstants/geo_types.h"

#include <iosfwd>

namespace recob {

  class EndPoint2D  {

  public:

    EndPoint2D();  ///Default constructor

   private:
    double         fDriftTime;    ///< vertex's drift time
    geo::WireID    fWireID;      ///< vertex's wireID
    int            fID;           ///< vertex's ID
    double         fStrength;     ///< vertex's strength
    geo::View_t    fView;         ///< view for this end point
    double         fTotalCharge;  ///< total charge of hits associated with end point


  public:
    EndPoint2D(double driftTime,
	       geo::WireID wireID,
	       double strength,
	       int id,
	       geo::View_t view,
	       double totalQ);

    double         Charge()    const;
    geo::View_t    View()      const;
    double         DriftTime() const;
    geo::WireID    WireID()    const;
    int            ID()        const;
    double         Strength()  const;

    friend std::ostream& operator << (std::ostream& o, const EndPoint2D& c);

  };
}


inline double       recob::EndPoint2D::Charge()    const { return fTotalCharge; }
inline geo::View_t  recob::EndPoint2D::View()      const { return fView;        }
inline double       recob::EndPoint2D::DriftTime() const { return fDriftTime;   }
inline geo::WireID  recob::EndPoint2D::WireID()    const { return fWireID;     }
inline int          recob::EndPoint2D::ID()        const { return fID;          }
inline double       recob::EndPoint2D::Strength()  const { return fStrength;    }


#endif //RECOB_ENDPOINT2D_H
