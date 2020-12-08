/**
 * @file    TrackTrajectory_test.cc
 * @brief   Simple test on a recob::TrackTrajectory object
 * @author  Gianluca Petrillo (petrillo@fnal.gov)
 * @date    January 3, 2016
 * @version 1.0
 *
 * This test simply creates recob::TrackTrajectory objects and verifies that the
 * values it can access are the right ones.
 *
 * See http://www.boost.org/libs/test for the Boost test library home page.
 *
 */


// Boost libraries
/*
 * Boost: define the name of the module;
 * and do that before the inclusion of Boost unit test headers
 * because it will change what they provide.
 * Among the those, there is a main() function and some wrapping catching
 * unhandled exceptions and considering them test failures, and probably more.
 * This also makes fairly complicate to receive parameters from the command line
 * (for example, a random seed).
 */
#define BOOST_TEST_MODULE ( tracktrajectory_test )
#include "cetlib/quiet_unit_test.hpp" // BOOST_AUTO_TEST_CASE()
#include <boost/test/test_tools.hpp> // BOOST_CHECK()
#include <boost/test/tools/floating_point_comparison.hpp> // BOOST_CHECK_CLOSE()

// LArSoft libraries
#include "lardataobj/RecoBase/TrackTrajectory.h"
#include "lardataobj/RecoBase/TrajectoryPointFlags.h"
#include "larcoreobj/SimpleTypesAndConstants/PhysicalConstants.h" // util::pi()

// ROOT libraries (for the legacy interface)
#include "TVector3.h"
#include "TMatrixD.h"

// C/C++ standard libraries
#include <set>
#include <iterator> // std::prev(), std::next()
#include <algorithm> // std::equal(), std::accumulate()
#include <tuple> // std::tie()
#include <iostream>


//------------------------------------------------------------------------------
//--- Test code
//
struct Expected_t {

  recob::TrackTrajectory::Positions_t positions;
  recob::TrackTrajectory::Momenta_t   momenta;
  recob::TrackTrajectory::Flags_t     flags;
  bool hasMomenta;
  double length;
  double theta;
  double phi;
  double zenith;
  double azimuth;

}; // struct Expected_t

template <typename T>
void CheckValue(T v, T exp, T tol, std::string tag = "") {
  if (!tag.empty()) BOOST_TEST_MESSAGE(tag);
  if (std::abs(exp) < (tol / 100.)) BOOST_CHECK_SMALL(v, tol);
  else                              BOOST_CHECK_CLOSE(v, exp, tol);
} // CheckValue()

template <typename VectA, typename VectB>
void CheckVectorsEqual(VectA const& v, VectB const& exp) {
  BOOST_CHECK_EQUAL(v.X(), exp.X());
  BOOST_CHECK_EQUAL(v.Y(), exp.Y());
  BOOST_CHECK_EQUAL(v.Z(), exp.Z());
} // CheckVectorsEqual()

template <typename VectA, typename VectB>
void CheckVectorsClose(VectA const& v, VectB const& exp, double tol = 0.01) {
  CheckValue(v.X(), exp.X(), tol, "  X()");
  CheckValue(v.Y(), exp.Y(), tol, "  Y()");
  CheckValue(v.Z(), exp.Z(), tol, "  Z()");
} // CheckVectorsClose()


recob::TrackTrajectory::Rotation_t makeRotationMatrix(TMatrixD const& Tm) {
  recob::TrackTrajectory::Rotation_t m;
  m.SetRotationMatrix(Tm);
  return m;
} // makeRotationMatrix()


