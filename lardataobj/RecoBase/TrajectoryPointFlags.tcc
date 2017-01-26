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
template <typename Stream, typename T>
void recob::details::printBinary(Stream&& out, T const& data, size_t bits) {
  // TODO implement the real thing
  out << data;
} // recob::details::printBinary()



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
  
} // recob::TrajectoryPointFlags::dump()


//------------------------------------------------------------------------------


#endif // LARDATAOBJ_RECOBASE_TRAJECTORYPOINTFLAGS_TCC
