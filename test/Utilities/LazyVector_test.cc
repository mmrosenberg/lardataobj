/**
 * @file    LazyVector_test.cc
 * @brief   Implementation tests for a `util::LazyVector` object.
 * @author  Gianluca Petrillo (petrillo@fnal.gov)
 * @date    July 18, 2018
 * @version 1.0
 *
 */


// LArSoft (larcore) libraries
#include "lardataobj/Utilities/LazyVector.h"

#define BOOST_TEST_MODULE ( LazyVector_test )
#include "cetlib/quiet_unit_test.hpp" // BOOST_AUTO_TEST_CASE()
#include <boost/test/test_tools.hpp> // BOOST_CHECK()

// C/C++ standard libraries
#include <stdexcept> // std::out_of_range


//------------------------------------------------------------------------------
void TestLazyVector_defaultConstructed() {

  using Vector_t = util::LazyVector<int>;
  Vector_t v;

  BOOST_CHECK(v.empty());
  BOOST_CHECK_EQUAL(v.size(), 0U);
  BOOST_CHECK(v.data_empty());
  BOOST_CHECK_EQUAL(v.data_size(), 0U);

  BOOST_CHECK_EQUAL(v.data_defvalue(), 0);

  BOOST_CHECK_THROW(v.at(v.size()), std::out_of_range);
  BOOST_CHECK_THROW(v.const_at(v.size()), std::out_of_range);
  BOOST_CHECK_THROW(v.at(v.size() + 1U), std::out_of_range);
  BOOST_CHECK_THROW(v.const_at(v.size() + 1U), std::out_of_range);

  //
  // need to resize before doing anything
  //
  v.resize(10);

  Vector_t::size_type iBegin = 0, iEnd = 0;

  //
  // assign a single value (at position 6): { ... [6]: -6 }
  //
  v[6]   = -6;
  iBegin =  6U;
  iEnd   =  7U;

  BOOST_CHECK(!v.empty());
  BOOST_CHECK_EQUAL(v.size(), 10U);
  BOOST_CHECK(!v.data_empty());
  BOOST_CHECK_EQUAL(v.data_size(), iEnd - iBegin);
  BOOST_CHECK_EQUAL(v.data_begin_index(), iBegin);
  BOOST_CHECK_EQUAL(v.data_end_index(), iEnd);

  BOOST_CHECK_EQUAL(v.at       (6), -6);
  BOOST_CHECK_EQUAL(v.const_at (6), -6);
  BOOST_CHECK_EQUAL(v.get      (6), -6);
  BOOST_CHECK_EQUAL(v.const_get(6), -6);
  BOOST_CHECK_EQUAL(v          [6], -6);
  BOOST_CHECK_EQUAL(v.data_address(6), &(v[6]));

  for (Vector_t::size_type i = 0; i < iBegin; ++i) {
    BOOST_CHECK_EQUAL(v.const_at(i), v.data_defvalue());
    BOOST_CHECK_EQUAL(v.const_get(i), v.data_defvalue());
    BOOST_CHECK_EQUAL(v.data_address(i), nullptr);
  }
  for (Vector_t::size_type i = iEnd; i < v.size(); ++i) {
    BOOST_CHECK_EQUAL(v.const_at(i), v.data_defvalue());
    BOOST_CHECK_EQUAL(v.const_get(i), v.data_defvalue());
    BOOST_CHECK_EQUAL(v.data_address(i), nullptr);
  }
  BOOST_CHECK_THROW(v.at(v.size()), std::out_of_range);
  BOOST_CHECK_THROW(v.const_at(v.size()), std::out_of_range);
  BOOST_CHECK_THROW(v.at(v.size() + 1U), std::out_of_range);
  BOOST_CHECK_THROW(v.const_at(v.size() + 1U), std::out_of_range);

  // meta-test: have we changed storage size with the non-const test?
  BOOST_CHECK_EQUAL(v.data_size(), iEnd - iBegin);

  //
  // assign another value (at position 8): { ... [6]: -6, [7]: <def>, [8]: -8 }
  //
  v[8]   = -8;
  iBegin =  6U;
  iEnd   =  9U;

  BOOST_CHECK(!v.empty());
  BOOST_CHECK_EQUAL(v.size(), 10U);
  BOOST_CHECK(!v.data_empty());
  BOOST_CHECK_EQUAL(v.data_size(), iEnd - iBegin);
  BOOST_CHECK_EQUAL(v.data_begin_index(), iBegin);
  BOOST_CHECK_EQUAL(v.data_end_index(), iEnd);

  BOOST_CHECK_EQUAL(v.at       (6U), -6);
  BOOST_CHECK_EQUAL(v.const_at (6U), -6);
  BOOST_CHECK_EQUAL(v.get      (6U), -6);
  BOOST_CHECK_EQUAL(v.const_get(6U), -6);
  BOOST_CHECK_EQUAL(v          [6U], -6);
  BOOST_CHECK_EQUAL(v.data_address(6U), &(v[6U]));

  BOOST_CHECK_EQUAL(v.at       (7U), v.data_defvalue());
  BOOST_CHECK_EQUAL(v.const_at (7U), v.data_defvalue());
  BOOST_CHECK_EQUAL(v.get      (7U), v.data_defvalue());
  BOOST_CHECK_EQUAL(v.const_get(7U), v.data_defvalue());
  BOOST_CHECK_EQUAL(v          [7U], v.data_defvalue());
  BOOST_CHECK_EQUAL(v.data_address(7U), &(v[7U]));

  BOOST_CHECK_EQUAL(v.at       (8U), -8);
  BOOST_CHECK_EQUAL(v.const_at (8U), -8);
  BOOST_CHECK_EQUAL(v.get      (8U), -8);
  BOOST_CHECK_EQUAL(v.const_get(8U), -8);
  BOOST_CHECK_EQUAL(v          [8U], -8);
  BOOST_CHECK_EQUAL(v.data_address(8U), &(v[8U]));

  for (Vector_t::size_type i = 0; i < iBegin; ++i) {
    BOOST_CHECK_EQUAL(v.const_at(i), v.data_defvalue());
    BOOST_CHECK_EQUAL(v.const_get(i), v.data_defvalue());
    BOOST_CHECK_EQUAL(v.data_address(i), nullptr);
  }
  for (Vector_t::size_type i = iEnd; i < v.size(); ++i) {
    BOOST_CHECK_EQUAL(v.const_at(i), v.data_defvalue());
    BOOST_CHECK_EQUAL(v.const_get(i), v.data_defvalue());
    BOOST_CHECK_EQUAL(v.data_address(i), nullptr);
  }
  BOOST_CHECK_THROW(v.at(v.size()), std::out_of_range);
  BOOST_CHECK_THROW(v.const_at(v.size()), std::out_of_range);
  BOOST_CHECK_THROW(v.at(v.size() + 1U), std::out_of_range);
  BOOST_CHECK_THROW(v.const_at(v.size() + 1U), std::out_of_range);

  // meta-test: have we changed storage size with the non-const test?
  BOOST_CHECK_EQUAL(v.data_size(), iEnd - iBegin);


  //
  // assign another value (at position 4):
  // { ... [4]: -4, [5]: <def>, [6]: -6, [7]: <def>, [8]: -8 }
  //
  v[4]   = -4;
  iBegin =  4U;
  iEnd   =  9U;

  BOOST_CHECK(!v.empty());
  BOOST_CHECK_EQUAL(v.size(), 10U);
  BOOST_CHECK(!v.data_empty());
  BOOST_CHECK_EQUAL(v.data_size(), iEnd - iBegin);
  BOOST_CHECK_EQUAL(v.data_begin_index(), iBegin);
  BOOST_CHECK_EQUAL(v.data_end_index(), iEnd);

  BOOST_CHECK_EQUAL(v.at       (4U), -4);
  BOOST_CHECK_EQUAL(v.const_at (4U), -4);
  BOOST_CHECK_EQUAL(v.get      (4U), -4);
  BOOST_CHECK_EQUAL(v.const_get(4U), -4);
  BOOST_CHECK_EQUAL(v          [4U], -4);
  BOOST_CHECK_EQUAL(v.data_address(4U), &(v[4U]));

  BOOST_CHECK_EQUAL(v.at       (5U), v.data_defvalue());
  BOOST_CHECK_EQUAL(v.const_at (5U), v.data_defvalue());
  BOOST_CHECK_EQUAL(v.get      (5U), v.data_defvalue());
  BOOST_CHECK_EQUAL(v.const_get(5U), v.data_defvalue());
  BOOST_CHECK_EQUAL(v          [5U], v.data_defvalue());
  BOOST_CHECK_EQUAL(v.data_address(5U), &(v[5U]));

  BOOST_CHECK_EQUAL(v.at       (6U), -6);
  BOOST_CHECK_EQUAL(v.const_at (6U), -6);
  BOOST_CHECK_EQUAL(v.get      (6U), -6);
  BOOST_CHECK_EQUAL(v.const_get(6U), -6);
  BOOST_CHECK_EQUAL(v          [6U], -6);
  BOOST_CHECK_EQUAL(v.data_address(6U), &(v[6U]));

  BOOST_CHECK_EQUAL(v.at       (7U), v.data_defvalue());
  BOOST_CHECK_EQUAL(v.const_at (7U), v.data_defvalue());
  BOOST_CHECK_EQUAL(v.get      (7U), v.data_defvalue());
  BOOST_CHECK_EQUAL(v.const_get(7U), v.data_defvalue());
  BOOST_CHECK_EQUAL(v          [7U], v.data_defvalue());
  BOOST_CHECK_EQUAL(v.data_address(7U), &(v[7U]));

  BOOST_CHECK_EQUAL(v.at       (8U), -8);
  BOOST_CHECK_EQUAL(v.const_at (8U), -8);
  BOOST_CHECK_EQUAL(v.get      (8U), -8);
  BOOST_CHECK_EQUAL(v.const_get(8U), -8);
  BOOST_CHECK_EQUAL(v          [8U], -8);
  BOOST_CHECK_EQUAL(v.data_address(8U), &(v[8U]));

  for (Vector_t::size_type i = 0; i < iBegin; ++i) {
    BOOST_CHECK_EQUAL(v.const_at(i), v.data_defvalue());
    BOOST_CHECK_EQUAL(v.const_get(i), v.data_defvalue());
    BOOST_CHECK_EQUAL(v.data_address(i), nullptr);
  }
  for (Vector_t::size_type i = iEnd; i < v.size(); ++i) {
    BOOST_CHECK_EQUAL(v.const_at(i), v.data_defvalue());
    BOOST_CHECK_EQUAL(v.const_get(i), v.data_defvalue());
    BOOST_CHECK_EQUAL(v.data_address(i), nullptr);
  }
  BOOST_CHECK_THROW(v.at(v.size()), std::out_of_range);
  BOOST_CHECK_THROW(v.const_at(v.size()), std::out_of_range);
  BOOST_CHECK_THROW(v.at(v.size() + 1U), std::out_of_range);
  BOOST_CHECK_THROW(v.const_at(v.size() + 1U), std::out_of_range);

  // meta-test: have we changed storage size with the non-const test?
  BOOST_CHECK_EQUAL(v.data_size(), iEnd - iBegin);


  //
  // cut off one element actually stored
  //
  v.resize(iEnd - 1U);
  iEnd -= 1U;

  BOOST_CHECK(!v.empty());
  BOOST_CHECK_EQUAL(v.size(), iEnd);
  BOOST_CHECK(!v.data_empty());
  BOOST_CHECK_EQUAL(v.data_size(), iEnd - iBegin);
  BOOST_CHECK_EQUAL(v.data_begin_index(), iBegin);
  BOOST_CHECK_EQUAL(v.data_end_index(), iEnd);

  BOOST_CHECK_EQUAL(v.at       (4U), -4);
  BOOST_CHECK_EQUAL(v.const_at (4U), -4);
  BOOST_CHECK_EQUAL(v.get      (4U), -4);
  BOOST_CHECK_EQUAL(v.const_get(4U), -4);
  BOOST_CHECK_EQUAL(v          [4U], -4);
  BOOST_CHECK_EQUAL(v.data_address(4U), &(v[4U]));

  BOOST_CHECK_EQUAL(v.at       (5U), v.data_defvalue());
  BOOST_CHECK_EQUAL(v.const_at (5U), v.data_defvalue());
  BOOST_CHECK_EQUAL(v.get      (5U), v.data_defvalue());
  BOOST_CHECK_EQUAL(v.const_get(5U), v.data_defvalue());
  BOOST_CHECK_EQUAL(v          [5U], v.data_defvalue());
  BOOST_CHECK_EQUAL(v.data_address(5U), &(v[5U]));

  BOOST_CHECK_EQUAL(v.at       (6U), -6);
  BOOST_CHECK_EQUAL(v.const_at (6U), -6);
  BOOST_CHECK_EQUAL(v.get      (6U), -6);
  BOOST_CHECK_EQUAL(v.const_get(6U), -6);
  BOOST_CHECK_EQUAL(v          [6U], -6);
  BOOST_CHECK_EQUAL(v.data_address(6U), &(v[6U]));

  BOOST_CHECK_EQUAL(v.at       (7U), v.data_defvalue());
  BOOST_CHECK_EQUAL(v.const_at (7U), v.data_defvalue());
  BOOST_CHECK_EQUAL(v.get      (7U), v.data_defvalue());
  BOOST_CHECK_EQUAL(v.const_get(7U), v.data_defvalue());
  BOOST_CHECK_EQUAL(v          [7U], v.data_defvalue());
  BOOST_CHECK_EQUAL(v.data_address(7U), &(v[7U]));

  for (Vector_t::size_type i = 0; i < iBegin; ++i) {
    BOOST_CHECK_EQUAL(v.const_at(i), v.data_defvalue());
    BOOST_CHECK_EQUAL(v.const_get(i), v.data_defvalue());
    BOOST_CHECK_EQUAL(v.data_address(i), nullptr);
  }
  for (Vector_t::size_type i = iEnd; i < v.size(); ++i) {
    BOOST_CHECK_EQUAL(v.const_at(i), v.data_defvalue());
    BOOST_CHECK_EQUAL(v.const_get(i), v.data_defvalue());
    BOOST_CHECK_EQUAL(v.data_address(i), nullptr);
  }
  BOOST_CHECK_THROW(v.at(v.size()), std::out_of_range);
  BOOST_CHECK_THROW(v.const_at(v.size()), std::out_of_range);
  BOOST_CHECK_THROW(v.at(v.size() + 1U), std::out_of_range);
  BOOST_CHECK_THROW(v.const_at(v.size() + 1U), std::out_of_range);

  // meta-test: have we changed storage size with the non-const test?
  BOOST_CHECK_EQUAL(v.data_size(), iEnd - iBegin);


  //
  // expand nominal size (content should be unchanged)
  //
  v.resize(v.size() + 10U);

  BOOST_CHECK(!v.empty());
  BOOST_CHECK_EQUAL(v.size(), iEnd + 10U);
  BOOST_CHECK(!v.data_empty());
  BOOST_CHECK_EQUAL(v.data_size(), iEnd - iBegin);
  BOOST_CHECK_EQUAL(v.data_begin_index(), iBegin);
  BOOST_CHECK_EQUAL(v.data_end_index(), iEnd);

  BOOST_CHECK_EQUAL(v.at       (4U), -4);
  BOOST_CHECK_EQUAL(v.const_at (4U), -4);
  BOOST_CHECK_EQUAL(v.get      (4U), -4);
  BOOST_CHECK_EQUAL(v.const_get(4U), -4);
  BOOST_CHECK_EQUAL(v          [4U], -4);
  BOOST_CHECK_EQUAL(v.data_address(4U), &(v[4U]));

  BOOST_CHECK_EQUAL(v.at       (5U), v.data_defvalue());
  BOOST_CHECK_EQUAL(v.const_at (5U), v.data_defvalue());
  BOOST_CHECK_EQUAL(v.get      (5U), v.data_defvalue());
  BOOST_CHECK_EQUAL(v.const_get(5U), v.data_defvalue());
  BOOST_CHECK_EQUAL(v          [5U], v.data_defvalue());
  BOOST_CHECK_EQUAL(v.data_address(5U), &(v[5U]));

  BOOST_CHECK_EQUAL(v.at       (6U), -6);
  BOOST_CHECK_EQUAL(v.const_at (6U), -6);
  BOOST_CHECK_EQUAL(v.get      (6U), -6);
  BOOST_CHECK_EQUAL(v.const_get(6U), -6);
  BOOST_CHECK_EQUAL(v          [6U], -6);
  BOOST_CHECK_EQUAL(v.data_address(6U), &(v[6U]));

  BOOST_CHECK_EQUAL(v.at       (7U), v.data_defvalue());
  BOOST_CHECK_EQUAL(v.const_at (7U), v.data_defvalue());
  BOOST_CHECK_EQUAL(v.get      (7U), v.data_defvalue());
  BOOST_CHECK_EQUAL(v.const_get(7U), v.data_defvalue());
  BOOST_CHECK_EQUAL(v          [7U], v.data_defvalue());
  BOOST_CHECK_EQUAL(v.data_address(7U), &(v[7U]));

  for (Vector_t::size_type i = 0; i < iBegin; ++i) {
    BOOST_CHECK_EQUAL(v.const_at(i), v.data_defvalue());
    BOOST_CHECK_EQUAL(v.const_get(i), v.data_defvalue());
    BOOST_CHECK_EQUAL(v.data_address(i), nullptr);
  }
  for (Vector_t::size_type i = iEnd; i < v.size(); ++i) {
    BOOST_CHECK_EQUAL(v.const_at(i), v.data_defvalue());
    BOOST_CHECK_EQUAL(v.const_get(i), v.data_defvalue());
    BOOST_CHECK_EQUAL(v.data_address(i), nullptr);
  }
  BOOST_CHECK_THROW(v.at(v.size()), std::out_of_range);
  BOOST_CHECK_THROW(v.const_at(v.size()), std::out_of_range);
  BOOST_CHECK_THROW(v.at(v.size() + 1U), std::out_of_range);
  BOOST_CHECK_THROW(v.const_at(v.size() + 1U), std::out_of_range);

  // meta-test: have we changed storage size with the non-const test?
  BOOST_CHECK_EQUAL(v.data_size(), iEnd - iBegin);


  //
  // cut all data
  //
  v.resize(iBegin);
  iEnd = iBegin;

  BOOST_CHECK(!v.empty());
  BOOST_CHECK_EQUAL(v.size(), iEnd);
  BOOST_CHECK(v.data_empty());
  BOOST_CHECK_EQUAL(v.data_size(), iEnd - iBegin);

  for (Vector_t::size_type i = 0; i < iBegin; ++i) {
    BOOST_CHECK_EQUAL(v.const_at(i), v.data_defvalue());
    BOOST_CHECK_EQUAL(v.const_get(i), v.data_defvalue());
    BOOST_CHECK_EQUAL(v.data_address(i), nullptr);
  }
  for (Vector_t::size_type i = iEnd; i < v.size(); ++i) {
    BOOST_CHECK_EQUAL(v.const_at(i), v.data_defvalue());
    BOOST_CHECK_EQUAL(v.const_get(i), v.data_defvalue());
    BOOST_CHECK_EQUAL(v.data_address(i), nullptr);
  }
  BOOST_CHECK_THROW(v.at(v.size()), std::out_of_range);
  BOOST_CHECK_THROW(v.const_at(v.size()), std::out_of_range);
  BOOST_CHECK_THROW(v.at(v.size() + 1U), std::out_of_range);
  BOOST_CHECK_THROW(v.const_at(v.size() + 1U), std::out_of_range);

  // meta-test: have we changed storage size with the non-const test?
  BOOST_CHECK_EQUAL(v.data_size(), iEnd - iBegin);


  //
  // erase everything
  //
  v.clear();

  BOOST_CHECK(v.empty());
  BOOST_CHECK_EQUAL(v.size(), 0U);
  BOOST_CHECK(v.data_empty());
  BOOST_CHECK_EQUAL(v.data_size(), 0U);

  BOOST_CHECK_THROW(v.at(v.size()), std::out_of_range);
  BOOST_CHECK_THROW(v.const_at(v.size()), std::out_of_range);
  BOOST_CHECK_THROW(v.at(v.size() + 1U), std::out_of_range);
  BOOST_CHECK_THROW(v.const_at(v.size() + 1U), std::out_of_range);


} // TestLazyVector_defaultConstructed()