void TestTrackTrajectory(
  recob::TrackTrajectory const& traj,
  Expected_t const& expected
  )
{

  //----------------------------------------------------------------------------
  const size_t NPoints = expected.positions.size();
  BOOST_CHECK_EQUAL(traj.NPoints(), NPoints);
  BOOST_CHECK_EQUAL(traj.NumberTrajectoryPoints(), NPoints);

  for (size_t i = 0; i <= NPoints + 1; ++i) {
    BOOST_TEST_MESSAGE("HasPoint() position #" << i);
    BOOST_CHECK_EQUAL(traj.HasPoint(i), (i < NPoints));
  } // for

  if (NPoints == 0) return; // nothing else is defined

  BOOST_CHECK_EQUAL(traj.HasMomentum(), expected.hasMomenta);

  //----------------------------------------------------------------------------
  // some preparation:
  std::set<size_t> validPoints;
  for (size_t i = 0; i < NPoints; ++i) {
    if (expected.flags[i].isSet(recob::TrackTrajectory::flag::NoPoint))
      continue;
    validPoints.insert(i);
  } // for
  std::size_t const firstValidPoint = validPoints.empty()
    ? recob::TrackTrajectory::InvalidIndex: *(validPoints.begin());
  std::size_t const lastValidPoint = validPoints.empty()
    ? recob::TrackTrajectory::InvalidIndex: *(validPoints.rbegin());

  //----------------------------------------------------------------------------
  BOOST_CHECK_EQUAL(traj.LastPoint(), NPoints - 1);

  //----------------------------------------------------------------------------
  for (size_t i = 0; i < NPoints; ++i) {

    bool const isValid = (validPoints.count(i) > 0);
    BOOST_TEST_MESSAGE("HasValidPoint() position #" << i);
    BOOST_CHECK_EQUAL(traj.HasValidPoint(i), isValid);

    if (isValid) {
      BOOST_CHECK_EQUAL(traj.NextValidPoint(i), i);
      BOOST_CHECK_EQUAL(traj.PreviousValidPoint(i), i);
    }
    else { // if not a valid point
      constexpr auto InvalidIndex = recob::TrackTrajectory::InvalidIndex;
      auto iNext = validPoints.upper_bound(i);
      if (iNext == validPoints.end()) { // after last valid point
        BOOST_CHECK_EQUAL(traj.PreviousValidPoint(i), lastValidPoint);
        BOOST_CHECK_EQUAL(traj.NextValidPoint(i), InvalidIndex);
      }
      else if (iNext == validPoints.begin()) { // before first valid point
        BOOST_CHECK_EQUAL(traj.PreviousValidPoint(i), InvalidIndex);
        BOOST_CHECK_EQUAL(traj.NextValidPoint(i), firstValidPoint);
      }
      else { // in the middle of the trajectory
        BOOST_CHECK_EQUAL(traj.PreviousValidPoint(i), *(std::prev(iNext)));
        BOOST_CHECK_EQUAL(traj.NextValidPoint(i), *iNext);
      }
    }

  } // for

  if (!validPoints.empty()) {
    BOOST_CHECK_EQUAL(traj.FirstValidPoint(), firstValidPoint);
    BOOST_CHECK_EQUAL(traj.LastValidPoint(), lastValidPoint);
  }

  BOOST_CHECK_EQUAL(traj.CountValidPoints(), validPoints.size());

  //----------------------------------------------------------------------------
  BOOST_TEST_MESSAGE("Vertex()");
  CheckVectorsEqual(traj.Vertex(), expected.positions[firstValidPoint]);

  BOOST_TEST_MESSAGE("Start()");
  CheckVectorsEqual(traj.Start(), expected.positions[firstValidPoint]);

  BOOST_TEST_MESSAGE("End()");
  CheckVectorsEqual(traj.End(), expected.positions[lastValidPoint]);

  //----------------------------------------------------------------------------
  for (size_t i = 0; i < NPoints; ++i) {

    BOOST_TEST_MESSAGE("LocationAtPoint() position #" << i);
    CheckVectorsEqual(traj.LocationAtPoint(i), expected.positions[i]);

  } // for


  //----------------------------------------------------------------------------
  TVector3 Vstart, Vend;
  std::tie(Vstart, Vend) = traj.Extent<TVector3>();
  BOOST_CHECK_EQUAL(Vstart[0], expected.positions[firstValidPoint].X());
  BOOST_CHECK_EQUAL(Vstart[1], expected.positions[firstValidPoint].Y());
  BOOST_CHECK_EQUAL(Vstart[2], expected.positions[firstValidPoint].Z());
  BOOST_CHECK_EQUAL(Vend[0], expected.positions[lastValidPoint].X());
  BOOST_CHECK_EQUAL(Vend[1], expected.positions[lastValidPoint].Y());
  BOOST_CHECK_EQUAL(Vend[2], expected.positions[lastValidPoint].Z());


  recob::Trajectory::Point_t start, end;
  std::tie(start, end) = traj.Extent(); // assign both start and end
  BOOST_TEST_MESSAGE("Extent() start");
  CheckVectorsEqual(start, expected.positions[firstValidPoint]);
  BOOST_TEST_MESSAGE("Extent() end");
  CheckVectorsEqual(end, expected.positions[lastValidPoint]);


  //----------------------------------------------------------------------------
  BOOST_CHECK_CLOSE(traj.Length(), expected.length, 0.01); // 0.01%
  if (validPoints.size() >= 2) {
    std::size_t const secondValidPoint = *(std::next(validPoints.begin()));
    BOOST_CHECK_CLOSE(traj.Length(firstValidPoint + 1),
      expected.length - (
         expected.positions[secondValidPoint]
         - expected.positions[firstValidPoint]
      ).R(),
      0.01
      );
  } // if


  //----------------------------------------------------------------------------
  BOOST_TEST_MESSAGE("VertexDirection()");
  CheckVectorsClose
    (traj.VertexDirection(), expected.momenta[firstValidPoint].Unit());
  BOOST_CHECK_CLOSE(traj.VertexDirection().Mag2(), 1.0, 0.01);

  BOOST_TEST_MESSAGE("StartDirection()");
  CheckVectorsClose
    (traj.StartDirection(), expected.momenta[firstValidPoint].Unit());
  BOOST_CHECK_CLOSE(traj.StartDirection().Mag2(), 1.0, 0.01);

  BOOST_TEST_MESSAGE("EndDirection()");
  CheckVectorsClose
    (traj.EndDirection(), expected.momenta[lastValidPoint].Unit());
  BOOST_CHECK_CLOSE(traj.EndDirection().Mag2(), 1.0, 0.01);


  //----------------------------------------------------------------------------
  BOOST_CHECK_CLOSE(traj.Theta(), expected.theta, 0.01);
  BOOST_CHECK_CLOSE(traj.Phi(), expected.phi, 0.01);
  BOOST_CHECK_CLOSE(traj.ZenithAngle(), expected.zenith, 0.01);
  BOOST_CHECK_CLOSE(traj.AzimuthAngle(), expected.azimuth, 0.01);


  //----------------------------------------------------------------------------

  BOOST_TEST_MESSAGE("VertexMomentumVector()");
  CheckVectorsClose
    (traj.VertexMomentumVector(), expected.momenta[firstValidPoint]);

  BOOST_TEST_MESSAGE("StartMomentumVector()");
  CheckVectorsClose
    (traj.StartMomentumVector(), expected.momenta[firstValidPoint]);

  BOOST_TEST_MESSAGE("EndMomentumVector()");
  CheckVectorsClose(traj.EndMomentumVector(), expected.momenta[lastValidPoint]);


  //----------------------------------------------------------------------------
  BOOST_TEST_MESSAGE("VertexMomentum()");
  BOOST_CHECK_CLOSE
    (traj.VertexMomentum(), expected.momenta[firstValidPoint].R(), 0.01);

  BOOST_TEST_MESSAGE("StartMomentum()");
  BOOST_CHECK_CLOSE
    (traj.StartMomentum(), expected.momenta[firstValidPoint].R(), 0.01);

  BOOST_TEST_MESSAGE("EndMomentum()");
  BOOST_CHECK_CLOSE
    (traj.EndMomentum(), expected.momenta[lastValidPoint].R(), 0.01);


  //----------------------------------------------------------------------------
  for (size_t i = 0; i < NPoints; ++i) {

    BOOST_TEST_MESSAGE("DirectionAtPoint() position #" << i);
    CheckVectorsClose(traj.DirectionAtPoint(i), expected.momenta[i].Unit());

  } // for


  //----------------------------------------------------------------------------
  for (size_t i = 0; i < NPoints; ++i) {

    if (validPoints.count(i) == 0) continue; // invalid points can be whatever

    BOOST_TEST_MESSAGE("MomentumVectorAtPoint() position #" << i);
    if (traj.HasMomentum())
      CheckVectorsClose(traj.MomentumVectorAtPoint(i), expected.momenta[i]);
    else {
      CheckVectorsClose
        (traj.MomentumVectorAtPoint(i), expected.momenta[i].Unit());
    }

    BOOST_TEST_MESSAGE("MomentumAtPoint() position #" << i);
    if (traj.HasMomentum()) {
      BOOST_CHECK_CLOSE
        (traj.MomentumAtPoint(i), expected.momenta[i].R(), 0.01);
    }
    else
      BOOST_CHECK_CLOSE(traj.MomentumAtPoint(i), 1.0, 0.01);

  } // for


  //----------------------------------------------------------------------------
  TVector3 AstartDir, AendDir;
  std::tie(AstartDir, AendDir) = traj.Direction<TVector3>();
  BOOST_CHECK_CLOSE
    (AstartDir[0], expected.momenta[firstValidPoint].Unit().X(), 0.01);
  BOOST_CHECK_CLOSE
    (AstartDir[1], expected.momenta[firstValidPoint].Unit().Y(), 0.01);
  BOOST_CHECK_CLOSE
    (AstartDir[2], expected.momenta[firstValidPoint].Unit().Z(), 0.01);
  BOOST_CHECK_CLOSE
    (AendDir[0], expected.momenta[lastValidPoint].Unit().X(), 0.01);
  BOOST_CHECK_CLOSE
    (AendDir[1], expected.momenta[lastValidPoint].Unit().Y(), 0.01);
  BOOST_CHECK_CLOSE
    (AendDir[2], expected.momenta[lastValidPoint].Unit().Z(), 0.01);

  recob::Trajectory::Vector_t startDir, endDir;
  std::tie(startDir, endDir) = traj.Direction();
  BOOST_TEST_MESSAGE("Direction() start");
  CheckVectorsClose(startDir, expected.momenta[firstValidPoint].Unit());
  BOOST_TEST_MESSAGE("Direction() end");
  CheckVectorsClose(endDir, expected.momenta[lastValidPoint].Unit());


  //----------------------------------------------------------------------------

  for (size_t i = 0; i < NPoints; ++i) {

    if (validPoints.count(i) == 0) continue; // invalid points can be whatever

    auto const& dir = expected.momenta[i];
    recob::Trajectory::Vector_t localDir(0., 0., dir.R());

    BOOST_TEST_MESSAGE("Test transformation to local at point #" << i);
    auto toLocal = traj.GlobalToLocalRotationAtPoint(i);
    CheckVectorsClose(toLocal * dir, localDir);

    BOOST_TEST_MESSAGE("Test transformation to global at point #" << i);
    auto toGlobal = traj.LocalToGlobalRotationAtPoint(i);
    CheckVectorsClose(toGlobal * localDir, dir);

  } // for

  //----------------------------------------------------------------------------
  for (size_t i = 0; i < NPoints; ++i) {

    if (validPoints.count(i) == 0) continue; // invalid points can be whatever

    auto const& dir = expected.momenta[i];
    recob::Trajectory::Vector_t localDir(0., 0., dir.R());

    BOOST_TEST_MESSAGE("Test legacy transformation to local at point #" << i);
    TMatrixD TtoLocal = traj.GlobalToLocalRotationAtPoint<TMatrixD>(i);
    auto toLocal = makeRotationMatrix(TtoLocal);
    CheckVectorsClose(toLocal * dir, localDir);

    BOOST_TEST_MESSAGE("Test legacy transformation to global at point #" << i);
    TMatrixD TtoGlobal = traj.LocalToGlobalRotationAtPoint<TMatrixD>(i);
    auto toGlobal = makeRotationMatrix(TtoGlobal);
    CheckVectorsClose(toGlobal * localDir, dir);

  } // for

  //----------------------------------------------------------------------------

} // TestTrackTrajectory()


