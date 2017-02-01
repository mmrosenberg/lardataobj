/**
 * @file   FlagSet.h
 * @brief  Class holding flags.
 * @author Gianluca Petrillo (petrillo@fnal.gov)
 * @date   January 25, 2017
 * @see    BitMask.h
 * 
 */

#ifndef LARDATAOBJ_UTILITIES_FLAGSET_H
#define LARDATAOBJ_UTILITIES_FLAGSET_H

// LArSoft headers

#include "lardataobj/Utilities/BitMask.h"

// C/C++ standard library
#include <string>
#include <exception>

namespace util {
  
  namespace flags {
    
#if 0
    /** ************************************************************************
     * @brief A class containing a set of flags.
     * @tparam NFlags number of flags to be allocated
     * @tparam Storage underlying integral type whose bits represent the flags
     * 
     * A FlagSet contains a set of flags. Each flag can be in one of two
     * states ("set" and "unset"), or can be not defined at all ("undefined").
     * 
     * Note that the object might have a `capacity()` larger than just `NFlags`.
     * The flags after the first `NFlags` are "unsupported", in the sense that
     * in future implementations they might disappear. For the rest, they behave
     * just like the other flags though.
     */
    template
      <unsigned int NFlags, typename Storage = details::smallestUInt_t<NFlags>>
    class FlagSet {
      
      static_assert(details::computePages<Storage>(NFlags) <= 1,
        "Too many flags for this storage type.");
      
        public:
      using This_t = FlagSet<NFlags, Storage>; ///< Type of this class.
      
      using Mask_t = BitMask<Storage>; ///< Type of bit mask for this flag set.
      
      /// Type of bits for this flag set.
      using Bits_t = typename Mask_t::Bits_t;
      
      /// Type of index of flag.
      using FlagIndex_t = typename Mask_t::FlagIndex_t;
      
      /// Type identifying a single flag.
      using Flag_t = typename Mask_t::Flag_t;
      
      
      /// @{ 
      /// @name Exceptions
      
      /// Generic BitMask exception.
      using Exception = typename Mask_t::Exception;
      
      /// Out-of-range flag index.
      using OutOfRangeError = typename Mask_t::OutOfRangeError;
      
      /// Flag not defined.
      using FlagNotDefinedError = typename Mask_t::FlagNotDefinedError;
      
      /// @}
      
      
      /// Default constructor: no flag defined at all.
      explicit constexpr FlagSet() = default;
      
      
      /**
       * @brief Constructor: merges all arguments in the argument list.
       * @tparam Others type of the remaining parameters to be merged
       * @param first first argument to be merged (here, a Flag_t)
       * @param others remaining arguments to be merged
       * @see createMask()
       * 
       * The effect is equivalent to call `createMask(first, others...)`.
       */
      template <typename... Others>
      explicit constexpr FlagSet(Flag_t flag, Others... others)
        : data(flag, others...)
        {}
      
      /**
       * @brief Constructor: merges all arguments in the argument list.
       * @tparam Others type of the remaining parameters to be merged
       * @param first first argument to be merged (here, a Mask_t)
       * @param others remaining arguments to be merged
       * @see createMask()
       * 
       * The effect is equivalent to call `createMask(first, others...)`.
       */
      template <typename... Others>
      explicit constexpr FlagSet(Mask_t mask, Others... others)
        : data(mask, others...)
        {}
      
      
      /// @{
      /// @name Access to flags
      
      /// Returns all the flags in the form of a mask.
      constexpr Mask_t mask() const { return data; }
      
      /**
       * @brief Returns whether the flag index is valid.
       * @param flagIndex index of the flag to check
       * @return whether the flag index is valid
       * 
       * Valid flag index values go from 0 up to the size (`size()`) of this
       * flag set.
       */
      constexpr bool isFlag(FlagIndex_t flagIndex) const;
      
      /**
       * @brief Returns whether the flag index is valid.
       * @param flag flag to check
       * @return whether the flag is valid
       * 
       * Valid flag index values go from 0 up to the size (`size()`) of this
       * flag set.
       * 
       * @note This method is unable to check flags that are beyond the capacity
       *       of the flag set itself. For example, in a flag set represented
       *       with 8 bits, the Flag_t value corresponding to flag #12 would be
       *       `Flag_t(0x1000)`, which is beyond the range that `Flag_t` can
       *       represent.
       */
      constexpr bool isFlag(Flag_t flag) const;
      
      
      /**
       * @brief Returns whether the specified flag is defined.
       * @param flag flag to be checked
       * @return whether the specified flag is defined
       * @see isDefined(Bits_t)
       * 
       * Each flag must be defined before it can be used. A common convention is
       * to use the "undefined" state of the flag to denote that its value is 
       * currently unknown.
       * 
       * A flag is defined by setting (`set()`) or unsetting (`unset()`) it.
       * 
       * The access is unchecked, and if the flag is not valid (`isFlag()`) the
       * behaviour is not defined.
       */
      constexpr bool isDefined(Flag_t flag) const
        { return mask().isDefined(flag); }
      
