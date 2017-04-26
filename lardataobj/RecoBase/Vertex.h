////////////////////////////////////////////////////////////////////////////
//
// \brief Definition of vertex object for LArSoft
//
// \author brebel@fnal.gov
//
////////////////////////////////////////////////////////////////////////////

#ifndef RB_VERTEX_H
#define RB_VERTEX_H

#include <iosfwd>

#include "larcoreobj/SimpleTypesAndConstants/PhysicalConstants.h"

namespace recob {

  class Vertex  {

  public:

    Vertex();  // Default constructor

  private:

    double fXYZ[3];    ///< location of vertex
    int    fID;        ///< id number for vertex

  public:

    explicit  Vertex(double *xyz,
		     int     id=util::kBogusI);
    void      XYZ(double *xyz) const;
    int ID()                   const;

    friend bool          operator <   (const Vertex & a, const Vertex & b);
    friend std::ostream& operator <<  (std::ostream& o,  const Vertex & a);


  };
}


inline int recob::Vertex::ID() const { return fID; }

#endif // RB_VERTEX_H
