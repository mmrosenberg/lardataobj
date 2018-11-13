////////////////////////////////////////////////////////////////////////////
//
// \brief Definition of PFParticleMetadata object for LArSoft
//
// \author pandora@hep.phy.cam.ac.uk
//
////////////////////////////////////////////////////////////////////////////

#ifndef Recob_PFParticleMetadata_H
#define Recob_PFParticleMetadata_H

#include <map>
#include <string>

namespace larpandoraobj {
  
   /**
   * @brief Metadata associated to PFParticles 
   *
   * Originally defined in larpandora, moved to lardataobj for usage 
   * in other packages avoiding circular dependencies 
   * 
   * This metadata contains information provided by pandora 
   * (e.g. IsClearCosmic, TrackScore, NuScore, etc)
   * which is stored and accessed via a std::map<std::string, float> 
   */	
  class PFParticleMetadata {
	  
  public:

      PFParticleMetadata();	  ///< Default constructor
	  
	  typedef std::map<std::string, float> PropertiesMap;

	  PFParticleMetadata(const PropertiesMap &propertiesMap); ///< Constructor given a properties map (std::map<string,float>)
	  
	  /// @name Accessors
      /// @{
	  // Returns the properties map	
	  const PropertiesMap &GetPropertiesMap() const;
	  /// @}
	  
	  /// @name Set method
      /// @{
	  // Sets the properties map	
	  void SetPropertiesMap(const PropertiesMap &propertiesMap);
	  /// @}
	  
	private:
	
	  PropertiesMap   m_propertiesMap;                ///< The properties map
  
  }; // class PFParticle
} // namespace recob

#endif //Recob_PFParticle_H
