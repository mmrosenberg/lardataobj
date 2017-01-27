/**
 * @file   FlagSet_test.cc
 * @brief  Test for the util::FlagSet class
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

struct MyFlags {
  
  using Storage_t = unsigned short;
  using MyFlag_t = util::Flag_t<Storage_t>;
  
  static constexpr MyFlag_t F0 { 0 };
  static constexpr MyFlag_t F1 { 1 };
  static constexpr MyFlag_t F2 { 2 };
  static constexpr MyFlag_t F3 { 3 };
  static constexpr MyFlag_t F4 { 4 };
  static constexpr MyFlag_t F5 { 5 };
  static constexpr MyFlag_t F6 { 6 };
  static constexpr MyFlag_t F7 { 7 };
  static constexpr MyFlag_t F8 { 8 };
  
}; // struct MyFlags


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
    BOOST_CHECK_EQUAL(flags.isUndefined(i), !isDefined);
    BOOST_CHECK_EQUAL(flags.isDefined(i), isDefined);
    BOOST_CHECK_EQUAL(flags.isFlag(flag), isFlag);
    BOOST_CHECK_EQUAL(flags.isUndefined(flag), !isDefined);
    BOOST_CHECK_EQUAL(flags.isDefined(flag), isDefined);
    
    // In fact, the fact that the flag is not supported does not mean it can't
    // be used. But not with test().
    if (!isFlag) {
      BOOST_CHECK_THROW(flags.test(i), typename FlagSet::OutOfRangeError);
      BOOST_CHECK_THROW(flags.test(flag), typename FlagSet::OutOfRangeError);
    }
    else if (!isDefined) {
      BOOST_CHECK_THROW(flags.test(i), typename FlagSet::FlagNotDefinedError);
      BOOST_CHECK_THROW
        (flags.test(flag), typename FlagSet::FlagNotDefinedError);
      continue;
    }
    else {
      BOOST_CHECK_EQUAL(flags.test(i), isSet);
      BOOST_CHECK_EQUAL(flags.test(flag), isSet);
    }
    
    BOOST_CHECK_EQUAL(flags.get(i), isSet);
    BOOST_CHECK_EQUAL(flags.get(flag), isSet);
    if (isDefined) {
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
  static_assert(util::FlagSet<4U>::capacity() >= 4,
    "Capacity mismatch for util::FlagSet<4U>");
  static_assert(util::FlagSet<8U>::capacity() >= 8,
    "Capacity mismatch for util::FlagSet<8U>");
  static_assert(util::FlagSet<12U>::capacity() >= 12,
    "Capacity mismatch for util::FlagSet<12U>");
  static_assert(util::FlagSet<16U>::capacity() >= 16,
    "Capacity mismatch for util::FlagSet<16U>");
  static_assert(util::FlagSet<32U>::capacity() >= 32,
    "Capacity mismatch for util::FlagSet<32U>");
  
  // make sure the object is as small as possible
  static_assert(sizeof(util::FlagSet<4U>) == 2, // ideally it would be 1
    "Capacity mismatch for util::FlagSet<4U>");
  static_assert(sizeof(util::FlagSet<8U>) == 2,
    "Capacity mismatch for util::FlagSet<8U>");
  static_assert(sizeof(util::FlagSet<12U>) == 4, // ideally it would be 3
    "Capacity mismatch for util::FlagSet<12U>");
  static_assert(sizeof(util::FlagSet<16U>) == 4,
    "Capacity mismatch for util::FlagSet<16U>");
  static_assert(sizeof(util::FlagSet<32U>) == 8,
    "Capacity mismatch for util::FlagSet<32U>");
  
  
  using FlagSet_t = util::FlagSet<7U, MyFlags::Storage_t>;
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
  
  static_assert(!flags0.isDefined(MyFlags::F0.index()), "Unexpected flag #0 definition.");
  static_assert(!flags0.isDefined(MyFlags::F1.index()), "Unexpected flag #1 definition.");
  static_assert(!flags0.isDefined(MyFlags::F2.index()), "Unexpected flag #2 definition.");
  static_assert(!flags0.isDefined(MyFlags::F3.index()), "Unexpected flag #3 definition.");
  static_assert(!flags0.isDefined(MyFlags::F4.index()), "Unexpected flag #4 definition.");
  static_assert(!flags0.isDefined(MyFlags::F5.index()), "Unexpected flag #5 definition.");
  static_assert(!flags0.isDefined(MyFlags::F6.index()), "Unexpected flag #6 definition.");
  static_assert(!flags0.isDefined(MyFlags::F7.index()), "Unexpected flag #7 definition.");
  static_assert(!flags0.isSet(MyFlags::F0.index()), "Unexpected flag #0 value.");
  static_assert(!flags0.isSet(MyFlags::F1.index()), "Unexpected flag #1 value.");
  static_assert(!flags0.isSet(MyFlags::F2.index()), "Unexpected flag #2 value.");
  static_assert(!flags0.isSet(MyFlags::F3.index()), "Unexpected flag #3 value.");
  static_assert(!flags0.isSet(MyFlags::F4.index()), "Unexpected flag #4 value.");
  static_assert(!flags0.isSet(MyFlags::F5.index()), "Unexpected flag #5 value.");
  static_assert(!flags0.isSet(MyFlags::F6.index()), "Unexpected flag #6 value.");
  static_assert(!flags0.isSet(MyFlags::F7.index()), "Unexpected flag #7 value.");
  static_assert(!flags0.isUnset(MyFlags::F0.index()), "Unexpected flag #0 value.");
  static_assert(!flags0.isUnset(MyFlags::F1.index()), "Unexpected flag #1 value.");
  static_assert(!flags0.isUnset(MyFlags::F2.index()), "Unexpected flag #2 value.");
  static_assert(!flags0.isUnset(MyFlags::F3.index()), "Unexpected flag #3 value.");
  static_assert(!flags0.isUnset(MyFlags::F4.index()), "Unexpected flag #4 value.");
  static_assert(!flags0.isUnset(MyFlags::F5.index()), "Unexpected flag #5 value.");
  static_assert(!flags0.isUnset(MyFlags::F6.index()), "Unexpected flag #6 value.");
  static_assert(!flags0.isUnset(MyFlags::F7.index()), "Unexpected flag #7 value.");
  
} // FlagSetStaticTest()


void FlagSetTest() {
  
  using FlagSet_t = util::FlagSet<7U, MyFlags::Storage_t>;
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
  flags.set(5, MyFlags::F7, 1);
  defined.insert(5);
  defined.insert(7);
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
  flags.remove(3, MyFlags::F4); // flag 3 was already not defined
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
  
} // FlagSetTest()


//------------------------------------------------------------------------------
//--- registration of tests

BOOST_AUTO_TEST_CASE(FlagSetTestCase) {
  
  FlagSetStaticTest();
  FlagSetTest();
  
} // BOOST_AUTO_TEST_CASE(FlagSetTestCase)
