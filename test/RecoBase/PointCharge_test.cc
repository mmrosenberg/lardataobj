/**
 * @file    PointCharge_test.cc
 * @brief   Simple test on a recob::PointCharge object.
 * @author  Gianluca Petrillo (petrillo@fnal.gov)
 * @date    December 20, 2017
 * @version 1.0
 *
 * This test simply creates recob::PointCharge objects and verifies that the
 * values it can access are the right ones.
 *
 * See http://www.boost.org/libs/test for the Boost test library home page.
 */

// C/C++ standard library

// Boost libraries
#define BOOST_TEST_MODULE ( charge_test )
#include "cetlib/quiet_unit_test.hpp" // BOOST_AUTO_TEST_CASE()
#include <boost/test/test_tools.hpp> // BOOST_CHECK()
// #include <boost/test/tools/floating_point_comparison.hpp> // BOOST_CHECK_CLOSE()

// LArSoft libraries
#include "lardataobj/RecoBase/PointCharge.h"



//------------------------------------------------------------------------------
//--- Test code
//


void CheckCharge(
  recob::PointCharge const& obj,
  bool hasCharge,
  recob::PointCharge::Charge_t charge
) {

  // verify that the values are as expected
  if (hasCharge) {
    BOOST_CHECK(obj.hasCharge());
    BOOST_CHECK_EQUAL(obj.charge(), charge);
  }
  else {
    BOOST_CHECK(!obj.hasCharge());
    // charge() return value is undefined
  }

} // CheckCharge()


void ChargeTestDefaultConstructor() {

  //
  // Part I: initialization of inputs
  //
  // these are the values expected for a default-constructed wire
  bool const hasCharge = false;
  recob::PointCharge::Charge_t const charge = recob::PointCharge::InvalidCharge;

  //
  // Part II: default constructor
  //
  // step II.1: create a charge with the default constructor
  recob::PointCharge chargeInfo;

  // step II.2: verify that the values are as expected
  CheckCharge(chargeInfo, hasCharge, charge);

} // ChargeTestDefaultConstructor()


void ChargeTestValueConstructors() {
  //
  // Part I: initialization of inputs
  //
  // these are the values expected for a value-constructed wire
  bool const hasCharge = true;
  recob::PointCharge::Charge_t const charge = 10.0;

  //
  // Part II: default constructor
  //
  // step II.1: create a charge with the value constructor
  recob::PointCharge chargeInfo(charge);

  // step II.2: verify that the values are as expected
  CheckCharge(chargeInfo, hasCharge, charge);

} // ChargeTestValueConstructors()


//------------------------------------------------------------------------------
//--- registration of tests
//

BOOST_AUTO_TEST_CASE(ChargeTestDefaultConstructor_testcase) {
  ChargeTestDefaultConstructor();
} // ChargeTestDefaultConstructor_testcase

BOOST_AUTO_TEST_CASE(ChargeTestValueConstructor_testcase) {
  ChargeTestValueConstructors();
} // ChargeTestValueConstructor_testcase

