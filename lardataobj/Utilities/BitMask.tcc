/**
 * @file   BitMask.tcc
 * @brief  Class holding flags: template implementation file.
 * @author Gianluca Petrillo (petrillo@fnal.gov)
 * @date   January 25, 2017
 * @see    BitMask.h
 * 
 */

#ifndef LARDATAOBJ_UTILITIES_BITMASK_TCC
#define LARDATAOBJ_UTILITIES_BITMASK_TCC

#ifndef LARDATAOBJ_UTILITIES_BITMASK_H
# error "Do not include BitMask.tcc directly: include BitMask.h instead."
#endif // !LARDATAOBJ_UTILITIES_BITMASK_H


// C/C++ standard library
#include <type_traits> // std::integral_constant, std::make_unsigned_t, ...
#include <algorithm> // std::for_each()


namespace util {
  
  namespace flags {
  
    namespace details {
      
      //------------------------------------------------------------------------
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
      
      
      //------------------------------------------------------------------------
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
          typename FirstUnsignedTypeNoSmallerThanImpl<NBits, TestTypes...>
          ::type;
        
        static_assert(sizeof(type) * 8 >= NBits, "No type is large enough!");
      }; // FirstUnsignedTypeNoSmallerThan
      
      /// Trait containing the smallest integral type accommodating `NBits`
      /// bits.
      template <unsigned int NBits>
      struct SmallestUIntType {
        
        using type = typename FirstUnsignedTypeNoSmallerThan
          <NBits, char, short int, int, long int, long long int>::type;
        
      }; // struct SmallestUIntType
      
      /// Returns the number of Storage variables needed to hold that many bits
      template <typename Storage>
      constexpr unsigned int computePages(unsigned int bits) {
        return 1
          + ((sizeof(Storage) * 8 >= bits)? 0: computePages<Storage>(bits - 8));
      }
      
      //------------------------------------------------------------------------
      
      template <typename T>
      constexpr Index_t firstBitIndexImpl(T bits, Index_t carry)
        { return (bits & 1) ? carry: firstBitIndexImpl(bits >> 1, carry + 1); }
      
      /// Returns the index of the least significant set bit, or max() if none.
      template <typename T>
      constexpr Index_t firstBitIndex(T bits) {
        return bits? firstBitIndexImpl(bits, 0): sizeof(T) * 8;
      }
      
      
      //------------------------------------------------------------------------
      template <typename Storage>
      char flagChar(
        BitMask<Storage> const& flags,
        typename BitMask<Storage>::Flag_t flag,
        char const unset = '0', char const set = '1', char const undefined = '-',
        char const unexpected = '?'
        )
      {
        if (flags.isUndefined(flag)) return undefined;
        if (flags.isSet(flag)) return set;
        if (flags.isUnset(flag)) return unset;
        return unexpected;
      } // flagChar()
      
      //------------------------------------------------------------------------
      template <typename Storage>
      constexpr typename util::flags::BitMask<Storage>::Mask_t createMaskImpl
        (util::flags::BitMask<Storage> const baseMask)
        { return baseMask; }
          
      template <typename Storage, typename First, typename... Others>
      constexpr typename util::flags::BitMask<Storage>::Mask_t createMaskImpl(
        util::flags::BitMask<Storage> const baseMask,
        First first, Others... others
        )
      { 
        return createMaskImpl(
          util::flags::BitMask<Storage>::mergeIntoMask(baseMask, first),
          others...
          );
      }
      
          
      //------------------------------------------------------------------------
      
    } // namespace details
    
  } // namespace flags
  
} // namespace util



//------------------------------------------------------------------------------
//--- Flag_t<>
//---
template <typename Storage>
constexpr util::flags::Index_t util::flags::Flag_t<Storage>::index() const
  { return details::firstBitIndex(bits); } 


//------------------------------------------------------------------------------
//--- Bits_t<>
//---
template <typename Storage>
constexpr bool util::flags::Bits_t<Storage>::all(This_t bits) const
  { return select(bits) == bits; }

template <typename Storage>
constexpr bool util::flags::Bits_t<Storage>::any(This_t bits) const
  { return bool(select(bits)) || !bits; }

template <typename Storage>
constexpr bool util::flags::Bits_t<Storage>::none(This_t bits) const
  { return !any(bits) || !bits; }

template <typename Storage>
constexpr bool util::flags::Bits_t<Storage>::only(This_t bits) const
  { return !exclude(bits); }


