////////////////////////////////////////////////////////////////////////
// \version $Id:
//
// \brief Definition of ParticleID analysis object
//
// \author brebel@fnal.gov, tjyang@fnal.gov
////////////////////////////////////////////////////////////////////////

#include "lardataobj/AnalysisBase/ParticleID.h"
#include "messagefacility/MessageLogger/MessageLogger.h"

namespace anab{

  //----------------------------------------------------------------------
  ParticleID::ParticleID()
  {
  }

  //----------------------------------------------------------------------
  ParticleID::ParticleID(std::vector<anab::sParticleIDAlgScores> &ParticleIDAlgScores)
  {
    fParticleIDAlgScores = ParticleIDAlgScores;
  }

  //----------------------------------------------------------------------
  // ostream operator.
  //
  std::ostream& operator<< (std::ostream & o, ParticleID const& a)
  {
    for (size_t i=0; i < a.fParticleIDAlgScores.size(); i++){
      o << "\n ParticleIDAlg " << a.fParticleIDAlgScores.at(i).fAlgName
	<< "\n -- Variable type: " << a.fParticleIDAlgScores.at(i).fVariableType
  << "\n -- Track direction: " << a.fParticleIDAlgScores.at(i).fTrackDir
	<< "\n -- Assuming PDG: " << a.fParticleIDAlgScores.at(i).fAssumedPdg
  << "\n -- Number of degrees of freedom: " << a.fParticleIDAlgScores.at(i).fNdf
	<< "\n -- Value: " << a.fParticleIDAlgScores.at(i).fValue
	<< "\n -- Using planeID: " << a.fParticleIDAlgScores.at(i).fPlaneID;
    }
    o <<std::endl;


    return o;
  }

}
