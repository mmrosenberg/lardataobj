////////////////////////////////////////////////////////////////////////////
//
// \brief Definition of PFParticle object for LArSoft
//
// \author usher@slac.stanford.edu
//
////////////////////////////////////////////////////////////////////////////

#ifndef Recob_PFParticle_H
#define Recob_PFParticle_H

#include <vector>
#include <ostream>
#include <limits>

namespace recob {

  /**
   * @brief Hierarchical representation of particle flow
   *
   * The "Particle Flow" Particle is an object connecting to others of the
   * same type to define a hierarchy of a single parent generating multiple
   * daughter particles.
   * 
   * This structure was originally proposed to accommodate the information
   * produced by Pandora. The PFParticle is a small entity that is expected
   * to be associated with `recob::Track`, `recob::Cluster`, `recob::Hit`,
   * `recob::Vertex` and also `recob::Seed` to describe all the reconstructed
   * quantities related to it.
   *
   * The parentage relationships are expressed by indices in the collection
   * all the particles belong to. This requires additional care when creating
   * that collection, since each relation is defined in the particles with
   * indices pointing to a collection that does not yet exist when the particle
   * is created.
   * The relation is expressed as one parent and many daughters. For "primary"
   * particles, which have no parent, the special parent value
   * `recob::PFParticle::kPFParticlePrimary` must be used.
   *
   * Note that the parentage relation can not be expressed as simple _art_
   * associations, which can't express relations between two objects of the
   * same type.
   */
  class PFParticle {

  public:

      PFParticle();  ///< Default constructor necessary for gccxml - not really for public use

  private:

      int                 fPdgCode;   ///< A preliminary estimate of the PFParticle type using the PDG code
      size_t              fSelf;      ///< Self reference
      size_t              fParent;    ///< Index into PFParticle collection for parent
      std::vector<size_t> fDaughters; ///< Vector of indices into PFParticle Collection for daughters


  public:
      
    /// Define index to signify primary particle
    static constexpr size_t kPFParticlePrimary = std::numeric_limits<size_t>::max();
      
    /// Primary constructor
    PFParticle(int pdgCode, size_t self, size_t parent, const std::vector<size_t>& daughters);
    
    PFParticle(int pdgCode, size_t self, size_t parent, std::vector<size_t>&& daughters);
      
    /// Destructor definition
     ~PFParticle() = default;
      
    /// Copy constructor (using defaults)
    PFParticle(const PFParticle& other) = default;
    PFParticle(PFParticle&& other)      = default;
      
    /// Copy assignment operator (using defaults)
    PFParticle& operator= (const PFParticle& other) = default;
    PFParticle& operator= (PFParticle&& other)      = default;
    
    /// @{
    /// @name Accessors
    int                        PdgCode()            const {return fPdgCode;}
    bool                       IsPrimary()          const {return fParent == PFParticle::kPFParticlePrimary;}
    int                        NumDaughters()       const {return fDaughters.size();}
    size_t                     Self()               const {return fSelf;}
    size_t                     Parent()             const {return fParent;}
    /**
     * @brief Returns the ID of the specified daughter.
     * @param idx index of the daughter to be queried (`0` to `NumDaughters()-1`)
     * @return the ID of the specified daughter
     * @throw std::out_of_range if the requested daughter does not exist
     *
     * The returned value describes the ID of the `idx`-th daughter of this
     * PFParticle. Note that this is not the same as the index of that PFParticle
     * in the data product or PFParticle collection.
     *
     * This function checks the validity of the index (`idx`). For unchecked access,
     * use `Daughters()[idx]` instead.
     */
    size_t                     Daughter(size_t idx) const {return Daughters().at(idx);}
    const std::vector<size_t>& Daughters()          const {return fDaughters;}
    /// @}

    friend std::ostream& operator << (std::ostream& o, const PFParticle& c);
    friend bool          operator <  (const PFParticle& a, const PFParticle& b);
    
  }; // class PFParticle
} // namespace recob

#endif //Recob_PFParticle_H