//------------------------------------------------------------------------------
template <typename Storage>
constexpr util::flags::Bits_t<Storage> util::flags::Bits_t<Storage>::combine
  (This_t bits) const
  { return This_t(Storage_t(data | bits.data)); }

template <typename Storage>
constexpr util::flags::Bits_t<Storage> util::flags::Bits_t<Storage>::select
  (This_t bits) const
  { return This_t(Storage_t(data & bits.data)); }

template <typename Storage>
constexpr util::flags::Bits_t<Storage> util::flags::Bits_t<Storage>::exclude
  (This_t bits) const
  { return select(bits.invert()); }

template <typename Storage>
constexpr util::flags::Bits_t<Storage> util::flags::Bits_t<Storage>::invert
  () const
  { return This_t(Storage_t(~data)); }


//------------------------------------------------------------------------------
template <typename Storage>
void util::flags::Bits_t<Storage>::set(This_t bits)
  { setBits(data, bits.data); }

template <typename Storage>
void util::flags::Bits_t<Storage>::unset(This_t bits)
  { unsetBits(data, bits.data); }

template <typename Storage>
void util::flags::Bits_t<Storage>::keepOnly(This_t bits)
  { unsetBits(data, bits.negate().data); }
  
  
//------------------------------------------------------------------------------
template <typename Storage>
constexpr util::flags::Bits_t<Storage> util::flags::operator| 
  (util::flags::Bits_t<Storage> left, util::flags::Bits_t<Storage> right)
  { return left.combine(right); }

template <typename Storage>
constexpr util::flags::Bits_t<Storage> util::flags::operator| (
  util::flags::Bits_t<Storage> left,
  typename util::flags::Bits_t<Storage>::Flag_t right
  )
  { return left | util::flags::Bits_t<Storage>(right); }

template <typename Storage>
constexpr util::flags::Bits_t<Storage> util::flags::operator| (
  typename util::flags::Bits_t<Storage>::Flag_t left,
  util::flags::Bits_t<Storage> right
  )
  { return right | left; }

template <typename Storage>
constexpr util::flags::Bits_t<Storage> util::flags::operator| (
  util::flags::Flag_t<Storage> left,
  util::flags::Flag_t<Storage> right
  )
  { return util::flags::Bits_t<Storage>(left) | right; }


template <typename Storage>
constexpr util::flags::Bits_t<Storage> util::flags::operator+
  (util::flags::Bits_t<Storage> left, util::flags::Bits_t<Storage> right)
  { return left | right; }

template <typename Storage>
constexpr util::flags::Bits_t<Storage> util::flags::operator+ (
  util::flags::Bits_t<Storage> left,
  typename util::flags::Bits_t<Storage>::Flag_t right
  )
  { return left | right; }

template <typename Storage>
constexpr util::flags::Bits_t<Storage> util::flags::operator+ (
  typename util::flags::Bits_t<Storage>::Flag_t left,
  util::flags::Bits_t<Storage> right
  )
  { return left | right; }

template <typename Storage>
constexpr util::flags::Bits_t<Storage> util::flags::operator+ (
  util::flags::Flag_t<Storage> left,
  util::flags::Flag_t<Storage> right
  )
  { return left | right; }



//------------------------------------------------------------------------------
//--- BitMask<>
//---
template <typename Storage>
constexpr util::flags::BitMask<Storage>::BitMask
  (FromValuesTag, Bits_t defined, Bits_t values)
  : values(values)
  , presence(defined.combine(values))
  {}

template <typename Storage>
constexpr util::flags::BitMask<Storage>::BitMask
  (FromValuesTag tag, Bits_t bits)
  : BitMask(tag, bits, bits)
  {}

template <typename Storage>
constexpr util::flags::BitMask<Storage>::BitMask
  (FromValuesTag tag, Storage_t defined, Storage_t values)
  : BitMask(tag, Bits_t(defined), Bits_t(values))
  {}

template <typename Storage>
constexpr util::flags::BitMask<Storage>::BitMask
  (FromValuesTag tag, Storage_t values)
  : BitMask(tag, Bits_t(values))
  {}

//------------------------------------------------------------------------------
template <typename Storage>
constexpr typename util::flags::BitMask<Storage>::Bits_t
util::flags::BitMask<Storage>::definedOnly() const
  { return values.select(presence); }

