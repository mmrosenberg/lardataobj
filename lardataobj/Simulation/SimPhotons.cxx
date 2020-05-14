/**
 * @file   lardataobj/Simulation/SimPhotons.cxx
 * @brief  Simulation objects for optical detectors (implementation file).
 * @author Ben Jones
 * @date   06/04/2010
 * @see    `lardataobj/Simulation/SimPhotons.h`
 */

// library header
#include "lardataobj/Simulation/SimPhotons.h"


// -----------------------------------------------------------------------------
// ---  sim::SimPhotons
// -----------------------------------------------------------------------------
sim::SimPhotons& sim::SimPhotons::operator+= (const SimPhotons &rhs) {

  this->insert(this->begin(), rhs.cbegin(), rhs.cend());
  return *this;

} // sim::SimPhotons::operator+=()


//----------------------------------------------------------------------------
sim::SimPhotons sim::SimPhotons::operator+ (const SimPhotons &rhs) const
  { return sim::SimPhotons(*this) += rhs; }


// -----------------------------------------------------------------------------
// ---  sim::SimPhotonsLite
// -----------------------------------------------------------------------------
sim::SimPhotonsLite& sim::SimPhotonsLite::operator+=
  (const sim::SimPhotonsLite &rhs)
{

  for(auto const& phot: rhs.DetectedPhotons)
    DetectedPhotons[phot.first] += phot.second;

  return *this;
} // sim::SimPhotonsLite::operator+=()


//----------------------------------------------------------------------------
sim::SimPhotonsLite sim::SimPhotonsLite::operator+
  (const sim::SimPhotonsLite &rhs) const
  { return sim::SimPhotonsLite(*this) += rhs; }


// -----------------------------------------------------------------------------
// ---  sim::SimPhotonsCollection
// -----------------------------------------------------------------------------

