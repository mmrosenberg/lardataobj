/**
 * @file    Trajectory.h
 * @brief   Data product for reconstructed trajectory in space
 * @date    December 9, 2016
 * @version 2.3 (20170118)
 * 
 * Changes
 * --------
 * 
 * 20170118 [v2.3]
 *   renamed recob::Track into recob::Trajectory
 * 
 */

#ifndef LARDATAOBJ_RECOBASE_TRAJECTORY_H
#define LARDATAOBJ_RECOBASE_TRAJECTORY_H

#include "TMatrixDfwd.h" // forward declarations for legacy interface

#include "lardataobj/RecoBase/TrackingTypes.h"

// C/C++ standard libraries
#include <vector>
#include <utility> // std::pair<>


// ROOT forward declarations (for legacy interface)
class TVector3;

namespace trkf {
  class BezierTrack;
}

namespace recob {
  
  
  /** **************************************************************************
   * @brief A trajectory in space reconstructed from hits.
   * @see recob::Track,
   *      recob::trackutil::makeTrajectory()
   * 
   * The trajectory class contains a trajectory in 6D space representing the
   * path
   * walked by a particle. A trajectory point is made of a 3D position component
   * (measured in centimeters) and a momentum component (measured in GeV/c).
   * 
   * The object contains information about the trajectory, and no
   * uncertainty, as that is considered a fit result (see `recob::Track`).
   * By convention the first point is considered "start" (or "vertex") and the
   * last point "end", although this assignment can be arbitrary and should not
   * be relied upon for physics decisions.
   * 
   * The momentum content is in part optional: the trajectory creating algorithm
   * may decide not to provide information about the modulus of the momentum.
   * In that case, `Trajectory::HasMomentum()` will return false and every
   * attempt to access the momentum vector will yield only the trajectory
   * direction.
   * 
   * 
   * Invariants
   * -----------
   * 
   * * there must be at least two trajectory points;
   * * there must be one momentum per position;
   *   recob::trackutil::makeTrajectory() can make up a simple momentum vector
   *   from the trajectory, for algorithms reconstructing the trajectory which
   *   do not estimate momenta; `Trajectory::HasMomentum()` reports whether
   *   momentum modulus information is available;
   * * a list of hits associated with the trajectory is considered a mandatory
   *   part of the trajectory information; see
   *   `recob::TrajectoryCollectionMaker`.
   * 
   * 
   */
  class Trajectory {
    friend class trkf::BezierTrack;
      public:
    /// Type used for coordinates and values in general.
    using Coord_t = tracking::Coord_t;
    
    /// Type for representation of position in physical 3D space.
    using Point_t = tracking::Point_t;
    
    /// Type for representation of momenta in 3D space.
    using Vector_t = tracking::Vector_t;
    
    /// Type of trajectory point list.
    using Positions_t = tracking::Positions_t;
    
    /// Type of momentum list.
    using Momenta_t = tracking::Momenta_t;
    
    /// Mnemonics for the access to begin and end of trajectory.
    enum Ends_t {
      kStart, ///< Index representing the start of the trajectory.
      kVertex = kStart, ///< Index representing the start of the trajectory.
      kEnd,   ///< Index representing the end of the trajectory.
      NEnds   ///< Number of ends.
    }; // enum Ends_t
    
    
    /// A point in the trajectory, with position and momentum.
    using TrajectoryPoint_t = tracking::TrajectoryPoint_t;
    