//------------------------------------------------------------------------------
template <typename Storage>
constexpr size_t util::flags::BitMask<Storage>::capacity()
  { return sizeof(Storage) * 8; } 

//------------------------------------------------------------------------------
template <typename Storage>
constexpr bool util::flags::BitMask<Storage>::isDefined(Flag_t flag) const
  { return presence.any(flag); }

template <typename Storage>
constexpr bool util::flags::BitMask<Storage>::isDefined(Bits_t bits) const
  { return presence.all(bits); }

//------------------------------------------------------------------------------
template <typename Storage>
constexpr bool util::flags::BitMask<Storage>::isUndefined(Flag_t flag) const
  { return !isDefined(flag); }

template <typename Storage>
constexpr bool util::flags::BitMask<Storage>::isUndefined(Bits_t bits) const
  { return !isDefined(bits); }

//------------------------------------------------------------------------------
template <typename Storage>
constexpr bool util::flags::BitMask<Storage>::get(Flag_t flag) const
  { return values.any(flag); }

//------------------------------------------------------------------------------
template <typename Storage>
constexpr bool util::flags::BitMask<Storage>::isSet(Flag_t flag) const
  { return get(flag) && isDefined(flag); }

//------------------------------------------------------------------------------
template <typename Storage>
constexpr bool util::flags::BitMask<Storage>::isUnset(Flag_t flag) const
  { return !get(flag) && isDefined(flag); }

//------------------------------------------------------------------------------
template <typename Storage>
constexpr bool util::flags::BitMask<Storage>::all(Bits_t bits) const
  { return isDefined(bits) && values.all(bits); }

//------------------------------------------------------------------------------
template <typename Storage>
constexpr bool util::flags::BitMask<Storage>::any(Bits_t bits) const {
  // check values only for the flags in bits that are actually defined;
  // values.any() will return true if no bit is specified,
  // which is semantically correct but does not serve the scope here;
  // if no bits are defined, this method should return false instead.
  return values.any(presence.select(bits)) && !(presence.select(bits).empty());
}

//------------------------------------------------------------------------------
template <typename Storage>
constexpr bool util::flags::BitMask<Storage>::none(Bits_t bits) const
  { return isDefined(bits) && values.none(bits); }

//------------------------------------------------------------------------------
template <typename Storage>
constexpr bool util::flags::BitMask<Storage>::anySet(Mask_t const& mask) const {
  return
    !definedOnly().select(mask.definedOnly()).empty()
    || mask.values.select(mask.presence).empty();
}

//------------------------------------------------------------------------------
template <typename Storage>
constexpr bool util::flags::BitMask<Storage>::noneSet(Mask_t const& mask) const
  { return !anySet(mask); }


//------------------------------------------------------------------------------
template <typename Storage>
constexpr bool util::flags::BitMask<Storage>::match(Mask_t const& mask) const {
  // 1) no undefined flags that are defined in mask
  // 2) all our flags defined also in mask must have the same values as in mask
  return mask.presence.exclude(presence).empty()
    && values.select(mask.presence) == mask.definedOnly();
}


//------------------------------------------------------------------------------
template <typename Storage>
template <typename Stream>
void util::flags::BitMask<Storage>::dump(Stream&& out, unsigned int nBits) const
{
  out << '{';
  
  if (nBits > 0) {
    
    typename BitMask<Storage>::FlagIndex_t i = nBits;
    out << details::flagChar(*this, --i);
    
    while (i-- > 0) {
      if ((i % 4) == 3) out << ':';
      out << details::flagChar(*this, i);
    } // while
    
  } // if we have bits
  
  out << '}';
} // util::flags::BitMask<>::dump()


//------------------------------------------------------------------------------
template <typename Storage>
template <typename... Args>
constexpr typename util::flags::BitMask<Storage>::Mask_t
util::flags::BitMask<Storage>::create(Args... args)
  { return details::createMaskImpl(Mask_t(), args...); }

//------------------------------------------------------------------------------
template <typename Storage>
void util::flags::BitMask<Storage>::defineSingle(Flag_t flag)
  { presence.set(flag); }

//------------------------------------------------------------------------------
template <typename Storage>
template <typename... Flags>
void util::flags::BitMask<Storage>::undefineImpl(Flag_t flag, Flags... others)
  { undefineSingle(flag); undefineImpl(others...); }