      /**
       * @brief Returns whether all specified bits are defined.
       * @param bits bits to check
       * @return whether all bits set in bits are defined
       * @see isDefined(Flag_t)
       * 
       * This method checks if all bits that are set in the argument are
       * defined.
       * The return value will be true only if all of them are defined.
       * 
       * Each flag must be defined before it can be used. A common convention is
       * to use the "undefined" state of the flag to denote that its value is 
       * currently unknown.
       * 
       * A flag is defined by setting (`set()`) or unsetting (`unset()`) it.
       * 
       * The access is unchecked, and if the flag is not valid (`isFlag()`) the
       * behaviour is not defined.
       */
      constexpr bool isDefined(Bits_t bits) const
        { return mask().isDefined(bits); }
      
      /**
       * @brief Returns whether the specified flag is undefined.
       * @param flag flag to be checked
       * @return whether the specified flag is undefined
       * @see isDefined(Bits_t), isUndefined(Flag_t)
       * 
       * This is exactly the negation of `isDefined()`.
       */
      constexpr bool isUndefined(Flag_t flag) const
        { return mask().isUndefined(flag); }
      
      /**
       * @brief Returns whether all specified bits are undefined.
       * @param bits bits to check
       * @return whether all bits set in bits are undefined
       * @see isDefined(Bits_t), isUndefined(Flag_t)
       * 
       * This is exactly the negation of `isDefined()`.
       */
      constexpr bool isUndefined(Bits_t bits) const
        { return mask().isUndefined(bits); }
      
      /**
       * @brief Returns if the specified flag is on ("set").
       * @param flag index of the flag to test
       * @return whether the specified flag is set
       * @see test()
       * 
       * This method provides an unchecked access to a single flag value.
       * If the flag is `isUndefined()`, the behaviour of this method is also
       * undefined.
       * 
       * See `test()` for a checked access.
       */
      constexpr bool get(Flag_t flag) const
        { return mask().get(flag); }
      
      /**
       * @brief Returns if the specified flag is set.
       * @param flag index of the flag to test
       * @return whether the specified flag is set
       * @throw FlagNotDefinedError if the flag was not defined at all
       * @throw OutOfRangeError if the flag index denotes a non-existing flag
       * 
       * This method provides a checked access to a single flag value.
       * If the flag is not supported (see `isFlag()`), or if it is
       * `isUndefined()`, an exception is thrown.
       * 
       * See `get()` for an unchecked access.
       */
      bool test(Flag_t flag) const;
      
      /**
       * @brief Returns if the specified flag is set.
       * @param flag index of the flag to test
       * @return whether the specified flag is set
       * 
       * This method provides an unchecked access to a single flag value.
       * If the flag is `isUndefined()`, the behaviour of this method is also
       * undefined.
       */
      constexpr bool isSet(Flag_t flag) const
        { return mask().isSet(flag); }
      
      /**
       * @brief Returns if the specified flag is unset.
       * @param flag index of the flag to test
       * @return whether the specified flag is unset
       * 
       * This method provides an unchecked access to a single flag value.
       * If the flag is `isUndefined()`, the behaviour of this method is also
       * undefined.
       */
      constexpr bool isUnset(Flag_t flag) const
        { return mask().isUnset(flag); }
      
      /**
       * @brief Returns whether all the specified flags are set.
       * @param bits bits to check
       * @return whether all bits are set
       * @see isSet(Flag_t)
       * 
       * This method is equivalent to calling `isSet(Flag_t)` on each single
       * flag defined in `bits`. The result is true only if all of them are set,
       * that includes all of them being defined.
       */
      constexpr bool all(Bits_t bits) const
        { return mask().all(bits); }
      
      /**
       * @brief Returns whether at least one of the specified bits is set.
       * @param bits bits to check
       * @return whether any of the bits is set
       * @see isSet(Flag_t)
       * 
       * This method is equivalent to calling `isSet(Flag_t)` on each single
       * flag defined in `bits`. The result is true only if at least one of them
       * is set (including that one being defined).
       */
      constexpr bool any(Bits_t bits) const
        { return mask().any(bits); }
      
      /**
       * @brief Returns whether all the specified bits are unset.
       * @param bits bits to check
       * @return whether all bits are unset
       * @see isUnset(Flag_t)
       * 
       * This method is equivalent to calling `isUnset(Flag_t)` on each single
       * flag defined in `bits`. The result is true only if all of them are
       * unset, that includes all of them being defined.
       */
      constexpr bool none(Bits_t bits) const
        { return mask().none(bits); }
      
