////////////////////////////////////////////////////////////////////////////
//
// \brief Definition of track object for LArSoft
//
// \author brebel@fnal.gov
//
////////////////////////////////////////////////////////////////////////////

#include "messagefacility/MessageLogger/MessageLogger.h"

#include "lardataobj/RecoBase/Track.h"
#include "lardataobj/RecoBase/TrackingPlane.h"
#include "cetlib_except/exception.h"

#include <iomanip>
#include <iostream>

namespace recob {

  //----------------------------------------------------------------------
  // ostream operator.
  //
  std::ostream& operator<< (std::ostream& stream, Track const& a)
  {
    auto const& start = a.VertexDirection();
    auto const& end   = a.EndDirection();
    stream << std::setiosflags(std::ios::fixed) << std::setprecision(3)
           << "\n Track ID "       << std::setw(4) << std::right << a.ID()
           << " Theta = "            << std::setw(6) << std::right << a.Theta()
           << " Phi = "              << std::setw(6) << std::right << a.Phi()
           << "\n  StartCosines : ( " << start.X() << " ; " << start.Y() << " ; " << start.Z()
           << ")  EndCosines : ( "   << end.X() << " ; " << end.Y() << " ; " << end.Z()
           << ")"
           << "\n  #Position and Direction = "
           << std::setw(5) << std::right << a.NumberTrajectoryPoints();
    stream << std::endl;

    return stream;
  }

  //----------------------------------------------------------------------------
  bool operator < (const Track & a, const Track & b)
  {
    if(a.ID() != b. ID())
      return a.ID() < b.ID();

    return false; //They are equal
  }

  //----------------------------------------------------------------------------
  Track::SVector6 Track::VertexParametersGlobal6D() const {
    Track::SVector6 result;
    result[0] = Vertex().X();
    result[1] = Vertex().Y();
    result[2] = Vertex().Z();
    result[3] = VertexDirection().X();
    result[4] = VertexDirection().Y();
    result[5] = VertexDirection().Z();
    return result;
  }

  Track::SVector6 Track::EndParametersGlobal6D() const {
    Track::SVector6 result;
    result[0] = End().X();
    result[1] = End().Y();
    result[2] = End().Z();
    result[3] = EndDirection().X();
    result[4] = EndDirection().Y();
    result[5] = EndDirection().Z();
    return result;
  }

  Track::SVector5 Track::VertexParametersLocal5D() const {
    //return tracking::Plane::Global6DToLocal5DParameters(VertexParametersGlobal6D(), fTraj.Vertex(), fTraj.VertexDirection());
    return Track::SVector5(0.,0.,0.,0.,(HasMomentum() ? 1./VertexMomentum() : 1. ));
  }

  Track::SVector5 Track::EndParametersLocal5D() const {
    //return tracking::Plane::Global6DToLocal5DParameters(EndParametersGlobal6D(), fTraj.End(), fTraj.EndDirection());
    return Track::SVector5(0.,0.,0.,0.,(HasMomentum() ? 1./EndMomentum() : 1. ));
  }

  Track::SMatrixSym66 Track::VertexCovarianceGlobal6D() const {
    return tracking::Plane::Local5DToGlobal6DCovariance(fCovVertex,HasMomentum(),fTraj.VertexMomentumVector(),fTraj.VertexDirection());
  }

  Track::SMatrixSym66 Track::EndCovarianceGlobal6D() const {
    return tracking::Plane::Local5DToGlobal6DCovariance(fCovEnd,HasMomentum(),fTraj.EndMomentumVector(),fTraj.EndDirection());
  }

}
