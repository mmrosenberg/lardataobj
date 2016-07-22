/// $Id: SimChannel.cxx,v 1.3 2010/03/26 20:08:36 brebel Exp $
///
/// \file  Simulation/SimChannel.cxx
///
///
/// \author  seligman@nevis.columbia.edu
///
////////////////////////////////////////////////////////////////////////

#include <limits> // std::numeric_limits
#include <utility>
#include <stdexcept>

#include "larsimobj/Simulation/SimChannel.h"
#include "larsimobj/Simulation/sim.h"
#include "larcoreobj/SimpleTypesAndConstants/PhysicalConstants.h"

#include "messagefacility/MessageLogger/MessageLogger.h"

namespace sim{

  //-------------------------------------------------
  IDE::IDE()
    : trackID     (util::kBogusI)
    , numElectrons(util::kBogusD)
    , energy      (util::kBogusD)
    , x           (util::kBogusD)
    , y		  (util::kBogusD)
    , z		  (util::kBogusD)
  {}

  //-------------------------------------------------
  IDE::IDE(sim::IDE const& ide,int offset)
    : trackID     (ide.trackID+offset)
    , numElectrons(ide.numElectrons)
    , energy      (ide.energy)
    , x           (ide.x)
    , y           (ide.y)
    , z           (ide.z)
  {}

  // Default constructor
  //-------------------------------------------------
  SimChannel::SimChannel() 
    : fChannel(raw::InvalidChannelID)
  {}

  //-------------------------------------------------
  SimChannel::SimChannel(raw::ChannelID_t channel)
    : fChannel(channel)
  {}

  //-------------------------------------------------
  bool compare_tdc(std::pair<unsigned int, std::vector<sim::IDE> > const& a,
                   std::pair<unsigned int, std::vector<sim::IDE> > const& b)
  {
    return a.first < b.first;
  }
  
  //-------------------------------------------------
  void SimChannel::AddIonizationElectrons(int          trackID,
                                          unsigned int tdc,
                                          double       numberElectrons,
                                          double      *xyz,
                                          double       energy)
  {
    // look at the map to see if the current TDC already
    // exists, if not, add it, if so, just add a new track id to the 
    // vector
    
    // no electrons? no energy? no good!
    if ((numberElectrons < std::numeric_limits<double>::epsilon())
      || (energy <= std::numeric_limits<double>::epsilon()))
    {
      // will throw
      LOG_ERROR("SimChannel")
      << "AddIonizationElectrons() trying to add to TDC #"
      << tdc
      << " "
      << numberElectrons
      << " electrons with "
      << energy
      << " MeV of energy from track ID="
      << trackID;
      return;
    } // if no energy or no electrons
    
    auto itr = std::lower_bound(fTDCIDEs.begin(),
                                fTDCIDEs.end(),
                                std::make_pair(tdc, std::vector<sim::IDE>()),
                                compare_tdc);
    
    // check if this tdc value is in the vector, it is possible that
    // the lower bound is different from the given tdc, in which case
    // we need to add something for that tdc
    if(itr        == fTDCIDEs.end() ||
       itr->first != tdc){
      std::vector<sim::IDE> idelist;
      idelist.emplace_back(trackID,
                           numberElectrons,
                           xyz[0],
                           xyz[1],
                           xyz[2],
                           energy);
      fTDCIDEs.emplace(itr, tdc, std::move(idelist) );
    }
    else if(itr->first == tdc){
      
      // loop over the IDE vector for this tdc and add the electrons
      // to the entry with the same track id
      for(auto ide : itr->second){
        
        if( ide.trackID == trackID ){
          // make a weighted average for the location information
          double weight    = ide.numElectrons + numberElectrons;
          ide.x            = (ide.x * ide.numElectrons + xyz[0]*numberElectrons)/weight;
          ide.y            = (ide.y * ide.numElectrons + xyz[1]*numberElectrons)/weight;
          ide.z            = (ide.z * ide.numElectrons + xyz[2]*numberElectrons)/weight;
          ide.numElectrons = weight;
          ide.energy       = ide.energy + energy;
          
          // found the track id we wanted, so return;
          return;
        }
      }
      
      // if we never found the track id, then this is the first instance of
      // the track id for this tdc, so add ide to the vector
      std::vector<sim::IDE> ides;
      ides.emplace_back(trackID,
                        numberElectrons,
                        xyz[0],
                        xyz[1],
                        xyz[2],
                        energy);
      
      fTDCIDEs.push_back( std::make_pair(tdc, std::move(ides) ) );
    }
    
    return;
  }


  //-------------------------------------------------
  double SimChannel::Charge(unsigned int tdc) const
  {
    double charge = 0.;

    auto itr = std::lower_bound(fTDCIDEs.begin(),
                                fTDCIDEs.end(),
                                std::make_pair(tdc, std::vector<sim::IDE>()),
                                compare_tdc);
    
      // check to see if this tdc value is in the map
    if(itr        != fTDCIDEs.end() &&
       itr->first == tdc){
      
        // loop over the list for this tdc value and add up
        // the total number of electrons
      for(auto ide : itr->second){
        charge += ide.numElectrons;
      } // end loop over sim::IDE for this tdc
      
    } // end if this tdc is represented in the map
  
    return charge;
  }