    /// Type for representation of space rotations.
    using Rotation_t = tracking::Rotation_t;
    
    
    /// Default constructor; do not use it! it's needed by ROOT I/O.
    Trajectory() = default;
    
    
    /**
     * @brief Constructor: specifies all the data for the trajectory.
     * @param positions (_moved_) trajectory as a sorted list of points
     * @param momenta (_moved_) momentum along the trajectory, one per point
     * @param hasMomenta whether the information on momentum modulus is provided
     * @throws std::runtime_error if the invariants are violated
     * @see recob::trackutil::makeTrajectory()
     * 
     * The most convenient way to create a recob::Trajectory is to use
     * `recob::trackutil::makeTrajectory()`.
     * 
     * 
     * Requirements
     * -------------
     * 
     * - one momentum is required for each trajectory point
     * - at least two points must be provided
     * 
     */
    Trajectory(
      Positions_t&& positions,
      Momenta_t&& momenta,
      bool hasMomenta
      );
    
    
    /// @{
    /// @name Access to trajectory information
    
    
    /**
     * @brief Returns the number of stored trajectory points.
     * @return the number of stored trajectory points
     * @see TrajectoryAtPoint(), PositionAtPoint(), MomentumVectorAtPoint(),
     *      DirectionAtPoint()
     * 
     * For each point, both position and momentum are available.
     */
    size_t NumberTrajectoryPoints() const
      { return NPoints(); }
    
    /**
     * @brief Returns the number of stored trajectory points.
     * @return the number of stored trajectory points
     * @see TrajectoryAtPoint(), PositionAtPoint(), MomentumVectorAtPoint(),
     *      DirectionAtPoint()
     * 
     * For each point, both position and momentum are available.
     */
    size_t NPoints() const
      { return fPositions.size(); }
    
    /// Returns the index of the first point in the trajectory (yep, it's `0`).
    size_t FirstPoint() const
      { return 0U; }
    
    /// Returns the index of the last point in the trajectory.
    size_t LastPoint() const
      { return NPoints() - 1; }
    
    /**
     * @brief Returns whether the specified trajectory point is available.
     * @param i index of the trajectory point
     * @return whether the specified trajectory point is available.
     */
    bool HasPoint(size_t i) const
      { return i < NPoints(); }
    
    
    /**
     * @brief Fills position and direction at the specified trajectory point.
     * @param i index of the trajectory point
     * @param pos (output) filled with the position at the given point [cm]
     * @param dir (output) filled with the direction at the given point
     * @return false if the specified point was invalid
     * @deprecated Use TrajectoryPoint() instead (and note that it returns
     *   momentum, not direction!)
     * 
     */
    bool TrajectoryAtPoint(size_t i, TVector3& pos, TVector3& dir) const;
    
    
    /**
     * @brief Returns position and momentum at the specified trajectory point.
     * @param i index of the trajectory point
     * @return the information at the specified trajectory point
     * 
     * Note that this method returns the momentum, not the direction.
     * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~{.cpp}
     * auto trajPoint = traj.TrajectoryPoint(0);
     * std::cout << "Start of trajectory at " << trajPoint.position
     *   << " cm, with momentum " << trajPoint.momentum << " GeV/c"
     *   << std::endl;
     * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
     * 
     * If the specified index is not valid, result is undefined.
     */
    TrajectoryPoint_t TrajectoryPoint(size_t i) const
      { return { LocationAtPoint(i), MomentumVectorAtPoint(i) }; }
    
    
    /// Returns the position of the first point of the trajectory [cm].
    Point_t const& Vertex() const
      { return Start(); }
    
    /// Returns the position of the first point of the trajectory [cm].
    Point_t const& Start() const
      { return LocationAtPoint(FirstPoint()); }
    
    /// Returns the position of the last point of the trajectory [cm].
    Point_t const& End() const
      { return LocationAtPoint(LastPoint()); }
    
    
    /**
     * @brief Returns the position at the specified trajectory point.
     * @param i index of the point in the trajectory
     * @return position at the specified trajectory point [cm]
     * 
     * If the point index is invalid, the result is undefined.
     */
    Point_t const& LocationAtPoint (size_t i) const
      { return fPositions[i]; }
    
    
    /**
     * @brief Fills the first and last point in the trajectory.
     * @param start (_output_) position of the beginning of the trajectory
     * @param end (_output_) position of the end of the trajectory
     * 
     * The labelling of start and end is consistent within the trajectory but is
     * not guaranteed to be physically correct.
     * 
     * @deprecated Use physics vectors instead (see `Extent()`)
     */
    [[deprecated("Use point interface instead")]]
    void Extent(std::vector<double>& start, std::vector<double>& end) const;
    
