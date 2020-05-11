/**
 * @file   lardataobj/Simulation/SimPhotons.cxx
 * @brief  Simulation objects for optical detectors (implementation file).
 * @author Ben Jones
 * @date   06/04/2010
 * @see    `lardataobj/Simulation/SimPhotons.h`
 */

#include "lardataobj/Simulation/SimPhotons.h"

namespace sim
{

  //----------------------------------------------------------------------------
  SimPhotonsCollection::SimPhotonsCollection()
  {
  }

  //----------------------------------------------------------------------------
  /*
    SimPhotons SimPhotonsCollection::GetHit(int key)
  {
    if( !((*this)[key]) )
      (*this)[key] = new SimPhotons();
     return (*this)[key];
  }
  */
  //----------------------------------------------------------------------------
  /*
  SimPhotonsCollection & SimPhotonsCollection::operator+=(const SimPhotonsCollection &rhs)
  {
    for(SimPhotonsCollection::const_iterator it = rhs.begin(); it!=rhs.end(); it++){
      GetHit(it->first).operator+=(it->second);
    }
    SetSDName("CompositeHitCollection");
    return *this;
  }
  */
  //----------------------------------------------------------------------------
  /*
  const SimPhotonsCollection SimPhotonsCollection::operator+(const SimPhotonsCollection &rhs) const
  {
    return SimPhotonsCollection(*this)+=rhs;
  }
  */
  //----------------------------------------------------------------------------
  OnePhoton::OnePhoton()
  {
  }

  //----------------------------------------------------------------------------
  SimPhotonsLite::SimPhotonsLite()
  {
  }


  //----------------------------------------------------------------------------
  SimPhotons::SimPhotons()
  {
  }

  //----------------------------------------------------------------------------
  SimPhotons & SimPhotons::operator+=(const SimPhotons &rhs)
  {
    for(SimPhotons::const_iterator it = rhs.begin(); it!=rhs.end(); it++){
      push_back(*it);
    }
    return *this;
  }

  //----------------------------------------------------------------------------
  const SimPhotons SimPhotons::operator+(const SimPhotons &rhs) const
  {
    return SimPhotons(*this)+=rhs;
  }

  //----------------------------------------------------------------------------
  SimPhotonsLite & SimPhotonsLite::operator+=(const SimPhotonsLite &rhs)
  {

    for(auto const& phot : rhs.DetectedPhotons)
      this->DetectedPhotons[phot.first] += phot.second;

    return *this;
  }

  //----------------------------------------------------------------------------
  const SimPhotonsLite SimPhotonsLite::operator+(const SimPhotonsLite &rhs) const
  {
    return SimPhotonsLite(*this)+=rhs;
  }

}
