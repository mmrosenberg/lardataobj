////////////////////////////////////////////////////////////////////////
//
// Definition of basic digits for auxiliary detectors
//
// brebel@fnal.gov
//
////////////////////////////////////////////////////////////////////////

#ifndef RAWDATA_AUXDETDIGIT_H
#define RAWDATA_AUXDETDIGIT_H

#include <vector>
#include <iosfwd>
#include <string>

///Raw data description
namespace raw {
  
  class AuxDetDigit {

  public:
    AuxDetDigit(); // Default constructor
    
  private:

    std::vector<short> fADC;        ///< vector of adc counts
    unsigned short     fChannel;    ///< channel in the readout
    std::string        fAuxDetName; ///< name of the detector
    unsigned long long fTimeStamp;  ///< timestamp, upper 32 bits
                                    ///< for the seconds since 1970
                                    ///< lower 32 for nanoseconds
    

static_assert(sizeof(unsigned long long)==8,"unsigned long long is not 8 bytes");

  public:
    
    AuxDetDigit(unsigned short     channel,
		std::vector<short> adclist,
		std::string        name="UknownAuxDet",
		unsigned long long           timeStamp=UINT64_MAX);
    
    
    // Get Methods
    size_t             NADC()        const;
    short              ADC(size_t i) const;
    unsigned short     Channel()     const;
    std::string const& AuxDetName()  const;
    unsigned long long TimeStamp()   const;

  };
}


inline size_t             raw::AuxDetDigit::NADC()       const { return fADC.size(); }
inline unsigned short     raw::AuxDetDigit::Channel()    const { return fChannel;    }
inline std::string const& raw::AuxDetDigit::AuxDetName() const { return fAuxDetName; }
inline unsigned long long raw::AuxDetDigit::TimeStamp()  const { return fTimeStamp;  }


#endif // RAWDATA_AUXDETDIGIT_H

////////////////////////////////////////////////////////////////////////
