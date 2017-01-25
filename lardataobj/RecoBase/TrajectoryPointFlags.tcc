/**
 * @file    TrajectoryPointFlags.tcc
 * @brief   Set of flags pertaining a point of the track.
 * @authors Giuseppe Cerati (cerati@fnal.gov),
 *          Gianluca Petrillo (petrillo@fnal.gov)
 * @date    January 18, 2017
 * @see     TrajectoryPointFlags.h
 *
 */

#ifndef LARDATAOBJ_RECOBASE_TRAJECTORYPOINTFLAGS_TCC
#define LARDATAOBJ_RECOBASE_TRAJECTORYPOINTFLAGS_TCC

#ifndef LARDATAOBJ_RECOBASE_TRAJECTORYPOINTFLAGS_H
# error "Do not include TrajectoryPointFlags.tcc. Include in TrajectoryPointFlags.h instead."
#endif // !LARDATAOBJ_RECOBASE_TRAJECTORYPOINTFLAGS_H


//------------------------------------------------------------------------------
inline constexpr recob::details::BitMask_t recob::details::makeMaskImpl()
  { return 0; }

#ifdef __ROOTCLING__
// genreflex ROOT 6.6.8 will emit a warning if this specialisation is not
// present (JIRA ticket 8545)
template <>
inline constexpr recob::details::BitMask_t recob::details::makeMaskImpl<>
  (FlagIndex_t first)
  { return (BitMask_t(1) << first); }
#endif // __ROOTCLING__

template <typename... OtherFlags>
constexpr recob::details::BitMask_t recob::details::makeMaskImpl
  (FlagIndex_t first, OtherFlags... others)
{
  return (sizeof...(OtherFlags) == 0)
    ? (BitMask_t(1) << first): (makeMaskImpl(first) | makeMaskImpl(others...));
} // recob::details::makeMaskImpl()



//------------------------------------------------------------------------------
template <typename Stream>
void recob::TrajectoryPointFlags::dump(
  Stream&& out,
  unsigned int verbosity,
  std::string indent, std::string indentFirst
  ) const
{
  /*
   * Information printed out (`verbosity` argument)
   * -----------------------------------------------
   * 
   * * level `0`: number of the flags set, and index
   * * level `1`: name of the flags set, and index
   * 
   */
  
  out << indentFirst << "{";
  unsigned int count = 0;
  for (FlagIndex_t index = 0; index < flag::maxFlags(); ++index) {
    if (!get(index)) continue;
    if (count++) out << ',';
    out << ' ';
    switch (verbosity) {
      case 0:
        out << index;
        break;
      default:
        out << flag::name(index);
    } // switch
  } // for
  
  out << " }";
  if (hasOriginalHitIndex()) out << ", hit index: " << fromHit();
  else out << " (no hit index)";
  
} // recob::TrajectoryPointFlags<FlagTraits>::dump()


//------------------------------------------------------------------------------


#endif // LARDATAOBJ_RECOBASE_TRAJECTORYPOINTFLAGS_TCC
