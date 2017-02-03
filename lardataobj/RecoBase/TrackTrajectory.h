/**
 * @file    TrackTrajectory.h
 * @brief   Data product for reconstructed trajectory in space
 * @date    December 9, 2016
 * @version 2.4 (20170119)
 * 
 * Changes
 * --------
 * 
 * - 20170119 [v2.4]  
 *   taken from recob::Trajectory, with added flags
 * 
 */

#ifndef LARDATAOBJ_RECOBASE_TRACKTRAJECTORY_H
#define LARDATAOBJ_RECOBASE_TRACKTRAJECTORY_H

// ROOT libraries
#include "lardataobj/RecoBase/Trajectory.h"
#include "lardataobj/RecoBase/TrajectoryPointFlags.h"

// C/C++ standard libraries
#include <vector>
#include <limits> // std::numeric_limits<>


namespace recob {
  
  
  /** **************************************************************************
   * @brief A trajectory in space reconstructed from hits.
   * @see recob::Trajectory, recob::Track,
   *      recob::trackutil::makeTrajectory()
   * 
   * The track trajectory class contains a trajectory in 6D space representing
   * the path walked by a particle. A trajectory point is made of a 3D position
   * component (measured in centimeters) and a momentum component (measured in
   * GeV/c).
   * The associated hits are integral part of the track trajectory.
   * To store additional point-by-point information, the track trajectory
   * augments `recob::Trajectory`, of which it presents most of the interface,
   * with point metadata called for convenience "flags".
   * 
   * Each point is formally associated to a reconstructed hit, as for
   * `recob::Trajectory` requirements. One flag set is provided for each point,
   * whose flags describe the point and/or the hit.
   * 
   * The meaning of the flags is documented also in the flags class
   * `recob::TrajectoryPointFlagTraits`, which can be accessed as typedef
   * `flags` in this class (e.g. `recob::TrackTrajectory::flag::NoPoint`).
   * 
   * 
   * Invariants
   * -----------
   * 
   * The same as `recob::Trajectory`, plus:
   * * there must be one flag set per trajectory point
   * * there must be at least two points with the flag `NoPoint` not set
   * 
   */
  class TrackTrajectory: private recob::Trajectory {
    using Trajectory_t = recob::Trajectory;

    friend class trkf::BezierTrack;
    
      public:
    /// Type used for coordinates and values in general.
    using Coord_t = tracking::Coord_t;
    
    /// Type for representation of position in physical 3D space.
    using Point_t = tracking::Point_t;
    
    /// Type for representation of momenta in 3D space.
    using Vector_t = tracking::Vector_t;
    
    /// Type for flags of a point/hit
    using PointFlags_t = recob::TrajectoryPointFlags;
    
    /// Flag traits (including the definition of flag mnemonics).
    using flag = PointFlags_t::flag;
    
    /// Type of trajectory point list.
    using Positions_t = tracking::Positions_t;
    
    /// Type of momentum list.
    using Momenta_t = tracking::Momenta_t;
    
    /// Type of point flag list.
    using Flags_t = std::vector<PointFlags_t>;
    
    /// Mnemonics for the access to begin and end of trajectory.
    using Ends_t = Trajectory_t::Ends_t;
    
    /// A point in the trajectory, with position and momentum.
    using TrajectoryPoint_t = tracking::TrajectoryPoint_t;
    
    /// Type for representation of space rotations.
    using Rotation_t = tracking::Rotation_t;
    
    
    /// Default constructor; do not use it! it's needed by ROOT I/O.
    TrackTrajectory() = default;
    
    
    /**
     * @brief Constructor: specifies all the data for the trajectory.
     * @param positions (_moved_) trajectory as a sorted list of points
     * @param momenta (_moved_) momentum along the trajectory, one per point
     * @param flags (_moved_) flag sets, one flag set per point
     * @param hasMomenta whether the information on momentum modulus is provided
     * @throws std::runtime_error if the invariants are violated
     * @see recob::trackutil::makeTrackTrajectory()
     * 
     * The most convenient way to create a recob::Trajectory is to use
     * `recob::trackutil::makeTrackTrajectory()`.
     * 
     * 
     * Requirements
     * -------------
     * 
     * - one momentum is required for each trajectory point
     * - one flag is required for each trajectory point
     * - at least two points must be provided
     * 
     */
    TrackTrajectory(
      Positions_t&& positions,
      Momenta_t&& momenta,
      Flags_t&& flags,
      bool hasMomenta
      );
    
    
    /// Returns the plain trajectory of this object
    Trajectory_t const& Trajectory() const
      { return static_cast<Trajectory_t const&>(*this); }
    
    
    using Trajectory_t::NumberTrajectoryPoints;
    
    using Trajectory_t::NPoints;
    
    using Trajectory_t::FirstPoint;
    
