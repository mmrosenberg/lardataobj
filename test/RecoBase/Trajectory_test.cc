/**
 * @file    Trajectory_test.cc
 * @brief   Simple test on a recob::Trajectory object
 * @author  Gianluca Petrillo (petrillo@fnal.gov)
 * @date    January 3, 2016
 * @version 1.0
 *
 * This test simply creates recob::Trajectory objects and verifies that the
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
#include "lardataobj/RecoBase/Trajectory.h"
#include "larcoreobj/SimpleTypesAndConstants/PhysicalConstants.h" // util::pi()

// ROOT libraries (for the legacy interface)
#include "TVector3.h"
#include "TMatrixD.h"

// C/C++ standard libraries
#include <algorithm> // std::equal(), std::accumulate()
#include <tuple> // std::tie()
#include <iostream>


//------------------------------------------------------------------------------
//--- Test code
//
struct Expected_t {

  recob::Trajectory::Positions_t positions;
  recob::Trajectory::Momenta_t   momenta;
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


recob::Trajectory::Rotation_t makeRotationMatrix(TMatrixD const& Tm) {
  recob::Trajectory::Rotation_t m;
  m.SetRotationMatrix(Tm);
  return m;
} // makeRotationMatrix()


void TestTrajectory(
  recob::Trajectory const& traj,
  Expected_t const& expected
  )
{

  //----------------------------------------------------------------------------
  const size_t NPoints = traj.NumberTrajectoryPoints();
  BOOST_CHECK_EQUAL(NPoints, expected.positions.size());
  BOOST_CHECK_EQUAL(traj.NumberTrajectoryPoints(), expected.positions.size());

  for (size_t i = 0; i <= NPoints + 1; ++i) {
    BOOST_TEST_MESSAGE("HasPoint() position #" << i);
    BOOST_CHECK_EQUAL(traj.HasPoint(i), (i < NPoints));
  } // for

  if (NPoints == 0) return; // nothing else is defined

  BOOST_CHECK_EQUAL(traj.HasMomentum(), expected.hasMomenta);

  //----------------------------------------------------------------------------
  BOOST_CHECK_EQUAL(traj.LastPoint(), NPoints - 1);

  //----------------------------------------------------------------------------
  BOOST_TEST_MESSAGE("Vertex()");
  CheckVectorsEqual(traj.Vertex(), expected.positions[0]);

  BOOST_TEST_MESSAGE("Start()");
  CheckVectorsEqual(traj.Start(), expected.positions[0]);

  BOOST_TEST_MESSAGE("End()");
  CheckVectorsEqual(traj.End(), expected.positions[NPoints - 1]);

  //----------------------------------------------------------------------------
  for (size_t i = 0; i < NPoints; ++i) {

    BOOST_TEST_MESSAGE("LocationAtPoint() position #" << i);
    CheckVectorsEqual(traj.LocationAtPoint(i), expected.positions[i]);

  } // for


  //----------------------------------------------------------------------------
  TVector3 Vstart, Vend;
  std::tie(Vstart, Vend) = traj.Extent<TVector3>();
  BOOST_CHECK_EQUAL(Vstart[0], expected.positions[0].X());
  BOOST_CHECK_EQUAL(Vstart[1], expected.positions[0].Y());
  BOOST_CHECK_EQUAL(Vstart[2], expected.positions[0].Z());
  BOOST_CHECK_EQUAL(Vend[0], expected.positions[NPoints - 1].X());
  BOOST_CHECK_EQUAL(Vend[1], expected.positions[NPoints - 1].Y());
  BOOST_CHECK_EQUAL(Vend[2], expected.positions[NPoints - 1].Z());


  recob::Trajectory::Point_t start, end;
  std::tie(start, end) = traj.Extent(); // assign both start and end
  BOOST_TEST_MESSAGE("Extent() start");
  CheckVectorsEqual(start, expected.positions[0]);
  BOOST_TEST_MESSAGE("Extent() end");
  CheckVectorsEqual(end, expected.positions[NPoints - 1]);


  //----------------------------------------------------------------------------
  BOOST_CHECK_CLOSE(traj.Length(), expected.length, 0.01); // 0.01%
  if (NPoints >= 2){
    BOOST_CHECK_CLOSE(traj.Length(1),
      expected.length - (expected.positions[1] - expected.positions[0]).R(),
      0.01
      );
  } // if


  //----------------------------------------------------------------------------
  BOOST_TEST_MESSAGE("VertexDirection()");
  CheckVectorsClose(traj.VertexDirection(), expected.momenta[0].Unit());
  BOOST_CHECK_CLOSE(traj.VertexDirection().Mag2(), 1.0, 0.01);

  BOOST_TEST_MESSAGE("StartDirection()");
  CheckVectorsClose(traj.StartDirection(), expected.momenta[0].Unit());
  BOOST_CHECK_CLOSE(traj.StartDirection().Mag2(), 1.0, 0.01);

  BOOST_TEST_MESSAGE("EndDirection()");
  CheckVectorsClose(traj.EndDirection(), expected.momenta[NPoints - 1].Unit());
  BOOST_CHECK_CLOSE(traj.EndDirection().Mag2(), 1.0, 0.01);


  //----------------------------------------------------------------------------
  BOOST_CHECK_CLOSE(traj.Theta(), expected.theta, 0.01);
  BOOST_CHECK_CLOSE(traj.Phi(), expected.phi, 0.01);
  BOOST_CHECK_CLOSE(traj.ZenithAngle(), expected.zenith, 0.01);
  BOOST_CHECK_CLOSE(traj.AzimuthAngle(), expected.azimuth, 0.01);


  //----------------------------------------------------------------------------

  BOOST_TEST_MESSAGE("VertexMomentumVector()");
  CheckVectorsClose(traj.VertexMomentumVector(), expected.momenta[0]);

  BOOST_TEST_MESSAGE("StartMomentumVector()");
  CheckVectorsClose(traj.StartMomentumVector(), expected.momenta[0]);

  BOOST_TEST_MESSAGE("EndMomentumVector()");
  CheckVectorsClose(traj.EndMomentumVector(), expected.momenta[NPoints - 1]);


  //----------------------------------------------------------------------------
  BOOST_TEST_MESSAGE("VertexMomentum()");
  BOOST_CHECK_CLOSE(traj.VertexMomentum(), expected.momenta[0].R(), 0.01);

  BOOST_TEST_MESSAGE("StartMomentum()");
  BOOST_CHECK_CLOSE(traj.StartMomentum(), expected.momenta[0].R(), 0.01);

  BOOST_TEST_MESSAGE("EndMomentum()");
  BOOST_CHECK_CLOSE
    (traj.EndMomentum(), expected.momenta[NPoints - 1].R(), 0.01);


  //----------------------------------------------------------------------------
  for (size_t i = 0; i < NPoints; ++i) {

    BOOST_TEST_MESSAGE("DirectionAtPoint() position #" << i);
    CheckVectorsClose(traj.DirectionAtPoint(i), expected.momenta[i].Unit());

  } // for


  //----------------------------------------------------------------------------
  for (size_t i = 0; i < NPoints; ++i) {

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
  BOOST_CHECK_CLOSE(AstartDir[0], expected.momenta[0].Unit().X(), 0.01);
  BOOST_CHECK_CLOSE(AstartDir[1], expected.momenta[0].Unit().Y(), 0.01);
  BOOST_CHECK_CLOSE(AstartDir[2], expected.momenta[0].Unit().Z(), 0.01);
  BOOST_CHECK_CLOSE(AendDir[0], expected.momenta[NPoints - 1].Unit().X(), 0.01);
  BOOST_CHECK_CLOSE(AendDir[1], expected.momenta[NPoints - 1].Unit().Y(), 0.01);
  BOOST_CHECK_CLOSE(AendDir[2], expected.momenta[NPoints - 1].Unit().Z(), 0.01);

  recob::Trajectory::Vector_t startDir, endDir;
  std::tie(startDir, endDir) = traj.Direction();
  BOOST_TEST_MESSAGE("Direction() start");
  CheckVectorsClose(startDir, expected.momenta[0].Unit());
  BOOST_TEST_MESSAGE("Direction() end");
  CheckVectorsClose(endDir, expected.momenta[NPoints - 1].Unit());


  //----------------------------------------------------------------------------

  for (size_t i = 0; i < NPoints; ++i) {

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

} // TestTrajectory()


//------------------------------------------------------------------------------
void TrajectoryTestDefaultConstructor() {

  BOOST_TEST_MESSAGE("Testing the default recob::Trajectory constructor");

  //
  // Part I: initialization of trajectory inputs
  //
  // these are the values expected for a default-constructed trajectory
  Expected_t expected;
  expected.positions.clear();
  expected.momenta.clear();
  expected.hasMomenta = false;
  expected.length = 0.0;

  //
  // Part II: default constructor
  //
  // step II.1: create a trajectory with the default constructor
  recob::Trajectory traj;

  for (unsigned int v = 0; v <= recob::Trajectory::MaxDumpVerbosity; ++v) {
    std::cout << "Default-constructed trajectory dump with verbosity level "
      << v << ":" << std::endl;
    traj.Dump(std::cout, v, "    ", "  ");
    std::cout << std::endl;
  } // for

  // step II.2: verify that the values are as expected
  TestTrajectory(traj, expected);

} // TrajectoryTestDefaultConstructor()


//------------------------------------------------------------------------------
void TrajectoryTestMainConstructor() {

  BOOST_TEST_MESSAGE("Testing the main recob::Trajectory constructor");

  //
  // Part I: initialization of trajectory inputs
  //
  const recob::Trajectory::Coord_t V2_2 = std::sqrt(0.5);
  Expected_t expected;
  // we describe a trajectory with uniform electric and magnetic fields aligned
  // on z; curvature is 1 on the x/y plane.
  expected.positions = {
    recob::Trajectory::Point_t(  -1.0,   0.0,  0.0 ),
    recob::Trajectory::Point_t( -V2_2, +V2_2,  1.0 ),
    recob::Trajectory::Point_t(   0.0,  +1.0,  2.0 ),
    recob::Trajectory::Point_t( +V2_2, +V2_2,  3.0 ),
    recob::Trajectory::Point_t(  +1.0,   0.0,  4.0 ),
    recob::Trajectory::Point_t( +V2_2, -V2_2,  5.0 ),
    recob::Trajectory::Point_t(   0.0,  -1.0,  6.0 ),
    recob::Trajectory::Point_t( -V2_2, -V2_2,  7.0 ),
    recob::Trajectory::Point_t(  -1.0,   0.0,  8.0 ),
    recob::Trajectory::Point_t( -V2_2, +V2_2,  9.0 ),
    recob::Trajectory::Point_t(   0.0,  +1.0, 10.0 ),
    recob::Trajectory::Point_t( +V2_2, +V2_2, 11.0 ),
    recob::Trajectory::Point_t(  +1.0,   0.0, 12.0 ),
    recob::Trajectory::Point_t( +V2_2, -V2_2, 13.0 ),
    recob::Trajectory::Point_t(   0.0,  -1.0, 14.0 ),
    recob::Trajectory::Point_t( -V2_2, -V2_2, 15.0 ),
    recob::Trajectory::Point_t(  -1.0,   0.0, 16.0 ),
    recob::Trajectory::Point_t( -V2_2, +V2_2, 17.0 ),
    recob::Trajectory::Point_t(   0.0,  +1.0, 18.0 ),
    recob::Trajectory::Point_t( +V2_2, +V2_2, 19.0 ),
    recob::Trajectory::Point_t(  +1.0,   0.0, 20.0 ),
    recob::Trajectory::Point_t( +V2_2, -V2_2, 21.0 ),
    recob::Trajectory::Point_t(   0.0,  -1.0, 22.0 ),
    recob::Trajectory::Point_t( -V2_2, -V2_2, 23.0 ),
    recob::Trajectory::Point_t(  -1.0,   0.0, 24.0 )
  };
  expected.momenta = {
    recob::Trajectory::Vector_t(   0.0,  +1.0, 1.0 ),
    recob::Trajectory::Vector_t( +V2_2, +V2_2, 1.0 ),
    recob::Trajectory::Vector_t(  +1.0,   0.0, 1.0 ),
    recob::Trajectory::Vector_t( +V2_2, -V2_2, 1.0 ),
    recob::Trajectory::Vector_t(   0.0,  -1.0, 1.0 ),
    recob::Trajectory::Vector_t( -V2_2, -V2_2, 1.0 ),
    recob::Trajectory::Vector_t(  -1.0,   0.0, 1.0 ),
    recob::Trajectory::Vector_t( -V2_2, +V2_2, 1.0 ),
    recob::Trajectory::Vector_t(   0.0,  +1.0, 1.0 ),
    recob::Trajectory::Vector_t( +V2_2, +V2_2, 1.0 ),
    recob::Trajectory::Vector_t(  +1.0,   0.0, 1.0 ),
    recob::Trajectory::Vector_t( +V2_2, -V2_2, 1.0 ),
    recob::Trajectory::Vector_t(   0.0,  -1.0, 1.0 ),
    recob::Trajectory::Vector_t( -V2_2, -V2_2, 1.0 ),
    recob::Trajectory::Vector_t(  -1.0,   0.0, 1.0 ),
    recob::Trajectory::Vector_t( -V2_2, +V2_2, 1.0 ),
    recob::Trajectory::Vector_t(   0.0,  +1.0, 1.0 ),
    recob::Trajectory::Vector_t( +V2_2, +V2_2, 1.0 ),
    recob::Trajectory::Vector_t(  +1.0,   0.0, 1.0 ),
    recob::Trajectory::Vector_t( +V2_2, -V2_2, 1.0 ),
    recob::Trajectory::Vector_t(   0.0,  -1.0, 1.0 ),
    recob::Trajectory::Vector_t( -V2_2, -V2_2, 1.0 ),
    recob::Trajectory::Vector_t(  -1.0,   0.0, 1.0 ),
    recob::Trajectory::Vector_t( -V2_2, +V2_2, 1.0 ),
    recob::Trajectory::Vector_t(   0.0,  +1.0, 1.0 )
  };
  expected.hasMomenta = true;
  expected.length
    = (expected.positions.size() - 1) * std::sqrt(3.0 - 2.0 * V2_2); //
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
  recob::Trajectory traj(std::move(positions), std::move(momenta), true);

  for (unsigned int v = 0; v <= recob::Trajectory::MaxDumpVerbosity; ++v) {
    std::cout << "Trajectory dump with verbosity level "
      << v << ":" << std::endl;
    traj.Dump(std::cout, v, "    ", "  ");
    std::cout << std::endl;
  } // for

  // step II.2: verify that the values are as expected
  TestTrajectory(traj, expected);

  //
  // Part III: complete constructor, no momentum
  //

  // step III.1: amend the expectation for a momentumless track
  std::transform(expected.momenta.begin(), expected.momenta.end(),
    expected.momenta.begin(), [](auto const& v){ return v.unit(); });
  expected.hasMomenta = false;

  // step III.2: create a track with no momentum information
  positions = expected.positions; // copy again
  recob::Trajectory::Momenta_t directions = expected.momenta;
  recob::Trajectory mltraj(std::move(positions), std::move(directions), false);

  for (unsigned int v = 0; v <= recob::Trajectory::MaxDumpVerbosity; ++v) {
    std::cout << "Momentumless trajectory dump with verbosity level "
      << v << ":" << std::endl;
    mltraj.Dump(std::cout, v, "    ", "  ");
    std::cout << std::endl;
  } // for

  // step III.3: verify that the values are as expected
  TestTrajectory(mltraj, expected);

} // TrajectoryTestMainConstructor()


//------------------------------------------------------------------------------
//--- registration of tests
//
// Boost needs now to know which tests we want to run.
// Tests are "automatically" registered, hence the BOOST_AUTO_TEST_CASE()
// macro name. The argument is the name of the test; each step may have a
// number of checks and it will fail if any of them does.
//

BOOST_AUTO_TEST_CASE(TrajectoryTestDefaultConstructorTestCase) {
  TrajectoryTestDefaultConstructor();
}

BOOST_AUTO_TEST_CASE(TrajectoryTestMainConstructorTestCase) {
  TrajectoryTestMainConstructor();
}

