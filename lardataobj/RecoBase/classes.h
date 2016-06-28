/**
 * @file classes.h
 * @brief Dictionary selection for RecoBase
 * 
 * Original author Rob Kutschke, modified by klg
 * 
 * @note The system is not able to deal with
 * `art::Wrapper<std::vector<std::string>>`.
 * The problem is somewhere inside ROOT's REFLEX mechanism
 * and Philippe Canal says that it is (as of March 2010) a known problem.
 * He also says that they do not have any plans to fix it soon.
 * We can always work around it by putting the string inside another object.
 */

#include "canvas/Persistency/Common/PtrVector.h" 
#include "canvas/Persistency/Common/Wrapper.h"
#include "canvas/Persistency/Common/Assns.h"

#include "larcoreobj/SimpleTypesAndConstants/geo_types.h"
#include "lardataobj/RawData/RawDigit.h"
#include "lardataobj/RecoBase/Cluster.h"
#include "lardataobj/RecoBase/Hit.h"
#include "lardataobj/RecoBase/Shower.h"
#include "lardataobj/RecoBase/Track.h"
#include "lardataobj/RecoBase/Seed.h"
#include "lardataobj/RecoBase/EndPoint2D.h"
#include "lardataobj/RecoBase/SpacePoint.h"
#include "lardataobj/RecoBase/Vertex.h"
#include "lardataobj/RecoBase/Event.h"
#include "lardataobj/RecoBase/OpHit.h"
#include "lardataobj/RecoBase/OpFlash.h"
#include "lardataobj/RecoBase/Wire.h"
#include "lardataobj/RecoBase/PFParticle.h"
#include "lardataobj/RecoBase/PCAxis.h"
#include "lardataobj/RecoBase/TrackHitMeta.h"