    using Trajectory_t::LastPoint;
    
    using Trajectory_t::HasPoint;
    
    using Trajectory_t::TrajectoryAtPoint;
    
    /**
     * @brief Returns the flags for the specified trajectory point.
     * @param i index of the point in the trajectory
     * @return flags for the specified trajectory point [cm]
     * 
     * If the point index is invalid, the result is undefined.
     */
    PointFlags_t const& FlagsAtPoint(size_t i) const
      { return fFlags[i]; }
    
    /**
     * @brief Returns whether the specified point has `NoPoint` flag unset.
     * @return whether the specified point has `NoPoint` flag unset
     * 
     * A point with flag `NoPoint` set is actually an invalid point, that the
     * algorithm could not at all set, but it has still a hit associated with
     * it.
     * 
     * If the point index is invalid, false is returned.
     */
    bool HasValidPoint(size_t i) const
      {
        return Trajectory().HasPoint(i)
		    && !FlagsAtPoint(i).isSet(flag::NoPoint);
      }
    
    /**
     * @brief Returns the index of the first valid point in the trajectory.
     * @return index of the first point in the trajectory, or `InvalidIndex`
     * 
     * Returns the index of the first point with the flag `NoPoint` unset.
     * It never returns `InvalidIndex` unless the track trajectory is invalid.
     */
    size_t FirstValidPoint() const
      { return NextValidPoint(0U); }
    
    /**
     * @brief Returns the index of the next valid point in the trajectory.
     * @param index starting index
     * @return index of next valid point in the trajectory, or `InvalidIndex`
     * 
     * Returns the index of the first point with the flag `NoPoint` unset,
     * starting with the point with the specified index (included), and moving
     * forward toward the end of the trajectory.
     * It returns `InvalidIndex` if point at index is invalid and there are no
     * valid points left after it.
     */
    size_t NextValidPoint(size_t index) const
      { return ToValidPoint<+1>(index); }
    
    /**
     * @brief Returns the index of the previous valid point in the trajectory.
     * @param index starting index
     * @return index of previous valid point in trajectory, or `InvalidIndex`
     * 
     * Returns the index of the first point with the flag `NoPoint` unset,
     * starting with the point with the specified index (included), and moving
     * backward toward the start of the trajectory.
     * It returns `InvalidIndex` if point at index is invalid and there are no
     * valid points before it.
     */
    size_t PreviousValidPoint(size_t index) const
      { return ToValidPoint<-1>(index); }
    
    /**
     * @brief Returns the index of the last valid point in the trajectory.
     * @return index of the last point in the trajectory, or `InvalidIndex`
     * 
     * Returns the index of the last point with the flag `NoPoint` unset.
     * It never returns `InvalidIndex` unless the track trajectory is invalid.
     */
    size_t LastValidPoint() const
      { return PreviousValidPoint(LastPoint()); }
    
    /**
     * @brief Computes and returns the number of points with valid location.
     * @return number of points in the trajectory with valid location
     * 
     * This method is slow, taking O(NPoints()) time.
     */
    unsigned int CountValidPoints() const;
    
    
    using Trajectory_t::TrajectoryPoint;
    
    /// Returns the position of the first valid point of the trajectory [cm].
    Point_t const& Vertex() const
      { return Start(); }
    
    /// Returns the position of the first valid point of the trajectory [cm].
    Point_t const& Start() const
      { return LocationAtPoint(FirstValidPoint()); }
    
    /// Returns the position of the last valid point of the trajectory [cm].
    Point_t const& End() const
      { return LocationAtPoint(LastValidPoint()); }
    
    using Trajectory_t::LocationAtPoint;
    
    /**
     * @brief Fills the first and last valid point in the trajectory.
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
     * @brief Returns a copy of the first and last valid point in the
     *        trajectory.
     * @return a pair: the first and last point in the trajectory
     * 
     * The labelling of start and end is consistent within the trajectory but is
     * not guaranteed to be physically correct.
     * 
     * Example:
     * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~{.cpp}
     * 
     * recob::TrackTrajectory::Point_t start, end;
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
     * All invalid points are skipped. If after skipping, less than two valid
     * points are left, 0 is returned.
     * 
     * The length approximation is just the sum of Euclidean distances between
     * each valid trajectory point and the next (starting from the first valid
     * one with index `startAt` or larger).
     * 
     * This operation is slow, and the result should be stored in a variable.
     */
    double Length (size_t startAt = 0) const;
    
    
    [[deprecated("Use NumberTrajectoryPoints() instead")]]
    size_t NumberFitMomentum() const
      { return HasMomentum()? NPoints(): 0U; }
    
    /// Returns the direction of the trajectory at the first point.
    Vector_t VertexDirection() const
      { return StartDirection(); }
 
