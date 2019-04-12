////////////////////////////////////////////////////////////////////////////////////
///
/// \file   lardataobj/RecoBase/TrackHitMeta.h
///
/// \brief  Class to keep data related to `recob::Hit` associated with `recob::Track`.
///
/// \author R. Sulej
///
////////////////////////////////////////////////////////////////////////////////////

#ifndef LARDATAOBJ_RECOBASE_TRACKHITMETA_H
#define LARDATAOBJ_RECOBASE_TRACKHITMETA_H

#include <iosfwd>

// #include "larcoreobj/SimpleTypesAndConstants/geo_vectors.h"

namespace recob {

/**
 * \brief Data related to `recob::Hit` associated with `recob::Track`.
 * \ingroup DataProductRecoBase
 *
 * The purpose is to collect several variables that do not work well alone inside
 * track class and are related to 2D hits along the 3D trajectory. So in the first
 * place it is the hit index along the trajectory.
 * There is also dx associated to hit to help dE/dx calculations.
 *
 * Please, add other variables that may fit here. One candidate is 3D position.
 *
 * PLEASE, remember to add **errors on values** whenever this is possible to calculate.
 *
 * The expected association takes the form of:
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~{.cpp}
 * art::Assns<recob::Track, recob::Hit, recob::TrackHitMeta>
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * It is also expected and recommended practice to create the association list
 * sorted by track, as in the
 * @ref LArSoftProxyDefinitionOneToManySeqAssn "one-to-many sequential association"
 * definition.
 *
 */
class TrackHitMeta
{
public:
	/// Default needed by ROOT.
	TrackHitMeta() = default;

	/// Constructor with initialization.
	TrackHitMeta(unsigned int idx, double dx = 0.0)
	  : fIndex(idx), fDx(dx)
	  {}

	/// Hit index along the track trajectory.
	unsigned int Index() const { return fIndex; }

	/**
	 * \brief Length of the track segments associated with the 2D hit.
	 * \return lenth of the track segments [cm]
	 *
	 * The length is the sum of lengths of the two segments: half-distance to the
	 * next hit in the same plane and half-distance to the preceding hit in the
	 * same plane.
	 */
	double Dx() const { return fDx; }

	// /// Candidate to keep 3D trajectory point here instead of inside recob::Track
	//geo::Point_t const & Position3D(void) const { return fPosition3D; }

private:
	unsigned int fIndex = 0U; ///< Stored index of the hit in the sequence.
	double fDx = 0.0; ///< Stored _dx_ size, in centimeters.

	//geo::Point_t fPosition3D;
}; // class TrackHitMeta


inline std::ostream& operator<< (std::ostream& o, const TrackHitMeta & a)
  { o << a.Index(); return o; }

inline bool operator < (const TrackHitMeta & a, const TrackHitMeta & b)
  { return a.Index() < b.Index(); }


} // namespace recob

#endif // LARDATAOBJ_RECOBASE_TRACKHITMETA_H
