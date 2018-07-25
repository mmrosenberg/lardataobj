//
// Build a dictionary.
//
// $Author:  $
// $Date: 2010/04/12 18:12:28 $
//
// Original author Rob Kutschke, modified by klg and Steven Gardiner

#include "canvas/Persistency/Common/Wrapper.h"
#include "canvas/Persistency/Common/Assns.h"

// nutools includes
#include "nusimdata/SimulationBase/MCParticle.h"
#include "nusimdata/SimulationBase/MCTruth.h"

// Simulation includes
#include "lardataobj/Simulation/SimEnergyDeposit.h"
#include "lardataobj/Simulation/SimChannel.h"
#include "lardataobj/Simulation/OpDetBacktrackerRecord.h"
#include "lardataobj/Simulation/SimPhotons.h"
#include "lardataobj/Simulation/BeamGateInfo.h"
#include "lardataobj/Simulation/AuxDetSimChannel.h"
#include "lardataobj/Simulation/SupernovaTruth.h"
#include "lardataobj/Simulation/GeneratedParticleInfo.h"
// This line is from Jose Soto dated 2018-07-13
//#include "lardataobj/Simulation/SimDriftedElectronCluster.h"
// These two lines are from Hans Wenzel dated 2018-06-20
// Template the wrapper for the vector (typedef okay)
//template class art::Wrapper< sim::SimEnergyDepositCollection >;
// Try picking the include first
#include "lardataobj/Simulation/SimDriftedElectronCluster.h"