    /**
     * @brief Returns a copy of the first and last point in the trajectory.
     * @return a pair: the first and last point in the trajectory
     * 
     * The labelling of start and end is consistent within the trajectory but is
     * not guaranteed to be physically correct.
     * 
     * Example:
     * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~{.cpp}
     * 
     * recob::Trajectory::Point_t start, end;
     * 
     * std::tie(start, end) = traj.Extent(); // assign both start and end
     * 
     * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
     */
    std::pair<Point_t, Point_t> Extent() const
      { return { Start(), End() }; }
    
    
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
    double Length (size_t startAt = 0) const;
    
    /// @}
    
    
    /// @{
    /// @name Access to direction and momentum information
    
    /**
     * @brief Returns the number of stored momenta.
     * 
     * It's the same as trajectory points, unless there is no momentum
     * (then it's 0).
     * 
     * @deprecated Use NumberTrajectoryPoints() instead
     */
    [[deprecated("Use NumberTrajectoryPoints() instead")]]
    size_t NumberFitMomentum() const
      { return HasMomentum()? NPoints(): 0U; }
    
    
    /// Returns the direction of the trajectory at the first point.
    Vector_t VertexDirection() const
      { return StartDirection(); }
 
    /// Returns the direction of the trajectory at the first point.
    Vector_t StartDirection() const
      { return DirectionAtPoint(FirstPoint()); }
    
    /// Returns the direction of the trajectory at the last point.
    Vector_t EndDirection() const
      { return DirectionAtPoint(LastPoint()); }
    
    
    /**
     * @brief Trajectory angle at point, with respect to positive _z_ direction.
     * @param p the index point to extract the angle from (default: start)
     * @return angle with respect to positive _z_, in @f$ [0,\pi] @f$ [radians]
     * 
     * The reference direction is the positive _z_ axis. Although this usually
     * matches the beam direction, this is not guaranteed and if the explicit
     * angle from beam direction is needed, the scalar product of the beam
     * direction and the direction from `StartDirection()` should be used
     * instead.
     * 
     * If the point number is invalid, the behaviour is undefined.
     */
    double Theta(size_t p = 0) const
      { return MomentumVectorAtPoint(p).Theta(); }
 
    /**
     * @brief Azimuthal angle at a point on the trajectory, with respect to _z_.
     * @param p the point index to extract the angle from (default: start)
     * @return the azimuthal angle, in @f$ [-\pi,\pi[ @f$ [radians]
     * @see Theta(), ZenithAngle()
     * 
     * The angle is measured on the plane orthogonal to the _z_ axis, in the
     * same reference where `Theta()` is measured. The angle is measured
     * counterclockwise from the _x_ axis. Skyward direction is expected to be
     * @f$ +\pi/2 @f$ radians in this system, provided that the standard
     * reference frame with the _y_ axis pointing skyward is chosen.
     * 
     * If the point number is invalid, the behaviour is undefined.
     */
    double Phi(size_t p = 0) const
      { return MomentumVectorAtPoint(p).Phi(); }
    
    
    /**
     * @brief "Zenith" angle of trajectory, with respect to the vertical axis.
     * @param p the point index to extract the angle from (default: start)
     * @return opposite of the actual zenith angle, in @f$ [0,\pi] @f$ [radians]
     * @see AzimuthAngle()
     * 
     * The angle is defined with respect to the negative _y_ direction.
     * It describes the angle at the specified point on the trajectory.
     * Therefore, a trajectory starting along the positive _y_ direction returns
     * @f$ \pi @f$ radians, while a trajectory going downward along the negative
     * _y_ direction returns 0.
     * 
     * This is designed so that vertical cosmic rays produce trajectories with
     * angle 0 radians.
     * 
     * If the point number is invalid, the behaviour is undefined.
     */
    double ZenithAngle(size_t p = 0) const;
    
