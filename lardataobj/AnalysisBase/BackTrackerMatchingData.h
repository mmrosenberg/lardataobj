// Author: Andy Furmanski (afurmans@fnal.gov)
// Date: August 2017
// Purpose: Store information from a reco-truth matching module (which is based on the BackTracker service)
// Stores the cleanliness and completeness of a match
// Cleanliness = charge in reco object from true object / total charge in reco object
// Completeness = charge in reco object from true object / total charge deposited by true object
namespace anab{

  struct BackTrackerMatchingData{
    double cleanliness;
    double completeness;
  };

}
