
#ifndef MCHITCOLLECTION_H
#define MCHITCOLLECTION_H

// C++ includes
#include <vector>
#include "lardataobj/MCBase/MCHit.h"

namespace sim {
  
  class MCHitCollection : public std::vector<sim::MCHit> {

  public:

    /// Default ctor
    MCHitCollection(const unsigned int ch= ::sim::kINVALID_UINT) 
    {
      Reset();
      fChannel    = ch;
    }

    /// Method to reset
    void Reset()
    {
      fChannel = ::sim::kINVALID_UINT;
      std::vector<sim::MCHit>::clear();
    }

  private:

    unsigned int fChannel; ///< Channel number


  public:

    /// Getter for channel number
    unsigned int Channel() const { return fChannel; }

    /// For sorting
    inline bool operator< ( const MCHitCollection& rhs ) const { return fChannel < rhs.fChannel; }

    /// wrapper for push_back
    inline void push_back(const MCHit& hit)
    {

      bool sort = (!empty() && hit < (*rbegin()));

      std::vector<sim::MCHit>::push_back(hit);
      
      if(sort) std::sort(begin(),end());
      
    }
  };
}

// Define a pointer comparison
namespace std {
  template <>
  class less<sim::MCHitCollection*>
  {
  public:
    bool operator()( const sim::MCHitCollection* lhs, const sim::MCHitCollection* rhs )
    { return (*lhs) < (*rhs); }
  };
}

#endif 
