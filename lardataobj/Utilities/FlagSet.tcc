/**
 * @file   FlagSet.tcc
 * @brief  Class holding flags.
 * @author Gianluca Petrillo (petrillo@fnal.gov)
 * @date   January 25, 2017
 * @see    FlagSet.h
 * 
 */

#ifndef LARDATAOBJ_UTILITIES_FLAGSET_TCC
#define LARDATAOBJ_UTILITIES_FLAGSET_TCC

#ifndef LARDATAOBJ_UTILITIES_FLAGSET_H
# error "Do not include FlagSet.tcc directly: include FlagSet.h instead."
#endif // !LARDATAOBJ_UTILITIES_FLAGSET_H


// C/C++ standard library
#include <type_traits> // std::integral_constant, std::make_unsigned_t, ...
#include <algorithm> // std::for_each()
#include <limits> // std::numeric_limits<>


namespace util {
  
  namespace details {
    
    //--------------------------------------------------------------------------
    template <typename First, typename... Others>
    struct GrowingSizeTypes;
    
    template <typename First, typename Next, typename... Others>
    struct GrowingSizeTypes<First, Next, Others...>
      : public std::integral_constant<
         bool,
         (sizeof(First) <= sizeof(Next))
         && GrowingSizeTypes<Next, Others...>::value
         >
      {};
    
    template <typename First>
    struct GrowingSizeTypes<First>: public std::true_type {};
    
    
    //--------------------------------------------------------------------------
    template <unsigned int NBits, typename... TestTypes>
    struct FirstUnsignedTypeNoSmallerThanImpl;
    
    template <unsigned int NBits, typename TestType, typename... Others>
    struct FirstUnsignedTypeNoSmallerThanImpl<NBits, TestType, Others...> {
      static_assert(sizeof...(Others) > 0, "Wrong specialisation!");
      static_assert(GrowingSizeTypes<TestType, Others...>(),
        "Types are not in increasing size order!");
      
      using unsigned_test_type = std::make_unsigned_t<TestType>;
      static_assert(sizeof(TestType) == sizeof(unsigned_test_type),
        "Strange: signed and unsigned type sizes differ.");
      
      using type = std::conditional_t<
        (sizeof(unsigned_test_type) * 8 >= NBits),
        unsigned_test_type,
        typename FirstUnsignedTypeNoSmallerThanImpl<NBits, Others...>::type
        >;
      
    }; // FirstUnsignedTypeNoSmallerThanImpl<>
    
    template <unsigned int NBits, typename TestType>
    struct FirstUnsignedTypeNoSmallerThanImpl<NBits, TestType>
      { using type = TestType; };
    
    template <unsigned int NBits, typename... TestTypes>
    struct FirstUnsignedTypeNoSmallerThan {
      
      using type =
        typename FirstUnsignedTypeNoSmallerThanImpl<NBits, TestTypes...>::type;
      
      static_assert(sizeof(type) * 8 >= NBits, "No type is large enough!");
    }; // FirstUnsignedTypeNoSmallerThan
    
    /// Trait containing the smallest integral type accommodating `NBits` bits.
    template <unsigned int NBits>
    struct SmallestUIntType {
      
      using type = typename FirstUnsignedTypeNoSmallerThan
        <NBits, char, short int, int, long int, long long int>::type;
      
    }; // struct SmallestUIntType
    
    /// Returns the number of Storage variables needed to hold that many bits
    template <typename Storage>
    constexpr unsigned int computePages(unsigned int bits)
      {
        return 1
          + ((sizeof(Storage) * 8 >= bits)? 0: computePages<Storage>(bits - 8));
      }
    
    //--------------------------------------------------------------------------
    
    template <typename T>
    constexpr FlagIndex_t firstBitIndexImpl(T bits, FlagIndex_t carry)
      { return (bits & 1) ? carry: firstBitIndexImpl(bits >> 1, carry + 1); }
    
    /// Returns the index of the least significant set bit, or max() if none.
    template <typename T>
    constexpr FlagIndex_t firstBitIndex(T bits) {
      return bits? firstBitIndexImpl(bits, 0): std::numeric_limits<T>::max();
    }
    
    
    //--------------------------------------------------------------------------
    template <unsigned int NBits, typename Storage>
    char flagChar(
      FlagSet<NBits, Storage> const& flags,
      typename FlagSet<NBits, Storage>::Flag_t flag,
      char const unset = '0', char const set = '1', char const undefined = '-',
      char const unexpected = '?'
      )
    {
      if (flags.isUndefined(flag)) return undefined;
      if (flags.isSet(flag)) return set;
      if (flags.isUnset(flag)) return unset;
      return unexpected;
    } // flagChar()
    
    //--------------------------------------------------------------------------
    
  } // namespace details
  
  
  
  
} // namespace util

//------------------------------------------------------------------------------
template <typename Storage>
constexpr util::FlagIndex_t util::Flag_t<Storage>::index() const
  { return details::firstBitIndex(bits); } 

