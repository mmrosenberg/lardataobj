////////////////////////////////////////////////////////////////////////////
//
// \brief Definition of vertex object for LArSoft
//
// \author brebel@fnal.gov
//
////////////////////////////////////////////////////////////////////////////
#include "messagefacility/MessageLogger/MessageLogger.h"

#include "lardataobj/RecoBase/Vertex.h"

#include <iomanip>

namespace recob{

  //----------------------------------------------------------------------
  Vertex::Vertex()
  {
    status_ = Invalid;
    id_ = util::kBogusI;
    chi2_ = util::kBogusD;
    ndof_ = util::kBogusI;
  }

  //----------------------------------------------------------------------
  Vertex::Vertex(double *xyz,
		 int id)
    : id_(id)
  {
    pos_ = Point_t(xyz[0],xyz[1],xyz[2]);
    status_ = Valid;
    chi2_ = util::kBogusD;
    ndof_ = util::kBogusI;
  }

  //----------------------------------------------------------------------
  void Vertex::XYZ(double *xyz) const
  {
    xyz[0] = pos_.X();
    xyz[1] = pos_.Y();
    xyz[2] = pos_.Z();

    return;
  }

  //----------------------------------------------------------------------
  // ostream operator.
  //
  std::ostream& operator<< (std::ostream& o, const Vertex & a)
  {
    o << std::setprecision(5);
    o << "Vertex ID    "  << a.id_ << std::setw(5)
      << " (x,y,z)  = ("  << a.pos_.X() << "," << a.pos_.Y() << ","
      << a.pos_.Z() << ")";

    return o;
  }

  //----------------------------------------------------------------------
  // < operator.
  //
  bool operator < (const Vertex & a, const Vertex & b)
  {
    double xyza[3] = {0.};
    double xyzb[3] = {0.};
    a.XYZ(xyza);
    b.XYZ(xyzb);

    return xyza[2] < xyzb[2];

  }


}
