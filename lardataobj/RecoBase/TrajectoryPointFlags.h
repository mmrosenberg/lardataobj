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
#include <stdexcept> // std::out_of_range
#include <limits> // std::numeric_limits<>
#include <utility> // std::forward(), std::declval()
#include <cstddef> // std::size_t


namespace recob {
  
  namespace details {
    
    /// Prints a number in binary form
    template <typename Stream, typename T>
    void printBinary(Stream&& out, T const& data, size_t bits);
    
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
    
    using FlagIndex_t = size_t; ///< Type for bit indices.
    
    /// Number of flags allocated (may be unused and unassigned).
    static constexpr FlagIndex_t MaxFlags = 32;
    
    /// @{
    /**
     * @name Trajectory flags.
     * 
     * The definition of the flags will result somehow ambiguous, because it
     * requires a judgement that is in the end arbitrary enough that it may be
     * inconsistent between different algorithms.
     * 
     * The design idea is that typically at most one flag is set. For example,
     * if a hit is known to be associated to another trajectory, the point will
     * be marked as `Shared` but not `Merged`, and if it is `Merged` or `Shared`
     * it will not be flagged as `Suspicious`.
     * Exceptions may apply. For example, a hit known to be shared with a
     * &delta; ray which is reconstructed as a distinct trajectory may have both
     * the `Shared` and the `DeltaRay` flags.
     * 
     * Also note that there may be better ways than these flags to discover
     * information. For example, the `DetectorIssue` flag might have been left
     * unset because no check was done on the actual data quality, or because
     * the failure of the channel was not yet known at the time the trajectory
     * was reconstructed.
     * 
     */
    
    /// First trajectory flag index.
    static constexpr FlagIndex_t BeginTrajectoryFlags = 0;
    
    /// Hit was not included for the computation of the trajectory.
    static constexpr FlagIndex_t HitIgnored = 0;
    
    /// The trajectory point is not defined.
    static constexpr FlagIndex_t NoPoint = 1;
    
    /**
     * @brief The point reconstruction is somehow questionable.
     * @see Merged, DeltaRay
     * 
     * For example, the hit might look ambiguous but there is no evidence of
     * another track nearby or a &delta; ray emission. Reconstruction of the
     * point might be biased, but we can't guess which type of bias we have.
     */
    static constexpr FlagIndex_t Suspicious = 2;
    
    /// The hit might have contribution from particles other than this.
    static constexpr FlagIndex_t Merged = 3;
    
    /// The hit might have contribution from a &delta; ray.
    static constexpr FlagIndex_t DeltaRay = 4;
    
    /// The hit is associated to a problematic channel.
    static constexpr FlagIndex_t DetectorIssue = 5;
    
    /// The hit is known to be associated also to another trajectory.
    static constexpr FlagIndex_t Shared = 6;
    
    /// Reserved for a future trajectory flag.
    static constexpr FlagIndex_t TrajReserved1 = 7;
    
    /// After-the-last trajectory flag index
    static constexpr FlagIndex_t EndTrajectoryFlags = 8;
    
    /// @}
    //------------------------------------------------------------------------
    /// @{
    /**
     * @name Track flags.
     * 
     * As for the trajectory flags, the ones pertaining the track fitting are
     * designed to be set in the most exclusive way possible.
     * 
     * For example, a hit that is `Rejected` is not flagged as `ExcludedFromFit`
     * (although it is indeed not included in the fit).
     * 
     * 
     */
    
    /// First track flag index.
    static constexpr FlagIndex_t BeginTrackFlags = EndTrajectoryFlags;
    
    /// The point belongs to this track but it was not included in the fit
    /// because dubious in some sense.
    static constexpr FlagIndex_t ExcludedFromFit = 8;
    
    /// The hit is extraneous to this track.
    static constexpr FlagIndex_t Rejected = 9;
    
    /**
     * @brief The hit content has been elaborated before being used in the fit.
     * 
     * In this case, it is good practise for the fitting algorithm to associate
     * the fitted point to the new hit.
     * Nevertheless, a simpler code might assume that all associated hits are
     * from the same data product, and fail to find the new one which will
     * necessarily have to belong to a new hit collection.
     * 
     * If you find yourself in the situation where you consider setting this
     * flag, it may be useful to consult with LArSoft experts to determine if
     * further support is needed for your use case.
     */
    static constexpr FlagIndex_t Reinterpreted = 10;
    
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
      "Too many experiment-defined flags");
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
  class TrajectoryPointFlags {
    