      /// @}
      
      
      /// @{
      /// @name Setting flags
      
      /**
       * @brief Sets all specified flags
       * @tparam Flag types of the first flag
       * @tparam OtherFlags types of other optional flags
       * @param flag the first flag to be set
       * @param others flags also to be set
       * 
       * All specified flags are set. Flags set are automatically defined.
       * 
       * Each argument can be of one of the following supported types:
       *  * `Flag_t`: single flag (a FlagIndex_t can be implicitly converted to
       *      this one, too)
       */
      template <typename Flag, typename... OtherFlags>
      void set(Flag first, OtherFlags... others)
        { return mask().set(first, others...); }
      
      /**
       * @brief Sets all flags specified by the index iterator range
       * @tparam BeginIter type of iterator to the first of the flags
       * @tparam EndIter type of end iterator
       * @param begin iterator to the index of the first flag
       * @param end iterator past the index of the last flag
       * @see set()
       * 
       * Each flag is set as if `set(flag)` were called.
       */
      template <typename BeginIter, typename EndIter>
      void rangeSet(BeginIter begin, EndIter end)
        { return mask().rangeSet(begin, end); }
      
      /**
       * @brief Unsets all specified flags
       * @tparam Flag types of the first flag
       * @tparam OtherFlags types of other optional flags
       * @param flag the first flag to be unset
       * @param others flags also to be unset
       * @see set()
       * 
       * All specified flags are unset. Flags unset are automatically defined.
       * The types accepted are the same as for `set()`.
       */
      template <typename Flag, typename... OtherFlags>
      void unset(Flag first, OtherFlags... others)
        { return mask().unset(first, others...); }
      
      /**
       * @brief Unsets all flags specified by the index iterator range
       * @tparam BeginIter type of iterator to the first of the flags
       * @tparam EndIter type of end iterator
       * @param begin iterator to the index of the first flag
       * @param end iterator past the index of the last flag
       * @see unset()
       * 
       * Each flag is unset as if `unset(flag)` were called.
       */
      template <typename BeginIter, typename EndIter>
      void rangeUnset(BeginIter begin, EndIter end)
        { return mask().rangeUnset(begin, end); }
      
      
      /**
       * @brief Declares all specified flags as undefined.
       * @tparam Flag types of the first flag
       * @tparam OtherFlags types of other optional flags
       * @param flag the first flag to be removed
       * @param others flags also to be removed
       * @see set(), unset(), isDefined()
       * 
       * All specified flags are marked back as undefined.
       * Already undefined flags are not affected.
       * The value of an undefined flag is... well, undefined.
       * 
       * There is no equivalent direct way to just define a flag, but rather
       * when a flag is assigned a value the first time, that flag is at the
       * same time defined.
       * 
       * Each argument can be of one of the following supported types:
       *  * `Flag_t`: single flag (a FlagIndex_t can be implicitly converted to
       *      this one, too)
       */
      template <typename Flag, typename... OtherFlags>
      void remove(Flag first, OtherFlags... others)
        { return mask().remove(first, others...); }
      
      /// @}
      
      
      /// @{
      /// @name Number of flags
      
      /// Returns the number of flags the set supports.
      static constexpr size_t size()
        { return NFlags; }
      
      /// Returns the number of flags the set has room for.
      static constexpr size_t capacity()
        { return Mask_t::capacity(); }
      
      /// @}
      
      /// Comparison: all flags must be the same
      /// @bug Also the value of undefined flags is currently checked
      constexpr bool operator== (Mask_t const& other) const
        { return mask() == other.mask(); }
      
      /// Comparison: not all flags must be the same
      /// @bug Also the value of undefined flags is currently checked
      constexpr bool operator!= (Mask_t const& other) const
        { return mask() != other.mask(); }
      
      
      /// Dumps on screen only the "official" flags (see `size()`).
      template <typename Stream>
      void dump(Stream&& out) const
        { mask().dump(std::forward<Stream>(out), size()); }
      
      
      /**
       * @brief Creates a new BitMask.
       * @tparam Args types of the arguments
       * @param args the data to create the mask from
       * @return a BitMask with the features specified in the arguments
       * 
       * If no argument is provided, the mask is returned with all the flags
       * undefined (like in default constructor).
       * For the details, see `BitMask<Storage>::createMask()`.
       * 
       */
      template <typename... Args>
      static constexpr Mask_t createMask(Args... args)
        { return Mask_t::create(args...); }
      
        private:
      
      Mask_t data; ///< Storage of the flag bits.
      
