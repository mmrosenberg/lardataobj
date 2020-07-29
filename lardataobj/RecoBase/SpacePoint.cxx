////////////////////////////////////////////////////////////////////////////
//
// Implementation of SpacePoint class for LArSoft
//
// msoderbe@syr.edu
//
////////////////////////////////////////////////////////////////////////////

#include "lardataobj/RecoBase/SpacePoint.h"

#include <iomanip>
#include <algorithm> // std::swap()

namespace recob{

  //----------------------------------------------------------------------
  SpacePoint::SpacePoint() :
    fID(-1),
    fXYZ { 0.0 },
    fErrXYZ { 0.0 },
    fChisq(0.)
  {
  }

  //----------------------------------------------------------------------
  SpacePoint::SpacePoint(Double32_t const*xyz,
       Double32_t const*err,
       Double32_t  chisq,
       int    id)
    : fID(id)
    , fChisq(chisq)
  {
    for(int i = 0; i < 3; ++i) fXYZ[i]    = xyz[i];
    for(int i = 0; i < 6; ++i) fErrXYZ[i] = err[i];
  }

  //----------------------------------------------------------------------
  double SpacePoint::covariance(unsigned int i, unsigned int j) const {

    return fErrXYZ[covIndex(i, j)];

  } // SpacePoint::covariance()


  //----------------------------------------------------------------------
  constexpr std::size_t SpacePoint::covIndex(unsigned int i, unsigned int j) {

    constexpr std::size_t offsets[3U] = { 0U, 1U, 3U };

    if (i < j) std::swap(i, j);
    return offsets[i] + j;

  } // SpacePoint::covIndex()

  //----------------------------------------------------------------------
  // ostream operator.
  //
  std::ostream& operator<< (std::ostream& o, const SpacePoint & a)
  {
    o << std::setiosflags(std::ios::fixed) << std::setprecision(2);
    o << " SpacePoint ID " << std::setw(5) << std::right << a.ID()
      << " (X,Y,Z) = ("    << std::setw(5) << std::right << a.XYZ()[0]
      << " , "             << std::setw(5) << std::right << a.XYZ()[1]
      << " , "             << std::setw(5) << std::right << a.XYZ()[2]
      << ")" ;

    return o;
  }


  //----------------------------------------------------------------------
  // < operator.
  //
  bool operator < (const SpacePoint & a, const SpacePoint & b)
  {
    if(a.ID() != b. ID())
      return a.ID()<b.ID();

    return false; //They are equal

  }

}

