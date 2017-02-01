/**
 * @file    TrajectoryPointFlags_test.cc
 * @brief   Simple test on a recob::TrajectoryPointFlags object
 * @author  Gianluca Petrillo (petrillo@fnal.gov)
 * @date    January 25, 2016
 * @version 1.0
 *
 * This test simply creates recob::TrajectoryPointFlags objects and verifies
 * that the values it can access are the right ones.
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
#define BOOST_TEST_MODULE ( trajectorypointflags_test )
#include "cetlib/quiet_unit_test.hpp" // BOOST_AUTO_TEST_CASE()
#include <boost/test/test_tools.hpp> // BOOST_CHECK()

// LArSoft libraries
#include "lardataobj/RecoBase/TrajectoryPointFlags.h"

// C/C++ standard library
#include <stdexcept> // std::out_of_range
#include <set>
#include <iostream> // std::cout


//------------------------------------------------------------------------------
//--- Test code
//
void TrajectoryPointFlagsTest_DefaultConstructor() {
  
  using trkflag = recob::TrajectoryPointFlags::flag;
  
  constexpr auto InvalidHitIndex = recob::TrajectoryPointFlags::InvalidHitIndex;
  
  /*
   * constexpr TrajectoryPointFlags()
   */
  constexpr recob::TrajectoryPointFlags flags;
  
  // we build our expectation based on the following mask:
  constexpr auto expectedMask = recob::TrajectoryPointFlags::DefaultFlagsMask();
  std::set<unsigned int> expectedDefined, expectedSet;
  for (unsigned int i = 0; i < expectedMask.capacity(); ++i) {
    if (expectedMask.isUndefined(i)) continue;
    expectedDefined.insert(i);
    if (expectedMask.isSet(i)) expectedSet.insert(i);
  } // for
  
  /*
   * void dump(...) const;
   */
  for (unsigned int level = 0; level < 2; ++level) {
    std::cout << "Default-constructed flags, dump(verbosity=" << level << "):"
      << std::endl;
    flags.dump(std::cout, level, "  ");
    std::cout << "\n" << std::endl;
  } // for
  
  /*
   * FlagIndex_t nFlags() const
   */
  BOOST_CHECK_EQUAL(flags.nFlags(), trkflag::maxFlags());
  for(
    recob::TrajectoryPointFlags::FlagIndex_t i = 0;
    i <= trkflag::maxFlags();
    ++i)
  {
    bool const allocated = (i < trkflag::maxFlags());
    bool const defined = (expectedDefined.count(i) > 0);
    bool const set = (expectedSet.count(i) > 0);
    
    BOOST_TEST_MESSAGE("Testing flag #" << i);
    
    /*
     * constexpr bool isAllocated(FlagIndex_t flag) const
     */
    BOOST_CHECK_EQUAL(flags.isAllocated(i), allocated);
    
    // for unallocated flag indices, most of the flag interface has
    // undefined behaviour
    if (!allocated) {
      /*
       * bool test(FlagIndex_t flag) const
       */
      BOOST_CHECK_THROW
        (flags.test(i), recob::TrajectoryPointFlags::Flags_t::OutOfRangeError);
      
      continue;
    } // if not allocated
    
    /*
     * constexpr bool isFlag(FlagIndex_t flag) const
     */
    BOOST_CHECK_EQUAL(flags.isFlag(i), allocated);
    
    /*
     * constexpr bool isDefined(FlagIndex_t flag) const
     */
    BOOST_CHECK_EQUAL(flags.isDefined(i), defined);
    
    /*
     * bool test(FlagIndex_t flag) const
     */
    if (!defined) {
      BOOST_CHECK_THROW(
        flags.test(i), recob::TrajectoryPointFlags::Flags_t::FlagNotDefinedError
        );
      continue;
    }
    BOOST_CHECK_EQUAL(flags.test(i), set);
    
    /*
     * get(FlagIndex_t flag) const
     */
    BOOST_CHECK_EQUAL(flags.get(i), set);
    
    /*
     * bool isSet(FlagIndex_t flag) const
     */
    BOOST_CHECK_EQUAL(flags.isSet(i), set);
    
    /*
     * bool isUnset(FlagIndex_t flag) const
     */
    BOOST_CHECK_EQUAL(!flags.isUnset(i), !set);
    
  } // for
  
  /*
   * bool isHitIgnored() const
   */
  BOOST_CHECK(flags.isHitIgnored());
  
  /*
   * bool isPointValid() const
   */
  BOOST_CHECK(flags.isPointValid());
  
  /*
   * Mask_t mask() const
   */
  BOOST_CHECK_EQUAL
    (flags.mask(), recob::TrajectoryPointFlags::DefaultFlagsMask());
  
  // assuming the default to be: trkflag::NoPoint
  /*
   * bool none(Mask_t bits) const
   */
  BOOST_CHECK(!flags.none(recob::TrajectoryPointFlags::makeMask
    (trkflag::NoPoint, trkflag::HitIgnored)
    ));
  
  /*
   * bool none(Flags...) const
   */
  BOOST_CHECK(!flags.none(trkflag::NoPoint, trkflag::HitIgnored));
  
  /*
   * bool any(Mask_t bits) const
   */
  BOOST_CHECK(flags.any(recob::TrajectoryPointFlags::makeMask
    (trkflag::NoPoint, trkflag::HitIgnored)
    ));
  
  /*
   * bool any(Flags...) const
   */
  BOOST_CHECK(flags.any(trkflag::NoPoint, trkflag::HitIgnored));
  
  /*
   * bool all(Mask_t bits) const
   */
  BOOST_CHECK(!flags.all(recob::TrajectoryPointFlags::makeMask
    (trkflag::NoPoint, trkflag::HitIgnored)
    ));
  
  /*
   * bool all(Flags...) const
   */
  BOOST_CHECK(!flags.all(trkflag::NoPoint, trkflag::HitIgnored));
  
  /*
   * hasOriginalHitIndex() const
   */
  BOOST_CHECK(!flags.hasOriginalHitIndex());
  
  /*
   * HitIndex_t fromHit() const
   */
  BOOST_CHECK_EQUAL(flags.fromHit(), InvalidHitIndex);
  
} // TrajectoryPointFlagsTest_DefaultConstructor()