    /**
     * @brief "Azimuth" angle of trajectory, with respect to the sky.
     * @param p the point index to extract the angle from (default: start)
     * @return the azimuth angle, in @f$ [-\pi,\pi[ @f$ [radians]
     * @see ZenithAngle()
     * 
     * The angle is defined on the plane orthogonal to the _y_ direction.
     * It describes the angle of the trajectory at the specified point.
     * The angle is measured starting from the positive _z_ direction,
     * counterclockwise. Therefore, a trajectory start lying on the demiplane of
     * _y_ axis and positive _z_ axis returns 0 radians, while one lying on 
     * the demiplane of _y_ axis and positive _x_ axis returns @f$ +\pi/2 @f$
     * radians.
     *
     * If the point number is invalid, the behaviour is undefined.
     */
    double AzimuthAngle(size_t p = 0) const;
    
    
    /// Returns the momentum of the trajectory at the first point [GeV/c].
    Vector_t const& VertexMomentumVector() const
      { return StartMomentumVector(); }
    
    /// Returns the momentum of the trajectory at the first point [GeV/c].
    Vector_t const& StartMomentumVector() const
      { return MomentumVectorAtPoint(FirstPoint()); }
    
    /// Returns the momentum of the trajectory at the last point [GeV/c].
    Vector_t const& EndMomentumVector() const
      { return MomentumVectorAtPoint(LastPoint()); }
    
    
    /// Computes and returns the modulus of momentum at the first point [GeV/c].
    double VertexMomentum() const
      { return StartMomentum(); }
    
    /// Computes and returns the modulus of momentum at the first point [GeV/c].
    double StartMomentum() const
      { return StartMomentumVector().R(); }
    
    /// Computes and returns the modulus of momentum at the last point [GeV/c].
    double EndMomentum() const
      { return EndMomentumVector().R(); }
    
    
    /**
     * @brief Computes and returns the direction of the trajectory at a point.
     * @param i index of the point in the trajectory
     * @return the direction at that point
     * 
     * The direction is computed as unit vector parallel to the momentum at that
     * trajectory point.
     * If the index is not contained in the trajectory, the result is undefined.
     */
    Vector_t DirectionAtPoint(size_t i) const;
    
    
    /**
     * @brief Returns whether information about the momentum is available.
     * @return whether information about the momentum is available
     * 
     * The trajectory may or may not store valid momentum information. If not,
     * the methods returning momentum information will stick to a modulus
     * 1 GeV/c and the momentum numerically matches the direction.
     */
    bool HasMomentum() const
      { return fHasMomentum; }
    
    
    /**
     * @brief Computes and returns the modulus of the momentum at a point.
     * @param i index of the point in the trajectory
     * @return modulus of the momentum at that point [GeV/c]
     * @see HasMomentum()
     * 
     * The modulus of the momentum at the specified trajectory point is computed
     * and returned.
     * If the trajectory does not have momentum information, the value 1 GeV/c
     * is always returned. This can be tested trajectory by trajectory by
     * HasMomentum().
     * If the index is not valid in the trajectory, the result is undefined.
     */
    double MomentumAtPoint(size_t i) const
      { return MomentumVectorAtPoint(i).R(); }
    
    /**
     * @brief Returns the momentum vector at a point.
     * @param i index of the point in the trajectory
     * @return the momentum at that point [GeV/c]
     * @see HasMomentum(), MomentumAtPoint(), DirectionAtPoint()
     * 
     * The momentum at the specified trajectory point is returned.
     * If the trajectory does not have momentum information, the returned value
     * will represent the direction, that is a momentum with modulus 1 GeV/c.
     * This can be tested trajectory by trajectory by HasMomentum().
     * If the index is not valid in the trajectory, the result is undefined.
     */
    Vector_t const& MomentumVectorAtPoint(size_t i) const
      { return fMomenta[i]; }
    
