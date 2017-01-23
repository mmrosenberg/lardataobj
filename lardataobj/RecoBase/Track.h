////////////////////////////////////////////////////////////////////////////
// \version $Id: Track.h,v 1.5 2010/02/15 20:32:46 brebel Exp $
//
// \brief Definition of track object for LArSoft
//
// \author brebel@fnal.gov
//
////////////////////////////////////////////////////////////////////////////

#ifndef TRACK_H
#define TRACK_H

#include <vector>

#include "TMatrixD.h"
#include "TVector3.h"
#include "Math/SMatrix.h"
#include "lardataobj/RecoBase/TrackTrajectory.h"

#ifndef __GCCXML__
#include "larcoreobj/SimpleTypesAndConstants/geo_types.h"
#include "larcoreobj/SimpleTypesAndConstants/PhysicalConstants.h"
#include <iosfwd>
#endif

namespace recob {

  class Track {

  public:

    using TrajectoryPoint_t = TrackTrajectory::TrajectoryPoint_t;
    using Point_t           = TrackTrajectory::Point_t;
    using Vector_t          = TrackTrajectory::Vector_t;
    using Positions_t       = TrackTrajectory::Positions_t;
    using Momenta_t         = TrackTrajectory::Momenta_t;
    using PointFlags_t      = TrackTrajectory::PointFlags_t;
    using Flags_t           = TrackTrajectory::Flags_t;
    using Rotation_t        = TrackTrajectory::Rotation_t;

    using SMatrixSym11 = ROOT::Math::SMatrix<double,1,1,ROOT::Math::MatRepSym<double,1> >;//fixme
    using SMatrixSym55 = ROOT::Math::SMatrix<double,5,5,ROOT::Math::MatRepSym<double,5> >;
    using SMatrixSym66 = ROOT::Math::SMatrix<double,6,6,ROOT::Math::MatRepSym<double,6> >;
    using SMatrix65    = ROOT::Math::SMatrix<double,6,5>;
    using SMatrix56    = ROOT::Math::SMatrix<double,5,6>;
    using SMatrix55    = ROOT::Math::SMatrix<double,5,5>;
    using SMatrix51    = ROOT::Math::SMatrix<double,5,1>;
    using SVector6     = ROOT::Math::SVector<double,6>;
    using SVector5     = ROOT::Math::SVector<double,5>;

    //Default constructor
    Track();

  protected:

    TrackTrajectory fTraj;   ///< Stored trajectory data member
    int fPId;                ///< Particle ID hypothesis used in the fit (if any)
    float fChi2;             ///< Fit chi2
    int fNdof;               ///< Number of degrees of freedom of the fit
    SMatrixSym55 fCovVertex; ///< Covariance matrix (local 5D representation) at start point (vertex)
    SMatrixSym55 fCovEnd;    ///< Covariance matrix (local 5D representation) at end point

    std::vector< std::vector <double> > fdQdx;          ///< charge deposition per unit length at points
                                                        ///< along track outer vector index is over view
    int                                 fID;            ///< track's ID

#ifndef __GCCXML__

  public:

    Track(TrackTrajectory const& Traj,
	  int PId, float Chi2, int Ndof, SMatrixSym55 const& CovVertex, SMatrixSym55 const& CovEnd)
      : fTraj(Traj), fPId(PId), fChi2(Chi2), fNdof(Ndof), fCovVertex(CovVertex), fCovEnd(CovEnd),fdQdx(0),fID(-1) { };

    Track(TrackTrajectory&& Traj,
	  int PId, float Chi2, int Ndof, SMatrixSym55&& CovVertex, SMatrixSym55&& CovEnd)
      : fTraj(std::move(Traj)), fPId(PId), fChi2(Chi2), fNdof(Ndof), fCovVertex(std::move(CovVertex)), fCovEnd(std::move(CovEnd)),fdQdx(0),fID(-1) { };

    Track(Positions_t&& positions, Momenta_t&& momenta, Flags_t&& flags, bool hasMomenta,
	  int PId, float Chi2, int Ndof, SMatrixSym55&& CovVertex, SMatrixSym55&& CovEnd)
      : fTraj(std::move(positions),std::move(momenta),std::move(flags),hasMomenta), fPId(PId), fChi2(Chi2), fNdof(Ndof), fCovVertex(std::move(CovVertex)), fCovEnd(std::move(CovEnd)),fdQdx(0),fID(-1) { };

