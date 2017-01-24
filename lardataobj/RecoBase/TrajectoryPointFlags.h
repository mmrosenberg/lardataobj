/**
 * @file   TrajectoryPointFlags.h
 * @brief  Set of flags pertaining a point of the track.
 * @author Giuseppe Cerati (cerati@fnal.gov),
 *         Gianluca Petrillo (petrillo@fnal.gov)
 * @date   January 18, 2017
 *
 */

#ifndef LARDATAOBJ_RECOBASE_TRAJECTORYPOINTFLAGS_H
#define LARDATAOBJ_RECOBASE_TRAJECTORYPOINTFLAGS_H

// C/C++ standard libraries
#include <bitset>
#include <array>
#include <string>
#include <limits> // std::numeric_limits<>
#include <initializer_list>
#include <utility> // std::forward()
#include <cstddef> // std::size_t


namespace recob {
  
  namespace details {
    
    using FlagIndex_t = std::size_t;
    using BitMask_t = unsigned long long;
    
    /// Creates a mask with no bits set
    inline constexpr BitMask_t makeMaskImpl();
    
    /// Creates a mask with all and only the specified bits set
    template <typename... OtherFlags>
    constexpr BitMask_t makeMaskImpl(FlagIndex_t first, OtherFlags... others);
    
  } // namespace details
  
  
  /**
   * @brief Namespace for the trajectory point flags
   * 
   * This class is a glorified namespace for the definition of named flags.
   * 
   * Each flag must have an identifier that is of integral type `FlagIndex_t`.
   * There are four types of flags: pertaining a trajectory (and pattern
   * recognition output), pertaining a track (and track fit output),
   * experiment specific and user specific.
   * 
   * The trajectory flags have a meaning in the bare context of reconstructed
   * trajectory in space and patter recognition (@see `recob::Trajectory` and
   * @see `recob::TrackTrajectory1), and they should preserve the same meaning
   * after fitting into a track.
   * 
   * The track flags have a meaning only in the context of a track
   * (@see `recob::Track`).
   * 
   * The experiment-specific flags are placeholders that are expected to get
   * a meaning that is consistent within a single experiment, and unrelated
   * between experiments.
   * 
   * The user-specific flags are placeholders that are expected to get a
   * meaning that is specific to an algorithm. Objects from different
   * algorithms will give these flags unrelated meaning.
   * 
   * Code shared in LArSoft using the flags should rely only on the flags of
   * the first two categories. Code that requires access to
   * experiment-specific flags should be also experiment-specific.
   * 
   */
  struct TrajectoryPointFlagTraits {
    
    using FlagIndex_t = details::FlagIndex_t; ///< Type for bit indices.
    
    /// Number of flags allocated (may be unused and unassigned).
    static constexpr FlagIndex_t MaxFlags = 32;
    
    /// @{
    /// @name Trajectory flags.
    
    /// First trajectory flag index.
    static constexpr FlagIndex_t BeginTrajectoryFlags = 0;
    
    /// Hit was not included for the computation of the trajectory.
    static constexpr FlagIndex_t HitIgnored = 0;
    
    /// The trajectory point is not defined.
    static constexpr FlagIndex_t NoPoint = 1;
    
    /// Reserved for a future trajectory flag.
    static constexpr FlagIndex_t TrajReserved6 = 2;
    
    /// Reserved for a future trajectory flag.
    static constexpr FlagIndex_t TrajReserved5 = 3;
    
    /// Reserved for a future trajectory flag.
    static constexpr FlagIndex_t TrajReserved4 = 4;
    
    /// Reserved for a future trajectory flag.
    static constexpr FlagIndex_t TrajReserved3 = 5;
    
    /// Reserved for a future trajectory flag.
    static constexpr FlagIndex_t TrajReserved2 = 6;
    
    /// Reserved for a future trajectory flag.
    static constexpr FlagIndex_t TrajReserved1 = 7;
    
    /// After-the-last trajectory flag index
    static constexpr FlagIndex_t EndTrajectoryFlags = 8;
    
    /// @}
    //------------------------------------------------------------------------
    /// @{
    /// @name Track flags.
    
