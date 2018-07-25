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
#include <vector>


namespace recob {

  class Slice  {

  public:
    Slice(); // Needed to hide constexpr from GCCXML.

  private:

    int                  fID;           ///< id for this slice
    float                fAspectRatio;  ///< absolute value of the linear correlation coefficient (0 = round blob, 1 = line)

  public:
    explicit Slice(int id, float aspectratio);

    int                  ID()            const;
    float                AspectRatio()   const;
    void                 SetID(const int id) { fID = id;}
    void                 SetAspectRatio(const float aspectratio) {fAspectRatio = aspectratio;}

  };
}


inline int   recob::Slice::ID()          const { return fID; }
inline float recob::Slice::AspectRatio() const { return fAspectRatio; }

#endif // RB_SLICE_H
