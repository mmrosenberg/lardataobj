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
  Track::Track(std::vector<TVector3>               const& xyz,
	       std::vector<TVector3>               const& dxdydz,
	       std::vector<TMatrixT<double> >      const& cov,
	       std::vector< std::vector <double> >        dQdx,
	       std::vector<double>                        fitMomentum,
	       int                                        ID)
    : fPId(0), fChi2(-1.), fNdof(0.)
    , fID   (ID)
    , fdQdx (dQdx)
  {
    if(xyz.size() != dxdydz.size() || xyz.size() < 1)
      throw cet::exception("Track Constructor") << "Position, direction vectors "
						<< " size problem:\n"
						<< "\t position size = "    << xyz.size()
						<< "\n\t direction size = " << dxdydz.size() << "\n";

    Positions_t pos = Positions_t();
    // for c2: remove std::move from push_back
    for (auto p : xyz) pos.push_back(Point_t(p.X(),p.Y(),p.Z()));
    auto size = pos.size();
    Momenta_t mom = Momenta_t();
    // for c2: remove std::move from push_back
    for (auto m : dxdydz) mom.push_back(Vector_t(m.X(),m.Y(),m.Z()));
    bool hasMom = false;
    if ( fitMomentum.size()==2 ) {
      if (fitMomentum[0]!=util::kBogusD && fitMomentum[1]!=util::kBogusD) {
	hasMom = true;
	for (unsigned int i=0;i<size-1;++i) mom[i]*=fitMomentum[0];
	mom[size-1]*=fitMomentum[1];
      }
    } else if (fitMomentum.size()==size) {
      hasMom = true;
      for (unsigned int i=0;i<size-1;++i) mom[i]*=fitMomentum[i];
    }
    fTraj = TrackTrajectory(std::move(pos),std::move(mom),Flags_t(size),hasMom);
    if (cov.size()==2) {
      for (unsigned int i=0; i<5; i++) {
	for (unsigned int j=i; j<5; j++) {
	  fCovVertex.At(i,j) = cov[0](i,j);
	  fCovEnd.At(i,j) = cov[1](i,j);
	}
      }
    }
  }

  //----------------------------------------------------------------------
  size_t Track::NumberdQdx(geo::View_t view) const
  {
    if (fdQdx.size() == 0) return 0;
    if(view == geo::kUnknown){
      mf::LogWarning("Track") << "asking for unknown view to get number of dQdX entries"
			      << " return the size for the 0th view vector";
      return fdQdx.at(0).size();
    }

    return fdQdx.at(view).size();
  }

  //----------------------------------------------------------------------
  const double& Track::DQdxAtPoint(unsigned int p,
				   geo::View_t view) const
  {
    if(view == geo::kUnknown){
      mf::LogWarning("Track") << "asking for unknown view to get number of dQdX entries"
			      << " return the size for the 0th view vector";
      return fdQdx.at(0).at(p);
    }

    return fdQdx.at(view).at(p);
  }

  //----------------------------------------------------------------------
  // ostream operator.
  //
  std::ostream& operator<< (std::ostream& stream, Track const& a)
  {
    //double dcoss[3];
    //double dcose[3];
    //a.Direction(dcoss,dcose);
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
           << std::setw(5) << std::right << a.NumberTrajectoryPoints()
           << " #Covariance = "      << std::setw(6) << std::right << a.NumberCovariance()
           << " #dQdx = "            << std::setw(6) << std::right;
    for(size_t i = 0; i < a.fdQdx.size(); ++i)
      stream << " " << a.fdQdx.at(i).size();

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
  void Track::Extent(std::vector<double>& start, std::vector<double>& end) const
    { details::legacy::FillTwoVectors(Vertex(), End(), start, end); }


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