    // Bezier-like constructor
    Track(std::vector<TVector3>               const& xyz,
    	  std::vector<TVector3>               const& dxdydz,
    	  std::vector< std::vector <double> > dQdx = std::vector< std::vector<double> >(0),
    	  std::vector<double>                 fitMomentum = std::vector<double>(2, util::kBogusD),
    	  int                                 ID = -1)
      : Track(xyz,dxdydz,std::vector<TMatrixT<double> >(),dQdx,fitMomentum,ID) { };

    // Kalman-like constructor
    Track(std::vector<TVector3>        const& xyz,
	  std::vector<TVector3>        const& dxdydz,
	  std::vector<TMatrixD >       const& cov,
	  std::vector< std::vector <double> > dQdx = std::vector< std::vector<double> >(0),
	  std::vector<double>                 fitMomentum = std::vector<double>(2, util::kBogusD),
	  int                                 ID = -1);

    inline const recob::TrackTrajectory& Trajectory()  const { return fTraj; }

    inline size_t       NumberTrajectoryPoints()         const { return fTraj.NumberTrajectoryPoints(); }
    inline size_t       NPoints()                        const { return fTraj.NPoints(); }
    inline size_t       FirstPoint()                     const { return fTraj.FirstPoint(); }
    inline size_t       LastPoint()                      const { return fTraj.LastPoint(); }
    inline size_t       FirstValidPoint()                const { return fTraj.FirstValidPoint(); }
    inline size_t       NextValidPoint(size_t index)     const { return fTraj.NextValidPoint(index); }
    inline size_t       PreviousValidPoint(size_t index) const { return fTraj.PreviousValidPoint(index); }
    inline size_t       LastValidPoint()                 const { return fTraj.LastValidPoint(); }
    inline bool         HasPoint(size_t i)               const { return fTraj.HasPoint(i); }
    inline bool         HasValidPoint(size_t i)          const { return fTraj.HasValidPoint(i); }
    inline unsigned int CountValidPoints()               const { return fTraj.CountValidPoints(); }

    inline TrajectoryPoint_t   TrajectoryPoint(size_t i) const { return fTraj.TrajectoryPoint(i); }
    inline PointFlags_t const& FlagsAtPoint(size_t i)    const { return fTraj.FlagsAtPoint(i); }

    inline Point_t const& Start()  const { return fTraj.Start(); }
    //inline Point_t const& Vertex() const { return fTraj.Vertex(); }
    //inline Point_t const& End()    const { return fTraj.End(); }
    //inline Point_t const& LocationAtPoint(size_t i) const { return fTraj.LocationAtPoint(i); }

    inline Vector_t StartDirection()  const { return fTraj.StartDirection(); }
    //inline Vector_t VertexDirection() const { return fTraj.VertexDirection(); }
    //inline Vector_t EndDirection()    const { return fTraj.EndDirection(); }
    //inline Vector_t const& DirectionAtPoint(size_t i) const { return fTraj.DirectionAtPoint(i); }

    const SMatrixSym55& StartCovariance() const { return fCovVertex; }
    //const SMatrixSym55& VertexCovariance() const { return fCovVertex; }
    //const SMatrixSym55& EndCovariance()    const { return fCovEnd; }

    inline std::pair<Point_t, Point_t>           Extent()      const { return fTraj.Extent(); }
    inline std::pair<Vector_t, Vector_t>         Direction()   const { return fTraj.Direction(); }
    inline std::pair<SMatrixSym55, SMatrixSym55> Covariances() const { return std::pair<SMatrixSym55, SMatrixSym55>(fCovVertex,fCovEnd); }

    inline bool            HasMomentum()                   const { return fTraj.HasMomentum(); }
    inline double          MomentumAtPoint(unsigned int p) const { return fTraj.MomentumAtPoint(p); }
    inline double          VertexMomentum()                const { return fTraj.VertexMomentum(); }
    inline double          StartMomentum()                 const { return fTraj.StartMomentum(); }
    inline double          EndMomentum()                   const { return fTraj.EndMomentum();  }
    inline Vector_t const& VertexMomentumVector()          const { return fTraj.VertexMomentumVector() ; }
    inline Vector_t const& StartMomentumVector()           const { return fTraj.StartMomentumVector(); }
    inline Vector_t const& EndMomentumVector()             const { return fTraj.EndMomentumVector(); }
    inline Vector_t const& MomentumVectorAtPoint(size_t i) const { return fTraj.MomentumVectorAtPoint(i); }

