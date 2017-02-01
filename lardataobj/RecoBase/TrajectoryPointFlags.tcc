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
   * * level `0`: mask of the flags, and index
   * * level `1`: name of the flags set, and index
   * 
   */
  
  //----------------------------------------------------------------------------
  out << indentFirst;
  if (verbosity == 0) {
    fFlags.dump(std::forward<Stream>(out));
  }
  else { // verbosity >= 1
    out << "{";
    unsigned int count = 0;
    for (FlagIndex_t index = 0; index < flag::maxFlags(); ++index) {
      if (!isDefined(index)) continue;
      if (count++) out << ',';
      out << ' ';
      if (isUnset(index)) out << '!';
      switch (verbosity) {
        case 0:
          out << index;
          break;
        default:
          out << flag::name(index);
      } // switch
    } // for
    out << " }";
  }
  if (hasOriginalHitIndex()) out << ", hit index: " << fromHit();
  else out << " (no hit index)";
  
} // recob::TrajectoryPointFlags::dump()


//------------------------------------------------------------------------------


#endif // LARDATAOBJ_RECOBASE_TRAJECTORYPOINTFLAGS_TCC
