////////////////////////////////////////////////////////////////////////////
// \version $Id: Edge.cxx,v 1.5 2010/02/15 20:32:46 usher Exp $
//
// \file  Edge.cxx
// \brief Definition of Principal Components Axis object for LArSoft
//
// \author usher@slac.stanford.edu
//
// \see   Edge.h
//
////////////////////////////////////////////////////////////////////////////

#include "lardataobj/RecoBase/Edge.h"

#include <iomanip>
#include <iostream>

namespace recob
{

//----------------------------------------------------------------------
Edge::Edge()
{
}

//----------------------------------------------------------------------
Edge::Edge(const double length, size_t firstPointID, size_t secondPointID, size_t id) :
  fLength(length),
  fFirstPointID(firstPointID),
  fSecondPointID(secondPointID),
  fID(id)
{
}
  
//----------------------------------------------------------------------
// ostream operator.
//
std::ostream& operator<< (std::ostream & o, const Edge & a)
{
    o << std::setiosflags(std::ios::fixed) << std::setprecision(2);
    o << " Edge ID " << a.fID << " has length " << std::setw(6) << a.fLength << " cm" << std::endl;
    o << "   - First Point ID: " << a.fFirstPointID << ", second point ID: " << a.fSecondPointID << std::endl;
    
    return o;
}

//----------------------------------------------------------------------------
// comparison operator
//
bool operator < (const Edge& a, const Edge& b)
{
  if(a.getID() != b. getID())
    return a.getID() < b.getID();

  return false; //They are equal
}

} // End of namespace

