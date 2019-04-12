// Author: Andy Furmanski (afurmans@fnal.gov)
// Date: August 2017
// Purpose: Store information from a reco-truth matching module (which is based on the BackTracker service)
// Stores the cleanliness and completeness of a match
// Cleanliness = charge in reco object from true object / total charge in reco object
// Completeness = charge in reco object from true object / total charge deposited by true object

#ifndef ANAB_BACKTRACKERMATCHINGDATA_H
#define ANAB_BACKTRACKERMATCHINGDATA_H

namespace anab{

  struct BackTrackerMatchingData{
    double cleanliness;
    double completeness;
  };

  //per hit/MCParticle assn...
  // added by Wes Ketchum (wketchum@fnal.gov) and Yun-Tse Tsai (yuntse@slac.stanford.edu)
  struct BackTrackerHitMatchingData{
    float       ideFraction; //fraction of energy in hit from this particle
    int         isMaxIDE;    //is this particle the max contributor to this hit?
    float       ideNFraction; // fraction of number of electrons on the wire in hit from this particle
    int         isMaxIDEN;    // is this particle the max contributor to this hit in terms of number of electrons?
    float       numElectrons; // Number of electrons collected at the readout wire
    float       energy;       // energy deposited by ionization by this track ID [MeV]
  };

}

#endif //BACKTRACKERMATCHINGDATA_FLASHMATCH