//------------------------------------------------------------------------------
template <typename Storage>
void util::flags::BitMask<Storage>::undefineSingle(Flag_t flag)
  { presence.unset(flag); }

//------------------------------------------------------------------------------
template <typename Storage>
template <typename... Flags>
void util::flags::BitMask<Storage>::setImpl(Flag_t flag, Flags... others)
  { setSingle(flag); setImpl(others...); }

//------------------------------------------------------------------------------
template <typename Storage>
void util::flags::BitMask<Storage>::setSingle(Flag_t flag)
  { defineSingle(flag); values.set(flag); }

//------------------------------------------------------------------------------
template <typename Storage>
template <typename... Flags>
void util::flags::BitMask<Storage>::unsetImpl(Flag_t flag, Flags... others)
  { unsetSingle(flag); unsetImpl(others...); }

//------------------------------------------------------------------------------
template <typename Storage>
void util::flags::BitMask<Storage>::unsetSingle(Flag_t flag)
  { defineSingle(flag); values.unset(flag); }

//------------------------------------------------------------------------------
template <typename Storage>
template <typename BeginIter, typename EndIter>
void util::flags::BitMask<Storage>::rangeSet(BeginIter begin, EndIter end)
  { std::for_each(begin, end, [this](auto&& flag){ this->set(flag); }); }

//------------------------------------------------------------------------------
template <typename Storage>
template <typename BeginIter, typename EndIter>
void util::flags::BitMask<Storage>::rangeUnset(BeginIter begin, EndIter end)
  { std::for_each(begin, end, [this](auto&& flag){ this->unset(flag); }); }


//------------------------------------------------------------------------------
template <typename Storage>
constexpr typename util::flags::BitMask<Storage>::Mask_t
util::flags::BitMask<Storage>::mergeIntoMask
  (Mask_t baseMask, Mask_t mask)
{
  return Mask_t(fromValues,
    baseMask.presence.combine(mask.presence),
    (baseMask.values.exclude(mask.presence)) | mask.values
    );
} // util::flags::BitMask<>::mergeIntoMask(Mask_t)


//------------------------------------------------------------------------------
template <typename Storage>
constexpr typename util::flags::BitMask<Storage>::Mask_t
util::flags::BitMask<Storage>::mergeIntoMask
  (Mask_t baseMask, Bits_t bits)
{
  return Mask_t(fromValues,
    baseMask.presence.combine(bits),
    baseMask.values.combine(bits)
    );
} // util::flags::BitMask<>::mergeIntoMask(Mask_t)


//------------------------------------------------------------------------------
template <typename Storage>
constexpr typename util::flags::BitMask<Storage>::Mask_t
util::flags::BitMask<Storage>::mergeIntoMask
  (Mask_t baseMask, Flag_t flags)
  { return mergeIntoMask(baseMask, Bits_t(flags)); }


//------------------------------------------------------------------------------
template <typename Storage>
constexpr typename util::flags::BitMask<Storage>::Mask_t
util::flags::BitMask<Storage>::combineWithMask
  (Mask_t A, Mask_t B)
{
  return Mask_t(fromValues,
    A.presence.combine(B.presence),
    A.presence.combine(B.presence).select(A.values.combine(B.values))
    );
} // util::flags::BitMask<>::combineWithMask(Mask_t)


//------------------------------------------------------------------------------
template <typename Storage>
constexpr typename util::flags::BitMask<Storage>::Mask_t
util::flags::BitMask<Storage>::combineWithMask
  (Mask_t baseMask, Bits_t bits)
  { return mergeIntoMask(baseMask, bits); }


//------------------------------------------------------------------------------
template <typename Storage>
constexpr typename util::flags::BitMask<Storage>::Mask_t
util::flags::BitMask<Storage>::combineWithMask
  (Mask_t baseMask, Flag_t flags)
  { return combineWithMask(baseMask, Bits_t(flags)); }


//------------------------------------------------------------------------------
template <typename Storage>
constexpr typename util::flags::BitMask<Storage>::Mask_t
util::flags::BitMask<Storage>::intersectWithMask
  (Mask_t A, Mask_t B)
{
  //
  // flags undefined in B are copied from A; for the others, B wins.
  //
  return Mask_t(fromValues,
    A.presence.combine(B.presence),
    // the bits that are defined in both get AND'ed:
    A.presence.select(B.presence).select(A.values).select(B.values)
    // the bits that are not, are copied:
      .combine(
        A.values.combine(B.values).exclude(A.presence.select(B.presence))
      )
    );
}