    /// First track flag index.
    static constexpr FlagIndex_t BeginTrackFlags = EndTrajectoryFlags;
    
    /// Reserved for a future track flag.
    static constexpr FlagIndex_t TrackReserved8 = 8;
    
    /// Reserved for a future track flag.
    static constexpr FlagIndex_t TrackReserved7 = 9;
    
    /// Reserved for a future track flag.
    static constexpr FlagIndex_t TrackReserved6 = 10;
    
    /// Reserved for a future track flag.
    static constexpr FlagIndex_t TrackReserved5 = 11;
    
    /// Reserved for a future track flag.
    static constexpr FlagIndex_t TrackReserved4 = 12;
    
    /// Reserved for a future track flag.
    static constexpr FlagIndex_t TrackReserved3 = 13;
    
    /// Reserved for a future track flag.
    static constexpr FlagIndex_t TrackReserved2 = 14;
    
    /// Reserved for a future track flag.
    static constexpr FlagIndex_t TrackReserved1 = 15;
    
    /// After-the-last track flag index.
    static constexpr FlagIndex_t EndTrackFlags = 16;
    
    /// @}
    //------------------------------------------------------------------------
    /// @{
    /// @name Flag reserved for the experiments (not to be used in LArSoft).
    
    /// First flag reserved to experiment.
    static constexpr FlagIndex_t BeginExperimentReservedFlags = EndTrackFlags;
    
    /// After-the-last flag reserved to experiment.
    static constexpr FlagIndex_t EndExperimentReservedFlags = 24;
    
    /// @}
    //------------------------------------------------------------------------
    /// @{
    /// @name Flag reserved for the users (algorithm-specific).
    
    /// First flag reserved to users.
    static constexpr FlagIndex_t BeginUserReservedFlags
      = EndExperimentReservedFlags;
    
    /// After-the-last flag reserved to users.
    static constexpr FlagIndex_t EndUserReservedFlags = 32;
    /// @}
    //------------------------------------------------------------------------
    
    
    /// Number of flags allocated (may be unused and unassigned).
    static constexpr FlagIndex_t maxFlags() { return MaxFlags; }
    
    
    /// Returns whether the specified index represents a valid flag
    static constexpr bool isFlag(FlagIndex_t flag)
      { return flag < MaxFlags; }
    
    /// @{
    /// @name Flag names
    
    /// Returns a string with the name of the specified flag
    static std::string name(FlagIndex_t flag)
      { return (flag < MaxFlags)? names[flag]: invalidFlagName(flag); }
    
    /// Type storing flag names.
    using NameMap_t = std::array<std::string, MaxFlags>;
    
    /// Returns a map of flag names.
    static NameMap_t initNames();
    
    /// @}
    
      private:
    static_assert(EndTrajectoryFlags <= BeginTrackFlags,
      "Too many trajectory flags");
    static_assert(EndTrackFlags <= BeginExperimentReservedFlags,
      "Too many track flags");
    static_assert(EndExperimentReservedFlags <= BeginUserReservedFlags,
      "Too many expriment-defined flags");
    static_assert(EndUserReservedFlags <= MaxFlags,
      "Too many user-defined flags");
    
    static const NameMap_t names; ///< Names of the flags.
    
    /// Combines a base name and an index into a flag name.
    static std::string decorateFlagName
      (std::string baseName, FlagIndex_t flag);

    /// Returns the name of an invalid flag with the specified index.
    static std::string invalidFlagName(FlagIndex_t flag);
    
    /// Initializes a range of flag names with default (decorated) names.
    static void initDefaultFlagRangeNames(
      NameMap_t& flagNames,
      FlagIndex_t BeginFlags, FlagIndex_t EndFlags, std::string baseName
      );
    
    /// Initialises all flag names with a default name.
    static void initDefaultFlagsNames(NameMap_t& flagNames);
    
