/**
 * @file   lardataobj/Simulation/SimPhotons.h
 * @brief  Simulation objects for optical detectors.
 * @author Ben Jones
 * @date   06/04/2010
 * @see    `lardataobj/Simulation/SimPhotons.cxx`
 *
 * This file contains the definitions of the classes which are stored in the
 * event representing scintillation photons detected by the optical detectors.
 *
 * A scintillation photon collection (`sim::SimPhotons`) stores data for each
 * photon which steps inside the optical detector volume.
 * Currently the quantities stored are position, time, 4-momentum
 * and the ID of the GEANT4 track emitting it.
 * A `sim::SimPhotonsCollection` is a set of `sim::SimPhotonss`, one per
 * optical detector in the collection.
 *
 * The `sim::SimPhotons` is filled in by the `larg4::OpFastScintillation` class
 * in `LegacyLArG4` module and will be used to generate the optical detector
 * response later in the simulation chain.
 *
 * `sim::OnePhoton`, `sim::SimPhotons` and `sim::SimPhotonsCollection` are all
 * persistent under ROOT I/O.
 *
 * The current implementation resembles that of an C++ STL container in
 * some respects but needs more work before it is polished product.
 */


#ifndef LARDATAOBJ_SIMULATION_SIMPHOTONS_H
#define LARDATAOBJ_SIMULATION_SIMPHOTONS_H


// LArSoft libraries
#include "larcoreobj/SimpleTypesAndConstants/geo_optical_vectors.h"
#include "larcoreobj/SimpleTypesAndConstants/geo_vectors.h"

// C/C++ standard libraries
#include <map>
#include <vector>
#include <string>
#include <limits> // std::numeric_limits<>


// -----------------------------------------------------------------------------
// forward declarations
namespace sim {
  class OnePhoton;
  class SimPhotons;
  class SimPhotonsLite;
  class SimPhotonsCollection;
} // namespace sim


// -----------------------------------------------------------------------------
/**
 * @brief All information of a photon entering the sensitive optical detector
 *        volume.
 */
struct sim::OnePhoton {

  /// Scintillation position in world coordinates [cm]
  geo::Point_t InitialPosition;

  /**
   * @brief Where photon enters the optical detector in local coordinates [cm]
   * 
   * The coordinates are in the local reference of the sensitive optical
   * detector, in centimeters.
   */
  geo::OpticalPoint_t FinalLocalPosition;

  /// Arrival time to the detector in
  /// @ref DetectorClocksGeant4Time "simulation time scale" [ns]
  float Time { std::numeric_limits<float>::min() };

  /// Scintillation photon energy [GeV]
  float Energy { 0.0 };

  /// ID of the GEANT4 track causing the scintillation.
  int MotherTrackID { std::numeric_limits<int>::min() };

  /// Whether the photon reaches the sensitive detector.
  bool SetInSD { true };

}; // sim::OnePhoton


// -----------------------------------------------------------------------------
/**
 * @brief Compact representation of photons on a channel.
 * @see   `sim::SimPhotons`
 *
 * Compared to `sim::SimPhotons`, this object contains only the _total count_
 * of photon arriving at a certain time on the channel. The time is
 * discretized in ticks.
 *
 */
class sim::SimPhotonsLite {
  public:

  /// Default constructor (do not use! it's for ROOT only).
  SimPhotonsLite() = default;

  /// Constructor: associated to optical detector channel `chan`, and empty.
  SimPhotonsLite(int chan)
    : OpChannel(chan)
    {}

  int OpChannel; ///< Optical detector channel associated to this data.

  /// Number of photons detected at each given time: time tick -> photons.
  std::map<int, int> DetectedPhotons;

  /// Add all photons from `rhs` to this ones, at their original time.
  SimPhotonsLite& operator+=(const SimPhotonsLite &rhs);

  /// Creates a new `sim::SimPhotonsLite` with all photons from `rhs` and
  /// this object.
  SimPhotonsLite operator+(const SimPhotonsLite &rhs) const;

  /// Returns whether `other` is on the same channel (`OpChannel`) as this.
  bool operator== (const SimPhotonsLite &other) const;

}; // sim::SimPhotonsLite


// -----------------------------------------------------------------------------
/**
 * @brief Collection of photons which recorded on one channel.
 */