//------------------------------------------------------------------------------
void TrajectoryPointFlagsTest_FlagsConstructor() {
  
  using trkflag = recob::TrajectoryPointFlags::flag;
  constexpr auto InvalidHitIndex = recob::TrajectoryPointFlags::InvalidHitIndex;
  
  std::set<recob::TrajectoryPointFlags::Flag_t> const expectedBits
    = { trkflag::NoPoint, trkflag::HitIgnored };
  
  constexpr auto flagbitmask = recob::TrajectoryPointFlags::makeMask
    (trkflag::NoPoint, trkflag::HitIgnored);
  
  /*
   * constexpr TrajectoryPointFlags(HitIndex_t fromHit, Flags... flags)
   */
  constexpr recob::TrajectoryPointFlags flags
    (InvalidHitIndex, trkflag::NoPoint, trkflag::HitIgnored);
  
  /*
   * void dump(...) const;
   */
  for (unsigned int level = 0; level < 2; ++level) {
    std::cout << "Flag-constructed flags, dump(verbosity=" << level << "):"
      << std::endl;
    flags.dump(std::cout, level, "  ");
    std::cout << "\n" << std::endl;
  } // for
  
  /*
   * FlagIndex_t nFlags() const
   */
  BOOST_CHECK_EQUAL(flags.nFlags(), trkflag::maxFlags());
  for(
    recob::TrajectoryPointFlags::FlagIndex_t i = 0;
    i <= trkflag::maxFlags();
    ++i)
  {
    bool const allocated = (i < trkflag::maxFlags());
    
    BOOST_TEST_MESSAGE("Testing flag #" << i);
    
    /*
     * constexpr bool isAllocated(FlagIndex_t flag) const
     */
    BOOST_CHECK_EQUAL(flags.isAllocated(i), allocated);
    
    // for unallocated flag indices, most of the flag interface has
    // undefined behaviour
    if (!allocated) {
      /*
       * bool test(FlagIndex_t flag) const
       */
      BOOST_CHECK_THROW(flags.test(i), std::out_of_range);
      
      continue;
    } // if not allocated
    
    bool const expected = (expectedBits.count(i) > 0);
    
    /*
     * constexpr bool isFlag(FlagIndex_t flag) const
     */
    BOOST_CHECK_EQUAL(flags.isFlag(i), allocated);
    
    /*
     * bool test(FlagIndex_t flag) const
     */
    BOOST_CHECK_EQUAL(flags.test(i), expected);
    
    /*
     * bool get(FlagIndex_t flag) const
     */
    BOOST_CHECK_EQUAL(flags.get(i), expected);
    
    /*
     * bool isSet(FlagIndex_t flag) const
     */
    BOOST_CHECK_EQUAL(flags.isSet(i), expected);
    
    /*
     * bool isUnset(FlagIndex_t flag) const
     */
    BOOST_CHECK_EQUAL(flags.isUnset(i), !expected);
    
    /*
     * bool isHitIgnored() const
     */
    BOOST_CHECK_EQUAL
      (flags.isHitIgnored(), (expectedBits.count(trkflag::HitIgnored) > 0));
    
    /*
     * bool isPointValid() const
     */
    BOOST_CHECK_EQUAL
      (flags.isPointValid(), (expectedBits.count(trkflag::NoPoint) == 0));
    
  } // for
    
  /*
   * Mask_t mask() const
   */
  BOOST_CHECK_EQUAL(flags.mask(), flagbitmask);
  
  /*
   * bool none(Mask_t bits) const
   */
  BOOST_CHECK(!flags.none(recob::TrajectoryPointFlags::makeMask
    (trkflag::NoPoint, trkflag::HitIgnored)
    ));
  
  /*
   * bool none(Flags...) const
   */
  BOOST_CHECK(!flags.none(trkflag::NoPoint, trkflag::HitIgnored));
  
  /*
   * bool any(Mask_t bits) const
   */
  BOOST_CHECK(flags.any(recob::TrajectoryPointFlags::makeMask
    (trkflag::NoPoint, trkflag::HitIgnored)
    ));
  
  /*
   * bool any(Flags...) const
   */
  BOOST_CHECK(flags.any(trkflag::NoPoint, trkflag::HitIgnored));
  
  /*
   * bool all(Mask_t bits) const
   */
  BOOST_CHECK(flags.all(recob::TrajectoryPointFlags::makeMask
    (trkflag::NoPoint, trkflag::HitIgnored)
    ));
  
  /*
   * bool all(Flags...) const
   */
  BOOST_CHECK(flags.all(trkflag::NoPoint, trkflag::HitIgnored));
  
  /*
   * hasOriginalHitIndex() const
   */
  BOOST_CHECK(!flags.hasOriginalHitIndex());
  
  /*
   * HitIndex_t fromHit() const
   */
  BOOST_CHECK_EQUAL(flags.fromHit(), InvalidHitIndex);
  
  
} // TrajectoryPointFlagsTest_FlagsConstructor()


