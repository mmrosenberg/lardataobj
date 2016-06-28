//
// Build a dictionary.
//
// $Id: classes.h,v 1.8 2010/04/12 18:12:28  Exp $
// $Author:  $
// $Date: 2010/04/12 18:12:28 $
// 
// Original author Rob Kutschke, modified by klg
//
// Notes:
// 1) The system is not able to deal with
//    art::Wrapper<std::vector<std::string> >;
//    The problem is somewhere inside root's reflex mechanism
//    and Philippe Canal says that it is ( as of March 2010) a
//    known problem.  He also says that they do not have any
//    plans to fix it soon.  We can always work around it 
//    by putting the string inside another object.

#include "canvas/Persistency/Common/Wrapper.h"
#include "canvas/Persistency/Common/Assns.h"

// nutools includes
#include "nusimdata/SimulationBase/MCTruth.h"

// Simulation includes
#include "larsimobj/Simulation/SimChannel.h"
#include "larsimobj/Simulation/SimPhotons.h"
#include "larsimobj/Simulation/BeamGateInfo.h"
#include "larsimobj/Simulation/AuxDetSimChannel.h"
