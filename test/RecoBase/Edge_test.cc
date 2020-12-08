/**
 * @file    Edge_test.cc
 * @brief   Simple test on a recob::Edge object
 * @author  Gianluca Petrillo (petrillo@fnal.gov)
 * @date    July 6, 2017
 * @version 1.0
 *
 * This test simply creates recob::Edge objects and verifies that the values it
 * can access are the right ones.
 *
 * See http://www.boost.org/libs/test for the Boost test library home page.
 *
 * Timing:
 * version 1.0: ~1.5" (debug mode)
 */

// C/C++ standard library
#include <array>
#include <iostream>
#include <algorithm> // std::is_sorted(), std::lower_bound()
#include <stdexcept> // std::runtime_error

// Boost libraries
/*
 * Boost Magic: define the name of the module;
 * and do that before the inclusion of Boost unit test headers
 * because it will change what they provide.
 * Among the those, there is a main() function and some wrapping catching
 * unhandled exceptions and considering them test failures, and probably more.
 * This also makes fairly complicate to receive parameters from the command line
 * (for example, a random seed).
 */
#define BOOST_TEST_MODULE ( hit_test )
#include "cetlib/quiet_unit_test.hpp" // BOOST_AUTO_TEST_CASE()
#include <boost/test/test_tools.hpp> // BOOST_CHECK()
#include <boost/test/tools/floating_point_comparison.hpp> // BOOST_CHECK_CLOSE()

// LArSoft libraries
#include "lardataobj/RecoBase/Edge.h"
#include "lardataobj/RecoBase/SpacePoint.h"



//------------------------------------------------------------------------------
//--- Test code
//
void EdgeTestDefaultConstructor() {

  /*
   * Tested interface:
   *
   *    /// Default constructor (all invalid IDs).
   *    Edge() = default;
   *
   *    /// Returns the length of this edge [cm].
   *    double Length() const;
   *
   *    /// Returns the ID of the SpacePoint this edge emanates from.
   *    SpacePointID_t FirstPointID()  const;
   *
   *    /// Returns the ID of the SpacePoint this edge ends on.
   *    SpacePointID_t SecondPointID() const;
   *
   *    /// Returns the ID of this edge.
   *    ID_t ID() const;
   *
   *    /// Streaming operator: prints the edge `a` into the specified stream.
   *    std::ostream&  operator << (std::ostream & o, const Edge& a);
   *
   *    /// Comparison operator: strict ordering of edge by ID.
   *    inline bool operator< (const Edge& a, const Edge& b);
   *
   *    /// Comparison operator: strict ordering of edge by ID.
   *    inline bool operator< (const Edge& e, Edge::ID_t id);
   *
   *    /// Comparison operator: strict ordering of edge by ID.
   *    inline bool operator< (Edge::ID_t id, const Edge& e);
   *
   */

  recob::Edge const e;

  constexpr auto EinvID = recob::Edge::InvalidID;
  constexpr auto PinvID = recob::SpacePoint::InvalidID;

  BOOST_CHECK_EQUAL(e.ID(), EinvID);
  BOOST_CHECK_EQUAL(e.FirstPointID(), PinvID);
  BOOST_CHECK_EQUAL(e.SecondPointID(), PinvID);
  BOOST_CHECK_EQUAL(e.Length(), 0.0); // exactly so

  BOOST_CHECK(!(e      < e));
  BOOST_CHECK(!(e      < e.ID()));
  BOOST_CHECK(!(e.ID() < e));

  std::cout << "Printout of a default-constructed edge: " << e << std::endl;

} // EdgeTestDefaultConstructor()


void EdgeTestValueConstructor() {

  /*
   * Tested interface:
   *
   *    /// Constructor: assigns all values.
   *    Edge(
   *      const double length,
   *      SpacePointID_t firstPointID, SpacePointID_t secondPointID,
   *      ID_t id = InvalidID
   *      );
   *
   *    /// Returns the length of this edge [cm].
   *    double Length() const;
   *
   *    /// Returns the ID of the SpacePoint this edge emanates from.
   *    SpacePointID_t FirstPointID()  const;
   *
   *    /// Returns the ID of the SpacePoint this edge ends on.
   *    SpacePointID_t SecondPointID() const;
   *
   *    /// Returns the ID of this edge.
   *    ID_t ID() const;
   *
   *    /// Streaming operator: prints the edge `a` into the specified stream.
   *    std::ostream&  operator << (std::ostream & o, const Edge& a);
   *
   *    /// Comparison operator: strict ordering of edge by ID.
   *    inline bool operator< (const Edge& a, const Edge& b);
   *
   *    /// Comparison operator: strict ordering of edge by ID.
   *    inline bool operator< (const Edge& e, Edge::ID_t id);
   *
   *    /// Comparison operator: strict ordering of edge by ID.
   *    inline bool operator< (Edge::ID_t id, const Edge& e);
   *
   */

  recob::Edge const e(3.0, 5, 10, 3);

  BOOST_CHECK_EQUAL(e.ID(), 3);
  BOOST_CHECK_EQUAL(e.FirstPointID(), 5);
  BOOST_CHECK_EQUAL(e.SecondPointID(), 10);
  BOOST_CHECK_EQUAL(e.Length(), 3.0); // exactly so

  BOOST_CHECK(!(e      < e));
  BOOST_CHECK(!(e      < e.ID()));
  BOOST_CHECK(!(e.ID() < e));

  std::cout << "Printout of a value-constructed edge: " << e << std::endl;

  recob::Edge o(3.0, 5, 10, 4);

  BOOST_CHECK( (e      < o));
  BOOST_CHECK(!(o      < e));
  BOOST_CHECK( (e      < o.ID()));
  BOOST_CHECK(!(o.ID() < e));

} // EdgeTestValueConstructor()