//------------------------------------------------------------------------------
template <typename Storage>
constexpr typename util::flags::BitMask<Storage>::Mask_t
util::flags::BitMask<Storage>::intersectWithMask
  (Mask_t baseMask, Bits_t bits)
{
  // defined: (presentInA | B)
  // values:   (presentInA & B) & (valuesA & B)   : if in both terms, use AND
  //         | (!(presentInA & B) & (valuesA | B) : otherwise, set from either
  return Mask_t(fromValues,
    baseMask.presence.combine(bits),
    // the bits that are defined in both get AND'ed:
    baseMask.presence.select(bits).select(baseMask.values)
    // the bits that are not, are copied:
      .combine(
        baseMask.values.combine(bits).exclude(baseMask.presence.select(bits))
      )
    );
} // util::flags::BitMask<>::intersectWithMask(Mask_t)


//------------------------------------------------------------------------------
template <typename Storage>
constexpr typename util::flags::BitMask<Storage>::Mask_t
util::flags::BitMask<Storage>::intersectWithMask
  (Mask_t baseMask, Flag_t flags)
  { return intersectWithMask(baseMask, Bits_t(flags)); }


//------------------------------------------------------------------------------
template <typename Storage>
constexpr typename util::flags::BitMask<Storage>::Mask_t
util::flags::BitMask<Storage>::unsetMask(Mask_t baseMask, Mask_t mask) {
  return Mask_t(fromValues,
    baseMask.presence.combine(mask.presence),
    baseMask.presence.combine(mask.presence)
      .select(baseMask.values.exclude(mask.values))
    );
}

//------------------------------------------------------------------------------
template <typename Storage>
constexpr typename util::flags::BitMask<Storage>::Mask_t
util::flags::BitMask<Storage>::unsetMask(Mask_t baseMask, Bits_t bits) {
  return Mask_t(fromValues,
    baseMask.presence.combine(bits),
    baseMask.values.exclude(bits)
    );
} // util::flags::BitMask<>::unsetMask(Mask_t)


//------------------------------------------------------------------------------
template <typename Storage>
constexpr typename util::flags::BitMask<Storage>::Mask_t
util::flags::BitMask<Storage>::unsetMask
  (Mask_t baseMask, Flag_t flags)
  { return unsetMask(baseMask, Bits_t(flags)); }


//------------------------------------------------------------------------------
template <typename Storage>
constexpr typename util::flags::BitMask<Storage>::Mask_t
util::flags::BitMask<Storage>::negateMask(Mask_t mask) {
  return Mask_t
    (fromValues, mask.presence, mask.presence.select(mask.values.invert()));
}

//------------------------------------------------------------------------------
template <typename Storage>
constexpr typename util::flags::BitMask<Storage>::Mask_t
util::flags::BitMask<Storage>::negateMask(Bits_t bits)
  { return Mask_t(fromValues, bits, Bits_t(0)); }

template <typename Storage>
constexpr typename util::flags::BitMask<Storage>::Mask_t
util::flags::BitMask<Storage>::negateMask(Flag_t flags)
  { return negateMask(Bits_t(flags)); }


//------------------------------------------------------------------------------
//--- free functions and operators
//---
template <typename Storage>
constexpr util::flags::BitMask<Storage> util::flags::makeMask
  (Bits_t<Storage> bits)
  { return BitMask<Storage>(BitMask<Storage>::fromValues, bits); }


//------------------------------------------------------------------------------
template <typename Storage>
constexpr util::flags::BitMask<Storage> util::flags::operator|
  (BitMask<Storage> left, BitMask<Storage> right)
  { return BitMask<Storage>::combineWithMask(left, right); }

template <typename Storage>
constexpr util::flags::BitMask<Storage> util::flags::operator|
  (BitMask<Storage> left, typename BitMask<Storage>::Bits_t right)
  { return BitMask<Storage>::combineWithMask(left, right); }

template <typename Storage>
constexpr util::flags::BitMask<Storage> util::flags::operator|
  (typename BitMask<Storage>::Bits_t left, BitMask<Storage> right)
  { return BitMask<Storage>::combineWithMask(right, left); }


