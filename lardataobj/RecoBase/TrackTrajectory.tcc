/**
 * @file   TrackTrajectory.tcc
 * @brief  Template implementation for TrackTrajectory.h
 * @author Gianluca Petrillo (petrillo@fnal.gov)
 * @date   January 3, 2016
 * 
 * This file is directly included in `Trajectory.h`.
 * 
 */

#ifndef LARDATAOBJ_RECOBASE_TRACKTRAJECTORY_TCC
#define LARDATAOBJ_RECOBASE_TRACKTRAJECTORY_TCC

#ifndef LARDATAOBJ_RECOBASE_TRACKTRAJECTORY_H
# error "Do not include TrackTrajectory.tcc. Include in TrackTrajectory.h instead."
#endif // !LARDATAOBJ_RECOBASE_TRACKTRAJECTORY_H


// LArSoft libraries
#include "lardataobj/Utilities/DataIOmanip.h" // util::manip::vector3D

// C/C++ standard libraries
#include <algorithm> // std::max()


//------------------------------------------------------------------------------
template <typename Stream>
void recob::TrackTrajectory::Dump(
  Stream&& out,
  unsigned int verbosity,
  std::string indent, std::string indentFirst
  ) const
{
  /*
   * This implementation follows this table:
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
   */
  
  if (NPoints() < 2) {
    out << indentFirst
      << "invalid track trajectory with " << NPoints() << " points";
    return;
  }
  
  //----------------------------------------------------------------------------
  out << indentFirst
    << "track trajectory with " << NPoints() << " points at "
    << util::manip::vector3D(Start()) << " cm toward "
    << util::manip::vector3D(StartDirection());
  if (HasMomentum()) out << " with momentum " << StartMomentum() << " GeV/c";
//  else out << " (no momentum information)"; // this is already evident...
  
  if (verbosity <= 0) return;
  //----------------------------------------------------------------------------
  out << indentFirst
    << "\n" << indent << "ends at " << util::manip::vector3D(End())
    << " cm toward " << util::manip::vector3D(EndDirection());
  if (HasMomentum()) out << " with momentum " << EndMomentum() << " GeV/c";
  
  if (verbosity <= 1) return;
  //----------------------------------------------------------------------------
  out
    << " running " << Length() << " cm long";
  
  if (verbosity <= 2) return;
  //----------------------------------------------------------------------------
  
  unsigned int const nInvalidPoints = NPoints() - CountValidPoints();
  if (nInvalidPoints > 0)
    out << " (with " << nInvalidPoints << " invalid points)";
  
  if (verbosity <= 3) return;
  //----------------------------------------------------------------------------
  
  out << "\n" << indent
    << "starting with theta " << Theta() << " rad, phi " << Phi()
    << " rad; zenith: " << ZenithAngle() << " rad, azimuth: " << AzimuthAngle()
    << " rad";
  
  if (verbosity <= 4) return;
  //----------------------------------------------------------------------------
  auto const startIndex = FirstValidPoint();
  auto const endIndex = LastValidPoint();
  
  auto const nPoints = NPoints();
  unsigned int nPrintedPoints;
  switch (verbosity) {
    case 4:
      nPrintedPoints = 10; break;
    case 5:
      nPrintedPoints = 20; break;
    case 6:
    default:
      nPrintedPoints = nPoints; break;
  } // switch
  float delta = std::max(float(nPoints) / nPrintedPoints, 1.0f);
  // number of trajectory points printed per line:
  constexpr unsigned int pointsPerLine = 1;
  unsigned int pointsInLine = 0;
  out << " through:";
  
  for (unsigned int step = 0; step < nPrintedPoints; ++step) {
    size_t iPoint = (size_t) std::llround(delta * step);
    if (iPoint > LastPoint()) break;
    
    // new line every pointsPerLine points
    if (pointsInLine++ == 0) out << "\n" << indent;
    if (pointsInLine >= pointsPerLine) pointsInLine = 0;
    
    out << "  [#" << iPoint << "]";
    if (HasValidPoint(iPoint)) {
      out << " at "
        << util::manip::vector3D(LocationAtPoint(iPoint)) << " cm, "
        << util::manip::vector3D(MomentumVectorAtPoint(iPoint));
      if (HasMomentum()) out << " GeV/c";
    }
    out
      << " " << FlagsAtPoint(iPoint);
    if (iPoint == startIndex) out << " <START>";
    if (iPoint == endIndex) out << " <END>";
    
  } // for
  
  
  if (verbosity <= 7) return;
  //----------------------------------------------------------------------------
  static_assert(MaxDumpVerbosity == 7,
    "recob::TrackTrajectory: either Dump() code or MaxDumpVerbosity value must be updated"
    );
  
} // recob::TrackTrajectory::Dump()


//------------------------------------------------------------------------------
template <typename Stream>
void recob::TrackTrajectory::LowLevelDump
  (Stream&& out, std::string indent, std::string indentFirst) const
{
  out << indentFirst << "recob::TrackTrajectory[" << ((void*) this) << "]("
    << "\n" << indent;
  Trajectory().LowLevelDump(std::forward<Stream>(out), indent + "  ", "");
  out << ","
    << "\n" << indent << "fFlags={ // " << fFlags.size() << " elements";
  for (size_t i = 0; i < fFlags.size(); ++i) {
    out << "\n" << indent << " [" << i << "] "
      << fFlags[i];
  }
  out
    << "\n" << indent << "}"
    << "\n" << indent << ")";
  
} // recob::TrackTrajectory::LowLevelDump()

//------------------------------------------------------------------------------
template <int Dir>
size_t recob::TrackTrajectory::ToValidPoint(size_t index) const {
  
  static_assert((Dir == +1) || (Dir == -1),
    "recob::Trajectory::ToValidPoint<Dir>() must have Dir either -1 or +1"
    );
  
  ssize_t sindex = static_cast<ssize_t>(index);
  ssize_t const last = (Dir > 0)? (ssize_t) LastPoint(): 0;
  while (!HasValidPoint(sindex)) {
    sindex += Dir;
    if (Dir * sindex > Dir * last) return InvalidIndex;
  }
  return static_cast<size_t>(sindex);
  
} // recob::TrackTrajectory::ToValidPoint<+1>()



//------------------------------------------------------------------------------


#endif // LARDATAOBJ_RECOBASE_TRACKTRAJECTORY_TCC
