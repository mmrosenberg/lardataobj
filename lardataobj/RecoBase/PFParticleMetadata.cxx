////////////////////////////////////////////////////////////////////////////
//
// \brief Definition of PFParticleMetadata object for LArSoft
//
// \author pandora@hep.phy.cam.ac.uk
//
////////////////////////////////////////////////////////////////////////////

#include "lardataobj/RecoBase/PFParticleMetadata.h"

namespace larpandoraobj{
    
    PFParticleMetadata::PFParticleMetadata() 
	{}
	
	// Constructor with a given map 
	PFParticleMetadata::PFParticleMetadata(const PropertiesMap &propertiesMap) : 
	    m_propertiesMap(propertiesMap)
	{}

	
  //-------------------------------------------------------------------------------------------------------------
  const PFParticleMetadata::PropertiesMap &PFParticleMetadata::GetPropertiesMap() const
  {
    return m_propertiesMap;
  }

  //-----------------------------------------------------------------------------------------------------------------

  void PFParticleMetadata::SetPropertiesMap(const PropertiesMap &propertiesMap)
  {
    m_propertiesMap = propertiesMap;
  }
	
}// namespace
