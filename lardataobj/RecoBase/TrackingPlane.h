#ifndef TRACKINGPLANE_H
#define TRACKINGPLANE_H

//! \brief Class defining a plane for tracking. 
//!        It provides various functionalities to convert track parameters and covariance matrices from global to local coordinates.
//!
//! The plane is constructed from a point in global coordinates, and a direction unit vector orthogonal to the plane.
//! For instance, they may refer to the position and direction of a trajectory at a given point,
//! or to the center of a wire and the direction orthogonal to both the wire and drift directions.
//!
//! The plane and parameters are defined along the lines of trkf::SurfXYZPlane, but with a different notation for the angles
//! which are now called alpha and beta to avoid confusion with theta and phi in global spherical coordinates:
//! (alpha, beta) correspond to (alpha := Rotation angle about y'-axis (projected Lorentz angle) = atan2(nx, hypot(ny, nz)),
//!                               beta := Rotation angle about x-axis (wire angle) = atan2(-ny, nz))
//! The 3D local positions are defined in terms of the unit vectors u,v,w where w is along the normal direction to the plane,
//! u is perpendicular to w and coplanar to the global x axis and with position sign along the positive sign of x,
//! v forms a right handed orthonormal basis with u and w.
//! The 5D parameters are u,v,du/dw,dv/dw,1/p: 
//! u,v are the local positions, du/dw and dv/dw are the local directions, and 1/p is the inverse of the track momentum.
//! The global 6D coordinates are formed by the track position and momentum (or direction) at a given point.
//!
//! The discussion above refers to a detector with drift direction along the x axis; 
//! this class will have to be extended for a detector with a different drift direction.

#include "lardataobj/RecoBase/TrackingTypes.h"

namespace recob {
  namespace tracking {

    class Plane {

    public:

      //!
      //! Constructor from reference position on the plane and direction orthogonal to the plane
      Plane(const Point_t& planePos, const Vector_t& planeDir)
	: fPlanePos(planePos), fPlaneDir(planeDir.Unit()) { }

      //!
      //! Reference position on the plane
      Point_t const& position() const { return fPlanePos; }

      //!
      //! Reference direction orthogonal to the plane
      Vector_t const& direction() const { return fPlaneDir; }

      //! Function to convert parameters from global to local coordinates.
      //! Local coordinates are on the plane with origin at planePos and orthogonal to planeDir.
      //! It is responsibility of the user to make sure the global position lies on the plane
      static SVector5 Global6DToLocal5DParameters(const SVector6& par6d, const Point_t& planePos, const Vector_t& planeDir);
      inline SVector5 Global6DToLocal5DParameters(const SVector6& par6d) { return Global6DToLocal5DParameters(par6d, fPlanePos, fPlaneDir); }

      //! Function to convert parameters from local to global coordinates.
      //! Local coordinates are on the plane with origin at planePos and orthogonal to planeDir.
      static SVector6 Local5DToGlobal6DParameters(const SVector5& par5d, const Point_t& planePos, const Vector_t& planeDir);
      inline SVector6 Local5DToGlobal6DParameters(const SVector5& par5d) { return Local5DToGlobal6DParameters(par5d, fPlanePos, fPlaneDir); }

      //! Compute the jacobian to translate track covariance from global to local coordinates.
      //! The track momentum is needed to compute the jacobian.
      //! Local coordinates are on the plane orthogonal to planeDir (in most cases this will be the same direction as the momentum, but the function is generic).
      static SMatrix65 Local5DToGlobal6DJacobian(const Vector_t& momentum, const Vector_t& planeDir);
      inline SMatrix65 Local5DToGlobal6DJacobian(const Vector_t& momentum) { return Local5DToGlobal6DJacobian(momentum,fPlaneDir); }

      //! Compute the jacobian to translate track covariance from local to global coordinates.
      //! The track momentum is needed to compute the jacobian.
      //! Local coordinates are on the plane orthogonal to planeDir (in most cases this will be the same direction as the momentum, but the function is generic).
      static SMatrix56 Global6DToLocal5DJacobian(const Vector_t& momentum, const Vector_t& planeDir);
      inline SMatrix56 Global6DToLocal5DJacobian(const Vector_t& momentum) { return Global6DToLocal5DJacobian(momentum,fPlaneDir); }

      //!
      //! Calculate rotation matrices from global (x,y,z) to local (u,v,w) coordinates
      static Rotation_t Global3DToLocal3DRotation(const Vector_t& planeDir);
      inline Rotation_t Global3DToLocal3DRotation() { return Global3DToLocal3DRotation(fPlaneDir); }
      
      //!
      //! Calculate rotation matrices from local (u,v,w) to global (x,y,z) coordinates
      static Rotation_t Local3DToGlobal3DRotation(const Vector_t& planeDir);
      inline Rotation_t Local3DToGlobal3DRotation() { return Local3DToGlobal3DRotation(fPlaneDir); }
      
    private:
      Point_t fPlanePos;
      Vector_t fPlaneDir;
      //should we cache sin and cos?
    };

  }
}

#endif
