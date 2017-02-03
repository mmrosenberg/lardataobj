/**
 * @file    Trajectory.cxx
 * @brief   Data product for reconstructed trajectory in space
 * @date    December 9, 2016
 * @see     Trajectory.h
 * 
 */


#include "lardataobj/RecoBase/Trajectory.h"
#include "lardataobj/RecoBase/TrackingPlane.h"

// LArSoft libraries
#include "larcoreobj/SimpleTypesAndConstants/PhysicalConstants.h" // util::pi()

// ROOT libraries
#include "TVector3.h" // legacy interface
#include "TMatrixD.h" // legacy interface

// C/C++ standard libraries
#include <utility> // std::move()
#include <string> // std::to_string()
#include <stdexcept> // std::runtime_error


//------------------------------------------------------------------------------
namespace {
  
  template <typename Matrix>
  TMatrixD fillTMatrixD(Matrix const& mat, TMatrixD& Tmat) {
    ROOT::Math::Rotation3D asMat(mat);
    Tmat.ResizeTo(3, 3);
    asMat.GetRotationMatrix(Tmat);
    return Tmat;
  } // fillTMatrixD()
  
} // local namespace

//------------------------------------------------------------------------------
recob::Trajectory::Trajectory
  (Positions_t&& positions, Momenta_t&& momenta, bool hasMomenta)
  : fPositions(std::move(positions))
  , fMomenta(std::move(momenta))
  , fHasMomentum(hasMomenta)
{
  // invariant check
  if (fPositions.size() != fMomenta.size()) {
    throw std::runtime_error("recob::Trajectory constructed with "
      + std::to_string(fPositions.size()) + " positions and "
      + std::to_string(fMomenta.size())
      + " momenta! it requires the same number for both."
      );
  }
  if (fPositions.size() < 2) {
    throw std::runtime_error("recob::Trajectory constructed with "
      + std::to_string(fPositions.size())
      + " trajectory points! it requires at least 2."
      );
  }
} // recob::Trajectory::Trajectory()


//------------------------------------------------------------------------------
bool recob::Trajectory::TrajectoryAtPoint
  (size_t i, TVector3& pos, TVector3& dir) const
{
  if (!HasPoint(i)) return false;
  
  auto const& origPos = LocationAtPoint(i);
  decltype(auto) origDir = DirectionAtPoint(i);
  
  pos.SetXYZ(origPos.X(), origPos.Y(), origPos.Z());
  dir.SetXYZ(origDir.X(), origDir.Y(), origDir.Z());
  return true;
} // recob::Trajectory::TrajectoryAtPoint(TVector&)


//------------------------------------------------------------------------------
void recob::Trajectory::Extent
  (std::vector<double>& start, std::vector<double>& end) const
  { details::legacy::FillTwoVectors(Start(), End(), start, end); }



//------------------------------------------------------------------------------
/**
 * @brief Returns the approximate length of the trajectory.
 * @param startAt (_default: 0, from beginning_) point to start from
 * @return the approximate length of the trajectory [cm]
 * 
 * The residual length from the trajectory point startAt to the end of the
 * trajectory is computed and returned. By default, the whole trajectory
 * length is returned.
 * If a non-existing point is specified, 0 is returned.
 * 
 * The length approximation is just the sum of Euclidean distances between
 * all consecutive trajectory points (starting from the one with index
 * `startAt`).
 * 
 * This operation is slow, and the result should be stored in a variable.
 */
double recob::Trajectory::Length(size_t startAt /* = 0 */) const {
  
  // sanity check
  if (startAt >= LastPoint()) return 0.;
  
  // just sum the distance between all locations in the trajectory
  Point_t const* curr = &(LocationAtPoint(startAt));
  Point_t const* next = curr;
  Point_t const* last = &(End());
  Coord_t length = 0.0;
  while (next++ != last) {
    length += (*next - *curr).R();
    curr = next;
  } // while
  return length;
} // recob::Trajectory::Length()


//------------------------------------------------------------------------------
double recob::Trajectory::ZenithAngle(size_t p /* = 0 */) const {
  
  // The zenith angle is defined by the angle between the track starting
  // direction and the y axis.
  // The y component of the starting direction is in fact the cosine of that
  // angle (and std::acos() conveniently returns a value in [0;pi]).
  // Our convention has the zenith angle the supplemental of the standard one.
  
  return util::pi<Coord_t>() - std::acos(DirectionAtPoint(p).Y());
  
} // recob::Trajectory::ZenithAngle()


//------------------------------------------------------------------------------
double recob::Trajectory::AzimuthAngle(size_t p /* = 0 */) const {
  //
  // std::atan2(y, x) returns the angle of a point (x,y) respect to x axis.
  // In our case, the definition of the angle (0 for z axis, pi/2 for x axis)
  // translates atan2's y into our x, and x into z.
  //
  decltype(auto) startDir = DirectionAtPoint(p);
  return std::atan2(startDir.X(), startDir.Z());
} // recob::Trajectory::AzimuthAngle()


//------------------------------------------------------------------------------
/**
 * @brief Computes and returns the direction of the trajectory at a point
 * @param i index of the point in the trajectory
 * @return the direction at that point
 * 
 * The direction is computed as unit vector parallel to the momentum at that
 * trajectory point.
 * If the index is not contained in the trajectory, the result is undefined.
 */
recob::Trajectory::Vector_t recob::Trajectory::DirectionAtPoint(size_t i) const
{
  
  auto const& mom = MomentumVectorAtPoint(i);
  return HasMomentum()? (mom / mom.R()): mom;
  
} // recob::Trajectory::DirectionAtPoint()


//------------------------------------------------------------------------------
void recob::Trajectory::Direction(double* start, double* end) const
  { details::legacy::FillTwoVectors(StartDirection(), EndDirection(), start, end); }


//------------------------------------------------------------------------------
recob::Trajectory::Rotation_t recob::Trajectory::GlobalToLocalRotationAtPoint
  (size_t p) const
{
  return recob::tracking::Plane::Global3DToLocal3DRotation(DirectionAtPoint(p));
} // recob::Trajectory::GlobalToLocalRotationAtPoint()


//------------------------------------------------------------------------------
void recob::Trajectory::GlobalToLocalRotationAtPoint
  (unsigned int p, TMatrixD& rot) const
{
  auto trackRot = GlobalToLocalRotationAtPoint(p);
  fillTMatrixD(trackRot, rot);
} // recob::Trajectory::GlobalToLocalRotationAtPoint(TMatrixD)


//------------------------------------------------------------------------------
recob::Trajectory::Rotation_t recob::Trajectory::LocalToGlobalRotationAtPoint
  (size_t p) const
{
  return recob::tracking::Plane::Local3DToGlobal3DRotation(DirectionAtPoint(p));
} // recob::Trajectory::GlobalToLocalRotationAtPoint()


//------------------------------------------------------------------------------
void recob::Trajectory::LocalToGlobalRotationAtPoint
  (unsigned int p, TMatrixD &rot) const
{
  auto trackRot = LocalToGlobalRotationAtPoint(p);
  fillTMatrixD(trackRot, rot);
} // recob::Trajectory::LocalToGlobalRotationAtPoint(TMatrixD)


//------------------------------------------------------------------------------