    /**
     * @brief Fills the starting and ending direction of the trajectory.
     * @param start (_output_) direction at the beginning of the trajectory
     * @param end (_output_) direction at the end of the trajectory
     * 
     * The two arguments are expected to point each one to an area with room for
     * at least three `double` numbers.
     * The two filled vectors have norm 1.
     * 
     * The labelling of start and end is consistent within the trajectory but is
     * not guaranteed to be physically correct.
     * 
     * @deprecated Use physics vectors instead (see `Direction()`)
     */
    void Direction(double* start, double* end) const;
    
    
    /**
     * @brief Returns the trajectory directions at first and last point.
     * @return a pair with the first and last direction
     * 
     * The two returned vectors have norm 1.
     * The labelling of start and end is consistent within the trajectory but is
     * not guaranteed to be physically correct.
     * 
     * Example:
     * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~{.cpp}
     * 
     * recob::Trajectory::Vector_t startDir, endDir;
     * 
     * std::tie(startDir, endDir) = traj.Direction(); // assign start and end
     * 
     * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
     */
    std::pair<Vector_t, Vector_t> Direction() const
      { return { StartDirection(), EndDirection() }; }
    
    
    
    /**
     * @brief Returns a rotation matrix that brings trajectory direction along
     *        _z_.
     * @param p index of the trajectory point where to apply the rotation
     * @return a rotation matrix suitable to point the trajectory along _z_
     * 
     * The returned rotation matrix, applied to the direction vector of the
     * trajectory at point `p`, will make that direction point toward the
     * positive _z_ axis direction, in a sort of "first person view" of the
     * trajectory at that point.
     * If `p` does not denote a valid trajectory point, the result is undefined.
     * 
     * The return value can be used on a `Vector_t` to rotate it. For example:
     * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~{.cpp}
     * auto rot = traj.GlobalToLocalRotationAtPoint(0);
     * auto local = rot * traj.DirectionAtPoint(0);
     * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
     * local will be a `Vector_t` object (like the direction at the right side
     * of the product) that points to (0, 0, 1).
     * 
     * While the transformation that yields the rotation matrix is continuous,
     * the direction of the new "local" _x_ and _y_ axes is not defined.
     */
    Rotation_t GlobalToLocalRotationAtPoint(size_t p) const;
    
    /**
     * @brief Fills a rotation matrix that brings trajectory direction along
     *        _z_.
     * @param p index of the trajectory point where to apply the rotation
     * @param rot (_output_) the rotation matrix to be filled
     * 
     * @deprecated Use GlobalToLocalRotationAtPoint(size_t) instead.
     */
    void GlobalToLocalRotationAtPoint(unsigned int p, TMatrixD &rot) const;
    
    /**
     * @brief Returns a rotation matrix bringing relative directions to global.
     * @param p index of the trajectory point where to apply the rotation
     * @return a rotation matrix suitable to convert directions to global
     * 
     * The returned rotation matrix, applied to the unit vector (0, 0, 1) (local
     * _z_ axis direction), will turn it into the trajectory direction at point
     * `p`.
     * If `p` does not denote a valid trajectory point, the result is undefined.
     * 
     * While the transformation that yields the rotation matrix is continuous,
     * the conversion of the directions orthogonal to the local _z_ is not
     * defined.
     */
    Rotation_t LocalToGlobalRotationAtPoint(size_t p) const;
    
    /**
     * @brief Fills a rotation matrix bringing relative directions to global.
     * @param p index of the trajectory point where to apply the rotation
     * @param rot (_output_) the rotation matrix to be filled
     * 
     * @deprecated Use LocalToGlobalRotationAtPoint(size_t) instead.
     */
    void LocalToGlobalRotationAtPoint(unsigned int p, TMatrixD &rot) const;
    
