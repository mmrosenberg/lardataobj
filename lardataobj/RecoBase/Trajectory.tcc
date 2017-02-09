/**
 * @file   Trajectory.tcc
 * @brief  Template implementation for Trajectory.h
 * @author Gianluca Petrillo (petrillo@fnal.gov)
 * @date   January 3, 2016
 * 
 * This file is directly included in `Trajectory.h`.
 * 
 */

#ifndef LARDATAOBJ_RECOBASE_TRAJECTORY_TCC
#define LARDATAOBJ_RECOBASE_TRAJECTORY_TCC

#ifndef LARDATAOBJ_RECOBASE_TRAJECTORY_H
# error "Do not include Trajectory.tcc. Include in Trajectory.h instead."
#endif // !LARDATAOBJ_RECOBASE_TRAJECTORY_H


// LArSoft libraries
#include "lardataobj/Utilities/DataIOmanip.h" // util::manip::vector3D

// C/C++ standard libraries
#include <algorithm> // std::max()
#include <utility> // std::forward()


//------------------------------------------------------------------------------
template <typename Stream>
void recob::Trajectory::Dump(
  Stream&& out,
  unsigned int verbosity,
  std::string indent, std::string indentFirst
  ) const
{
  /*
   * This implementation follows this table:
   * 
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
   */
  
  if (NPoints() < 2) {
    out << indentFirst
      << "invalid trajectory with " << NPoints() << " points";
    return;
  }
  
  //----------------------------------------------------------------------------
  out << indentFirst
    << "trajectory with " << NPoints() << " points at "
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
  
  out << "\n" << indent
    << "starting with theta " << Theta() << " rad, phi " << Phi()
    << " rad; zenith: " << ZenithAngle() << " rad, azimuth: " << AzimuthAngle()
    << " rad";
  
  if (verbosity <= 3) return;
  //----------------------------------------------------------------------------
  auto const nPoints = NPoints();
  unsigned int nPrintedPoints;
  switch (verbosity) {
    case 4:
      nPrintedPoints =  9; break;
    case 5:
      nPrintedPoints = 19; break;
    case 6:
    default:
      nPrintedPoints = nPoints - 2; break;
  } // switch
  float delta = std::max(float(nPoints - 1) / (nPrintedPoints + 1), 1.0f);
  // number of trajectory points printed per line:
  constexpr unsigned int pointsPerLine = 2;
  unsigned int pointsInLine = 0;
  out << " through:";
  
  for (unsigned int step = 1; step <= nPrintedPoints; ++step) {
    size_t iPoint = delta * step;
    if (iPoint >= LastPoint()) break;
    
    // new line every pointsPerLine points
    if (pointsInLine++ == 0) out << "\n" << indent;
    if (pointsInLine >= pointsPerLine) pointsInLine = 0;
    
    out << "  [#" << iPoint << "] at "
      << util::manip::vector3D(LocationAtPoint(iPoint)) << " cm, "
      << util::manip::vector3D(MomentumVectorAtPoint(iPoint));
    if (HasMomentum()) out << " GeV/c";
    
  } // for
  
  
  if (verbosity <= 6) return;
  //----------------------------------------------------------------------------
  static_assert(MaxDumpVerbosity == 6,
    "recob::Trajectory: either Dump() code or MaxDumpVerbosity value must be updated"
    );
  
} // recob::Trajectory::Dump()


//------------------------------------------------------------------------------
template <typename Stream>
void recob::Trajectory::LowLevelDump
  (Stream&& out, std::string indent, std::string indentFirst) const
{
  out << indentFirst << "recob::Trajectory[" << ((void*) this) << "]("
    << "\n" << indent << "fPositions={ // " << fPositions.size() << " elements";
  for (size_t i = 0; i < fPositions.size(); ++i) {
    out << "\n" << indent << " [" << i << "] "
      << util::manip::vector3D(fPositions[i]);
  }
  out
    << "\n" << indent << "},"
    << "\n" << indent << "fMomenta={ // " << fMomenta.size() << " elements";
  for (size_t i = 0; i < fMomenta.size(); ++i) {
    out << "\n" << indent << " [" << i << "] "
      << util::manip::vector3D(fMomenta[i]);
  }
  out
    << "\n" << indent << "},"
    << "\n" << indent << "fHasMomenta=" << fHasMomentum
    << "\n" << indent << ")";
  
} // recob::Trajectory::LowLevelDump()


//------------------------------------------------------------------------------
template <typename Vect>
void recob::details::legacy::FillVector
  (Vect const& source, std::vector<double>& dest)
{
  dest.resize(3);
  dest[0] = source.X();
  dest[1] = source.Y();
  dest[2] = source.Z();
} // recob::details::legacy::FillVector(std::vector<double>)

template <typename Vect>
void recob::details::legacy::FillVector
  (Vect const& source, double* dest)
{
  // just hope there is enough space
  dest[0] = source.X();
  dest[1] = source.Y();
  dest[2] = source.Z();
} // recob::details::legacy::FillVector(double*)


template <typename SrcVect, typename DestVect>
void recob::details::legacy::FillTwoVectors(
  SrcVect const& firstSource, SrcVect const& secondSource,
  DestVect&& firstDest, DestVect&& secondDest
  )
{
  FillVector(firstSource, std::forward<DestVect>(firstDest));
  FillVector(secondSource, std::forward<DestVect>(secondDest));
} // recob::details::legacy::FillTwoVectors()


//------------------------------------------------------------------------------


#endif // LARDATAOBJ_RECOBASE_TRAJECTORY_TCC
