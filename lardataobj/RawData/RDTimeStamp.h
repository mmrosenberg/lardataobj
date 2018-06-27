////////////////////////////////////////////////////////////////////////
//
// A timestamp data product to be associated with raw::RawDigit
// Tom Junk, June 26, 2018
//
////////////////////////////////////////////////////////////////////////

#ifndef  RDTIMESTAMPDATAPRODUCT_H
#define  RDTIMESTAMPDATAPRODUCT_H

#include <stdint.h>

namespace raw {

  class RDTimeStamp
  {

  public:
    RDTimeStamp(uint64_t tstamp); // Default constructor
 
#ifndef __GCCXML__
    void SetTimeStamp(uint64_t tstamp);
    void SetTimeStamp_Low(uint32_t tslow);
    void SetTimeStamp_High(uint32_t tshigh);
    uint64_t GetTimeStamp() const;
    uint32_t GetTimeStamp_Low() const;
    uint32_t GetTimeStamp_High() const;
#endif

  private:
    uint64_t fTimeStamp;
  };

#ifndef __GCCXML__
  inline void RDTimeStamp::SetTimeStamp(uint64_t tstamp) { fTimeStamp = tstamp; }
  inline void RDTimeStamp::SetTimeStamp_Low(uint32_t tslow) { fTimeStamp &= 0xFFFFFFFF00000000; fTimeStamp |= tslow; }
  inline void RDTimeStamp::SetTimeStamp_High(uint32_t tshigh) { fTimeStamp &= 0xFFFFFFFF; fTimeStamp |= ( (uint64_t) tshigh << 32); }
  inline uint64_t RDTimeStamp::GetTimeStamp() const { return fTimeStamp; }
  inline uint32_t RDTimeStamp::GetTimeStamp_Low() const { return fTimeStamp & 0xFFFFFFFF; }
  inline uint32_t RDTimeStamp::GetTimeStamp_High() const { return fTimeStamp >> 32; }
#endif

}


#endif // RDTIMESTAMPDATAPRODUCT_H