//------------------------------------------------------------------------------
void TestLazyVector_sizeConstructed() {

  //
  // we verify that after construction the state of the object is the same as
  // for a default-constructed vector (except for the default value) which is
  // then resized to size 12:
  // * containing no data, all elements are assigned the default value
  //
  using Vector_t = util::LazyVector<int>;
  Vector_t v(12, 5); // 12 elements, default value is 5

  BOOST_CHECK(!v.empty());
  BOOST_CHECK_EQUAL(v.size(), 12U);
  BOOST_CHECK(v.data_empty());
  BOOST_CHECK_EQUAL(v.data_size(), 0U);

  BOOST_CHECK_EQUAL(v.data_defvalue(), 5);

  for (Vector_t::size_type i = 0; i < v.size(); ++i) {
    BOOST_CHECK_EQUAL(v.const_at(i), v.data_defvalue());
    BOOST_CHECK_EQUAL(v.const_get(i), v.data_defvalue());
  }

  BOOST_CHECK_THROW(v.at(v.size()), std::out_of_range);
  BOOST_CHECK_THROW(v.const_at(v.size()), std::out_of_range);
  BOOST_CHECK_THROW(v.at(v.size() + 1U), std::out_of_range);
  BOOST_CHECK_THROW(v.const_at(v.size() + 1U), std::out_of_range);

} // TestLazyVector_sizeConstructed()


