/**
 * @file    TrackTrajectory.cxx
 * @brief   Data product for reconstructed trajectory in space
 * @date    December 9, 2016
 * @see     TrackTrajectory.h
 * 
 */


#include "lardataobj/RecoBase/TrackTrajectory.h"

// LArSoft libraries

// C/C++ standard libraries
#include <utility> // std::move()
#include <string> // std::to_string()
#include <stdexcept> // std::runtime_error

//------------------------------------------------------------------------------
recob::TrackTrajectory::TrackTrajectory(
  Positions_t&& positions, Momenta_t&& momenta, Flags_t&& flags, bool hasMomenta
  )
  : Trajectory_t(std::move(positions), std::move(momenta), hasMomenta)
  , fFlags(std::move(flags))
{
  // additional invariant check
  if (fFlags.size() != NPoints()) {
    throw std::runtime_error("recob::TrackTrajectory constructed with "
      + std::to_string(NPoints()) + " points "
      + std::to_string(fFlags.size())
      + " point flags! it requires the same number for both."
      );
  }
  if (!AtLeastValidTrajectoryPoints(2U)) {
    throw std::runtime_error("recob::TrackTrajectory constructed with only "
      + std::to_string(CountValidPoints())
      + " valid positions! at least 2 are required."
      );
  }
} // recob::TrackTrajectory::TrackTrajectory()


//------------------------------------------------------------------------------
unsigned int recob::TrackTrajectory::CountValidPoints() const {
  
  unsigned int count = 0;
  for (size_t index = 0; index < NPoints(); ++index) {
    if (HasValidPoint(index)) ++count;
  } // for
  return count;
  
} // recob::TrackTrajectory::CountValidPoints()


//------------------------------------------------------------------------------
void recob::TrackTrajectory::Extent
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
double recob::TrackTrajectory::Length(size_t startAt /* = 0 */) const {
  
  // sanity check
  if (startAt >= LastPoint()) return 0.;
  
  // just sum the distance between all locations in the trajectory
  size_t iCurr = ToValidPoint<+1>(startAt);
  size_t iNext = iCurr;
  size_t iLast = LastValidPoint();
  Point_t const* curr = &(LocationAtPoint(iCurr));
  Coord_t length = 0.0;
  while ((iNext = ToValidPoint<+1>(++iNext)) <= iLast) {
    Point_t const* next = &LocationAtPoint(iNext);
    length += (*next - *curr).R();
    curr = next;
  } // while
  return length;
} // recob::TrackTrajectory::Length()


//------------------------------------------------------------------------------
void recob::TrackTrajectory::Direction(double* start, double* end) const {
  details::legacy::FillTwoVectors(StartDirection(), EndDirection(), start, end); 
} // recob::TrackTrajectory::Direction()


//------------------------------------------------------------------------------
bool recob::TrackTrajectory::AtLeastValidTrajectoryPoints
  (unsigned int min) const
{
  if (min == 0) return true;
  unsigned int left = min;
  for (size_t i = 0; i < NPoints(); ++i) {
    if (!HasValidPoint(i)) continue;
    if (--left == 0) return true;
  } // for
  return false;
  
} // moreThanTwoValidTrajectoryPoints()

//------------------------------------------------------------------------------
