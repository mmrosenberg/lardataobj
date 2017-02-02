/**
 * @file   TrajectoryPointFlags.cxx
 * @brief  Set of flags pertaining a point of the track (implementation file).
 * @author Giuseppe Cerati (cerati@fnal.gov),
 *         Gianluca Petrillo (petrillo@fnal.gov)
 * @date   January 18, 2017
 * @see    TrajectoryPointFlags.h
 *
 */


#include "TrajectoryPointFlags.h"


//------------------------------------------------------------------------------
//---  recob::TrajectoryPointFlagTraits
//---

// TODO: The following definitions are temporary, until we figure out what is
// requiring them.
constexpr recob::TrajectoryPointFlagTraits::Flag_t recob::TrajectoryPointFlagTraits::HitIgnored;
constexpr recob::TrajectoryPointFlagTraits::Flag_t recob::TrajectoryPointFlagTraits::NoPoint;
constexpr recob::TrajectoryPointFlagTraits::Flag_t recob::TrajectoryPointFlagTraits::Suspicious;
constexpr recob::TrajectoryPointFlagTraits::Flag_t recob::TrajectoryPointFlagTraits::Merged;
constexpr recob::TrajectoryPointFlagTraits::Flag_t recob::TrajectoryPointFlagTraits::DeltaRay;
constexpr recob::TrajectoryPointFlagTraits::Flag_t recob::TrajectoryPointFlagTraits::DetectorIssue;
constexpr recob::TrajectoryPointFlagTraits::Flag_t recob::TrajectoryPointFlagTraits::Shared;
constexpr recob::TrajectoryPointFlagTraits::Flag_t recob::TrajectoryPointFlagTraits::TrajReserved1;
constexpr recob::TrajectoryPointFlagTraits::Flag_t recob::TrajectoryPointFlagTraits::ExcludedFromFit;
constexpr recob::TrajectoryPointFlagTraits::Flag_t recob::TrajectoryPointFlagTraits::Rejected;
constexpr recob::TrajectoryPointFlagTraits::Flag_t recob::TrajectoryPointFlagTraits::Reinterpreted;



//------------------------------------------------------------------------------
const recob::TrajectoryPointFlagTraits::NameMap_t
recob::TrajectoryPointFlagTraits::names
  = recob::TrajectoryPointFlagTraits::initNames(); ///< Names of the flags


//------------------------------------------------------------------------------
std::string recob::TrajectoryPointFlagTraits::decorateFlagName
  (std::string baseName, Flag_t flag)
{
  return baseName + std::to_string(flag.index());
} // recob::TrajectoryPointFlagTraits::decorateFlagName()


//------------------------------------------------------------------------------
std::string recob::TrajectoryPointFlagTraits::invalidFlagName
  (Flag_t flag)
  { return "<" + decorateFlagName("InvalidFlag", flag) + ">"; }


//------------------------------------------------------------------------------
void recob::TrajectoryPointFlagTraits::initDefaultFlagRangeNames(
  NameMap_t& flagNames,
  FlagIndex_t BeginFlags, FlagIndex_t EndFlags, std::string baseName
) {
  if (EndFlags <= BeginFlags) return;
  unsigned int const NFlags = EndFlags - BeginFlags;
  
  // the first flag will have name with index NFlags, the last with name "1"
  for (unsigned int dFlag = 0; dFlag < NFlags; ++dFlag) {
    FlagIndex_t const flag = BeginFlags + dFlag;
    
    flagNames[flag] = decorateFlagName(baseName, NFlags - dFlag);
  } // for
  
} // recob::TrajectoryPointFlagTraits::initDefaultFlagRangeNames()


//------------------------------------------------------------------------------
void recob::TrajectoryPointFlagTraits::initDefaultFlagsNames
  (NameMap_t& flagNames)
{
  
  initDefaultFlagRangeNames(
    flagNames, BeginTrajectoryFlags, EndTrajectoryFlags,
    "TrajectoryReserved"
    );
  initDefaultFlagRangeNames(
    flagNames, BeginTrackFlags, EndTrackFlags,
    "TrackReserved"
    );
  initDefaultFlagRangeNames(
    flagNames, BeginExperimentReservedFlags, EndExperimentReservedFlags,
    "ExperimentFlag"
    );
  initDefaultFlagRangeNames(
    flagNames, BeginUserReservedFlags, EndUserReservedFlags,
    "UserFlag"
    );
  
} // recob::TrajectoryPointFlagTraits::initDefaultFlagsNames()


//------------------------------------------------------------------------------
void recob::TrajectoryPointFlagTraits::setFlagNames(NameMap_t& flagNames) {
  
  //
  // set the names of the new flags here
  //
  
  // trajectory flags
  std::get<HitIgnored.index()   >(flagNames) = "HitIgnored"   ;
  std::get<NoPoint.index()      >(flagNames) = "NoPoint"      ;
  std::get<Suspicious.index()   >(flagNames) = "Suspicious"   ;
  std::get<Merged.index()       >(flagNames) = "Merged"       ;
  std::get<DeltaRay.index()     >(flagNames) = "DeltaRay"     ;
  std::get<DetectorIssue.index()>(flagNames) = "DetectorIssue";
  std::get<Shared.index()       >(flagNames) = "Shared"       ;
  
  // track flags
  std::get<ExcludedFromFit.index()>(flagNames) = "ExcludedFromFit";
  std::get<Rejected.index()       >(flagNames) = "Rejected"       ;
  std::get<Reinterpreted.index()  >(flagNames) = "Reinterpreted"  ;
  
} // recob::TrajectoryPointFlagTraits::initNames()


//------------------------------------------------------------------------------
recob::TrajectoryPointFlagTraits::NameMap_t
recob::TrajectoryPointFlagTraits::initNames() {
  
  NameMap_t flagNames; // not initialized
  
  // initialize with default names
  initDefaultFlagsNames(flagNames);
  
  // overwrite custom names
  setFlagNames(flagNames);
  
  // done!
  return flagNames;
  
} // recob::TrajectoryPointFlagTraits::initNames()


//------------------------------------------------------------------------------
