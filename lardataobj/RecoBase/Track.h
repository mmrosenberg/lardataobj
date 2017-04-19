////////////////////////////////////////////////////////////////////////////
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
#include "lardataobj/RecoBase/TrackTrajectory.h"

#include "larcoreobj/SimpleTypesAndConstants/geo_types.h"
#include "larcoreobj/SimpleTypesAndConstants/PhysicalConstants.h"
#include <iosfwd>

//! A recob::Track consists of a recob::TrackTrajectory, plus additional members relevant for a 'fitted' track:
//! fit chi2, number of degrees of freedom, particle ID hypothesis used in the fit (if any), covariance matrices at start (vertex) and end positions.
//! Please refer to the recob::TrackTrajectory documentation for more information about it.
//!
//! In terms of interface, recob::Track extends recob::TrackTrajectory, so that methods of the stored recob::TrackTrajectory can be called directly from the recob::Track interface,
//! e.g.: size_t       NumberTrajectoryPoints()         const { return fTraj.NumberTrajectoryPoints(); }
//!
//! Two different parameter conventions are used in a recob::Track, and functions to convert from one to the other are provided:
//! 1. Trajectory points and momenta (or directions) are in form of 3-vectors, corresponding to a global cartesian 6D representation
//! 2. Covariance matrices are stored in a Local 5D representation (so that the covariance matrix is invertible),
//!    where the parameters are defined on the plane orthogonal to the track direction at a given track point.
//!    By construction the local parameters of the track itself are (0,0,0,0,1/p).
//! See TrackingPlane.h for more information.
//!

namespace recob {

  class Track {

  public:

    using Point_t           = tracking::Point_t;
    using Vector_t          = tracking::Vector_t;
    using Positions_t       = tracking::Positions_t;
    using Momenta_t         = tracking::Momenta_t;
    using Rotation_t        = tracking::Rotation_t;
    using TrajectoryPoint_t = tracking::TrajectoryPoint_t;

    using SMatrixSym55 = tracking::SMatrixSym55;
    using SMatrixSym66 = tracking::SMatrixSym66;
    using SMatrix65    = tracking::SMatrix65;
    using SMatrix56    = tracking::SMatrix56;
    using SVector6     = tracking::SVector6;
    using SVector5     = tracking::SVector5;

    using PointFlags_t = TrackTrajectory::PointFlags_t;
    using Flags_t      = TrackTrajectory::Flags_t;

  protected:

    TrackTrajectory fTraj;                         ///< Stored trajectory data member
    int fPId                = 0;                   ///< Particle ID hypothesis used in the fit (if any)
    float fChi2             = -1.;                 ///< Fit chi2
    int fNdof               = 0.;                  ///< Number of degrees of freedom of the fit
    SMatrixSym55 fCovVertex;                       ///< Covariance matrix (local 5D representation) at start point (vertex)
    SMatrixSym55 fCovEnd;                          ///< Covariance matrix (local 5D representation) at end point
    int              fID    = -1;                  ///< track's ID

    //deprecated:
    std::vector< std::vector <double> > fdQdx;          ///< charge deposition per unit length at points
                                                        ///< along track outer vector index is over view


  public:

    //Default constructor
    Track() = default;

    Track(TrackTrajectory const& Traj,
	  int PId, float Chi2, int Ndof, SMatrixSym55 const& CovVertex, SMatrixSym55 const& CovEnd, int tkID)
      : fTraj(Traj), fPId(PId), fChi2(Chi2), fNdof(Ndof), fCovVertex(CovVertex), fCovEnd(CovEnd),fID(tkID),fdQdx(0) { };

    Track(TrackTrajectory&& Traj,
	  int PId, float Chi2, int Ndof, SMatrixSym55&& CovVertex, SMatrixSym55&& CovEnd, int tkID)
      : fTraj(std::move(Traj)), fPId(PId), fChi2(Chi2), fNdof(Ndof), fCovVertex(std::move(CovVertex)), fCovEnd(std::move(CovEnd)),fID(tkID),fdQdx(0) { };

    Track(Positions_t&& positions, Momenta_t&& momenta, Flags_t&& flags, bool hasMomenta,
	  int PId, float Chi2, int Ndof, SMatrixSym55&& CovVertex, SMatrixSym55&& CovEnd, int tkID)
      : fTraj(std::move(positions),std::move(momenta),std::move(flags),hasMomenta), fPId(PId), fChi2(Chi2), fNdof(Ndof), fCovVertex(std::move(CovVertex)), fCovEnd(std::move(CovEnd)),fID(tkID),fdQdx(0) { };

    /// Access to the stored recob::TrackTrajectory
    inline const recob::TrackTrajectory& Trajectory()  const { return fTraj; }