    inline double  Length(size_t p=0) const { return fTraj.Length(p); }

    inline double  Theta()                const { return fTraj.Theta(); }
    inline double  Theta(size_t p)        const { return fTraj.Theta(p); }
    inline double  Phi()                  const { return fTraj.Phi();   }
    inline double  Phi(size_t p)          const { return fTraj.Phi(p);   }
    inline double  ZenithAngle()          const { return fTraj.ZenithAngle(); }
    inline double  ZenithAngle(size_t p)  const { return fTraj.ZenithAngle(p); }
    inline double  AzimuthAngle()         const { return fTraj.AzimuthAngle();   }
    inline double  AzimuthAngle(size_t p) const { return fTraj.AzimuthAngle(p);   }

    // Calculate rotation matrices between global (x,y,z) and local (u,v,w)
    // coordinate systems based on track direction (fDir).
    // The local w-axis points along the track direction.
    inline Rotation_t          GlobalToLocalRotationAtPoint(size_t p) const { return fTraj.GlobalToLocalRotationAtPoint(p); }
    inline Rotation_t          LocalToGlobalRotationAtPoint(size_t p) const { return fTraj.LocalToGlobalRotationAtPoint(p); }

    //deprecated methods, temporarily kept for backward compatibility but soon to be removed
    inline int      ID()                                                               const { return fID; }
    inline size_t   NumberCovariance()                                                 const { return 2; }
    inline TVector3 DirectionAtPoint (unsigned int p)                                  const { auto dir = fTraj.DirectionAtPoint(p); return TVector3(dir.X(),dir.Y(),dir.Z()); }
    inline TVector3 LocationAtPoint  (unsigned int p)                                  const { auto& loc = fTraj.LocationAtPoint(p); return TVector3(loc.X(),loc.Y(),loc.Z()); }
    inline TMatrixD CovarianceAtPoint(unsigned int p)                                  const { return (p==0 ? this->VertexCovariance() : this->EndCovariance()); }
    inline size_t   NumberFitMomentum()                                                const { return fTraj.NumberFitMomentum();  }
    inline TVector3 Vertex()                                                           const { auto& loc = fTraj.Vertex(); return TVector3(loc.X(),loc.Y(),loc.Z()); }
    inline TVector3 End()                                                              const { auto& loc = fTraj.End(); return TVector3(loc.X(),loc.Y(),loc.Z()); }
    inline TVector3 VertexDirection()                                                  const { auto dir = fTraj.VertexDirection(); return TVector3(dir.X(),dir.Y(),dir.Z()); }
    inline TVector3 EndDirection()                                                     const { auto dir = fTraj.EndDirection(); return TVector3(dir.X(),dir.Y(),dir.Z()); }
           TMatrixD VertexCovariance()                                                 const;
           TMatrixD EndCovariance()                                                    const;
    inline void     TrajectoryAtPoint(unsigned int p, TVector3& pos, TVector3& dir)    const { fTraj.TrajectoryAtPoint(p,pos,dir); }
    inline void     Extent(std::vector<double> &xyzStart, std::vector<double> &xyzEnd) const { return fTraj.Extent(xyzStart, xyzEnd); }
    inline void     Direction(double *dcosStart, double *dcosEnd)                      const { return fTraj.Direction(dcosStart, dcosEnd); }
           size_t   NumberdQdx(geo::View_t view=geo::kUnknown)                         const;
    const double&   DQdxAtPoint(unsigned int p, geo::View_t view=geo::kUnknown)        const;
    inline void     GlobalToLocalRotationAtPoint(unsigned int p, TMatrixD& rot)        const { return fTraj.GlobalToLocalRotationAtPoint(p,rot); }
    inline void     LocalToGlobalRotationAtPoint(unsigned int p, TMatrixD& rot)        const { return fTraj.LocalToGlobalRotationAtPoint(p,rot); }
    friend bool     operator <   (const Track & a, const Track & b);

  protected:

    friend std::ostream& operator << (std::ostream& stream, Track const& a);

#endif
  };
}

#endif // TRACK_H