    /// @}
    
    
    /**
     * @brief Prints trajectory content into a stream.
     * @tparam Stream type of the output stream
     * @param out stream to output the information into
     * @param verbosity verbosity level (default: `1`)
     * @param indent indentation string (default: none)
     * @param indentFirst indentation for first output line (default: as indent)
     * 
     * The amount of information dumped to screen is regulated by the 
     * Indentation string is prepended to each line, and the first line has its
     * own special indentation string (`indentFirst`).
     * 
     * The output can be multi-line, it ends with no end-of-line and it does not
     * inserts an end-of-line at its beginning (unless that is explicitly inside
     * `indentFirst`).
     * The lowest verbosity is guaranteed to be on a single line.
     * 
     * 
     * Information printed out (`verbosity` argument)
     * -----------------------------------------------
     * 
     * * level `0`: start position, direction, momentum modulus and number of
     *     points
     * * level `1`: also end position, direction and momentum modulus
     * * level `2`: also trajectory length
     * * level `3`: also angles at start
     * * level `4`: also 9 intermediate trajectory points
     * * level `5`: also 10 more intermediate trajectory points (19 total)
     * * level `6`: all trajectory points
     * 
     * @internal Default values are implemented in a different method.
     * 
     */
    template <typename Stream>
    void Dump(
      Stream&& out,
      unsigned int verbosity,
      std::string indent, std::string indentFirst
      ) const;
    
    /**
     * @brief Prints trajectory content into a stream.
     * @tparam Stream type of the output stream
     * @param out stream to output the information into
     * @param verbosity verbosity level (default: `1`)
     * @param indent indentation string (default: none)
     * @see Dump(Stream&&, unsigned int, std::string, std::string)
     * 
     * Implementation detail for Dump(Stream&&, unsigned int, std::string).
     */
    template <typename Stream>
    void Dump
      (Stream&& out, unsigned int verbosity = 1, std::string indent = {})
      const
      { Dump(std::forward<Stream>(out), verbosity, indent, indent); }
    
    /**
     * @brief Prints low-level trajectory content into a stream.
     * @tparam Stream type of the output stream
     * @param out stream to output the information into
     * @param indent indentation string (default: none)
     * @param indentFirst indentation for first output line (default: as indent)
     */
    template <typename Stream>
    void LowLevelDump
      (Stream&& out, std::string indent, std::string indentFirst) const;
    
    
    /// Largest verbosity level supported by Dump().
    static constexpr unsigned int MaxDumpVerbosity = 6;
    
    
      protected:
    
      private:
    
    Positions_t fPositions; ///< List of points the trajectory goes through.
    Momenta_t   fMomenta;   ///< Momentum of each of the points in trajectory.
    
    bool fHasMomentum = true; ///< Whether we have momentum modulus information.
    
    
  }; // class Trajectory
  
  
  /**
   * @brief Prints trajectory content into a stream.
   * @tparam Stream type of the output stream
   * @param out stream to output the information into
   * @param traj trajectory to be printed
   * @return a reference to stream
   * 
   * See `recob::Trajectory::Dump()` for details.
   */
  template <typename Stream>
  Stream& operator << (Stream&& out, Trajectory const& traj)
    { traj.Dump(std::forward<Stream>(out)); return out; }
  
  
  //----------------------------------------------------------------------------
  namespace details {
    namespace legacy {
      // These implementation details are shared with other classes
      // until the legacy support is gone.
      
      /// Converts a vector into STL vector
      template <typename Vect>
      void FillVector(Vect const& source, std::vector<double>& dest);
      
      /// Converts a vector into a C array
      template <typename Vect>
      void FillVector(Vect const& source, double* dest);
      
      /// Converts two vectors into another type of vector using FillVector
      template <typename SrcVect, typename DestVect>
      void FillTwoVectors(
        SrcVect const& firstSource, SrcVect const& secondSource, 
        DestVect&& firstDest, DestVect&& secondDest
        );
      
      
    } // namespace legacy
  } // namespace details
  
  
} // namespace recob


//------------------------------------------------------------------------------
//--- Inline implementation
//---

//------------------------------------------------------------------------------
//--- Template implementation
//---
#include "Trajectory.tcc"

//------------------------------------------------------------------------------


#endif // LARDATAOBJ_RECOBASE_TRAJECTORY_H