    //-------------------------------------------------
  double SimChannel::Energy(unsigned int tdc) const
  {
    double energy = 0.;

    auto itr = std::lower_bound(fTDCIDEs.begin(),
                                fTDCIDEs.end(),
                                std::make_pair(tdc, std::vector<sim::IDE>()),
                                compare_tdc);
    
      // check to see if this tdc value is in the map
    if(itr        != fTDCIDEs.end() &&
       itr->first == tdc){
      
        // loop over the list for this tdc value and add up
        // the total number of electrons
      for(auto ide : itr->second ){
        energy += ide.energy;
      } // end loop over sim::IDE for this tdc
      
    } // end if this tdc is represented in the map
  
    return energy;
  }

  
  //-----------------------------------------------------------------------
  // the start and end tdc values are assumed to be inclusive
  std::vector<sim::IDE> SimChannel::TrackIDsAndEnergies(unsigned int startTDC,
                                                        unsigned int endTDC) const
  {
    // make a map of track ID values to sim::IDE objects
    std::map<int, sim::IDE> idToIDE;

    std::vector<sim::IDE> ides;

    if(startTDC > endTDC ){
      mf::LogWarning("SimChannel") << "requested tdc range is bogus: "
				   << startTDC << " " << endTDC
				   << " return empty vector";
      return ides;
    }

      //find the lower bound for this tdc and then iterate from there
    auto itr = std::lower_bound(fTDCIDEs.begin(),
                                fTDCIDEs.end(),
                                std::make_pair(startTDC, std::vector<sim::IDE>()),
                                compare_tdc);
    
    while(itr != fTDCIDEs.end()){
      
      // check the tdc value for the iterator, break the loop if we
      // are outside the range
      if(itr->first > endTDC) break;
      
      // grab the vector of IDEs for this tdc
      auto const& idelist = itr->second;
      // now loop over them and add their content to the map
      for(auto ide : idelist){
        
        if( idToIDE.find(ide.trackID) != idToIDE.end() ){
          double nel1   = idToIDE[ide.trackID].numElectrons;
          double nel2   = ide.numElectrons;
          double en1    = idToIDE[ide.trackID].energy;
          double en2	  = ide.energy;
          double energy = en1  + en2;
          double weight = nel1 + nel2;
          
            // make a weighted average for the location information
          idToIDE[ide.trackID].x            = (ide.x*nel2 + idToIDE[ide.trackID].x*nel1)/weight;
          idToIDE[ide.trackID].y            = (ide.y*nel2 + idToIDE[ide.trackID].y*nel1)/weight;
          idToIDE[ide.trackID].z            = (ide.z*nel2 + idToIDE[ide.trackID].z*nel1)/weight;
          idToIDE[ide.trackID].numElectrons = weight;
          idToIDE[ide.trackID].energy = energy;
        } // end if the track id for this one is found
        else{
          idToIDE[ide.trackID] = sim::IDE(ide);
        }
      } // end loop over vector
      
      itr++;
    } // end loop over tdc values
    
      // now fill the vector with the ides from the map
    for(auto itr : idToIDE){
      ides.push_back(itr.second);
    }
    

    return ides;
  }

  //-----------------------------------------------------------------------
  // the start and end tdc values are assumed to be inclusive
  std::vector<sim::TrackIDE>  SimChannel::TrackIDEs(unsigned int startTDC,
                                                    unsigned int endTDC) const
  {

    std::vector<sim::TrackIDE> trackIDEs;

    if(startTDC > endTDC ){
      mf::LogWarning("SimChannel::TrackIDEs") << "requested tdc range is bogus: "
					      << startTDC << " " << endTDC
					      << " return empty vector";
      return trackIDEs;
    }

    double totalE = 0.;
    const std::vector<sim::IDE> ides = TrackIDsAndEnergies(startTDC, endTDC);
    for (auto const& ide : ides)
      totalE += ide.energy;

    // protect against a divide by zero below
    if(totalE < 1.e-5) totalE = 1.;
    
    // loop over the entries in the map and fill the input vectors    
    for (auto const& ide : ides){      
      if(ide.trackID == sim::NoParticleId) continue;
      trackIDEs.emplace_back(ide.trackID, ide.energy/totalE, ide.energy);
    }


    return trackIDEs;
  }

  //-----------------------------------------------------------------------
  // Merge the collection of IDEs from one sim channel to another.
  // Requires an agreed upon offset for G4 trackID
  std::pair<int,int> SimChannel::MergeSimChannel(SimChannel const& channel,
                                                 int               offset)
  {
    if( this->Channel() != channel.Channel() )
      throw std::runtime_error("ERROR SimChannel Merge: Trying to merge different channels!");

    std::pair<int,int> range_trackID(std::numeric_limits<int>::max(),
				     std::numeric_limits<int>::min());
    
    for(auto const& itr : channel.TDCIDEMap()){
      
      auto tdc  = itr.first;
      auto ides = itr.second;
      
      // find the entry from this SimChannel corresponding to the tdc from the other
      auto itrthis = std::lower_bound(fTDCIDEs.begin(),
                                      fTDCIDEs.end(),
                                      std::make_pair(tdc, std::vector<sim::IDE>()),
                                      compare_tdc);
      
      std::vector<sim::IDE> & curIDEVec = ides;
      if(itrthis        == fTDCIDEs.end() ||
         itrthis->first != tdc){
        fTDCIDEs.emplace_back(tdc, std::vector<sim::IDE>());
        curIDEVec = fTDCIDEs.back().second;
      }
      else if(itrthis->first == tdc)
        curIDEVec = itrthis->second;
      
      for(auto const& ide : ides){
        curIDEVec.emplace_back(ide, offset);
        if( ide.trackID+offset < range_trackID.first  )
          range_trackID.first = ide.trackID+offset;
        if( ide.trackID+offset > range_trackID.second )
          range_trackID.second = ide.trackID+offset;
      }//end loop over IDEs
      
    }//end loop over TDCIDEMap
    

    return range_trackID;
    
  }
  

}
