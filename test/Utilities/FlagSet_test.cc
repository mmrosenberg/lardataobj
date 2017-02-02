/**
 * @file   FlagSet_test.cc
 * @brief  Test for the util::flags::FlagSet class
 * @author Gianluca Petrillo (petrillo@fnal.gov)
 * @date   January 25, 2017
 * 
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
#include "lardataobj/Utilities/FlagSet.h"

// C/C++ standard library
#include <stdexcept> // std::exception
#include <set>
#include <array>
#include <iterator> // std::inserter()
#include <algorithm> // std::copy()
#include <iostream> // std::cout

template <typename T>
struct PrintTypeAsError;

template <unsigned int N>
class PrintIntAsError;

//------------------------------------------------------------------------------

namespace MyFlags {
  
  using Storage_t = unsigned short;
  using MyFlag_t = util::flags::Flag_t<Storage_t>;
  
  constexpr MyFlag_t F0 { 0 };
  constexpr MyFlag_t F1 { 1 };
  constexpr MyFlag_t F2 { 2 };
  constexpr MyFlag_t F3 { 3 };
  constexpr MyFlag_t F4 { 4 };
  constexpr MyFlag_t F5 { 5 };
  constexpr MyFlag_t F6 { 6 };
  constexpr MyFlag_t F7 { 7 };
  constexpr MyFlag_t F8 { 8 };
  
} // namespace MyFlags


template <typename FlagSet>
void CheckFlags(
  FlagSet const& flags,
  std::set<typename FlagSet::Flag_t> const& defined,
  std::set<typename FlagSet::Flag_t> const& set
) {
  
  using FlagIndex_t = typename FlagSet::FlagIndex_t;
  using Flag_t = typename FlagSet::Flag_t;
  
  for (FlagIndex_t i = 0; i <= flags.capacity(); ++i) {
    
    Flag_t flag { i };
    
    BOOST_TEST_MESSAGE("  flag #" << i);
    
    bool const isFlag = (i < flags.size());
    bool const isDefined = (defined.count(i) > 0);
    bool const isSet     = (set.count(i) > 0);
    
    BOOST_CHECK_EQUAL(flags.isFlag(i), isFlag);
    BOOST_CHECK_EQUAL(flags.isFlag(flag), isFlag);
    
    // In fact, the fact that the flag is not supported does not mean it can't
    // be used. But not with test().
    if (!isFlag) {
      BOOST_CHECK_THROW(flags.test(i), typename FlagSet::OutOfRangeError);
      BOOST_CHECK_THROW(flags.test(flag), typename FlagSet::OutOfRangeError);
      continue;
    }
    else if (!isDefined) {
      BOOST_CHECK_THROW(flags.test(i), typename FlagSet::FlagNotDefinedError);
      BOOST_CHECK_THROW
        (flags.test(flag), typename FlagSet::FlagNotDefinedError);
    }
    else {
      BOOST_CHECK_EQUAL(flags.test(i), isSet);
      BOOST_CHECK_EQUAL(flags.test(flag), isSet);
    }
    
    BOOST_CHECK_EQUAL(flags.isUndefined(i), !isDefined);
    BOOST_CHECK_EQUAL(flags.isDefined(i), isDefined);
    BOOST_CHECK_EQUAL(flags.isUndefined(flag), !isDefined);
    BOOST_CHECK_EQUAL(flags.isDefined(flag), isDefined);
    
    if (isDefined) {
      // if the flag is undefined, so it the result of get()
      BOOST_CHECK_EQUAL(flags.get(i), isSet);
      BOOST_CHECK_EQUAL(flags.get(flag), isSet);
      
      BOOST_CHECK_EQUAL(flags.isSet(i), isSet);
      BOOST_CHECK_EQUAL(flags.isUnset(i), !isSet);
      BOOST_CHECK_EQUAL(flags.isSet(flag), isSet);
      BOOST_CHECK_EQUAL(flags.isUnset(flag), !isSet);
    }
    else {
      BOOST_CHECK(!flags.isSet(i));
      BOOST_CHECK(!flags.isUnset(i));
      BOOST_CHECK(!flags.isSet(flag));
      BOOST_CHECK(!flags.isUnset(flag));
    }
  } // for
  
} // CheckFlags()


//------------------------------------------------------------------------------
//--- Test code
//

void FlagSetStaticTest() {
  
  // make sure the object is aware of having allocated flags enough
  static_assert(util::flags::FlagSet<4U>::capacity() >= 4,
    "Capacity mismatch for util::flags::FlagSet<4U>");
  static_assert(util::flags::FlagSet<8U>::capacity() >= 8,
    "Capacity mismatch for util::flags::FlagSet<8U>");
  static_assert(util::flags::FlagSet<12U>::capacity() >= 12,
    "Capacity mismatch for util::flags::FlagSet<12U>");
  static_assert(util::flags::FlagSet<16U>::capacity() >= 16,
    "Capacity mismatch for util::flags::FlagSet<16U>");
  static_assert(util::flags::FlagSet<32U>::capacity() >= 32,
    "Capacity mismatch for util::flags::FlagSet<32U>");
  
  // make sure the object is as small as possible
  static_assert(sizeof(util::flags::FlagSet<4U>) == 2, // ideally it would be 1
    "Capacity mismatch for util::flags::FlagSet<4U>");
  static_assert(sizeof(util::flags::FlagSet<8U>) == 2,
    "Capacity mismatch for util::flags::FlagSet<8U>");
  static_assert(sizeof(util::flags::FlagSet<12U>) == 4, // ideally it would be 3
    "Capacity mismatch for util::flags::FlagSet<12U>");
  static_assert(sizeof(util::flags::FlagSet<16U>) == 4,
    "Capacity mismatch for util::flags::FlagSet<16U>");
  static_assert(sizeof(util::flags::FlagSet<32U>) == 8,
    "Capacity mismatch for util::flags::FlagSet<32U>");
  
  
  using FlagSet_t = util::flags::FlagSet<7U, MyFlags::Storage_t>;
  using FlagIndex_t = FlagSet_t::FlagIndex_t;
  using Flag_t = FlagSet_t::Flag_t;
  
  constexpr FlagSet_t flags0;
  static_assert(flags0.size() == 7U, "Invalid size.");
  
  static_assert(!flags0.isDefined(MyFlags::F0), "Unexpected flag #0 definition.");
  static_assert(!flags0.isDefined(MyFlags::F1), "Unexpected flag #1 definition.");
  static_assert(!flags0.isDefined(MyFlags::F2), "Unexpected flag #2 definition.");
  static_assert(!flags0.isDefined(MyFlags::F3), "Unexpected flag #3 definition.");
  static_assert(!flags0.isDefined(MyFlags::F4), "Unexpected flag #4 definition.");
  static_assert(!flags0.isDefined(MyFlags::F5), "Unexpected flag #5 definition.");
  static_assert(!flags0.isDefined(MyFlags::F6), "Unexpected flag #6 definition.");
  static_assert(!flags0.isDefined(MyFlags::F7), "Unexpected flag #7 definition.");
  static_assert(!flags0.isSet(MyFlags::F0), "Unexpected flag #0 value.");
  static_assert(!flags0.isSet(MyFlags::F1), "Unexpected flag #1 value.");
  static_assert(!flags0.isSet(MyFlags::F2), "Unexpected flag #2 value.");
  static_assert(!flags0.isSet(MyFlags::F3), "Unexpected flag #3 value.");
  static_assert(!flags0.isSet(MyFlags::F4), "Unexpected flag #4 value.");
  static_assert(!flags0.isSet(MyFlags::F5), "Unexpected flag #5 value.");
  static_assert(!flags0.isSet(MyFlags::F6), "Unexpected flag #6 value.");
  static_assert(!flags0.isSet(MyFlags::F7), "Unexpected flag #7 value.");
  static_assert(!flags0.isUnset(MyFlags::F0), "Unexpected flag #0 value.");
  static_assert(!flags0.isUnset(MyFlags::F1), "Unexpected flag #1 value.");
  static_assert(!flags0.isUnset(MyFlags::F2), "Unexpected flag #2 value.");
  static_assert(!flags0.isUnset(MyFlags::F3), "Unexpected flag #3 value.");
  static_assert(!flags0.isUnset(MyFlags::F4), "Unexpected flag #4 value.");
  static_assert(!flags0.isUnset(MyFlags::F5), "Unexpected flag #5 value.");
  static_assert(!flags0.isUnset(MyFlags::F6), "Unexpected flag #6 value.");
  static_assert(!flags0.isUnset(MyFlags::F7), "Unexpected flag #7 value.");
  
  constexpr FlagSet_t flags1(MyFlags::F7, MyFlags::F6);
  static_assert(flags1.size() == 7U, "Invalid size.");
  
  static_assert(!flags1.isDefined(MyFlags::F0), "Unexpected flag #0 definition.");
  static_assert(!flags1.isDefined(MyFlags::F1), "Unexpected flag #1 definition.");
  static_assert(!flags1.isDefined(MyFlags::F2), "Unexpected flag #2 definition.");
  static_assert(!flags1.isDefined(MyFlags::F3), "Unexpected flag #3 definition.");
  static_assert(!flags1.isDefined(MyFlags::F4), "Unexpected flag #4 definition.");
  static_assert(!flags1.isDefined(MyFlags::F5), "Unexpected flag #5 definition.");
  static_assert( flags1.isDefined(MyFlags::F6), "Unexpected flag #6 definition.");
  static_assert( flags1.isDefined(MyFlags::F7), "Unexpected flag #7 definition.");
  static_assert(!flags1.isSet(MyFlags::F0), "Unexpected flag #0 value.");
  static_assert(!flags1.isSet(MyFlags::F1), "Unexpected flag #1 value.");
  static_assert(!flags1.isSet(MyFlags::F2), "Unexpected flag #2 value.");
  static_assert(!flags1.isSet(MyFlags::F3), "Unexpected flag #3 value.");
  static_assert(!flags1.isSet(MyFlags::F4), "Unexpected flag #4 value.");
  static_assert(!flags1.isSet(MyFlags::F5), "Unexpected flag #5 value.");
  static_assert( flags1.isSet(MyFlags::F6), "Unexpected flag #6 value.");
  static_assert( flags1.isSet(MyFlags::F7), "Unexpected flag #7 value.");
  static_assert(!flags1.isUnset(MyFlags::F0), "Unexpected flag #0 value.");
  static_assert(!flags1.isUnset(MyFlags::F1), "Unexpected flag #1 value.");
  static_assert(!flags1.isUnset(MyFlags::F2), "Unexpected flag #2 value.");
  static_assert(!flags1.isUnset(MyFlags::F3), "Unexpected flag #3 value.");
  static_assert(!flags1.isUnset(MyFlags::F4), "Unexpected flag #4 value.");
  static_assert(!flags1.isUnset(MyFlags::F5), "Unexpected flag #5 value.");
  static_assert(!flags1.isUnset(MyFlags::F6), "Unexpected flag #6 value.");
  static_assert(!flags1.isUnset(MyFlags::F7), "Unexpected flag #7 value.");
  
  
  
} // FlagSetStaticTest()


void FlagSetTest() {
  
  using FlagSet_t = util::flags::FlagSet<7U, MyFlags::Storage_t>;
  using FlagIndex_t = FlagSet_t::FlagIndex_t;
  using Flag_t = FlagSet_t::Flag_t;
  
  
  // the last flag is "unsupported"
  std::array<Flag_t, 3> const indices = { 4, MyFlags::F6, 7 };
  
  std::set<Flag_t> defined, set;
  
  BOOST_TEST_MESSAGE("Default construction");
  FlagSet_t flags;
  std::cout << "  => " << flags << std::endl;
  CheckFlags(flags, defined, set);
  
  BOOST_TEST_MESSAGE("Single bit set");
  flags.set(1);
  defined.insert(1);
  set.insert(1);
  std::cout << "  => " << flags << std::endl;
  CheckFlags(flags, defined, set);
  
  BOOST_TEST_MESSAGE("Two more bits set (one set again)");
  flags.set(5, MyFlags::F7, MyFlags::F3, 1);
  defined.insert(3);
  defined.insert(5);
  defined.insert(7);
  set.insert(3);
  set.insert(5);
  set.insert(7);
  std::cout << "  => " << flags << std::endl;
  CheckFlags(flags, defined, set);
  
  BOOST_TEST_MESSAGE("Bits set from a list of bits to set");
  flags.rangeSet(indices.begin(), indices.end());
  std::for_each(indices.begin(), indices.end(),
    [&defined](auto flag){ defined.insert(flag); }
    );
  std::for_each
    (indices.begin(), indices.end(), [&set](auto flag){ set.insert(flag); });
  std::cout << "  => " << flags << std::endl;
  CheckFlags(flags, defined, set);
  
  BOOST_TEST_MESSAGE("Undefine bits");
  flags.remove(2, 3, MyFlags::F4); // flag 2 was already not defined
  defined.erase(3);
  defined.erase(4);
  set.erase(4);
  std::cout << "  => " << flags << std::endl;
  CheckFlags(flags, defined, set);
  
  BOOST_TEST_MESSAGE("Unset bits");
  flags.unset(MyFlags::F7, 5);
  set.erase(7);
  set.erase(5);
  std::cout << "  => " << flags << std::endl;
  CheckFlags(flags, defined, set);
  
  BOOST_TEST_MESSAGE("Unset bit range");
  flags.rangeUnset(indices.begin(), indices.end());
  std::for_each(indices.begin(), indices.end(),
    [&defined](auto flag){ defined.insert(flag); }
    );
  std::for_each(indices.begin(), indices.end(),
    [&set](auto flag){ set.erase(flag); }
    );
  std::cout << "  => " << flags << std::endl;
  CheckFlags(flags, defined, set);
  
  flags.clear();
  CheckFlags(flags, {}, {});
  flags.set  (MyFlags::F1);
  flags.unset(MyFlags::F4);
  flags.unset(MyFlags::F5);
  
  /*
   * constexpr bool all(Bits_t bits) const
   * constexpr bool any(Bits_t bits) const
   * constexpr bool none(Bits_t bits) const
   */
  
  // Reminder: undefined flags will always contribute a "false".
  // flag 1 is set
  BOOST_CHECK( flags.all (MyFlags::F1));
  BOOST_CHECK( flags.any (MyFlags::F1));
  BOOST_CHECK(!flags.none(MyFlags::F1));
  
  // flag 1 is set, flag 4 is unset
  BOOST_CHECK(!flags.all (MyFlags::F1 + MyFlags::F4));
  BOOST_CHECK( flags.any (MyFlags::F1 + MyFlags::F4));
  BOOST_CHECK(!flags.none(MyFlags::F1 + MyFlags::F4));
  
  // flag 1 is set, flag 3 is undefined (had been defined and set though)
  BOOST_CHECK(!flags.all (MyFlags::F1 + MyFlags::F3));
  BOOST_CHECK( flags.any (MyFlags::F1 + MyFlags::F3));
  BOOST_CHECK(!flags.none(MyFlags::F1 + MyFlags::F3));
  
  // flag 4 is unset, flag 3 is undefined (had been defined though)
  BOOST_CHECK(!flags.all (MyFlags::F3 + MyFlags::F4));
  BOOST_CHECK(!flags.any (MyFlags::F3 + MyFlags::F4));
  BOOST_CHECK(!flags.none(MyFlags::F3 + MyFlags::F4));
  
  // flag 3 is undefined (had been defined though)
  BOOST_CHECK(!flags.all (MyFlags::F3));
  BOOST_CHECK(!flags.any (MyFlags::F3));
  BOOST_CHECK(!flags.none(MyFlags::F3));
  
  // flag 4 is unset
  BOOST_CHECK(!flags.all (MyFlags::F4));
  BOOST_CHECK(!flags.any (MyFlags::F4));
  BOOST_CHECK( flags.none(MyFlags::F4));
  
  /*
   * constexpr bool anySet(Mask_t const& mask) const
   * constexpr bool match(Mask_t const& mask) const
   */
  BOOST_CHECK( flags.match  (flags.mask()));
  BOOST_CHECK(!flags.match  (flags.mask() - MyFlags::F6)); // also unset F6
  BOOST_CHECK(!flags.match  (flags.mask() + MyFlags::F5)); // set F5
  BOOST_CHECK(!flags.match  (flags.mask() + MyFlags::F6)); // also set F6
  BOOST_CHECK(!flags.match  (flags.mask() - MyFlags::F1 + MyFlags::F6));
  BOOST_CHECK( flags.anySet (flags.mask()));
  BOOST_CHECK( flags.anySet (flags.mask() - MyFlags::F6));
  BOOST_CHECK( flags.anySet (flags.mask() + MyFlags::F5));
  BOOST_CHECK( flags.anySet (flags.mask() + MyFlags::F6));
  BOOST_CHECK(!flags.anySet (flags.mask() - MyFlags::F1 + MyFlags::F6));
  BOOST_CHECK(!flags.noneSet(flags.mask()));
  BOOST_CHECK(!flags.noneSet(flags.mask() - MyFlags::F6));
  BOOST_CHECK(!flags.noneSet(flags.mask() + MyFlags::F5));
  BOOST_CHECK(!flags.noneSet(flags.mask() + MyFlags::F6));
  BOOST_CHECK( flags.noneSet(flags.mask() - MyFlags::F1 + MyFlags::F6));
  
} // FlagSetTest()


