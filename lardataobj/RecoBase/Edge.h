////////////////////////////////////////////////////////////////////////////
// \version $Id: Edge.h,v 1.0 2010/02/15 20:32:46 usher Exp $
//
// \file  Edge.h
// \brief An object to define a "edge" which is used to connect
//        space points in a triangulation algorithm
//
// \author usher@slac.stanford.edu
//
// \see   Edge.cxx
//
////////////////////////////////////////////////////////////////////////////

#ifndef Edge_H
#define Edge_H

#ifndef __GCCXML__
#include <iosfwd>
#endif

namespace recob
{
    //
    // @brief Edge is an object containting the results of a Principal Components
    //        Analysis of a group of space points.

    class Edge
    {
    public:
    
        Edge();
    
    private:
    
        double       fLength;           ///< Length of this Edge
        size_t       fFirstPointID;     ///< ID of the SpacePoint edge emanates from
        size_t       fSecondPointID;    ///< ID of the SpacePoint edge ends on
        size_t       fID;               ///< Edge ID
    
#ifndef __GCCXML__
    public:
    
        Edge(const double length, size_t firstPointID, size_t secondPointID, size_t id=0);
    
        double              getLength()        const;
        size_t              getFirstPointID()  const;
        size_t              getSecondPointID() const;
        size_t              getID()            const;
    
        friend std::ostream&  operator << (std::ostream & o, const Edge& a);
        friend bool operator < (const Edge& a, const Edge& b);
    
#endif
    };
    
}

#ifndef __GCCXML__

inline double  recob::Edge::getLength()        const {return fLength;}
inline size_t  recob::Edge::getFirstPointID()  const {return fFirstPointID;}
inline size_t  recob::Edge::getSecondPointID() const {return fSecondPointID;}
inline size_t  recob::Edge::getID()            const {return fID;}


#endif

#endif // Edge_H