      /// Access to the data.
      Mask_t& mask() { return data; }
      
    }; // class FlagSet<>
    
    
#endif // 0
    /** ************************************************************************
     * @brief A class containing a set of flags.
     * @tparam NFlags number of flags to be allocated
     * @tparam Storage underlying integral type whose bits represent the flags
     * 
     * A FlagSet contains a set of flags. Each flag can be in one of two
     * states ("set" and "unset"), or can be not defined at all ("undefined").
     * 
     * Note that the object might have a `capacity()` larger than just `NFlags`.
     * The flags after the first `NFlags` are "unsupported", in the sense that
     * in future implementations they might disappear. For the rest, they behave
     * just like the other flags though.
     */
    template
      <unsigned int NFlags, typename Storage = details::smallestUInt_t<NFlags>>
    class FlagSet: public BitMask<Storage> {
      
      static_assert(details::computePages<Storage>(NFlags) <= 1,
        "Too many flags for this storage type.");
      
        public:
      using This_t = FlagSet<NFlags, Storage>; ///< Type of this class.
      
      using Mask_t = BitMask<Storage>; ///< Type of bit mask for this flag set.
      
      using Base_t = Mask_t; ///< Type of the base class.
      
      /// Type of bits for this flag set.
      using Bits_t = typename Mask_t::Bits_t;
      
      /// Type of index of flag.
      using FlagIndex_t = typename Mask_t::FlagIndex_t;
      
      /// Type identifying a single flag.
      using Flag_t = typename Mask_t::Flag_t;
      
      
      /// @{ 
      /// @name Exceptions
      
      /// Generic BitMask exception.
      using Exception = typename Mask_t::Exception;
      
      /// Out-of-range flag index.
      using OutOfRangeError = typename Mask_t::OutOfRangeError;
      
      /// Flag not defined.
      using FlagNotDefinedError = typename Mask_t::FlagNotDefinedError;
      
      /// @}
      
      
      using Base_t::Base_t; // inherit constructors
      
      
      /// @{
      /// @name Access to flags
      
      /// Returns all the flags in the form of a mask.
      constexpr Mask_t const& mask() const { return *this; }
      
      /**
       * @brief Returns whether the flag index is valid.
       * @param flagIndex index of the flag to check
       * @return whether the flag index is valid
       * 
       * Valid flag index values go from 0 up to the size (`size()`) of this
       * flag set.
       */
      constexpr bool isFlag(FlagIndex_t flagIndex) const;
      
      /**
       * @brief Returns whether the flag index is valid.
       * @param flag flag to check
       * @return whether the flag is valid
       * 
       * Valid flag index values go from 0 up to the size (`size()`) of this
       * flag set.
       * 
       * @note This method is unable to check flags that are beyond the capacity
       *       of the flag set itself. For example, in a flag set represented
       *       with 8 bits, the Flag_t value corresponding to flag #12 would be
       *       `Flag_t(0x1000)`, which is beyond the range that `Flag_t` can
       *       represent.
       */
      constexpr bool isFlag(Flag_t flag) const;
      
      
      /**
       * @brief Returns if the specified flag is set.
       * @param flag index of the flag to test
       * @return whether the specified flag is set
       * @throw FlagNotDefinedError if the flag was not defined at all
       * @throw OutOfRangeError if the flag index denotes a non-existing flag
       * 
       * This method provides a checked access to a single flag value.
       * If the flag is not supported (see `isFlag()`), or if it is
       * `isUndefined()`, an exception is thrown.
       * 
       * See `get()` for an unchecked access.
       */
      bool test(Flag_t flag) const;
      
      /// @}
      
      
      /// Dumps on screen only the "official" flags (see `size()`).
      template <typename Stream>
      void dump(Stream&& out) const
        { mask().dump(std::forward<Stream>(out), size()); }
      
      
      /// Returns the number of flags the set supports.
      static constexpr size_t size()
        { return NFlags; }
      
      
      /**
       * @brief Creates a new BitMask.
       * @tparam Args types of the arguments
       * @param args the data to create the mask from
       * @return a BitMask with the features specified in the arguments
       * 
       * If no argument is provided, the mask is returned with all the flags
       * undefined (like in default constructor).
       * For the details, see `BitMask<Storage>::createMask()`.
       * 
       */
      template <typename... Args>
      static constexpr Mask_t createMask(Args... args)
        { return Mask_t::create(args...); }
      
        private:
      
      
    }; // class FlagSet<>
    
    
    /// Output of a flag set into a stream.
    template <typename Stream, unsigned int NBits, typename Storage>
    Stream& operator<< (Stream&& out, FlagSet<NBits, Storage> const& flags)
      { flags.dump(std::forward<Stream>(out)); return out; }
    
  } // namespace flags
  
} // namespace util


//------------------------------------------------------------------------------
//---  template implementation
//---  

#include "FlagSet.tcc"

//------------------------------------------------------------------------------


#endif // LARDATAOBJ_UTILITIES_FLAGSET_H