    /// Sets the names of the flags after default initialization.
    static void setFlagNames(NameMap_t& flagNames);

  }; // class TrajectoryPointFlagTraits

  
  /**
   * @brief  Set of flags pertaining a point of the track.
   * @tparam FlagTraits type with the definition of the flag values
   * @see recob::TrackTrajectory
   * 
   * The "flags" contain metadata pertaining a single point in a trajectory
   * or track.
   * 
   * The metadata includes:
   * * a set of flags, including some for a trajectory, some specific to a
   *   fitted track, some reserved for future use and some available to the
   *   users
   * * an index pointing to the position of the hit in an originating
   *   trajectory, if such a trajectory exists. The specific convention to use
   *   this index must be documented by the using class (@see
   * `recob::TrackTrajectory`)
   * 
   * The meaning of the flags is described in the FlagTraits type.
   * This type needs to provide a `maxFlags()` static constexpr method to
   * express how many flags should be stored, d
   * 
   */
  template <typename FlagTraits>
  class TrajectoryPointFlags {
    
      public:
    
    /// Type of flag traits (indices and meaning of flags).
    using flag = TrajectoryPointFlagTraits;
    
    using FlagIndex_t = typename flag::FlagIndex_t; ///< Type of flag index.
    
    using HitIndex_t = unsigned int; ///< Type for hit index.
    
    using Flags_t = std::bitset<flag::maxFlags()>; ///< Type holding the flags.
    
    ///< Type of a bit mask.
    using Mask_t = decltype(std::declval<Flags_t>().to_ullong());
    
    /// Type to tag a constructor from bit mask
    struct FromMaskTag_t {};
    
    /// Tag used to activate a constructor from a bit mask.
    static const FromMaskTag_t fromMask;
    
    /// Value marking an invalid hit index.
    static constexpr HitIndex_t InvalidHitIndex
      = std::numeric_limits<HitIndex_t>::max();
    
    
    /// Default constructor.
    constexpr TrajectoryPointFlags();
    
    /**
     * @brief Constructor: copies all the flags.
     * @param fromHit the original hit index (_default: `InvalidHitIndex`_)
     * @param flags all the flags to set, as a bit mask
     * 
     * This constructor can be used in constexpr flag definitions:
     * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~{.cpp}
     * constexpr TrajectoryPointFlags flags
     *   (TrajectoryPointFlags::fromMask, InvalidHitIndex, otherFlags.bits());
     * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
     */
    constexpr TrajectoryPointFlags
      (FromMaskTag_t, HitIndex_t fromHit, Mask_t flags)
      : fFromHit(fromHit)
      , fFlags(flags)
      {}
    
    /**
     * @brief Constructor: activates only the specified flags.
     * @tparam Flags the type of flags to be set
     * @param fromHit the original hit index (_default: `InvalidHitIndex`_)
     * @param flags all the flags to set
     * 
     * This constructor can be used in constexpr flag definitions:
     * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~{.cpp}
     * constexpr TrajectoryPointFlags flags
     *   (InvalidHitIndex, NoPoint, Merged);
     * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
     */
    template <typename... Flags>
    constexpr TrajectoryPointFlags(HitIndex_t fromHit, Flags... flags)
      : TrajectoryPointFlags(fromMask, fromHit, makeMask(flags...))
      {}
    
    
    /// @{ 
    /// @name Access to flags
    
    /// Returns the number of defined flags.
    bool nFlags() const
      { return flag::maxFlags(); }
    
    /// Returns whether there is a flag with the specified index.
    bool isFlag(FlagIndex_t flag) const
      { return flag < flag::maxFlags(); }
    
    /// Returns whether a flag with the specified index is known.
    /// (all of them are)
    bool isPresent(FlagIndex_t flag) const
      { return isFlag(flag); }
    
    /// Returns the entire set of bits as a bit mask
    constexpr Mask_t bits() const { return fFlags.to_ullong(); }
    
    
    /**
     * @brief Returns whether the specified flag is set.
     * @param flag index of the flag to be tested 
     * @return whether the specified flag is set
     * @throw std::out_of_range if the flag is not defined (invalid index)
     * 
     * A single flag is tested.
     * 
     */
    bool test(FlagIndex_t flag) const
      { return fFlags.test(flag); }
    
    
    /**
     * @brief Returns whether the specified flag is set.
     * @param flag index of the flag to be tested 
     * @return whether the specified flag is set
     * 
     * A single flag is tested.
     * If the flag is not defined (invalid index), the result is undefined.
     */
    bool get(FlagIndex_t flag) const
      { return fFlags[flag]; }
    
    
    /**
     * @brief Returns true if the flag exists and is set.
     * @param flag the flag index
     * @return true if the flag exists and is set
     * @see isUnset()
     */
    bool isSet(FlagIndex_t flag) const
      { return isPresent(flag) && get(flag); }
    