//------------------------------------------------------------------------------
void TrajectoryPointFlagsTest_BitmaskConstructor() {
  
  using trkflag = recob::TrajectoryPointFlags::flag;
  
  using Flag_t = recob::TrajectoryPointFlags::Flag_t;
  std::set<Flag_t> const expectedValues
    = { trkflag::NoPoint, trkflag::Rejected };
  std::set<Flag_t> const expectedFlags
    = { trkflag::NoPoint, trkflag::Rejected, trkflag::HitIgnored };
  
  constexpr auto flagbitmask = recob::TrajectoryPointFlags::makeMask(
    trkflag::NoPoint, trkflag::Rejected,
    util::flags::Unset(trkflag::HitIgnored)
    );
  
  /*
   * constexpr TrajectoryPointFlags(FromMaskTag_t, HitIndex_t, Mask_t)
   */
  constexpr recob::TrajectoryPointFlags flags(12, flagbitmask);
  
  BOOST_CHECK_EQUAL(flags, recob::TrajectoryPointFlags(12, 
    trkflag::NoPoint + trkflag::Rejected + -trkflag::HitIgnored));
/*
  static_assert(flags == recob::TrajectoryPointFlags(12, 
    trkflag::NoPoint + trkflag::Rejected + -trkflag::HitIgnored),
    "Constexpr declaration with mask failed.");
  */
  
  /*
   * void dump(...) const;
   */
  for (unsigned int level = 0; level < 2; ++level) {
    std::cout << "Bitmask-constructed flags, dump(verbosity=" << level << "):"
      << std::endl;
    flags.dump(std::cout, level, "  ");
    std::cout << "\n" << std::endl;
  } // for
  
  /*
   * FlagIndex_t nFlags() const
   */
  BOOST_CHECK_EQUAL(flags.nFlags(), trkflag::maxFlags());
  for(
    recob::TrajectoryPointFlags::FlagIndex_t i = 0;
    i <= trkflag::maxFlags();
    ++i)
  {
    bool const allocated = (i < trkflag::maxFlags());
    
    BOOST_TEST_MESSAGE("Testing flag #" << i);
    
    /*
     * constexpr bool isAllocated(FlagIndex_t flag) const
     */
    BOOST_CHECK_EQUAL(flags.isAllocated(i), allocated);
    
    // for unallocated flag indices, most of the flag interface has
    // undefined behaviour
    if (!allocated) {
      /*
       * bool test(FlagIndex_t flag) const
       */
      BOOST_CHECK_THROW(flags.test(i), std::out_of_range);
      
      continue;
    } // if not allocated
    
    Flag_t flag { i }; // flag with bit index i set
    
    bool const defined = (expectedFlags.count(flag) > 0);
    bool const set = (expectedValues.count(flag) > 0);
    
    /*
     * constexpr bool isFlag(FlagIndex_t flagIndex) const
     */
    BOOST_CHECK_EQUAL(flags.isFlag(i), allocated);
    
    /*
     * constexpr bool isFlag(Flag_t flag) const
     */
    BOOST_CHECK_EQUAL(flags.isFlag(flag), allocated);
    
    /*
     * bool isDefined(Flag_t flag) const
     */
    BOOST_CHECK_EQUAL(flags.isDefined(i), defined);
    BOOST_CHECK_EQUAL(flags.isDefined(flag), defined);
    
    /*
     * bool test(Flag_t flag) const
     */
    if (defined) {
      BOOST_CHECK_EQUAL(flags.test(i), set); // implicit conversion to Flag_t
      BOOST_CHECK_EQUAL(flags.test(flag), set);
    }
    else {
      BOOST_CHECK_THROW(flags.test(i), util::flags::errors::FlagNotDefined);
      BOOST_CHECK_THROW(flags.test(flag), util::flags::errors::FlagNotDefined);
      continue;
    }
    
    /*
     * bool get(Flag_t flag) const
     */
    BOOST_CHECK_EQUAL(flags.get(i), set);
    BOOST_CHECK_EQUAL(flags.get(flag), set);
    
    /*
     * bool isSet(Flag_t flag) const
     */
    BOOST_CHECK_EQUAL(flags.isSet(i), set);
    BOOST_CHECK_EQUAL(flags.isSet(flag), set);
    
    /*
     * bool isUnset(Flag_t flag) const
     */
    BOOST_CHECK_EQUAL(flags.isUnset(i), !set);
    BOOST_CHECK_EQUAL(flags.isUnset(flag), !set);
    
  } // for
  
  /*
   * Mask_t mask() const
   */
  BOOST_CHECK_EQUAL(flags.mask(), flagbitmask);
  
  // The following tets is based on a definition of:
  // trkflag::NoPoint + trkflag::Rejected - trkflag::HitIgnored
  
  /*
   * bool all(Flags... flags) const;
   * bool any(Flags... flags) const;
   * bool none(Flags... flags) const;
   */
  BOOST_CHECK( flags.all(
    trkflag::NoPoint, trkflag::Rejected, trkflag::HitIgnored
    ));
  BOOST_CHECK( flags.any(
    trkflag::NoPoint, trkflag::Rejected, trkflag::HitIgnored
    ));
  BOOST_CHECK(!flags.none(
    trkflag::NoPoint, trkflag::Rejected, trkflag::HitIgnored
    ));
  
  BOOST_CHECK(!flags.none(trkflag::NoPoint, trkflag::HitIgnored));
  BOOST_CHECK( flags.any (trkflag::NoPoint, trkflag::HitIgnored));
  BOOST_CHECK(!flags.all (trkflag::NoPoint, trkflag::HitIgnored));
  
  BOOST_CHECK( flags.all (trkflag::NoPoint, trkflag::Rejected));
  BOOST_CHECK( flags.any (trkflag::NoPoint, trkflag::Rejected));
  BOOST_CHECK(!flags.none(trkflag::NoPoint, trkflag::Rejected));
  
  BOOST_CHECK(!flags.all(
    trkflag::NoPoint, trkflag::Rejected, trkflag::DetectorIssue
    ));
  BOOST_CHECK( flags.any(
    trkflag::NoPoint, trkflag::Rejected, trkflag::DetectorIssue
    ));
  BOOST_CHECK(!flags.none(
    trkflag::NoPoint, trkflag::Rejected, trkflag::DetectorIssue
    ));
  
  /*
   * bool none(Mask_t bits) const
   * bool any(Mask_t bits) const
   * bool all(Mask_t bits) const
   */
  BOOST_CHECK(!flags.none(recob::TrajectoryPointFlags::makeMask
    (trkflag::NoPoint, trkflag::HitIgnored)
    ));
  BOOST_CHECK(flags.any(recob::TrajectoryPointFlags::makeMask
    (trkflag::NoPoint, trkflag::HitIgnored)
    ));
  BOOST_CHECK(!flags.all(recob::TrajectoryPointFlags::makeMask
    (trkflag::NoPoint, trkflag::HitIgnored)
    ));
  
  /*
   * bool isHitIgnored() const
   */
  BOOST_CHECK_EQUAL
    (flags.isHitIgnored(), (expectedValues.count(trkflag::HitIgnored) > 0));
  
  /*
   * bool isPointValid() const
   */
  BOOST_CHECK_EQUAL
    (flags.isPointValid(), (expectedValues.count(trkflag::NoPoint) == 0));
  
  /*
   * hasOriginalHitIndex() const
   */
  BOOST_CHECK(flags.hasOriginalHitIndex());
  
  /*
   * HitIndex_t fromHit() const
   */
  BOOST_CHECK_EQUAL(flags.fromHit(), 12U);
  
  
} // TrajectoryPointFlagsTest_BitmaskConstructor()


