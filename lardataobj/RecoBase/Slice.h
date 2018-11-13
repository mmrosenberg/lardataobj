////////////////////////////////////////////////////////////////////////////
//
// \brief Definition of slice object for LArSoft
//
// \author tjyang@fnal.gov
//
////////////////////////////////////////////////////////////////////////////

#ifndef RB_SLICE_H
#define RB_SLICE_H


#include <iosfwd>
#include "lardataobj/RecoBase/TrackingTypes.h"

namespace recob {
  
  class Slice  {

  public:

    using Point_t = tracking::Point_t;
    using Vector_t  = tracking::Vector_t;

    Slice(); // Needed to hide constexpr from GCCXML.

    Slice(int id, Point_t center, Vector_t direction, Point_t end0Pos, Point_t end1Pos, float aspectratio, float charge)
    : fID(id), fCenter(center), fDirection(direction), fEnd0Pos(end0Pos), fEnd1Pos(end1Pos), fAspectRatio(aspectratio), fCharge(charge)
    {}

    int       ID() const { return fID; }
    Point_t   Center() const { return fCenter; }
    Vector_t  Direction() const { return fDirection; }
    Point_t   End0Pos() const { return fEnd0Pos; }
    Point_t   End1Pos() const { return fEnd1Pos; }
    float     AspectRatio() const { return fAspectRatio; }
    float     Charge() const { return fCharge; }

    void                 SetID(const int id) { fID = id;}
    void                 SetCenter(const Point_t center) { fCenter = center;}
    void                 SetDirection(const Vector_t direction) { fDirection = direction;}
    void                 SetEnd0Pos(const Point_t endpos) {fEnd0Pos = endpos;}
    void                 SetEnd1Pos(const Point_t endpos) {fEnd1Pos = endpos;}
    void                 SetAspectRatio(const float aspectratio) {fAspectRatio = aspectratio;}
    void                 SetCharge(const float charge) {fCharge = charge;}

  private:
    
    int       fID;           ///< id for this slice
    Point_t   fCenter;       ///< Center of the slice for flash matching
    Vector_t  fDirection;    ///< direction from a linear fit 
    Point_t   fEnd0Pos;      ///< Position of a SpacePoint at one end of the slice
    Point_t   fEnd1Pos;      ///< Position of the other end
    float     fAspectRatio;  ///< absolute value of the linear correlation coefficient (0 = round blob, 1 = line)
    float     fCharge;

  };
}
#endif // RB_SLICE_H
