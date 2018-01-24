// Build a dictionary.
//
// $Id: classes.h,v 1.8 2010/04/12 18:12:28  Exp $
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
  art::Wrapper< art::Assns< simb::MCParticle, recob::Hit,  anab::BackTrackerHitMatchingData > > dummyA;
  art::Wrapper< art::Assns< simb::MCParticle, recob::Track,  anab::BackTrackerMatchingData > > dummy1A;
  art::Wrapper< art::Assns< simb::MCParticle, recob::Shower,  anab::BackTrackerMatchingData > > dummy2A;
  art::Wrapper< art::Assns< simb::MCParticle, recob::PFParticle,  anab::BackTrackerMatchingData > > dummy3A;
  art::Wrapper< art::Assns< recob::Hit, simb::MCParticle, anab::BackTrackerHitMatchingData > > dummyB;
  art::Wrapper< art::Assns< recob::Track, simb::MCParticle, anab::BackTrackerMatchingData > > dummy1B;
  art::Wrapper< art::Assns< recob::Shower, simb::MCParticle, anab::BackTrackerMatchingData > > dummy2B;
  art::Wrapper< art::Assns< recob::PFParticle, simb::MCParticle, anab::BackTrackerMatchingData > > dummy3B;
}