//------------------------------------------------------------------------------
void TrajectoryPointFlagsDocumentationTest_TrajectoryPointFlags_FromMaskTag_t()
{
  
  /*
   * constexpr TrajectoryPointFlags(FromMaskTag_t, HitIndex_t, Mask_t)
   */
  using trkflag = recob::TrajectoryPointFlags::flag;
  constexpr recob::TrajectoryPointFlags flags [[gnu::unused]](
    12,
    recob::TrajectoryPointFlags::makeMask(
      trkflag::NoPoint,
      trkflag::HitIgnored
      )
    );
} // TrajectoryPointFlagsDocumentationTest_TrajectoryPointFlags_FromMaskTag_t()

void TrajectoryPointFlagsDocumentationTest_TrajectoryPointFlags_HitIndex_t_Flags
  ()
{
  
  /*
   * constexpr TrajectoryPointFlags(HitIndex_t fromHit, Flags... flags)
   */
  using trkflag = recob::TrajectoryPointFlags::flag;
  constexpr recob::TrajectoryPointFlags flags [[gnu::unused]](
    recob::TrajectoryPointFlags::InvalidHitIndex,
    trkflag::NoPoint,
    trkflag::Merged
    );
  
} // TrajectoryPointFlagsDocumentationTest_TrajectoryPointFlags_HitIndex_t_Flags