    //@{
    /// Various functions related to the presence and the number of (valid) points
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
    //@}

    //@{
    /// Access to i-th TrajectoryPoint or its Flags
    inline TrajectoryPoint_t   TrajectoryPoint(size_t i) const { return fTraj.TrajectoryPoint(i); }
    inline PointFlags_t const& FlagsAtPoint(size_t i)    const { return fTraj.FlagsAtPoint(i); }
    //@}

    //@{
    /// Access to track position at different points
    inline Point_t const& Start()  const { return fTraj.Start(); }
    //inline Point_t const& Vertex() const { return fTraj.Vertex(); }
    //inline Point_t const& End()    const { return fTraj.End(); }
    //inline Point_t const& LocationAtPoint(size_t i) const { return fTraj.LocationAtPoint(i); }
    //@}

    //@{
    /// Access to track direction at different points
    inline Vector_t StartDirection()  const { return fTraj.StartDirection(); }
    //inline Vector_t VertexDirection() const { return fTraj.VertexDirection(); }
    //inline Vector_t EndDirection()    const { return fTraj.EndDirection(); }
    //inline Vector_t const& DirectionAtPoint(size_t i) const { return fTraj.DirectionAtPoint(i); }
    //@}

    //@{
    /// Access to track momentum at different points.
    /// The user must check that HasMomentum() returns true to ensure the validity of the result of these functions.
    inline bool            HasMomentum()                   const { return fTraj.HasMomentum(); }
    inline double          MomentumAtPoint(unsigned int p) const { return fTraj.MomentumAtPoint(p); }
    inline double          VertexMomentum()                const { return fTraj.VertexMomentum(); }
    inline double          StartMomentum()                 const { return fTraj.StartMomentum(); }
    inline double          EndMomentum()                   const { return fTraj.EndMomentum();  }
    inline Vector_t const& VertexMomentumVector()          const { return fTraj.VertexMomentumVector() ; }
    inline Vector_t const& StartMomentumVector()           const { return fTraj.StartMomentumVector(); }
    inline Vector_t const& EndMomentumVector()             const { return fTraj.EndMomentumVector(); }
    inline Vector_t const& MomentumVectorAtPoint(size_t i) const { return fTraj.MomentumVectorAtPoint(i); }
    //@}

    //@{
    /// Access to covariance matrices
    const SMatrixSym55& StartCovariance() const { return fCovVertex; }
    //const SMatrixSym55& VertexCovariance() const { return fCovVertex; }
    //const SMatrixSym55& EndCovariance()    const { return fCovEnd; }
    //@}

    //@{
    /// Access to position, momentum or covariance at the start and end of the track
    inline std::pair<Point_t, Point_t>           Extent()      const { return fTraj.Extent(); }
    inline std::pair<Vector_t, Vector_t>         Direction()   const { return fTraj.Direction(); }
    inline std::pair<SMatrixSym55, SMatrixSym55> Covariances() const { return std::pair<SMatrixSym55, SMatrixSym55>(fCovVertex,fCovEnd); }
    //@}

    //@{
    /// Access to various track properties
    inline double  Length(size_t p=0) const { return fTraj.Length(p); }
    inline float   Chi2()             const { return fChi2; }
    inline float   Chi2PerNdof()      const { return fNdof>0 ? fChi2/float(fNdof) : util::kBogusF; }
    inline int     Ndof()             const { return fNdof; }
    inline int     ParticleId()       const { return fPId; }
    //@}

    //@{
    /// Access to spherical or geographical angles at vertex or at any point
    inline double  Theta()                const { return fTraj.Theta(); }
    inline double  Theta(size_t p)        const { return fTraj.Theta(p); }
    inline double  Phi()                  const { return fTraj.Phi(); }
    inline double  Phi(size_t p)          const { return fTraj.Phi(p); }
    inline double  ZenithAngle()          const { return fTraj.ZenithAngle(); }
    inline double  ZenithAngle(size_t p)  const { return fTraj.ZenithAngle(p); }
    inline double  AzimuthAngle()         const { return fTraj.AzimuthAngle(); }
    inline double  AzimuthAngle(size_t p) const { return fTraj.AzimuthAngle(p); }
    //@}

    //@{
    // Calculate rotation matrices between global (x,y,z) and local (u,v,w)
    // coordinate systems based on track direction (fDir).
    // The local w-axis points along the track direction.
    inline Rotation_t GlobalToLocalRotationAtPoint(size_t p) const { return fTraj.GlobalToLocalRotationAtPoint(p); }
    inline Rotation_t LocalToGlobalRotationAtPoint(size_t p) const { return fTraj.LocalToGlobalRotationAtPoint(p); }
    //@}