    /// Returns the direction of the trajectory at the first point.
    Vector_t StartDirection() const
      { return DirectionAtPoint(FirstValidPoint()); }
    
    /// Returns the direction of the trajectory at the last point.
    Vector_t EndDirection() const
      { return DirectionAtPoint(LastValidPoint()); }
    
    
    /**
     * @brief Trajectory angle at point, with respect to positive _z_ direction.
     * @param p the index point to extract the angle from (_no default!_)
     * @return angle with respect to positive _z_, in @f$ [0,\pi] @f$ [radians]
     * 
     * The angle @f$ \vartheta @f$ is returned, as defined in
     * `recob::Trajectory::Theta()`, for point with the specified index `p`.
     * 
     * If the point is invalid, the behaviour is undefined.
     * 
     * @note This function has no default value for `p`; if `p` is not specified
     *       at all, the method `Theta()` is called instead.
     *   
     */
    double Theta(size_t p) const
      { return Trajectory().Theta(p); }
 
    /**
     * @brief Trajectory angle at start, with respect to positive _z_ direction.
     * @return angle with respect to positive _z_, in @f$ [0,\pi] @f$ [radians]
     * 
     * The angle @f$ \vartheta @f$ is returned, as defined in
     * `recob::Trajectory::Theta()`, for the first valid point in the
     * trajectory.
     * 
     * @note This is _not_ equivalent to `Theta(0)`, but instead to
     *       `Theta(FirstValidPoint())`.
     */
    double Theta() const
      { return Theta(FirstValidPoint()); }
 
    /**
     * @brief Azimuthal angle at a point on the trajectory, with respect to _z_.
     * @param p the index point to extract the angle from (_no default!_)
     * @return the azimuthal angle, in @f$ [-\pi,\pi[ @f$ [radians]
     * @see Phi(), Theta(size_t), ZenithAngle(size_t)
     * 
     * The angle @f$ \phi @f$ is returned, as defined in
     * `recob::Trajectory::Phi()`, for point with the specified index `p`.
     * 
     * If the point is invalid, the behaviour is undefined.
     * 
     * @note This function has no default value for `p`; if `p` is not specified
     *       at all, the method `Phi()` is called instead.
     * 
     */
    double Phi(size_t p) const
      { return Trajectory().Phi(p); }
    
    /**
     * @brief Azimuthal angle at a first valid point, with respect to _z_.
     * @return angle with respect to positive _z_, in @f$ [0,\pi] @f$ [radians]
     * @see Phi(size_t), Theta(), ZenithAngle()
     * 
     * The angle @f$ \phi @f$ is returned, as defined in
     * `recob::Trajectory::Phi()`, for the first valid point in the trajectory.
     * 
     * @note This is _not_ equivalent to `Phi(0)`, but instead to
     *       `Phi(FirstValidPoint())`.
     */
    double Phi() const
      { return Phi(FirstValidPoint()); }
    
    
    /**
     * @brief "Zenith" angle of trajectory, with respect to the vertical axis.
     * @param p the index point to extract the angle from (_no default!_)
     * @return opposite of the actual zenith angle, in @f$ [0,\pi] @f$ [radians]
     * @see AzimuthAngle(size_t)
     * 
     * The zenith is returned, as defined in `recob::Trajectory::Zenith()`,
     * for point with the specified index `p`.
     * 
     * If the point is invalid, the behaviour is undefined.
     * 
     * @note This function has no default value for `p`; if `p` is not specified
     *       at all, the method `Zenith()` is called instead.
     * 
     */
    double ZenithAngle(size_t p) const
      { return Trajectory().ZenithAngle(p); }
    
    /**
     * @brief "Zenith" angle of trajectory, with respect to the vertical axis.
     * @return opposite of the actual zenith angle, in @f$ [0,\pi] @f$ [radians]
     * @see Zenith(size_t), Theta()
     * 
     * The zenith angle is returned, as defined in
     * `recob::Trajectory::Zenith()`, for the first valid point in the
     * trajectory.
     * 
     * @note This is _not_ equivalent to `Zenith(0)`, but instead to
     *       `Zenith(FirstValidPoint())`.
     */
    double ZenithAngle() const
      { return ZenithAngle(FirstValidPoint()); }
    
    /**
     * @brief "Azimuth" angle of trajectory, with respect to the sky.
     * @param p the index point to extract the angle from (_no default!_)
     * @return the azimuth angle, in @f$ [-\pi,\pi[ @f$ [radians]
     * @see AzimuthAngle(), ZenithAngle(size_t), Phi(size_t)
     * 
     * The azimuth is returned, as defined in `recob::Trajectory::Azimuth()`,
     * for point with the specified index `p`.
     * 
     * If the point is invalid, the behaviour is undefined.
     * 
     * @note This function has no default value for `p`; if `p` is not specified
     *       at all, the method `Azimuth()` is called instead.
     * 
     */
    double AzimuthAngle(size_t p) const
      { return Trajectory().AzimuthAngle(p); }
    
