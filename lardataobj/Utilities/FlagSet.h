/**
 * @file   FlagSet.h
 * @brief  Class holding flags.
 * @author Gianluca Petrillo (petrillo@fnal.gov)
 * @date   January 25, 2017
 * 
 */

#ifndef LARDATAOBJ_UTILITIES_FLAGSET_H
#define LARDATAOBJ_UTILITIES_FLAGSET_H


// C/C++ standard library
#include <string>
#include <exception>

namespace util {
  
  /// Type to denote the index of the flag.
  using FlagIndex_t = unsigned int;
  
  namespace details {
    
    /// Trait containing the smallest integral type accommodating `NBits` bits.
    template <unsigned int NBits>
    struct SmallestUIntType;
    
    /// The smallest integral type accommodating `NBits` bits.
    template <unsigned int NBits>
    using smallestUInt_t = typename SmallestUIntType<NBits>::type;
    
    /// Returns the number of Storage variables needed to hold that many bits
    template <typename Storage>
    constexpr unsigned int computePages(unsigned int bits);
    
    /// Returns a set of bits with only the one at the specified index set.
    template <typename Storage>
    constexpr Storage makeBits(FlagIndex_t index)
      { return Storage(1) << index; }
    
  } // namespace details
  
  
  
  /// Type identifying a flag. Operations are implemented as free functions.
  template <typename Storage>
  struct Flag_t {
    using Storage_t = Storage;
    
    Storage_t bits; ///< The bits representing this flag (only one is set)
    
    /// Constructs from the flag index.
    constexpr Flag_t(FlagIndex_t flagIndex)
      : bits(details::makeBits<Storage_t>(flagIndex))
      {}
    
    /// Returns the index of the (first) bit set.
    constexpr FlagIndex_t index() const;
    
    /// @{
    /// @name Comparison operators (based on the index)
    constexpr bool operator== (Flag_t other) const
      { return bits == other.bits; }
    constexpr bool operator!= (Flag_t other) const
      { return bits != other.bits; }
    constexpr bool operator< (Flag_t other) const
      { return bits < other.bits; }
    /// @}
    
  }; // Flag_t
  
  
  /**
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
  template <
    unsigned int NFlags,
    typename Storage = details::smallestUInt_t<NFlags>
    >
  class FlagSet {
    
    using Storage_t = Storage;
    
    // This will stay here until we really support multiple flag "pages"
    static_assert(details::computePages<Storage>(NFlags) <= 1,
      "Too many flags for this storage type.");
    
    using Bits_t = Storage_t; ///< Set of bits.
    
      public:
    
    using FlagSet_t = FlagSet<NFlags, Storage>; ///< This type.
    
    using FlagIndex_t = util::FlagIndex_t; ///< Type of index of flag.
    
    using Flag_t = util::Flag_t<Storage_t>; ///< Type identifying a single flag.
    
    /// Default constructor: no flag defined at all.
    explicit FlagSet() = default;
    
    
    /// @{
    /// @name Access to flags
    
    /**
     * @brief Returns whether the flag index is valid.
     * @param flag index of the flag to check
     * @return whether the flag index is valid
     * 
     * Valid flag index values go from 0 up to the size (`size()`) of this flag
     * set.
     */
    constexpr bool isFlag(Flag_t) const;
    
    /**
     * @brief Returns whether the flag is defined.
     * @param flag index of the flag to check
     * @return whether the flag is defined
     * 
     * A flag must be defined before it can be used. A common convention is to
     * use the "undefined" state of the flag to denote that its value is 
     * currently unknown.
     * 
     * A flag is defined by setting (`set()`) or unsetting (`unset()`) it.
     * 
     * The access is unchecked, and if the flag is not valid (`isFlag()`) the
     * behaviour is not defined.
     */
    constexpr bool isDefined(Flag_t flag) const;
    
    /**
     * @brief Returns whether the flag is undefined.
     * @param flag index of the flag to check
     * @return whether the flag is undefined
     * @see isUndefined()
     * 
     * This is exactly the negation of `isDefined()`.
     */
    constexpr bool isUndefined(Flag_t flag) const;
    
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
    constexpr bool get(Flag_t flag) const;
    
    /**
     * @brief Returns if the specified flag is set.
     * @param flag index of the flag to test
     * @return whether the specified flag is set
     * 
     * This method provides an unchecked access to a single flag value.
     * If the flag is `isUndefined()`, the behaviour of this method is also
     * undefined.
     */
    constexpr bool isSet(Flag_t flag) const;
    