      public:
    
    /// Type of flag traits (indices and meaning of flags).
    using flag = TrajectoryPointFlagTraits;
    
    using FlagIndex_t = typename flag::FlagIndex_t; ///< Type of flag index.
    
    using HitIndex_t = unsigned int; ///< Type for hit index.
    
    using Flags_t = std::bitset<flag::maxFlags()>; ///< Type holding the flags.
    
    /// Type of a bit mask.
    struct Mask_t {
      using BitMask_t = decltype(std::declval<Flags_t>().to_ullong());
      
      /* explicit */ constexpr Mask_t(BitMask_t mask): mask(mask) {}
      
      explicit constexpr operator BitMask_t() const { return mask; }
      
      constexpr BitMask_t bits() const { return mask; }
      
      bool operator== (Mask_t const& other) const
        { return other.bits() == bits(); }
      bool operator!= (Mask_t const& other) const
        { return other.bits() != bits(); }
      
      BitMask_t mask = 0; ///< bit map of the mask
    }; // Mask_t
    
    /// Type to tag a constructor from bit mask
    struct FromMaskTag_t {};
    
    /// Tag used to activate a constructor from a bit mask.
    static constexpr FromMaskTag_t fromMask = {};
    
    /// Value marking an invalid hit index.
    static constexpr HitIndex_t InvalidHitIndex
      = std::numeric_limits<HitIndex_t>::max();
    
    
    /// Default constructor.
    constexpr TrajectoryPointFlags() = default;
    
    /**
     * @brief Constructor: copies all the flags.
     * @param fromHit the original hit index (_default: `InvalidHitIndex`_)
     * @param flags all the flags to set, as a bit mask
     * 
     * This constructor can be used in constexpr flag definitions:
     * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~{.cpp}
     * using trkflag = recob::TrajectoryPointFlags::flag;
     * constexpr recob::TrajectoryPointFlags flags(
     *   12,
     *   recob::TrajectoryPointFlags::makeMask(
     *     trkflag::NoPoint,
     *     trkflag::HitIgnored
     *     )
     *   );
     * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
     */
    constexpr TrajectoryPointFlags
      (HitIndex_t fromHit, Mask_t flags)
      : fFromHit(fromHit)
      , fFlags(flags.bits())
      {}
    
    /**
     * @brief Constructor: activates only the specified flags.
     * @tparam Flags the type of flags to be set
     * @param fromHit the original hit index (_default: `InvalidHitIndex`_)
     * @param flags all the flags to set
     * 
     * This constructor can be used in constexpr flag definitions:
     * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~{.cpp}
     * using trkflag = recob::TrajectoryPointFlags::flag;
     * constexpr recob::TrajectoryPointFlags flags(
     *   recob::TrajectoryPointFlags::InvalidHitIndex,
     *   trkflag::NoPoint,
     *   trkflag::Merged
     *   );
     * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
     */
    template <typename... Flags>
    constexpr TrajectoryPointFlags(HitIndex_t fromHit, Flags... flags)
      : TrajectoryPointFlags(fromHit, makeMask(flags...))
      {}
    
    
    /// @{
    /// @name Access to flags by index
    
    /**
     * @brief Returns whether there is room for a flag with the specified index.
     * @return whether there is room for a flag with the specified index.
     * @@see isFlag()
     * 
     * The flag may still be not present, in the sense that the allocated bit
     * has no meaning associated to it and that flag is "unknown".
     */
    constexpr bool isAllocated(FlagIndex_t flag) const
      { return isFlag(flag); }
    
    /// Returns the number of defined flags.
    constexpr FlagIndex_t nFlags() const
      { return flag::maxFlags(); }
    
    /// Returns whether a flag with the specified index is known.
    /// (all of them are)
    constexpr bool isFlag(FlagIndex_t flag) const
      { return flag < flag::maxFlags(); }
    
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
      { return isFlag(flag) && get(flag); }
    
    /**
     * @brief Returns true if the flag exists and is not set.
     * @param flag the flag index
     * @return true if the flag exists and is not set
     * @see isSet()
     */
    bool isUnset(FlagIndex_t flag) const
      { return isFlag(flag) && !get(flag); }
    
    
    
