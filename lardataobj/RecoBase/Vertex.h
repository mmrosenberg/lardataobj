////////////////////////////////////////////////////////////////////////////
//
// \brief Definition of vertex object for LArSoft
//
// \author brebel@fnal.gov
//
////////////////////////////////////////////////////////////////////////////

#ifndef RB_VERTEX_H
#define RB_VERTEX_H

#include <iosfwd>

#include "larcoreobj/SimpleTypesAndConstants/PhysicalConstants.h"
#include "lardataobj/RecoBase/TrackingTypes.h"

namespace recob {

  class Vertex  {

  public:

    using Point_t      = tracking::Point_t;
    using SMatrixSym33 = tracking::SMatrixSym33;
    using SMatrixSym22 = tracking::SMatrixSym22;
    using SVector3     = tracking::SVector3;
    using SVector2     = tracking::SVector2;

    /// Status of the vertex. Here the convention is that when adding new enum values
    /// all valid go after 'Valid', and all valid with covariance go after 'ValidWithCovariance'
    enum Status { Invalid, Valid, ValidWithCovariance };

    Vertex();  ///< Default constructor

    explicit  Vertex(double *xyz, int id=util::kBogusI);

    Vertex(const Point_t& pos, const SMatrixSym33& cov, double chi2, int ndof, int id=util::kBogusI)
      : pos_(pos), cov_(cov), chi2_(chi2), ndof_(ndof), status_(ValidWithCovariance), id_(id) {}

    const Point_t&      position()   const { return pos_; }
    const SMatrixSym33& covariance() const { return cov_; }
    //
    double chi2() const { return chi2_; }
    double ndof() const { return ndof_; }
    //
    bool   isValid()           const { return status_>=Valid; }
    bool   isValidCovariance() const { return status_>=ValidWithCovariance; }
    Status status()            const { return status_; }

    void      XYZ(double *xyz) const;
    int       ID()             const;

    void setID(int newID) { id_ = newID; } 

    friend bool          operator <   (const Vertex & a, const Vertex & b);
    friend std::ostream& operator <<  (std::ostream& o,  const Vertex & a);

  private:

    Point_t pos_;      ///< Vertex 3D position
    SMatrixSym33 cov_; ///< Vertex covariance matrix 3x3
    double chi2_;      ///< Vertex fit chi2
    int ndof_;         ///< Vertex fit degrees of freedom
    Status status_;    ///< Vertex status, as define in Vertex::Status enum
    int    id_;        ///< id number for vertex

  };
}

inline int recob::Vertex::ID() const { return id_; }

#endif // RB_VERTEX_H