void EdgeTestSpacePointConstructor() {

  /*
   * Tested interface:
   *
   *    /// Default constructor (all invalid IDs).
   *    Edge() = default;
   *
   *    /// Constructor: assigns all values.
   *    Edge(
   *      const double length,
   *      SpacePointID_t firstPointID, SpacePointID_t secondPointID,
   *      ID_t id = InvalidID
   *      );
   *
   *    /// Constructor: uses the specified spacepoints.
   *    Edge(
   *      SpacePoint const& firstPoint, SpacePoint const& secondPoint,
   *      ID_t id = InvalidID
   *      );
   *
   *    /// Returns the length of this edge [cm].
   *    double Length() const;
   *
   *    /// Returns the ID of the SpacePoint this edge emanates from.
   *    SpacePointID_t FirstPointID()  const;
   *
   *    /// Returns the ID of the SpacePoint this edge ends on.
   *    SpacePointID_t SecondPointID() const;
   *
   *    /// Returns the ID of this edge.
   *    ID_t ID() const;
   *
   *    /// Streaming operator: prints the edge `a` into the specified stream.
   *    std::ostream&  operator << (std::ostream & o, const Edge& a);
   *
   *    /// Comparison operator: strict ordering of edge by ID.
   *    inline bool operator< (const Edge& a, const Edge& b);
   *
   *    /// Comparison operator: strict ordering of edge by ID.
   *    inline bool operator< (const Edge& e, Edge::ID_t id);
   *
   *    /// Comparison operator: strict ordering of edge by ID.
   *    inline bool operator< (Edge::ID_t id, const Edge& e);
   *
   */

  // BUG the double brace syntax is required to work around clang bug 21629
  // (https://bugs.llvm.org/show_bug.cgi?id=21629)
  std::array<double, 3U> const error = {{ 0.1, 0.1, 0.1 }};
  std::array<double, 3U> point;

  // BUG the double brace syntax is required to work around clang bug 21629
  // (https://bugs.llvm.org/show_bug.cgi?id=21629)
  point = {{ 1.0, 1.0, 1.0 }};
  recob::SpacePoint const p1(point.data(), error.data(), 1.0, 0);

  // BUG the double brace syntax is required to work around clang bug 21629
  // (https://bugs.llvm.org/show_bug.cgi?id=21629)
  point = {{ 4.0, 5.0, 13.0 }};
  recob::SpacePoint const p2(point.data(), error.data(), 1.0, 1);

  recob::Edge const e(p1, p2, 0);

  BOOST_CHECK_EQUAL(e.ID(), 0);
  BOOST_CHECK_EQUAL(e.FirstPointID(), 0);
  BOOST_CHECK_EQUAL(e.SecondPointID(), 1);
  BOOST_CHECK_CLOSE(e.Length(), 13.0, 1e-4); // tolerance: 10^-6

  std::cout << "Printout of a spacepoint-constructed edge: " << e << std::endl;

} // EdgeTestSpacePointConstructor()


//------------------------------------------------------------------------------
void EdgeClassDocumentationTest() {

  // prepare the space points for the test, already sorted
  // BUG the double brace syntax is required to work around clang bug 21629
  // (https://bugs.llvm.org/show_bug.cgi?id=21629)
  std::array<double, 3U> const error = {{ 0.1, 0.1, 0.1 }};
  std::array<double, 3U> const point = {{ 1.0, 1.0, 1.0 }};
  std::vector<recob::SpacePoint> points;
  for (unsigned int i = 0; i < 10; ++i)
    points.emplace_back(point.data(), error.data(), 1.0, i);

  // prepare the edge with two of the space points
  recob::Edge const edge(points[3], points[6], 0);

  // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  if (!std::is_sorted(points.begin(), points.end()))
    throw std::runtime_error("Space points not sorted!");

  // find the first space point
  auto const iFirstPoint = std::lower_bound
    (points.begin(), points.end(), edge.FirstPointID());

  if ((iFirstPoint == points.end()) || (iFirstPoint->ID() != edge.FirstPointID())) {
    throw std::runtime_error
      ("First point not found: ID=" + std::to_string(edge.FirstPointID()));
  }
  recob::SpacePoint const& firstPoint = *iFirstPoint;

  // find the second space point
  auto const iSecondPoint = std::lower_bound
    (points.begin(), points.end(), edge.SecondPointID());

  if ((iSecondPoint == points.end()) || (iSecondPoint->ID() != edge.SecondPointID())) {
    throw std::runtime_error
      ("Second point not found: ID=" + std::to_string(edge.SecondPointID()));
  }
  recob::SpacePoint const& secondPoint = *iSecondPoint;
  // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

  BOOST_CHECK_EQUAL(&firstPoint, &points[3]);
  BOOST_CHECK_EQUAL(&secondPoint, &points[6]);


} // EdgeClassDocumentationTest()


//------------------------------------------------------------------------------
//--- registration of tests
//
// Boost needs now to know which tests we want to run.
// Tests are "automatically" registered, hence the BOOST_AUTO_TEST_CASE()
// macro name. The argument is the name of the test; each step may have a
// number of checks and it will fail if any of them does.
//

BOOST_AUTO_TEST_CASE(EdgeTests) {
  EdgeTestDefaultConstructor();
  EdgeTestValueConstructor();
  EdgeTestSpacePointConstructor();
} // EdgeTests

BOOST_AUTO_TEST_CASE(EdgeDocumentationTests) {
  EdgeClassDocumentationTest();
}
