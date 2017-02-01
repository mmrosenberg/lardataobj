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
       * @param flag flag to test
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
       * @param index index of the flag to test
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
      bool test(FlagIndex_t flagIndex) const;
      
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
      
      /// Implementation detail of test()
      bool testImpl(Flag_t flag) const;
      
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