    /**
     * @brief Returns if the specified flag is unset.
     * @param flag index of the flag to test
     * @return whether the specified flag is unset
     * 
     * This method provides an unchecked access to a single flag value.
     * If the flag is `isUndefined()`, the behaviour of this method is also
     * undefined.
     */
    constexpr bool isUnset(Flag_t flag) const;
    
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
      { setImpl(first, others...); }
    
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
    void rangeSet(BeginIter begin, EndIter end);
    
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
      { unsetImpl(first, others...); }
    
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
    void rangeUnset(BeginIter begin, EndIter end);
    
    
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
     * There is no equivalent direct way to just define a flag, but rather when
     * a flag is assigned a value the first time, that flag is at the same time
     * defined.
     * 
     * Each argument can be of one of the following supported types:
     *  * `Flag_t`: single flag (a FlagIndex_t can be implicitly converted to
     *      this one, too)
     */
    template <typename Flag, typename... OtherFlags>
    void remove(Flag first, OtherFlags... others)
      { undefineImpl(first, others...); }
    
    /// @}
    
    
    /// @{
    /// @name Number of flags
    
    /// Returns the number of flags the set supports.
    static constexpr size_t size()
      { return NFlags; }
    
    /// Returns the number of flags the set has room for.
    static constexpr size_t capacity();
    
    /// @}
    
    /// Comparison: all flags must be the same
    /// @bug Also the value of undefined flags is currently checked
    constexpr bool operator== (FlagSet_t const& other) const
      { return (values == other.values) && (presence == other.presence); }
    
    /// Comparison: not all flags must be the same
    /// @bug Also the value of undefined flags is currently checked
    constexpr bool operator!= (FlagSet_t const& other) const
      { return (values != other.values) || (presence != other.presence); }
    
    
    /// @{
    /// @name Exceptions
    
    struct Exception: public std::exception {
      explicit Exception(std::string msg = "FlagSet error"): message(msg) {}
      virtual const char* what() const noexcept override
        { return message.c_str(); }
      std::string message;
    }; // Exception
    
    /// Exception thrown to convey that an invalid flag index was used
    struct OutOfRangeError: public Exception {
      explicit OutOfRangeError(std::string msg = "FlagSet out-of-range error")
        : Exception("FlagSet out-of-range error" + msg)
        {}
    }; // OutOfRangeError
    
    /// Exception thrown to convey that an undefined flag index was tested
    struct FlagNotDefinedError: public Exception {
      explicit FlagNotDefinedError
        (std::string msg = "FlagSet undefined flag error")
        : Exception(msg)
        {}
    }; // FlagNotDefinedError
    
    /// @}
    
      private:
    using PageMask_t = Storage_t; ///< Type used for masks within one page.
    
    Storage_t values   = { 0 }; ///< Storage of value bits.
    Storage_t presence = { 0 }; ///< Storage of definition bits.
    
    // Storage details
    // We store values and definition information in two different bit buckets;
    // the same Flag_t pattern is good for both.
    
    /// Marks a flag as defined. Value is still uninitialised!
    void defineSingle(Flag_t flag);
    
    /// Marks a flag as undefined.
    void undefineSingle(Flag_t flag);
    
    /// Implementation detail of remove()
    void undefineImpl() {}
    
    /// Implementation detail of remove()
    template <typename... Flags>
    void undefineImpl(Flag_t flag, Flags... others);
    
    /// Implementation detail of set()
    void setImpl() {}
    
    /// Implementation detail of set()
    template <typename... Flags>
    void setImpl(Flag_t flag, Flags... others);
    
    /// Implementation detail of set()
    void setSingle(Flag_t flag);
    
    /// Implementation detail of unset()
    void unsetImpl() {}
    
    /// Implementation detail of unset()
    template <typename... Flags>
    void unsetImpl(Flag_t flag, Flags... others);
    
    /// Implementation detail of unset()
    void unsetSingle(Flag_t flag);
    
    /// Sets the specified bits in data.
    static constexpr bool testBits(Storage_t data, Storage_t bits)
      { return data & bits; }
    
    /// Sets the specified bits in data.
    static void setBits(Storage_t& data, Storage_t bits)
      { data |= bits; }
    
    /// Unsets the specified bits in data.
    static void unsetBits(Storage_t& data, Storage_t bits)
      { data &= ~bits; }
    
  }; // FlagSet<>
  
  
  /// Output of a flag set into a stream.
  template <typename Stream, unsigned int NBits, typename Storage>
  Stream& operator<< (Stream&& out, FlagSet<NBits, Storage> const& flag);
  
  /// Output of a flag into a stream (prints its index).
  template <typename Stream, typename Storage>
  Stream& operator<< (Stream&& out, Flag_t<Storage> flag)
    { out << '[' << flag.index() << ']'; return out; }
  
  /// Convert a flag into a stream (shows its index).
  template <typename Storage>
  std::string to_string(Flag_t<Storage> const flag)
    { return std::to_string(flag.index()); }
  
} // namespace util


//------------------------------------------------------------------------------
//---  template implementation
//---  

#include "FlagSet.tcc"

//------------------------------------------------------------------------------


#endif // LARDATAOBJ_UTILITIES_FLAGSET_H