    //@{
    /// Track ID number, needed to relate a track to its possible track parent (e.g. in case of a refit).
    /// Note that art Assns to the same object are not currently supported.
    /// The < operator is based on the track ID.
    inline int  ID()       const { return fID; }
    friend bool operator < (const Track & a, const Track & b);
    //@}

    //@{
    /// Accessors to track parameters and covariance matrices in Local5D and Global6D coordinates
    SVector5            VertexParametersLocal5D()  const;
    SVector5            EndParametersLocal5D()     const;
    const SMatrixSym55& VertexCovarianceLocal5D()  const { return fCovVertex; }
    const SMatrixSym55& EndCovarianceLocal5D()     const { return fCovEnd; }
    SVector6            VertexParametersGlobal6D() const;
    SVector6            EndParametersGlobal6D()    const;
    SMatrixSym66        VertexCovarianceGlobal6D() const;
    SMatrixSym66        EndCovarianceGlobal6D()    const;
    //@}

    //@{
    //deprecated methods, temporarily kept for backward compatibility but soon to be removed
    Track(std::vector<TVector3>               const& xyz,
    	  std::vector<TVector3>               const& dxdydz,
    	  std::vector< std::vector <double> > dQdx = std::vector< std::vector<double> >(0),
    	  std::vector<double>                 fitMomentum = std::vector<double>(2, util::kBogusD),
    	  int                                 ID = -1)
      : Track(xyz,dxdydz,std::vector<TMatrixT<double> >(),dQdx,fitMomentum,ID) { };
    Track(std::vector<TVector3>        const& xyz,
	  std::vector<TVector3>        const& dxdydz,
	  std::vector<TMatrixD >       const& cov,
	  std::vector< std::vector <double> > dQdx = std::vector< std::vector<double> >(0),
	  std::vector<double>                 fitMomentum = std::vector<double>(2, util::kBogusD),
	  int                                 ID = -1);
    inline size_t   NumberCovariance()                                                 const { if (fCovVertex==SMatrixSym55() || fCovEnd==SMatrixSym55()) return 0; else return 2; } ///< Covariance matrices are either set or not
    inline TVector3 DirectionAtPoint (unsigned int p)                                  const { auto dir = fTraj.DirectionAtPoint(p); return TVector3(dir.X(),dir.Y(),dir.Z()); }
    inline TVector3 LocationAtPoint  (unsigned int p)                                  const { auto& loc = fTraj.LocationAtPoint(p); return TVector3(loc.X(),loc.Y(),loc.Z()); }
    inline TMatrixD CovarianceAtPoint(unsigned int p)                                  const { return (p==0 ? this->VertexCovariance() : this->EndCovariance()); }
    [[deprecated("Use NumberTrajectoryPoints() instead")]]
    inline size_t   NumberFitMomentum()                                                const { return HasMomentum()? NPoints(): 0U; }
    inline TVector3 Vertex()                                                           const { auto& loc = fTraj.Vertex(); return TVector3(loc.X(),loc.Y(),loc.Z()); }
    inline TVector3 End()                                                              const { auto& loc = fTraj.End(); return TVector3(loc.X(),loc.Y(),loc.Z()); }
    inline TVector3 VertexDirection()                                                  const { auto dir = fTraj.VertexDirection(); return TVector3(dir.X(),dir.Y(),dir.Z()); }
    inline TVector3 EndDirection()                                                     const { auto dir = fTraj.EndDirection(); return TVector3(dir.X(),dir.Y(),dir.Z()); }
           TMatrixD VertexCovariance()                                                 const;
           TMatrixD EndCovariance()                                                    const;
    inline void     TrajectoryAtPoint(unsigned int p, TVector3& pos, TVector3& dir)    const { fTraj.TrajectoryAtPoint(p,pos,dir); }
    [[deprecated("Use Extent() (with point interface) instead")]]
    void     Extent(std::vector<double> &xyzStart, std::vector<double> &xyzEnd) const;
    inline void     Direction(double *dcosStart, double *dcosEnd)                      const { return fTraj.Direction(dcosStart, dcosEnd); }
           size_t   NumberdQdx(geo::View_t view=geo::kUnknown)                         const;
    const double&   DQdxAtPoint(unsigned int p, geo::View_t view=geo::kUnknown)        const;
    inline void     GlobalToLocalRotationAtPoint(unsigned int p, TMatrixD& rot)        const { return fTraj.GlobalToLocalRotationAtPoint(p,rot); }
    inline void     LocalToGlobalRotationAtPoint(unsigned int p, TMatrixD& rot)        const { return fTraj.LocalToGlobalRotationAtPoint(p,rot); }
    //@}

  protected:

    friend std::ostream& operator << (std::ostream& stream, Track const& a);

  };
}

#endif // TRACK_H
