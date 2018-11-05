#ifndef TRACKINGTYPE_H
#define TRACKINGTYPE_H

// LArSoft libraries
#include "larcorealg/Geometry/geo_vectors_utils.h"

// ROOT libraries
#include "Math/GenVector/Rotation3D.h"
#include "Math/GenVector/AxisAngle.h"
#include "Math/SMatrix.h"

namespace recob {

  namespace tracking {

    /// Type used for coordinates and values in general.
    using Coord_t = Double32_t;
    
    /// Type for representation of position in physical 3D space.
    using Point_t = ROOT::Math::PositionVector3D<ROOT::Math::Cartesian3D<Coord_t>, ROOT::Math::GlobalCoordinateSystemTag>;
    
    /// Type for representation of momenta in 3D space.
    using Vector_t = ROOT::Math::DisplacementVector3D <ROOT::Math::Cartesian3D<Coord_t>, ROOT::Math::GlobalCoordinateSystemTag>;
    
    /// Type of trajectory point list.
    using Positions_t = std::vector<Point_t>;
    
    /// Type of momentum list.
    using Momenta_t = std::vector<Vector_t>;

    /// Type for representation of space rotations.
    using Rotation_t = ROOT::Math::Rotation3D;

    template <typename Point> Point_t toPoint(Point const& p) { return geo::vect::convertTo<Point_t>(p); }
    template <typename Point> std::vector<Point_t> convertCollToPoint (std::vector<Point> const& coll)
      { return geo::vect::convertCollTo<Point_t>(coll); }

    /// A point in the trajectory, with position and momentum.
    struct TrajectoryPoint_t {

      Point_t position; ///< position in the trajectory [cm].
      Vector_t momentum; ///< momentum at the trajectory [GeV/c].

      /// Default constructor: sets at origin with no momentum.
      TrajectoryPoint_t() = default;

      /// Constructor: assigns position and momentum.
      TrajectoryPoint_t(Point_t pos, Vector_t mom)
        : position(pos), momentum(mom) {}

      /// Returns the direction of the trajectory (unit vector of the momentum).
      Vector_t direction() const
        { return momentum.Unit(); }

    }; // TrajectoryPoint_t

    /// @{
    /// SMatrix and SVector
    using SMatrixSym22 = ROOT::Math::SMatrix<Double32_t,2,2,ROOT::Math::MatRepSym<Double32_t,2> >;
    using SMatrixSym33 = ROOT::Math::SMatrix<Double32_t,3,3,ROOT::Math::MatRepSym<Double32_t,3> >;
    using SMatrixSym55 = ROOT::Math::SMatrix<Double32_t,5,5,ROOT::Math::MatRepSym<Double32_t,5> >;
    using SMatrixSym66 = ROOT::Math::SMatrix<Double32_t,6,6,ROOT::Math::MatRepSym<Double32_t,6> >;
    using SMatrix65    = ROOT::Math::SMatrix<Double32_t,6,5>;
    using SMatrix56    = ROOT::Math::SMatrix<Double32_t,5,6>;
    using SMatrix55    = ROOT::Math::SMatrix<Double32_t,5,5>;
    using SMatrix66    = ROOT::Math::SMatrix<Double32_t,6,6>;
    using SVector6     = ROOT::Math::SVector<Double32_t,6>;
    using SVector5     = ROOT::Math::SVector<Double32_t,5>;
    using SVector3     = ROOT::Math::SVector<Double32_t,3>;
    using SVector2     = ROOT::Math::SVector<Double32_t,2>;
    /// @}
    
  }
  
}

#endif
