////////////////////////////////////////////////////////////////////////////
//
// \brief Definition of event object for LArSoft
//
// \author brebel@fnal.gov
//
////////////////////////////////////////////////////////////////////////////

#ifndef RB_EVENT_H
#define RB_EVENT_H


#include <iosfwd>
#include <vector>


namespace recob {

  class Vertex; // Forward declaration.

  class Event  {

  public:
    Event(); // Needed to hide constexpr from GCCXML.

  private:

    int                  fID;       ///< id for this event


  public:
    explicit Event(int id);

    double               Energy()        const;
    double               SigmaEnergy()   const;
    const recob::Vertex* PrimaryVertex(std::vector<const recob::Vertex*>& vtxs) const;
    int                  ID()            const;

    friend std::ostream& operator << (std::ostream& o, const Event & a);
    friend bool          operator <  (const Event & a, const Event & b);


  };
}


inline int  recob::Event::ID() const { return fID; }


#endif // RB_EVENT_H
