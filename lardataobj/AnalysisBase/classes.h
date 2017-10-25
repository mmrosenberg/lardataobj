// Build a dictionary.
//
// $Author:  $
// $Date: 2010/04/12 18:12:28 $
// 
// Original author Rob Kutschke, modified by klg

#include "canvas/Persistency/Common/PtrVector.h" 
#include "canvas/Persistency/Common/Wrapper.h"
#include "canvas/Persistency/Common/Assns.h"

#include "lardataobj/AnalysisBase/MVAOutput.h"
#include "lardataobj/AnalysisBase/Calorimetry.h"
#include "lardataobj/AnalysisBase/ParticleID.h"
#include "lardataobj/AnalysisBase/MVAPIDResult.h"
#include "lardataobj/AnalysisBase/FlashMatch.h"
#include "lardataobj/AnalysisBase/CosmicTag.h"
#include "lardataobj/AnalysisBase/T0.h"
#include "lardataobj/AnalysisBase/BackTrackerMatchingData.h"

#include "lardataobj/RecoBase/OpFlash.h"
#include "lardataobj/RecoBase/PFParticle.h"
#include "lardataobj/RecoBase/Track.h"
#include "lardataobj/RecoBase/PCAxis.h"
#include "lardataobj/RecoBase/Hit.h"
#include "lardataobj/RecoBase/Shower.h"
#include "lardataobj/RecoBase/Cluster.h"

#include "lardataobj/RawData/ExternalTrigger.h"

#include "nusimdata/SimulationBase/MCParticle.h"

namespace {
//template<> class art::Assns<recob::Track,    simb::MCParticle,          std::pair<double, double> >;
  art::Wrapper< art::Assns< simb::MCParticle, recob::Hit,  anab::BackTrackerHitMatchingData > > dummy;
  art::Wrapper< art::Assns< simb::MCParticle, recob::Track,  anab::BackTrackerMatchingData > > dummy;
  art::Wrapper< art::Assns< simb::MCParticle, recob::Shower,  anab::BackTrackerMatchingData > > dummy1;
  art::Wrapper< art::Assns< simb::MCParticle, recob::PFParticle,  anab::BackTrackerMatchingData > > dummy2;
}