    /// @{
    /// @name Access to flags by mask
    
    /// Returns the entire set of bits as a bit mask
    Mask_t bits() const { return fFlags.to_ullong(); }
    
    /// Returns whether any of the bits on the mask is set
    bool any(Mask_t bits) const
      { return !none(bits); }
    
    /// Returns whether any of the specified flags is set
    template <typename... Flags>
    bool any(Flags... flagIndices) const;
    
    /// Returns whether all of the bits on the mask are set
    bool all(Mask_t bits) const
      { return (fFlags & Flags_t(bits.bits())) == Flags_t(bits.bits()); }
    
    /// Returns whether all the specified flags are set
    template <typename... Flags>
    bool all(Flags... flagIndices) const;
    
    /// Returns whether all of the bits on the mask are not set
    bool none(Mask_t bits) const
      { return (fFlags & Flags_t(bits.bits())).none(); }
    
    /// Returns whether all of the specified flags are not set
    template <typename... Flags>
    bool none(Flags... flagIndices) const;
    
    
    /// @}
    
    /// @{
    /// @name Access to flags by meaning
    
    /// Returns whether the associated hit is considered ignored.
    bool isHitIgnored() const
      { return get(flag::HitIgnored); }
    
    /// Returns whether the associated point is valid.
    bool isPointValid() const
      { return !get(flag::NoPoint); }
    
    /// Returns whether the point has the `Merged` flag set.
    bool isMerged() const
      { return get(flag::Merged); }
    
    /// Returns whether the point has the `Shared` flag set.
    bool isShared() const
      { return get(flag::Shared); }
    
    /// Returns whether the point has the `DeltaRay` flag set.
    bool isDeltaRay() const
      { return get(flag::DeltaRay); }
    
    /// Returns whether the point has the `DetectorIssue` flag set.
    bool hasDetectorIssues() const
      { return get(flag::DetectorIssue); }
    
    /// Returns whether the point has the `Suspicious` flag set.
    bool isOtherwiseSuspicious() const
      { return get(flag::Suspicious); }
    
    /// Returns whether the point has no flag set among `Shared`, `DeltaRay` and
    /// `Merged`.
    bool isExclusive() const
      { return none(SomehowSharedMask()); }
    
    /// Returns whether the point has the `ExcludedFromFit` flag set.
    bool isExcludedFromFit() const
      { return get(flag::ExcludedFromFit); }
    
    /// Returns whether the point has the `Rejected` flag set.
    bool belongsToTrack() const
      { return !get(flag::Rejected); }
    
    /// Returns whether the point has the `Reinterpreted` flag set.
    bool isHitReinterpreted() const
      { return get(flag::Reinterpreted); }
    
    /// Returns whether the point has the `ExcludedFromFit` or `Rejected` flag
    /// set.
    bool isIncludedInFit() const
      { return none(ExcludedFromTrackFitMask()); }
    
    /**
     * @brief Returns whether the trajectory point has any problem flagged.
     * 
     * A problematic point is basically one with any of the defined trajectory
     * point flags set.
     */
    bool isPointFlawed() const
      { return any(ImperfectPointMask()); }
    
    /**
     * @brief Returns whether the trajectory point has no flagged problem.
     * 
     * A problematic point is basically one with any of the defined trajectory
     * point flags set.
     */
    bool isPointFlawless() const
      { return none(ImperfectPointMask()); }
    
    /// @}
    
    /// @{
    /// @name Access to hit index
    
    /// Returns whether the original hit index is valid.
    /// @see fromHit()
    constexpr bool hasOriginalHitIndex() const
      { return fromHit() != InvalidHitIndex; }
    
    
    /// Returns the original index of the hit.
    /// @return the index of the original hit (`InvalidHitIndex` if not set)
    /// @see hasOriginalHitIndex()
    constexpr HitIndex_t fromHit() const
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
     * using trkflag = recob::TrajectoryPointFlags::flag;
     * constexpr auto mask = recob::TrajectoryPointFlags::makeMask
     *   (trkflag::NoPoint, trkflag::Merged);
     * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
     */
    template <typename... Flags>
    static constexpr Mask_t makeMask(Flags... flags);
    
    /// Flags used in default construction.
    static constexpr Mask_t DefaultFlagsMask();
    