//------------------------------------------------------------------------------
template <unsigned int NFlags, typename Storage>
constexpr size_t util::FlagSet<NFlags, Storage>::capacity()
  { return sizeof(Storage) * 8; } 

//------------------------------------------------------------------------------
template <unsigned int NFlags, typename Storage>
constexpr bool util::FlagSet<NFlags, Storage>::isDefined(Flag_t flag) const
  { return testBits(presence, flag.bits); }

//------------------------------------------------------------------------------
template <unsigned int NFlags, typename Storage>
constexpr bool util::FlagSet<NFlags, Storage>::isUndefined(Flag_t flag) const
  { return !isDefined(flag); }

//------------------------------------------------------------------------------
template <unsigned int NFlags, typename Storage>
constexpr bool util::FlagSet<NFlags, Storage>::get(Flag_t flag) const
  { return testBits(values, flag.bits); }

//------------------------------------------------------------------------------
template <unsigned int NFlags, typename Storage>
bool util::FlagSet<NFlags, Storage>::test(Flag_t flag) const {
  if (!isFlag(flag))
    throw OutOfRangeError("Invalid flag was tested: #" + util::to_string(flag));
  if (!isDefined(flag)) {
    throw FlagNotDefinedError
      ("Undefined flag was tested: #" + util::to_string(flag));
  }
  return get(flag);
} // util::FlagSet<>::test()

//------------------------------------------------------------------------------
template <unsigned int NFlags, typename Storage>
constexpr bool util::FlagSet<NFlags, Storage>::isFlag(Flag_t flag) const
  { return flag.index() < size(); }

//------------------------------------------------------------------------------
template <unsigned int NFlags, typename Storage>
constexpr bool util::FlagSet<NFlags, Storage>::isSet(Flag_t flag) const
  { return get(flag) && isDefined(flag); }

//------------------------------------------------------------------------------
template <unsigned int NFlags, typename Storage>
constexpr bool util::FlagSet<NFlags, Storage>::isUnset(Flag_t flag) const {
  return !get(flag) && isDefined(flag);
}

//------------------------------------------------------------------------------
template <unsigned int NFlags, typename Storage>
void util::FlagSet<NFlags, Storage>::defineSingle(Flag_t flag)
  { setBits(presence, flag.bits); }

//------------------------------------------------------------------------------
template <unsigned int NFlags, typename Storage>
template <typename... Flags>
void util::FlagSet<NFlags, Storage>::undefineImpl(Flag_t flag, Flags... others)
  { undefineSingle(flag); undefineImpl(others...); }

//------------------------------------------------------------------------------
template <unsigned int NFlags, typename Storage>
void util::FlagSet<NFlags, Storage>::undefineSingle(Flag_t flag)
  { unsetBits(presence, flag.bits); }

//------------------------------------------------------------------------------
template <unsigned int NFlags, typename Storage>
template <typename... Flags>
void util::FlagSet<NFlags, Storage>::setImpl(Flag_t flag, Flags... others)
  { setSingle(flag); setImpl(others...); }

//------------------------------------------------------------------------------
template <unsigned int NFlags, typename Storage>
void util::FlagSet<NFlags, Storage>::setSingle(Flag_t flag)
  { defineSingle(flag); setBits(values, flag.bits); }

//------------------------------------------------------------------------------
template <unsigned int NFlags, typename Storage>
template <typename... Flags>
void util::FlagSet<NFlags, Storage>::unsetImpl(Flag_t flag, Flags... others)
  { unsetSingle(flag); unsetImpl(others...); }

//------------------------------------------------------------------------------
template <unsigned int NFlags, typename Storage>
void util::FlagSet<NFlags, Storage>::unsetSingle(Flag_t flag)
  { defineSingle(flag); unsetBits(values, flag.bits); }

//------------------------------------------------------------------------------
template <unsigned int NFlags, typename Storage>
template <typename BeginIter, typename EndIter>
void util::FlagSet<NFlags, Storage>::rangeSet(BeginIter begin, EndIter end)
  { std::for_each(begin, end, [this](auto&& flag){ this->set(flag); }); }

//------------------------------------------------------------------------------
template <unsigned int NFlags, typename Storage>
template <typename BeginIter, typename EndIter>
void util::FlagSet<NFlags, Storage>::rangeUnset(BeginIter begin, EndIter end)
  { std::for_each(begin, end, [this](auto&& flag){ this->unset(flag); }); }

//------------------------------------------------------------------------------
template <typename Stream, unsigned int NBits, typename Storage>
Stream& util::operator<< (Stream&& out, FlagSet<NBits, Storage> const& flag) {
  out << '{';
  
  if (flag.size() > 0) {
    
    unsigned int i = flag.size();
    out << details::flagChar(flag, --i);
    
    while (i-- > 0) {
      if ((i % 4) == 3) out << ':';
      out << details::flagChar(flag, i);
    } // while
    
  } // if we have bits
  
  out << '}';
  return out;
} // util::operator<< (Stream, FlagSet)


//------------------------------------------------------------------------------

#endif // LARDATAOBJ_UTILITIES_FLAGSET_TCC
