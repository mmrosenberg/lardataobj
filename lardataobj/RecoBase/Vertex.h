////////////////////////////////////////////////////////////////////////////
//
// \file  lardataobj/RecoBase/Vertex.h
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

  /**
   * @class recob::Vertex
   *
   * @brief Definition of vertex object for LArSoft
   *
   * Vertex object, containing 3D position, position covariance (also in 3D, but the matrix rank may be = 2),
   * vertex fit chi2 and number of degrees of freedom, plus a vertex status and a vertex id.
   * The vertex object may be associated (via art Assn) to the recob::Track objects used in the vertex fit, possibly with
   * a meta data, see recob::VertexAssnMeta.
   *
   * @author  Original author: brebel@fnal.gov, updated by G. Cerati (FNAL, MicroBooNE)
   * @date    2017
   * @version 2.0
   */

  class Vertex  {

  public:

    using Point_t      = tracking::Point_t;
    using SMatrixSym33 = tracking::SMatrixSym33;
    using SMatrixSym22 = tracking::SMatrixSym22;
    using SVector3     = tracking::SVector3;
    using SVector2     = tracking::SVector2;

    /// Status of the vertex. Here the convention is that when adding new enum values
    /// all invalid go before 'Invalid', all valid go after 'Valid', and all valid with covariance go after 'ValidWithCovariance'
    enum Status { Invalid, Valid, ValidWithCovariance };

    /// Default constructor, initializes status to Invalid, and data members to default or kBogus values.
    Vertex();

    /// Legacy constructor, preserved to avoid breaking code. Please try to use the new constructor.
    explicit Vertex(double *xyz, int id=util::kBogusI);

    /// Constructor initializing all data members.
    Vertex(const Point_t& pos, const SMatrixSym33& cov, double chi2, int ndof, int id=util::kBogusI)
      : pos_(pos), cov_(cov), chi2_(chi2), ndof_(ndof), status_(ValidWithCovariance), id_(id) {}

    /// Return vertex 3D position.
    const Point_t&      position()   const { return pos_; }
    /// Return vertex 3D covariance (be careful, the matrix may have rank=2).
    const SMatrixSym33& covariance() const { return cov_; }
    // Return vertex fit chi2.
    double chi2() const { return chi2_; }
    // Return vertex fit ndof.
    int ndof() const { return ndof_; }
    // Return vertex fit chi2 per ndof.
    double chi2PerNdof() const { return (ndof_>0. ? chi2_/ndof_ : util::kBogusD); }
    //
    // Return vertex status.
    Status status()            const { return status_; }
    bool   isValid()           const { return status_>=Valid; }
    bool   isValidCovariance() const { return status_>=ValidWithCovariance; }

    /// Legacy method to access vertex position, preserved to avoid breaking code. Please try to use Vertex::position().
    void      XYZ(double *xyz) const;

    /// Return vertex id.
    int       ID()             const;

    /// Set vertex id.
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