    /**
     * @brief Returns true if the flag exists and is not set.
     * @param flag the flag index
     * @return true if the flag exists and is not set
     * @see isSet()
     */
    bool isUnset(FlagIndex_t flag) const
      { return isPresent(flag) && !get(flag); }
    
    
    
    /// Returns whether the associated hit is considered ignored.
    bool isHitIgnored() const
      { return get(flag::HitIgnored); }
    
    /// Returns whether the associated point is valid.
    bool isPointValid() const
      { return !get(flag::NoPoint); }
    
    
    /// Returns whether the original hit index is valid.
    /// @see fromHit()
    bool hasOriginalHitIndex() const
      { return fromHit() != InvalidHitIndex; }
    
    
    /// Returns the original index of the hit.
    /// @return the index of the original hit (`InvalidHitIndex` if not set)
    /// @see hasOriginalHitIndex()
    HitIndex_t fromHit() const
      { return fFromHit; }
    
    /// @}
    
    
    /**
     * @brief Prints the flags content into a stream.
     * @tparam Stream type of the output stream
     * @param out stream to output the information into
     * @param verbosity verbosity level (default: `1`)
     * @param indent indentation string (default: none)
     * @param indentFirst indentation for first output line (default: as indent)
     * 
     * Prints on a single line all the flags that are set.
     * 
     * Currently `indent` is not used since the output is single line.
     * 
     * Information printed out (`verbosity` argument)
     * -----------------------------------------------
     * 
     * * level `0`: number of the flags set, and index
     * * level `1`: name of the flags set, and index
     * 
     */
    template <typename Stream>
    void dump(
      Stream&& out,
      unsigned int verbosity,
      std::string indent, std::string indentFirst
      ) const;
    
    /**
     * @brief Prints flag content into a stream.
     * @tparam Stream type of the output stream
     * @param out stream to output the information into
     * @param verbosity verbosity level (default: `1`)
     * @param indent indentation string (default: none)
     * @see Dump(Stream&&, unsigned int, std::string, std::string)
     * 
     * Implementation detail for
     * Dump(Stream&&, unsigned int, std::string, std::string).
     */
    template <typename Stream>
    void dump
      (Stream&& out, unsigned int verbosity = 1, std::string indent = {})
      const
      { dump(std::forward<Stream>(out), verbosity, indent, indent); }
    
    
    /**
     * @brief Returns a bit mask with only the specified bit set
     * @tparam Flags the type of flags to be set
     * @param flags all the flags to set
     * 
     * This method can be used in constexpr flag definitions:
     * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~{.cpp}
     * auto mask = TrajectoryPointFlags::makeMask(NoPoint, Merged);
     * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
     */
    template <typename... Flags>
    static constexpr Mask_t makeMask(Flags... flags)
      { return details::makeMaskImpl(flags...); }
    
      private:
#ifndef __GCCXML__
    /// Flags used in the construction.
    static constexpr Flags_t DefaultFlags();
#endif // __GCCXML__

    HitIndex_t fFromHit = InvalidHitIndex; ///< Index of the original hit.
    
    Flags_t fFlags; ///< Set of flags
    
  }; // TrajectoryPointFlags<>
  
  
  /// Dumps flags into a stream with default verbosity
  template <typename Stream, typename FlagTraits>
  Stream& operator<<
    (Stream&& out, recob::TrajectoryPointFlags<FlagTraits> const& flags)
    { flags.dump(std::forward<Stream>(out)); return out; }
  
  
} // namespace recob


//------------------------------------------------------------------------------
//--- template implementation
//---
  
#include "TrajectoryPointFlags.tcc"

//------------------------------------------------------------------------------

#endif // LARDATAOBJ_RECOBASE_TRAJECTORYPOINTFLAGS_H