class sim::SimPhotons: public std::vector<sim::OnePhoton> {

public:

  int fOpChannel; ///< Optical detector channel associated to this data.


  // --- BEGIN -- Vector types -------------------------------------------------
  /// @name Vector types
  /// @{

  typedef std::vector<OnePhoton>             list_type;
  typedef list_type::value_type              value_type;
  typedef list_type::iterator                iterator;
  typedef list_type::const_iterator          const_iterator;
  typedef list_type::reverse_iterator        reverse_iterator;
  typedef list_type::const_reverse_iterator  const_reverse_iterator;
  typedef list_type::size_type               size_type;
  typedef list_type::difference_type         difference_type;

  /// @}
  // --- END -- Vector types ---------------------------------------------------

  /// Default constructor (do not use! it's for ROOT only).
  SimPhotons() = default;

  /// Constructor: associated to optical detector channel `chan`, and empty.
  SimPhotons(int chan): fOpChannel(chan) {}


  /// Returns the optical channel number this object is associated to.
  int       OpChannel() const;

  /// Sets the optical detector channel number this object is associated to.
  void      SetChannel(int ch);


  /// Add all photons from `rhs` to this ones; no sorting is applied.
  SimPhotons& operator+=(const SimPhotons &rhs);

  /// Creates a new `sim::SimPhotons` with all photons from `rhs` and
  /// this object.
  SimPhotons operator+(const SimPhotons &rhs) const;

  /// Returns whether `other` is on the same channel (`OpChannel`) as this.
  bool operator== (const SimPhotons &other) const;

}; // sim::SimPhotons


// -----------------------------------------------------------------------------
/**
 * @brief Collection of `sim::SimPhotons`, indexed by channel number.
 *
 * The collection owns the photon data.
 */
class sim::SimPhotonsCollection : public std::map<int, sim::SimPhotons> {

  std::string fTheSDName; ///< Sensitive detector name.

public:

  // --- BEGIN -- Vector types -------------------------------------------------
  /// @name Vector types
  /// @{
  typedef std::map<int,SimPhotons>           list_type;
  typedef list_type::key_type                key_type;
  typedef list_type::mapped_type             mapped_type;
  typedef list_type::value_type              value_type;
  typedef list_type::iterator                iterator;
  typedef list_type::const_iterator          const_iterator;
  typedef list_type::reverse_iterator        reverse_iterator;
  typedef list_type::const_reverse_iterator  const_reverse_iterator;
  typedef list_type::size_type               size_type;
  typedef list_type::difference_type         difference_type;
  typedef list_type::key_compare             key_compare;
  typedef list_type::allocator_type          allocator_type;

  /// @}
  // --- END -- Vector types ---------------------------------------------------

  /// Constructor: an empty collection and no sensitive detector name.
  SimPhotonsCollection() = default;

  /// Returns the name of the sensitive detector for this collection.
  std::string const& GetSDName() const;

  /// Sets the name of the sensitive detector for this collection.
  void SetSDName(std::string const& TheSDName);

}; // sim::SimPhotonsCollection


// =============================================================================
// ===  Inline implementations
// =============================================================================
// -----------------------------------------------------------------------------
// ---  sim::SimPhotonsLite
// -----------------------------------------------------------------------------
inline bool sim::SimPhotonsLite::operator==
  (const sim::SimPhotonsLite& other) const
  { return OpChannel == other.OpChannel; }

// -----------------------------------------------------------------------------
// ---  sim::SimPhotons
// -----------------------------------------------------------------------------

inline int sim::SimPhotons::OpChannel() const { return fOpChannel; }

inline void sim::SimPhotons::SetChannel(int ch) { fOpChannel = ch; }

inline bool sim::SimPhotons::operator== (const sim::SimPhotons& other) const
  { return OpChannel() == other.OpChannel(); }

// -----------------------------------------------------------------------------
// ---  sim::SimPhotonsCollection
// -----------------------------------------------------------------------------

inline std::string const& sim::SimPhotonsCollection::GetSDName() const
  { return fTheSDName; }

inline void sim::SimPhotonsCollection::SetSDName(std::string const& TheSDName)
  { fTheSDName = TheSDName; }

// -----------------------------------------------------------------------------


#endif // LARDATAOBJ_SIMULATION_SIMPHOTONS_H