//------------------------------------------------------------------------------
void BitMaskDocTest_ConstructorFromValues1() {
  
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  using MyMask_t = util::flags::BitMask<unsigned int>;
  constexpr MyMask_t DefaultMask(MyMask_t::fromValues, 0x0300U);
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  
} // BitMaskDocTest_ConstructorFromValues1()


void BitMaskDocTest_ConstructorFromValues2() {
  
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  using MyMask_t = util::flags::BitMask<unsigned int>;
  constexpr MyMask_t DefaultMask(MyMask_t::fromValues, 0x0300U, 0x0200U);
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  
} // BitMaskDocTest_ConstructorFromValues2()


void BitMaskDocTest() {
  
  BitMaskDocTest_ConstructorFromValues1();
  BitMaskDocTest_ConstructorFromValues2();
  
} // BitMaskDocTest()


//------------------------------------------------------------------------------

void BitMaskCombineTests() {
  
  using Mask_t = util::flags::BitMask<unsigned short>;
  
  constexpr Mask_t maskA(Mask_t::fromValues, 0b011'011'011, 0b001'001'001);
  constexpr Mask_t maskB(Mask_t::fromValues, 0b000'111'111, 0b000'000'111);
  constexpr Mask_t::Bits_t bitsB            (0b000'000'111);
  
  /*
   * constexpr mergeIntoMask(Mask_t baseMask, Mask_t mask)
   */
  constexpr Mask_t maskMAB(Mask_t::fromValues, 0b011'111'111, 0b001'000'111);
  static_assert(Mask_t::mergeIntoMask(maskA, maskB) == maskMAB,
    "mergeIntoMask(Mask_t, Mask_t) failure");
  static_assert
    (maskA + maskB == maskMAB, "maskA + maskB failure");
  
  constexpr Mask_t maskMBA(Mask_t::fromValues, 0b011'111'111, 0b001'001'101);
  static_assert(Mask_t::mergeIntoMask(maskB, maskA) == maskMBA,
    "mergeIntoMask(Mask_t, Mask_t) failure");
  static_assert
    (maskB + maskA == maskMBA, "maskB + maskA failure");
  
  /*
   * mergeIntoMask(Mask_t baseMask, Bits_t bits)
   */
  constexpr Mask_t maskMAbB(Mask_t::fromValues, 0b011'011'111, 0b001'001'111);
  static_assert(Mask_t::mergeIntoMask(maskA, bitsB) == maskMAbB,
    "mergeIntoMask(Mask_t, Bits_t) failure");
  static_assert
    (maskA + bitsB == maskMAbB, "maskA + bitsB failure");
  
  constexpr Mask_t maskMbBA(Mask_t::fromValues, 0b011'011'111, 0b001'001'101);
  static_assert(bitsB + maskA == maskMbBA, "bitsB + maskA failure");
  
  /*
   * combineWithMask(Mask_t A, Mask_t B)
   */
  constexpr Mask_t maskCAB(Mask_t::fromValues, 0b011'111'111, 0b001'001'111);
  static_assert(Mask_t::combineWithMask(maskA, maskB) == maskCAB,
    "combineWithMask(Mask_t, Mask_t) failure");
  static_assert
    ((maskA | maskB) == maskCAB, "maskA | maskB failure");
  
  constexpr Mask_t maskCBA(Mask_t::fromValues, 0b011'111'111, 0b001'001'111);
  static_assert(Mask_t::combineWithMask(maskB, maskA) == maskCBA,
    "combineWithMask(Mask_t, Mask_t) failure");
  static_assert
    ((maskB | maskA) == maskCBA, "maskB | maskA failure");
  
  /*
   * combineWithMask(Mask_t mask, Bits_t bits)
   */
  constexpr Mask_t maskCAbB(Mask_t::fromValues, 0b011'011'111, 0b001'001'111);
  static_assert(Mask_t::combineWithMask(maskA, bitsB) == maskCAbB,
    "combineWithMask(Mask_t, Bits_t) failure");
  static_assert
    ((maskA | bitsB) == maskCAbB, "maskA | bitsB failure");
  
  constexpr Mask_t maskCbBA(Mask_t::fromValues, 0b011'011'111, 0b001'001'111);
  static_assert
    ((bitsB | maskA) == maskCbBA, "bitsB | maskA failure");
  
  /*
   * intersectWithMask(Mask_t A, Mask_t B)
   */
  constexpr Mask_t maskIAB(Mask_t::fromValues, 0b011'111'111, 0b001'000'101);
  static_assert(Mask_t::intersectWithMask(maskA, maskB) == maskIAB,
    "intersectWithMask(Mask_t, Mask_t) failure");
  static_assert((maskA & maskB) == maskIAB, "maskA & maskB failure");
  
  constexpr Mask_t maskIBA(Mask_t::fromValues, 0b011'111'111, 0b001'000'101);
  static_assert(Mask_t::intersectWithMask(maskB, maskA) == maskIBA,
    "intersectWithMask(Mask_t, Mask_t) failure");
  static_assert((maskB & maskA) == maskIBA, "maskB & maskA failure");
  
  /*
   * intersectWithMask(Mask_t mask, Bits_t bits)
   */
  constexpr Mask_t maskIAbB(Mask_t::fromValues, 0b011'011'111, 0b001'001'101);
  static_assert(Mask_t::intersectWithMask(maskA, bitsB) == maskIAbB,
    "intersectWithMask(Mask_t, Bits_t) failure");
  static_assert
    ((maskA & bitsB) == maskIAbB, "maskA & bitsB failure");
  
  constexpr Mask_t maskIbBA(Mask_t::fromValues, 0b011'011'111, 0b001'001'101);
  static_assert((bitsB & maskA) == maskIbBA, "bitsB & maskA failure");
  
  /*
   * unsetMask(Mask_t A, Mask_t B)
   */
  constexpr Mask_t maskUAB(Mask_t::fromValues, 0b011'111'111, 0b001'001'000);
  static_assert(Mask_t::unsetMask(maskA, maskB) == maskUAB,
    "unsetMask(Mask_t, Mask_t) failure");
  static_assert((maskA - maskB) == maskUAB, "maskA - maskB failure");
  
  constexpr Mask_t maskUBA(Mask_t::fromValues, 0b011'111'111, 0b000'000'110);
  static_assert(Mask_t::unsetMask(maskB, maskA) == maskUBA,
    "unsetMask(Mask_t, Mask_t) failure");
  static_assert((maskB - maskA) == maskUBA, "maskB - maskA failure");
  
  /*
   * unsetMask(Mask_t mask, Bits_t bits)
   */
  constexpr Mask_t maskUAbB(Mask_t::fromValues, 0b011'011'111, 0b001'001'000);
  static_assert(Mask_t::unsetMask(maskA, bitsB) == maskUAbB,
    "unsetMask(Mask_t, Bits_t) failure");
  static_assert((maskA - bitsB) == maskUAbB, "maskA - bitsB failure");
  
  constexpr Mask_t maskUbBA(Mask_t::fromValues, 0b011'011'111, 0b000'000'110);
  static_assert((bitsB - maskA) == maskUbBA, "bitsB - maskA failure");
  
  /*
   * negateMask(Mask_t mask)
   */
  constexpr Mask_t maskNA(Mask_t::fromValues, 0b011'011'011, 0b010'010'010);
  static_assert
    (Mask_t::negateMask(maskA) == maskNA, "negateMask(Mask_t) failure");
  static_assert(~maskA == maskNA, "~mask failure");
  
  /*
   * negateMask(Bits_t bits)
   */
  constexpr Mask_t maskNbB(Mask_t::fromValues, 0b000'000'111, 0b000'000'000);
  static_assert
    (Mask_t::negateMask(bitsB) == maskNbB, "negateMask(Bits_t) failure");
//  static_assert(~bitsB == maskNbB, "~bits failure");
  
  /*
   * unary operator+
   */
  constexpr Mask_t maskbB(Mask_t::fromValues, 0b000'000'111, 0b000'000'111);
  static_assert(+maskA == maskA, "+mask failure");
  static_assert(+bitsB == maskbB, "+bits failure");
  
  /*
   * unary operator-
   */
  static_assert(maskA + (-bitsB) == maskA - bitsB, "-bits failure");
  
  
} // BitMaskCombineTests()