    /**
     * @brief "Azimuth" angle of trajectory, with respect to the sky.
     * @return the azimuth angle, in @f$ [-\pi,\pi[ @f$ [radians]
     * @see AzimuthAngle(size_t), ZenithAngle(), Phi()
     * @see Zenith(size_t), Theta()
     * 
     * The azimuth angle is returned, as defined in
     * `recob::Trajectory::Azimuth()`, for the first valid point in the
     * trajectory.
     * 
     * @note This is _not_ equivalent to `Azimuth(0)`, but instead to
     *       `Azimuth(FirstValidPoint())`.
     */
    double AzimuthAngle() const
      { return AzimuthAngle(FirstValidPoint()); }
    
    
    /// Returns the momentum of the trajectory at the first valid point [GeV/c].
    Vector_t const& VertexMomentumVector() const
      { return StartMomentumVector(); }
    
    /// Returns the momentum of the trajectory at the first valid point [GeV/c].
    Vector_t const& StartMomentumVector() const
      { return MomentumVectorAtPoint(FirstValidPoint()); }
    
    /// Returns the momentum of the trajectory at the last valid point [GeV/c].
    Vector_t const& EndMomentumVector() const
      { return MomentumVectorAtPoint(LastValidPoint()); }
    
    
    /// Computes and returns the modulus of momentum at the first point [GeV/c].
    /// @see StartMomentum()
    double VertexMomentum() const
      { return StartMomentum(); }
    
    /// Computes and returns the modulus of momentum at the first point [GeV/c].
    /// @see StartMomentumVector()
    double StartMomentum() const
      { return StartMomentumVector().R(); }
    
    /// Computes and returns the modulus of momentum at the last point [GeV/c].
    /// @see EndMomentumVector()
    double EndMomentum() const
      { return EndMomentumVector().R(); }
    
    
    using Trajectory_t::DirectionAtPoint;
    
    using Trajectory_t::HasMomentum;
    
    using Trajectory_t::MomentumAtPoint;
    
    using Trajectory_t::MomentumVectorAtPoint;
    
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
     * @brief Returns the trajectory directions at first and last valid points.
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
    
    
    using Trajectory_t::GlobalToLocalRotationAtPoint;
    
    using Trajectory_t::LocalToGlobalRotationAtPoint;
    
    
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
     * * level `4`: also 9 intermediate valid trajectory points
     * * level `5`: also 10 more intermediate valid trajectory points (19 total)
     * * level `6`: all valid trajectory points
     * * level `7`: all trajectory points
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
    static constexpr unsigned int MaxDumpVerbosity = 7;
    
    /// Value returned on failed index queries
    static constexpr size_t InvalidIndex = std::numeric_limits<size_t>::max();
    
      private:
    
    Flags_t fFlags;               ///< Flags of each of the points in trajectory
    
    
    /**
     * @brief Returns the index of the first valid point from index on.
     * @tparam Dir the direction to move when an index has an invalid point
     * @param index the starting index
     * @return index of the first valid point from index on (or InvalidIndex)
     * 
     * The valid direction `Dir` values are only +1 and -1.
     * The first point considered is always the one at `index`.
     * If no valid point is found, `InvalidIndex` is returned.
     * The invariant guarantees that all these calls return a valid index:
     * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~{.cpp}
     * if (!(
     *      (ToValidPoint<+1>(0)                 != InvalidIndex) // FirstValidPoint()
     *   && (ToValidPoint<-1>(LastPoint())       != InvalidIndex) // LastValidPoint()
     *   && (ToValidPoint<+1>(FirstValidPoint()) != InvalidIndex) // LastValidPoint()
     *   && (ToValidPoint<-1>(LastValidPoint())  != InvalidIndex) // FirstValidPoint()
     *   )) throw std::logic_error("Invalid TrackTrajectory!");
     * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
     */
    template <int Dir>
    size_t ToValidPoint(size_t index) const;

    /// Returns whether there are at least `min` valid points in the trajectory.
    bool AtLeastValidTrajectoryPoints(unsigned int left) const;
    
  }; // class TrackTrajectory
  
  
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
  Stream& operator << (Stream&& out, TrackTrajectory const& traj)
    { traj.Dump(std::forward<Stream>(out)); return out; }
  
  
} // namespace recob


//------------------------------------------------------------------------------
//--- Inline implementation
//---

//------------------------------------------------------------------------------
//--- Template implementation
//---
#include "TrackTrajectory.tcc"

//------------------------------------------------------------------------------


#endif // LARDATAOBJ_RECOBASE_TRACKTRAJECTORY_H
