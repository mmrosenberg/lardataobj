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
constexpr recob::TrajectoryPointFlagTraits::FlagIndex_t recob::TrajectoryPointFlagTraits::MaxFlags;
constexpr recob::TrajectoryPointFlagTraits::FlagIndex_t recob::TrajectoryPointFlagTraits::BeginTrajectoryFlags;
constexpr recob::TrajectoryPointFlagTraits::FlagIndex_t recob::TrajectoryPointFlagTraits::HitIgnored;
constexpr recob::TrajectoryPointFlagTraits::FlagIndex_t recob::TrajectoryPointFlagTraits::NoPoint;
constexpr recob::TrajectoryPointFlagTraits::FlagIndex_t recob::TrajectoryPointFlagTraits::Suspicious;
constexpr recob::TrajectoryPointFlagTraits::FlagIndex_t recob::TrajectoryPointFlagTraits::Merged;
constexpr recob::TrajectoryPointFlagTraits::FlagIndex_t recob::TrajectoryPointFlagTraits::DeltaRay;
constexpr recob::TrajectoryPointFlagTraits::FlagIndex_t recob::TrajectoryPointFlagTraits::DetectorIssue;
constexpr recob::TrajectoryPointFlagTraits::FlagIndex_t recob::TrajectoryPointFlagTraits::Shared;
constexpr recob::TrajectoryPointFlagTraits::FlagIndex_t recob::TrajectoryPointFlagTraits::TrajReserved1;
constexpr recob::TrajectoryPointFlagTraits::FlagIndex_t recob::TrajectoryPointFlagTraits::EndTrajectoryFlags;
constexpr recob::TrajectoryPointFlagTraits::FlagIndex_t recob::TrajectoryPointFlagTraits::BeginTrackFlags;
constexpr recob::TrajectoryPointFlagTraits::FlagIndex_t recob::TrajectoryPointFlagTraits::ExcludedFromFit;
constexpr recob::TrajectoryPointFlagTraits::FlagIndex_t recob::TrajectoryPointFlagTraits::Rejected;
constexpr recob::TrajectoryPointFlagTraits::FlagIndex_t recob::TrajectoryPointFlagTraits::Reinterpreted;
constexpr recob::TrajectoryPointFlagTraits::FlagIndex_t recob::TrajectoryPointFlagTraits::TrackReserved5;
constexpr recob::TrajectoryPointFlagTraits::FlagIndex_t recob::TrajectoryPointFlagTraits::TrackReserved4;
constexpr recob::TrajectoryPointFlagTraits::FlagIndex_t recob::TrajectoryPointFlagTraits::TrackReserved3;
constexpr recob::TrajectoryPointFlagTraits::FlagIndex_t recob::TrajectoryPointFlagTraits::TrackReserved2;
constexpr recob::TrajectoryPointFlagTraits::FlagIndex_t recob::TrajectoryPointFlagTraits::TrackReserved1;
constexpr recob::TrajectoryPointFlagTraits::FlagIndex_t recob::TrajectoryPointFlagTraits::EndTrackFlags;
constexpr recob::TrajectoryPointFlagTraits::FlagIndex_t recob::TrajectoryPointFlagTraits::BeginExperimentReservedFlags;
constexpr recob::TrajectoryPointFlagTraits::FlagIndex_t recob::TrajectoryPointFlagTraits::EndExperimentReservedFlags;
constexpr recob::TrajectoryPointFlagTraits::FlagIndex_t recob::TrajectoryPointFlagTraits::BeginUserReservedFlags;
constexpr recob::TrajectoryPointFlagTraits::FlagIndex_t recob::TrajectoryPointFlagTraits::EndUserReservedFlags;
const recob::TrajectoryPointFlagTraits::NameMap_t
recob::TrajectoryPointFlagTraits::names
  = recob::TrajectoryPointFlagTraits::initNames(); ///< Names of the flags


//------------------------------------------------------------------------------
std::string recob::TrajectoryPointFlagTraits::decorateFlagName
  (std::string baseName, FlagIndex_t flag)
{
  return baseName + std::to_string(flag);
} // recob::TrajectoryPointFlagTraits::decorateFlagName()


//------------------------------------------------------------------------------
std::string recob::TrajectoryPointFlagTraits::invalidFlagName
  (FlagIndex_t flag)
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
  flagNames[HitIgnored]    = "HitIgnored"   ;
  flagNames[NoPoint]       = "NoPoint"      ;
  flagNames[Suspicious]    = "Suspicious"   ;
  flagNames[Merged]        = "Merged"       ;
  flagNames[DeltaRay]      = "DeltaRay"     ;
  flagNames[DetectorIssue] = "DetectorIssue";
  flagNames[Shared]        = "Shared"       ;
  
  // track flags
  flagNames[ExcludedFromFit] = "ExcludedFromFit";
  flagNames[Rejected]        = "Rejected"       ;
  flagNames[Reinterpreted]   = "Reinterpreted"  ;
  
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