//------------------------------------------------------------------------------
void SetUnsetTest() {
  
  using Mask_t = util::flags::BitMask<unsigned short>;
  
  Mask_t mask(util::flags::Set(MyFlags::F1), util::flags::Unset(MyFlags::F2));
  
  std::cout << "Testing Set()/Unset() on " << mask << std::endl;
  
  BOOST_CHECK(!mask.isDefined(0));
  BOOST_CHECK( mask.isDefined(1));
  BOOST_CHECK( mask.isDefined(2));
  BOOST_CHECK(!mask.isDefined(3));
  
  BOOST_CHECK(!mask.isSet(0));
  BOOST_CHECK( mask.isSet(1));
  BOOST_CHECK(!mask.isSet(2));
  BOOST_CHECK(!mask.isSet(3));
  
  BOOST_CHECK(!mask.isUnset(0));
  BOOST_CHECK(!mask.isUnset(1));
  BOOST_CHECK( mask.isUnset(2));
  BOOST_CHECK(!mask.isUnset(3));
  
  
} // SetUnsetTest()


//------------------------------------------------------------------------------
//--- registration of tests

BOOST_AUTO_TEST_CASE(BitMaskTestCase) {
  
  BitMaskCombineTests();
  BitMaskDocTest();
  
} // BOOST_AUTO_TEST_CASE(FlagSetTestCase)


BOOST_AUTO_TEST_CASE(FlagSetTestCase) {
  
  FlagSetStaticTest();
  FlagSetTest();
  SetUnsetTest();
  
} // BOOST_AUTO_TEST_CASE(FlagSetTestCase)