//------------------------------------------------------------------------------
void TestLazyVector_documentation_class() {

  /*
   * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~{.cpp}
   * // start with a lazy vector of nominal size 6 elements and no actual data:
   *  util::LazyVector<double> v(6U);
   * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
   */

  util::LazyVector<double> v(6U);

  BOOST_CHECK_EQUAL(v.size(), 6U);
  BOOST_CHECK(v.data_empty());

  /*
   * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~{.cpp}
   * // add a value `-4.0` at the previous-to-last element `4`:
   * // the vector will look like: { ... [4] -4.0 ... }
   * // (nominal size: 6, 1 stored datum)
   * v[4] = -4.0;
   * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
   */
  v[4] = -4.0;

  BOOST_CHECK_EQUAL(v.size(), 6U);
  BOOST_CHECK_EQUAL(v.data_size(), 1U);
  BOOST_CHECK_EQUAL(v.at(4U), -4.0);

  /*
   * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~{.cpp}
   * // add a value `-2.0` at the third element:
   * // the vector will look like: { ... [2] -2.0, [3] def, [4] -4.0 ... }
   * // (nominal size still 6, 3 stored data, the default value "def" is 0.0)
   * v[2] = -2.0;
   * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
   */
  v[2] = -2.0;

  BOOST_CHECK_EQUAL(v.size(), 6U);
  BOOST_CHECK_EQUAL(v.data_size(), 3U);
  BOOST_CHECK_EQUAL(v.at(2U), -2.0);
  BOOST_CHECK_EQUAL(v.at(3U), v.data_defvalue());
  BOOST_CHECK_EQUAL(v.at(4U), -4.0);

  /* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~{.cpp}
   *
   * // we want to set element #6 to -6.0: we need to expand the vector first.
   * v.resize(7U); // barely enough for element #6
   * // the vector will look like: { ... [2] -2.0, [3] def, [4] -4.0 [5] def [6] -6.0 }
   * // (nominal size 7, 5 stored data, the default value "def" is 0.0)
   * v[6] = -6.0;
   * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
   */
  v.resize(7U); // barely enough for element #6
  v[6] = -6.0;

  BOOST_CHECK_EQUAL(v.size(), 7U);
  BOOST_CHECK_EQUAL(v.data_size(), 5U);
  BOOST_CHECK_EQUAL(v.at(2U), -2.0);
  BOOST_CHECK_EQUAL(v.at(3U), v.data_defvalue());
  BOOST_CHECK_EQUAL(v.at(4U), -4.0);
  BOOST_CHECK_EQUAL(v.at(5U), v.data_defvalue());
  BOOST_CHECK_EQUAL(v.at(6U), -6.0);


} // TestLazyVector_documentation_class()


//------------------------------------------------------------------------------
void TestLazyVector_documentation_constructor_size() {
  /*
   * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~{.cpp}
   * util::LazyVector<int> v(3, 5);
   * v[0] =  0;
   * v[2] = -2;
   *
   * std::cout << "Default element [1]: " << v.at(1) << std::endl;
   * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
   */
  util::LazyVector<int> v(3, 5);
  v[0] =  0;
  v[2] = -2;

  BOOST_CHECK_EQUAL(v.const_get(1U), 5);

} // TestLazyVector_documentation_constructor_size()

//------------------------------------------------------------------------------
//--- registration of tests

BOOST_AUTO_TEST_CASE(LazyVectorTestCase) {

  //
  // behaviour tests
  //
  TestLazyVector_defaultConstructed();
  TestLazyVector_sizeConstructed();

  //
  // documentation tests
  //
  TestLazyVector_documentation_class();
  TestLazyVector_documentation_constructor_size();

} // BOOST_AUTO_TEST_CASE(LazyVectorTestCase)