void TrajectoryPointFlagsDocumentationTest_makeMask() {
  /*
   * Mask_t makeMask(Flags... flags)
   */
  
  using trkflag = recob::TrajectoryPointFlags::flag;
  constexpr auto mask [[gnu::unused]] = recob::TrajectoryPointFlags::makeMask
    (trkflag::NoPoint, trkflag::Merged);
} // TrajectoryPointFlagsDocumentationTest_makeMask()


void TrajectoryPointFlagsDocumentationTest() {
  // This test is meant to check that the code in the documentation,
  // which is copied here, is compilable.
  TrajectoryPointFlagsDocumentationTest_TrajectoryPointFlags_FromMaskTag_t();
  TrajectoryPointFlagsDocumentationTest_TrajectoryPointFlags_HitIndex_t_Flags();
  TrajectoryPointFlagsDocumentationTest_makeMask();
  
} // TrajectoryPointFlagsTest()


//------------------------------------------------------------------------------
//--- registration of tests
//
// Boost needs now to know which tests we want to run.
// Tests are "automatically" registered, hence the BOOST_AUTO_TEST_CASE()
// macro name. The argument is the name of the test; each step may have a
// number of checks and it will fail if any of them does.
//

BOOST_AUTO_TEST_CASE(TrajectoryPointFlagsTestCase) {
  
  TrajectoryPointFlagsTest_DefaultConstructor();
  TrajectoryPointFlagsTest_FlagsConstructor();
  TrajectoryPointFlagsTest_BitmaskConstructor();
  
} // BOOST_AUTO_TEST_CASE(TrajectoryPointFlagsTestCase)


BOOST_AUTO_TEST_CASE(TrajectoryPointFlagsDocumentationTestCase) {
  
  TrajectoryPointFlagsDocumentationTest();
  
} // BOOST_AUTO_TEST_CASE(TrajectoryPointFlagsDocumentationTestCase)