//------------------------------------------------------------------------------
template <typename Storage>
constexpr util::flags::BitMask<Storage> util::flags::operator&
  (BitMask<Storage> left, BitMask<Storage> right)
  { return BitMask<Storage>::intersectWithMask(left, right); }

template <typename Storage>
constexpr util::flags::BitMask<Storage> util::flags::operator&
  (BitMask<Storage> left, typename BitMask<Storage>::Bits_t right)
  { return BitMask<Storage>::intersectWithMask(left, right); }

template <typename Storage>
constexpr util::flags::BitMask<Storage> util::flags::operator&
  (typename BitMask<Storage>::Bits_t left, BitMask<Storage> right)
  { return makeMask(left) & right; }
  

template <typename Storage>
constexpr util::flags::BitMask<Storage> util::flags::operator&
  (Bits_t<Storage> left, Bits_t<Storage> right)
  { return BitMask<Storage>(left) & right; }

//------------------------------------------------------------------------------
template <typename Storage>
constexpr util::flags::BitMask<Storage> util::flags::operator+
  (BitMask<Storage> baseMask, BitMask<Storage> mask)
  { return BitMask<Storage>::mergeIntoMask(baseMask, mask); }

template <typename Storage>
constexpr util::flags::BitMask<Storage> util::flags::operator+
  (BitMask<Storage> baseMask, typename BitMask<Storage>::Bits_t bits)
  { return BitMask<Storage>::mergeIntoMask(baseMask, bits); }

template <typename Storage>
constexpr util::flags::BitMask<Storage> util::flags::operator+
  (typename BitMask<Storage>::Bits_t baseBits, BitMask<Storage> mask)
  { return makeMask(baseBits) + mask; }

//------------------------------------------------------------------------------
template <typename Storage>
constexpr util::flags::BitMask<Storage> util::flags::operator-
  (BitMask<Storage> baseMask, BitMask<Storage> mask)
  { return BitMask<Storage>::unsetMask(baseMask, mask); }

template <typename Storage>
constexpr util::flags::BitMask<Storage> util::flags::operator-
  (BitMask<Storage> baseMask, typename BitMask<Storage>::Bits_t bits)
  { return BitMask<Storage>::unsetMask(baseMask, bits); }

template <typename Storage>
constexpr util::flags::BitMask<Storage> util::flags::operator-
  (typename BitMask<Storage>::Bits_t baseBits, BitMask<Storage> mask)
  { return makeMask(baseBits) - mask; }

template <typename Storage>
constexpr util::flags::BitMask<Storage> util::flags::operator-
  (Bits_t<Storage> baseBits, Bits_t<Storage> bits)
  { return makeMask(baseBits) - bits; }

//------------------------------------------------------------------------------
template <typename Storage>
constexpr util::flags::BitMask<Storage> util::flags::operator+
  (BitMask<Storage> mask)
  { return mask; }

template <typename Storage>
constexpr util::flags::BitMask<Storage> util::flags::operator+
  (Bits_t<Storage> bits)
  { return makeMask(bits); }

//------------------------------------------------------------------------------
template <typename Storage>
constexpr util::flags::BitMask<Storage> util::flags::operator-
  (Bits_t<Storage> bits)
  { return BitMask<Storage>::negateMask(makeMask(bits)); }

template <typename Storage>
constexpr util::flags::BitMask<Storage> util::flags::operator-
  (Flag_t<Storage> flag)
  { return -(Bits_t<Storage>(flag)); }

//------------------------------------------------------------------------------
template <typename Storage>
constexpr util::flags::BitMask<Storage> util::flags::operator~
  (BitMask<Storage> mask)
  { return BitMask<Storage>::negateMask(mask); }

/*
template <typename Storage>
constexpr BitMask<Storage> operator~ (typename Bits_t<Storage> bits)
  { return ~BitMask<Storage>(bits); }
*/

//------------------------------------------------------------------------------
template <typename Storage>
constexpr util::flags::BitMask<Storage> util::flags::Set(Flag_t<Storage> flag)
  { return BitMask<Storage>(BitMask<Storage>::fromValues, flag, flag); }

template <typename Storage>
constexpr util::flags::BitMask<Storage> util::flags::Unset(Flag_t<Storage> flag)
{ 
  return BitMask<Storage>
    (BitMask<Storage>::fromValues, flag, Bits_t<Storage>(0)); 
}

//------------------------------------------------------------------------------



#endif // LARDATAOBJ_UTILITIES_BITMASK_TCC