//------------------------------------------------------------------------------
void TrackTrajectoryTestDefaultConstructor() {

  BOOST_TEST_MESSAGE("Testing the default recob::TrackTrajectory constructor");

  //
  // Part I: initialization of trajectory inputs
  //
  // these are the values expected for a default-constructed trajectory
  Expected_t expected;
  expected.positions.clear();
  expected.momenta.clear();
  expected.flags.clear();
  expected.hasMomenta = false;
  expected.length = 0.0;

  //
  // Part II: default constructor
  //
  // step II.1: create a trajectory with the default constructor
  recob::TrackTrajectory traj;

  for (unsigned int v = 0; v <= recob::TrackTrajectory::MaxDumpVerbosity; ++v) {
    std::cout
      << "Default-constructed track trajectory dump with verbosity level "
      << v << ":" << std::endl;
    traj.Dump(std::cout, v, "    ", "  ");
    std::cout << std::endl;
  } // for

  // step II.2: verify that the values are as expected
  TestTrackTrajectory(traj, expected);

} // TrackTrajectoryTestDefaultConstructor()


//------------------------------------------------------------------------------
void TrackTrajectoryTestMainConstructor() {

  BOOST_TEST_MESSAGE("Testing the main recob::TrackTrajectory constructor");

  //
  // Part I: initialization of trajectory inputs
  //
  const recob::TrackTrajectory::Coord_t V2_2 = std::sqrt(0.5);
  Expected_t expected;
  // we describe a trajectory with uniform electric and magnetic fields aligned
  // on z; curvature is 1 on the x/y plane.
  expected.positions = {
    recob::TrackTrajectory::Point_t(  -1.0,   0.0,  0.0 ),
    recob::TrackTrajectory::Point_t( -V2_2, +V2_2,  1.0 ),
    recob::TrackTrajectory::Point_t(   0.0,  +1.0,  2.0 ),
    recob::TrackTrajectory::Point_t( +V2_2, +V2_2,  3.0 ),
    recob::TrackTrajectory::Point_t(  +1.0,   0.0,  4.0 ),
    recob::TrackTrajectory::Point_t( +V2_2, -V2_2,  5.0 ),
    recob::TrackTrajectory::Point_t(   0.0,  -1.0,  6.0 ),
    recob::TrackTrajectory::Point_t( -V2_2, -V2_2,  7.0 ),
    recob::TrackTrajectory::Point_t(  -1.0,   0.0,  8.0 ),
    recob::TrackTrajectory::Point_t( -V2_2, +V2_2,  9.0 ),
    recob::TrackTrajectory::Point_t(   0.0,  +1.0, 10.0 ),
    recob::TrackTrajectory::Point_t( +V2_2, +V2_2, 11.0 ),
    recob::TrackTrajectory::Point_t(  +1.0,   0.0, 12.0 ),
    recob::TrackTrajectory::Point_t( +V2_2, -V2_2, 13.0 ),
    recob::TrackTrajectory::Point_t(   0.0,  -1.0, 14.0 ),
    recob::TrackTrajectory::Point_t( -V2_2, -V2_2, 15.0 ),
    recob::TrackTrajectory::Point_t(  -1.0,   0.0, 16.0 ),
    recob::TrackTrajectory::Point_t( -V2_2, +V2_2, 17.0 ),
    recob::TrackTrajectory::Point_t(   0.0,  +1.0, 18.0 ),
    recob::TrackTrajectory::Point_t( +V2_2, +V2_2, 19.0 ),
    recob::TrackTrajectory::Point_t(  +1.0,   0.0, 20.0 ),
    recob::TrackTrajectory::Point_t( +V2_2, -V2_2, 21.0 ),
    recob::TrackTrajectory::Point_t(   0.0,  -1.0, 22.0 ),
    recob::TrackTrajectory::Point_t( -V2_2, -V2_2, 23.0 ),
    recob::TrackTrajectory::Point_t(  -1.0,   0.0, 24.0 )
  };
  expected.momenta = {
    recob::TrackTrajectory::Vector_t(   0.0,  +1.0, 1.0 ),
    recob::TrackTrajectory::Vector_t( +V2_2, +V2_2, 1.0 ),
    recob::TrackTrajectory::Vector_t(  +1.0,   0.0, 1.0 ),
    recob::TrackTrajectory::Vector_t( +V2_2, -V2_2, 1.0 ),
    recob::TrackTrajectory::Vector_t(   0.0,  -1.0, 1.0 ),
    recob::TrackTrajectory::Vector_t( -V2_2, -V2_2, 1.0 ),
    recob::TrackTrajectory::Vector_t(  -1.0,   0.0, 1.0 ),
    recob::TrackTrajectory::Vector_t( -V2_2, +V2_2, 1.0 ),
    recob::TrackTrajectory::Vector_t(   0.0,  +1.0, 1.0 ),
    recob::TrackTrajectory::Vector_t( +V2_2, +V2_2, 1.0 ),
    recob::TrackTrajectory::Vector_t(  +1.0,   0.0, 1.0 ),
    recob::TrackTrajectory::Vector_t( +V2_2, -V2_2, 1.0 ),
    recob::TrackTrajectory::Vector_t(   0.0,  -1.0, 1.0 ),
    recob::TrackTrajectory::Vector_t( -V2_2, -V2_2, 1.0 ),
    recob::TrackTrajectory::Vector_t(  -1.0,   0.0, 1.0 ),
    recob::TrackTrajectory::Vector_t( -V2_2, +V2_2, 1.0 ),
    recob::TrackTrajectory::Vector_t(   0.0,  +1.0, 1.0 ),
    recob::TrackTrajectory::Vector_t( +V2_2, +V2_2, 1.0 ),
    recob::TrackTrajectory::Vector_t(  +1.0,   0.0, 1.0 ),
    recob::TrackTrajectory::Vector_t( +V2_2, -V2_2, 1.0 ),
    recob::TrackTrajectory::Vector_t(   0.0,  -1.0, 1.0 ),
    recob::TrackTrajectory::Vector_t( -V2_2, -V2_2, 1.0 ),
    recob::TrackTrajectory::Vector_t(  -1.0,   0.0, 1.0 ),
    recob::TrackTrajectory::Vector_t( -V2_2, +V2_2, 1.0 ),
    recob::TrackTrajectory::Vector_t(   0.0,  +1.0, 1.0 )
  };
  using trkflag = recob::TrackTrajectory::flag;
  expected.flags = {
    { recob::TrackTrajectory::PointFlags_t::InvalidHitIndex },
    { recob::TrackTrajectory::PointFlags_t::InvalidHitIndex, trkflag::NoPoint },
    { recob::TrackTrajectory::PointFlags_t::InvalidHitIndex },
    { recob::TrackTrajectory::PointFlags_t::InvalidHitIndex, trkflag::NoPoint },
    { recob::TrackTrajectory::PointFlags_t::InvalidHitIndex },
    { recob::TrackTrajectory::PointFlags_t::InvalidHitIndex, trkflag::NoPoint },
    { recob::TrackTrajectory::PointFlags_t::InvalidHitIndex },
    { recob::TrackTrajectory::PointFlags_t::InvalidHitIndex, trkflag::NoPoint },
    { recob::TrackTrajectory::PointFlags_t::InvalidHitIndex },
    { recob::TrackTrajectory::PointFlags_t::InvalidHitIndex, trkflag::NoPoint },
    { recob::TrackTrajectory::PointFlags_t::InvalidHitIndex },
    { recob::TrackTrajectory::PointFlags_t::InvalidHitIndex, trkflag::NoPoint },
    { recob::TrackTrajectory::PointFlags_t::InvalidHitIndex },
    { recob::TrackTrajectory::PointFlags_t::InvalidHitIndex, trkflag::NoPoint },
    { recob::TrackTrajectory::PointFlags_t::InvalidHitIndex },
    { recob::TrackTrajectory::PointFlags_t::InvalidHitIndex, trkflag::NoPoint },
    { recob::TrackTrajectory::PointFlags_t::InvalidHitIndex },
    { recob::TrackTrajectory::PointFlags_t::InvalidHitIndex, trkflag::NoPoint },
    { recob::TrackTrajectory::PointFlags_t::InvalidHitIndex },
    { recob::TrackTrajectory::PointFlags_t::InvalidHitIndex, trkflag::NoPoint },
    { recob::TrackTrajectory::PointFlags_t::InvalidHitIndex },
    { recob::TrackTrajectory::PointFlags_t::InvalidHitIndex, trkflag::NoPoint },
    { recob::TrackTrajectory::PointFlags_t::InvalidHitIndex },
    { recob::TrackTrajectory::PointFlags_t::InvalidHitIndex, trkflag::NoPoint },
    { recob::TrackTrajectory::PointFlags_t::InvalidHitIndex }
  };
  expected.hasMomenta = true;
  expected.length = 12. * std::sqrt(6);
  expected.theta = util::pi() / 4.0;
  expected.phi = util::pi() / 2.0;
  expected.zenith = 0.75 * util::pi();
  expected.azimuth = 0.0;

  //
  // Part II: complete constructor
  //
  // step II.1: create a track with momentum information
  auto positions = expected.positions;
  auto momenta = expected.momenta;
  auto flags = expected.flags;
  recob::TrackTrajectory traj
    (std::move(positions), std::move(momenta), std::move(flags), true);

  for (unsigned int v = 0; v <= recob::TrackTrajectory::MaxDumpVerbosity; ++v) {
    std::cout << "Track trajectory dump with verbosity level "
      << v << ":" << std::endl;
    traj.Dump(std::cout, v, "    ", "  ");
    std::cout << std::endl;
  } // for

  // step II.2: verify that the values are as expected
  TestTrackTrajectory(traj, expected);

  //
  // Part III: complete constructor, no momentum
  //

  // step III.1: amend the expectation for a momentumless track
  std::transform(expected.momenta.begin(), expected.momenta.end(),
    expected.momenta.begin(), [](auto const& v){ return v.unit(); });
  expected.hasMomenta = false;

  // step III.2: create a track with no momentum information
  positions = expected.positions; // copy again
  recob::TrackTrajectory::Momenta_t directions = expected.momenta;
  flags = expected.flags;
  recob::TrackTrajectory mltraj
    (std::move(positions), std::move(directions), std::move(flags), false);

  for (unsigned int v = 0; v <= recob::TrackTrajectory::MaxDumpVerbosity; ++v) {
    std::cout << "Momentumless trajectory dump with verbosity level "
      << v << ":" << std::endl;
    mltraj.Dump(std::cout, v, "    ", "  ");
    std::cout << std::endl;
  } // for

  // step III.3: verify that the values are as expected
  TestTrackTrajectory(mltraj, expected);

  //
  // Part IV: complete constructor, less valid points
  //

  // step IV.2: suppress the points
  expected.flags = {
    { recob::TrackTrajectory::PointFlags_t::InvalidHitIndex, trkflag::NoPoint },
    { recob::TrackTrajectory::PointFlags_t::InvalidHitIndex, trkflag::NoPoint },
    { recob::TrackTrajectory::PointFlags_t::InvalidHitIndex, trkflag::NoPoint },
    { recob::TrackTrajectory::PointFlags_t::InvalidHitIndex, trkflag::NoPoint },
    { recob::TrackTrajectory::PointFlags_t::InvalidHitIndex, trkflag::NoPoint },
    { recob::TrackTrajectory::PointFlags_t::InvalidHitIndex, trkflag::NoPoint },
    { recob::TrackTrajectory::PointFlags_t::InvalidHitIndex, trkflag::NoPoint },
    { recob::TrackTrajectory::PointFlags_t::InvalidHitIndex, trkflag::NoPoint },
    { recob::TrackTrajectory::PointFlags_t::InvalidHitIndex },
    { recob::TrackTrajectory::PointFlags_t::InvalidHitIndex, trkflag::NoPoint },
    { recob::TrackTrajectory::PointFlags_t::InvalidHitIndex, trkflag::NoPoint },
    { recob::TrackTrajectory::PointFlags_t::InvalidHitIndex, trkflag::NoPoint },
    { recob::TrackTrajectory::PointFlags_t::InvalidHitIndex },
    { recob::TrackTrajectory::PointFlags_t::InvalidHitIndex, trkflag::NoPoint },
    { recob::TrackTrajectory::PointFlags_t::InvalidHitIndex, trkflag::NoPoint },
    { recob::TrackTrajectory::PointFlags_t::InvalidHitIndex, trkflag::NoPoint },
    { recob::TrackTrajectory::PointFlags_t::InvalidHitIndex },
    { recob::TrackTrajectory::PointFlags_t::InvalidHitIndex, trkflag::NoPoint },
    { recob::TrackTrajectory::PointFlags_t::InvalidHitIndex, trkflag::NoPoint },
    { recob::TrackTrajectory::PointFlags_t::InvalidHitIndex, trkflag::NoPoint },
    { recob::TrackTrajectory::PointFlags_t::InvalidHitIndex, trkflag::NoPoint },
    { recob::TrackTrajectory::PointFlags_t::InvalidHitIndex, trkflag::NoPoint },
    { recob::TrackTrajectory::PointFlags_t::InvalidHitIndex, trkflag::NoPoint },
    { recob::TrackTrajectory::PointFlags_t::InvalidHitIndex, trkflag::NoPoint },
    { recob::TrackTrajectory::PointFlags_t::InvalidHitIndex, trkflag::NoPoint }
  };
  expected.hasMomenta = true;
  expected.length = 2. * std::sqrt(20.0);

  // step IV.2: create the track
  positions = expected.positions; // copy again
  momenta = expected.momenta;
  flags = expected.flags;
  recob::TrackTrajectory shorttraj
    (std::move(positions), std::move(momenta), std::move(flags), true);

  for (unsigned int v = 0; v <= recob::TrackTrajectory::MaxDumpVerbosity; ++v) {
    std::cout << "Short trajectory dump with verbosity level "
      << v << ":" << std::endl;
    shorttraj.Dump(std::cout, v, "    ", "  ");
    std::cout << std::endl;
  } // for

  // step IV.3: verify that the values are as expected
  TestTrackTrajectory(shorttraj, expected);

} // TrackTrajectoryTestMainConstructor()


//------------------------------------------------------------------------------
//--- registration of tests
//
// Boost needs now to know which tests we want to run.
// Tests are "automatically" registered, hence the BOOST_AUTO_TEST_CASE()
// macro name. The argument is the name of the test; each step may have a
// number of checks and it will fail if any of them does.
//

BOOST_AUTO_TEST_CASE(TrackTrajectoryTestDefaultConstructorTestCase) {
  TrackTrajectoryTestDefaultConstructor();
}

BOOST_AUTO_TEST_CASE(TrackTrajectoryTestMainConstructorTestCase) {
  TrackTrajectoryTestMainConstructor();
}