      private:
    HitIndex_t fFromHit = InvalidHitIndex; ///< Index of the original hit.
    
    /// Flags to define a non-perfect trajectory point
    static constexpr Mask_t ImperfectPointMask();
    
    /// Flags to define a hit that is in some way shared
    static constexpr Mask_t SomehowSharedMask();
    
    /// Flags to define a hit that is not included in the track fit
    static constexpr Mask_t ExcludedFromTrackFitMask();
    
    Flags_t fFlags { DefaultFlagsMask().bits() }; ///< Set of flags
    
  }; // TrajectoryPointFlags<>
  
  
  template <typename Stream>
  Stream& operator<<
    (Stream&& out, recob::TrajectoryPointFlags::Mask_t const& mask)
    {
      details::printBinary
        (std::forward<Stream>(out), mask.bits(), 8*sizeof(mask.bits()));
      return out;
    }
  
  /// Dumps flags into a stream with default verbosity
  template <typename Stream>
  Stream& operator<< (Stream&& out, recob::TrajectoryPointFlags const& flags)
    { flags.dump(std::forward<Stream>(out)); return out; }
  
  
} // namespace recob


//------------------------------------------------------------------------------
//--- inline implementation
//---
namespace recob {
  // Due to their constexpr-essiveness, the details that are used by the
  // constexpr methods need to be defined before the methods themselves.
  // Here we go.
  namespace details {
    
    using BitMask_t = TrajectoryPointFlags::Mask_t::BitMask_t;
    
    /// Creates a mask with all and only the specified bits set (empty)
    inline constexpr BitMask_t makeMaskImpl() { return 0; }

    /// Creates a mask with all and only the specified bits set
    template <typename... OtherFlags>
    constexpr BitMask_t makeMaskImpl
      (TrajectoryPointFlagTraits::FlagIndex_t first, OtherFlags... others)
    {
      return (sizeof...(OtherFlags) == 0)
        ? (BitMask_t(1) << first)
        : (makeMaskImpl(first) | makeMaskImpl(others...))
        ;
      
    } // recob::details::makeMaskImpl()
    
  } // namespace details
} // namespace recob

//------------------------------------------------------------------------------
template <typename... Flags>
inline constexpr recob::TrajectoryPointFlags::Mask_t
recob::TrajectoryPointFlags::makeMask(Flags... flags)
  { return details::makeMaskImpl(flags...); }

//------------------------------------------------------------------------------
template <typename... Flags>
bool recob::TrajectoryPointFlags::all(Flags... flagIndices) const
  { return all(makeMask(flagIndices...)); }

template <typename... Flags>
bool recob::TrajectoryPointFlags::any(Flags... flagIndices) const
  { return any(makeMask(flagIndices...)); }

template <typename... Flags>
bool recob::TrajectoryPointFlags::none(Flags... flagIndices) const
  { return none(makeMask(flagIndices...)); }

//------------------------------------------------------------------------------
inline constexpr recob::TrajectoryPointFlags::Mask_t
recob::TrajectoryPointFlags::ImperfectPointMask() {
  return makeMask(
    flag::NoPoint, flag::HitIgnored,
    flag::Suspicious,
    flag::Merged, flag::DeltaRay, flag::DetectorIssue, flag::Shared
    );
} // recob::TrajectoryPointFlags::ImperfectPointMask()

inline constexpr recob::TrajectoryPointFlags::Mask_t
recob::TrajectoryPointFlags::SomehowSharedMask()
  { return makeMask(flag::Merged, flag::DeltaRay, flag::Shared); }

inline constexpr recob::TrajectoryPointFlags::Mask_t
recob::TrajectoryPointFlags::ExcludedFromTrackFitMask()
  { return makeMask(flag::ExcludedFromFit, flag::Rejected); }


//------------------------------------------------------------------------------
inline constexpr typename recob::TrajectoryPointFlags::Mask_t
recob::TrajectoryPointFlags::DefaultFlagsMask() {
  return makeMask();
} // recob::TrajectoryPointFlags::DefaultFlagsMask()


//------------------------------------------------------------------------------
//--- template implementation
//---
  
#include "TrajectoryPointFlags.tcc"

//------------------------------------------------------------------------------

#endif // LARDATAOBJ_RECOBASE_TRAJECTORYPOINTFLAGS_H
