/**
 * @file   BitMask.h
 * @brief  Class holding flags.
 * @author Gianluca Petrillo (petrillo@fnal.gov)
 * @date   January 25, 2017
 * @see    FlagSet.h
 * 
 * This pure header defines the template class `util::flags::BitMask`, which is
 * used in `util::flags::FlagSet`, and a few auxiliary classes and types
 * (`util::flags::Index_t`, `util::flags::Flag_t`).
 * 
 */

#ifndef LARDATAOBJ_UTILITIES_BITMASK_H
#define LARDATAOBJ_UTILITIES_BITMASK_H


// C/C++ standard library
#include <string>
#include <exception>

namespace util {
  
  /**
   * @brief Classes and functions to manage bit masks and flags.
   * 
   * In this namespace a "hierarchy" of classes are defined:
   * 
   * * `Flag_t`: a single bit; it can be initialized with the bit index, but
   *     it is stored as a bit mask. A flag has two states: set and unset.
   * * `Bits_t`: a set of flags (represented as a bit mask).
   * * `Mask_t`: a set of flags which can have one of three states each:
   *     set, unset or undefined.
   * * `FlagSet`: a set of flags (like Mask_t), with the knowledge of a total
   *     number of supported flags.
   * 
   * The first three classes manage exactly how many bits they have storage for.
   * `FlagSet` instead has also the notion of how many flags are actually
   * supported.
   * A number of flag-wise operations are defined. The result of an operation
   * between two classes might be a class of a superior level in the hierarchy.
   * In this sense, the lower objects should be intended as shortcut
   * representations of `Bits_t`.
   * More specifically:
   * 
   * * a `Bits_t` can be created out of a `Flag_t`
   * * a `Mask_t` can be created out of a `Bits_t` (or a `Flag_t`)
   * * `~Flag_t` is not defined
   * * `Bits_t | Bits_t` and `Bits_t + Bits_t` are still `Bits_t` (and they are
   *     equivalent)
   * * `Bits_t & Bits_t` and `Bits_t - Bits_t` are `Mask_t`, to preserve the
   *     information of bits that are defined and unset
   * * `~Bits_t` is a `Mask_t` as well
   * 
   */
  namespace flags {
  
    /// Type to denote the index of the flag.
    using Index_t = unsigned int;
    
    namespace details {
      
      /// Trait containing the smallest integral type accommodating `NBits`
      /// bits.
      template <unsigned int NBits>
      struct SmallestUIntType;
      
      /// The smallest integral type accommodating `NBits` bits.
      template <unsigned int NBits>
      using smallestUInt_t = typename SmallestUIntType<NBits>::type;
      
      /// Returns the number of Storage variables needed to hold that many bits.
      template <typename Storage>
      constexpr unsigned int computePages(unsigned int bits);
      
      /// Returns a set of bits with only the one at the specified index set.
      template <typename Storage>
      constexpr Storage makeBits(Index_t index)
        { return Storage(1) << index; }
      
    } // namespace details
    
    
    
    //--------------------------------------------------------------------------
    /// Type identifying a flag. Operations are implemented as free functions.
    template <typename Storage>
    struct Flag_t {
      using Storage_t = Storage;
      using This_t = Flag_t<Storage>; ///< This type.
      
      Storage_t bits; ///< The bits representing this flag (only one is set)
      
      /// Constructs from the flag index.
      constexpr Flag_t(Index_t flagIndex)
        : bits(details::makeBits<Storage_t>(flagIndex))
        {}
      
      /// Returns the index of the (first) bit set.
      constexpr Index_t index() const;
      
      /**
       * @brief Returns a copy of this object.
       * @return a copy of this flag
       * 
       * When a flag is declared as `constexpr`, functions trying to take its
       * address will trigger a link failure (as there is no address for a
       * constexpr). Passing a copy of it should fix the problem.
       */
      constexpr Flag_t copy() const { return *this; }
      
      /// @{
      /// @name Comparison operators for flags
      
      constexpr bool operator== (This_t other) const
        { return bits == other.bits; }
      constexpr bool operator!= (This_t other) const
        { return bits != other.bits; }
      constexpr bool operator< (This_t other) const
        { return bits < other.bits; }
      constexpr bool operator> (This_t other) const
        { return bits > other.bits; }
      constexpr bool operator<= (This_t other) const
        { return bits <= other.bits; }
      constexpr bool operator>= (This_t other) const
        { return bits >= other.bits; }
      
      /// @}
      
    }; // Flag_t
    
    
    /// @{
    /// @name Comparison operators for flags (based on the index)
    template <typename Storage>
    constexpr bool operator== (Index_t left, Flag_t<Storage> right)
      { return left == right.index(); }
    template <typename Storage>
    constexpr bool operator== (Flag_t<Storage> left, Index_t right)
      { return left.index() == right; }
    
    template <typename Storage>
    constexpr bool operator!= (Index_t left, Flag_t<Storage> right)
      { return left != right.index(); }
    template <typename Storage>
    constexpr bool operator!= (Flag_t<Storage> left, Index_t right)
      { return left.index() != right; }
    
    template <typename Storage>
    constexpr bool operator< (Index_t left, Flag_t<Storage> right)
      { return left < right.index(); }
    template <typename Storage>
    constexpr bool operator< (Flag_t<Storage> left, Index_t right)
      { return left.index() < right; }
    
    template <typename Storage>
    constexpr bool operator> (Index_t left, Flag_t<Storage> right)
      { return left > right.index(); }
    template <typename Storage>
    constexpr bool operator> (Flag_t<Storage> left, Index_t right)
      { return left.index() > right; }
    
    template <typename Storage>
    constexpr bool operator<= (Index_t left, Flag_t<Storage> right)
      { return left <= right.index(); }
    template <typename Storage>
    constexpr bool operator<= (Flag_t<Storage> left, Index_t right)
      { return left.index() <= right; }
    
    template <typename Storage>
    constexpr bool operator>= (Index_t left, Flag_t<Storage> right)
      { return left >= right.index(); }
    template <typename Storage>
    constexpr bool operator>= (Flag_t<Storage> left, Index_t right)
      { return left.index() >= right; }
    
    /// @}
      
    /// Output of a flag into a stream (prints its index).
    template <typename Stream, typename Storage>
    Stream& operator<< (Stream&& out, Flag_t<Storage> flag)
      { out << '[' << flag.index() << ']'; return out; }
    
    /// Convert a flag into a stream (shows its index).
    template <typename Storage>
    std::string to_string(Flag_t<Storage> const flag)
      { return std::to_string(flag.index()); }

    
    //--------------------------------------------------------------------------
    /// Type identifying a set of bits.
    template <typename Storage>
    struct Bits_t {
      using Storage_t = Storage;
      using This_t = Bits_t<Storage>; ///< This type.
      
      /// Type of flag matching our storage.
      using Flag_t = util::flags::Flag_t<Storage_t>;
      
      Storage_t data = { 0 }; ///< The bits representing all set bits
      
      /// Default constructor: no bit set.
      constexpr Bits_t() = default;
      
      /// Constructs from a single flag.
      constexpr Bits_t(Flag_t flag)
        : data(flag.bits)
        {}
      
      /// Constructs from a set of bits.
      explicit constexpr Bits_t(Storage_t bits)
        : data(bits)
        {}
      
      /// @{
      /// @name Bit query operations
      
      /// Returns whether there is no bit set at all.
      constexpr bool empty() const
        { return data == Storage_t(0); }
      
      /// Returns wether all bits are set.
      constexpr bool all(This_t bits) const;
      
      /// Returns wether at least one of the bits is set.
      constexpr bool any(This_t bits) const;
      
      /// Returns wether all bits are unset.
      constexpr bool none(This_t bits) const;
      
      /// Returns wether no bits are set except (at most) the specified ones.
      constexpr bool only(This_t bits) const;
      
      /// @}
      
      
      /// @{
      /// @name Bit change operations
      
      /// Sets the specified bits.
      void set(This_t bits);
      
      /// Unsets the specified bits.
      void unset(This_t bits);
      
      /// Unsets the bits which are _not_ in `bits` argument.
      void keepOnly(This_t bits);
      
      /// Unsets all bits.
      void clear()
        { data = 0; }
      
      /// @}
      
      
      /// @{
      /// @name Bit manipulation operations returning a new object
      
      /// Returns only the bits that are also present in the argument.
      constexpr This_t select(This_t bits) const;
      
      /// Returns only the bits that are not present in the argument.
      constexpr This_t exclude(This_t bits) const;
      
      /// Returns our bits, plus the ones present in the argument.
      constexpr This_t combine(This_t bits) const;
      
      /// Returns all and only the bits that are not set.
      constexpr This_t invert() const;
      
      /// @}
      
      
      /// @{
      /// @name Comparison operators for bits
      
      constexpr bool operator== (This_t other) const
        { return data == other.data; }
      constexpr bool operator!= (This_t other) const
        { return data != other.data; }
      constexpr bool operator< (This_t other) const
        { return data < other.data; }
      constexpr bool operator> (This_t other) const
        { return data > other.data; }
      constexpr bool operator<= (This_t other) const
        { return data <= other.data; }
      constexpr bool operator>= (This_t other) const
        { return data >= other.data; }
      
      /// @}
      
      /// Returns true if there is at least one bit set.
      explicit constexpr operator bool() const { return bool(data); }
      
      /// Returns true if there is no bit set.
      constexpr bool operator!() const { return !data; }
      
      
      /// Returns data with all bits from base and from bits set.
      static void setBits(Storage_t& base, Storage_t bits)
        { base |= bits; }
      
      /// Returns data with all bits from base which are also in `bits`
      /// argument.
      static void onlyBits(Storage_t& base, Storage_t bits)
        { base &= bits; }
      
      /// Returns data with all bits from base, but the ones from bits unset.
      static void unsetBits(Storage_t& base, Storage_t bits)
        { onlyBits(base, ~bits); }
      
    }; // Bits_t
    
    
    /// @{
    /**
     * @name Flag and bit operations.
     * 
     * Any bitwise operation with a flag returns a `Bits_t`.
     * 
     */
    
    /// Returns a new Bits_t with all the bits from both arguments set.
    template <typename Storage>
    constexpr Bits_t<Storage> operator|
      (Bits_t<Storage> left, Bits_t<Storage> right);
    
    /// Returns a new Bits_t with all the bits from both arguments set.
    template <typename Storage>
    constexpr Bits_t<Storage> operator|
      (Bits_t<Storage> left, typename Bits_t<Storage>::Flag_t right);
    
    /// Returns a new Bits_t with all the bits from both arguments set.
    template <typename Storage>
    constexpr Bits_t<Storage> operator|
      (typename Bits_t<Storage>::Flag_t left, Bits_t<Storage> right);
    
    /// Returns a new Bits_t with all the bits from both arguments set.
    template <typename Storage>
    constexpr Bits_t<Storage> operator|
      (Flag_t<Storage> left, Flag_t<Storage> right);
    
    /// Returns a new Bits_t with all the bits from both arguments set.
    template <typename Storage>
    constexpr Bits_t<Storage> operator+
      (Bits_t<Storage> left, Bits_t<Storage> right);
    
    /// Returns a new Bits_t with all the bits from both arguments set.
    template <typename Storage>
    constexpr Bits_t<Storage> operator+
      (Bits_t<Storage> left, typename Bits_t<Storage>::Flag_t right);
    
    /// Returns a new Bits_t with all the bits from both arguments set.
    template <typename Storage>
    constexpr Bits_t<Storage> operator+
      (typename Bits_t<Storage>::Flag_t left, Bits_t<Storage> right);
    
    /// Returns a new Bits_t with all the bits from both arguments set.
    template <typename Storage>
    constexpr Bits_t<Storage> operator+
      (Flag_t<Storage> left, Flag_t<Storage> right);
    
    /// @}
    
    
    //--------------------------------------------------------------------------
    /// Namespace enclosing BitMask exceptions.
    namespace errors {
      
      /// @{
      /// @name BitMask exceptions
      
      /// Base class for exceptions thrown by flag-related utilities.
      struct Exception: public std::exception {
        explicit Exception(std::string msg = "Flag error"): message(msg) {}
        virtual const char* what() const noexcept override
          { return message.c_str(); }
        std::string message;
      }; // Exception
      
      /// Exception thrown to convey that an undefined flag index was tested
      struct FlagNotDefined: public Exception {
        explicit FlagNotDefined(std::string msg = "Flag undefined-flag error")
          : Exception(msg)
          {}
      }; // FlagNotDefined
      
      /// Exception thrown to convey that an invalid flag index was used
      struct OutOfRange: public Exception {
        explicit OutOfRange(std::string msg = "Flag out-of-range error")
          : Exception(msg)
          {}
      }; // OutOfRange
      
      /// @}
      
    } // namespace errors
    
    
    /**
     * @brief A class containing a set of flags.
     * @tparam NFlags number of flags to be allocated
     * @tparam Storage underlying integral type whose bits represent the flags
     * 
     * A BitMask contains a set of flags. Each flag can be in one of two
     * states ("set" and "unset"), or can be not defined at all ("undefined").
     * 
     * Note that the object might have a `capacity()` larger than just `NFlags`.
     * The flags after the first `NFlags` are "unsupported", in the sense that
     * in future implementations they might disappear. For the rest, they behave
     * just like the other flags though.
     */
    template <typename Storage>
    class BitMask {
      
      /// Type of underlying bit data representation.
      using Storage_t = Storage;
      
        public:
      
      using Mask_t = BitMask<Storage>; ///< This type.
      
      using FlagIndex_t = util::flags::Index_t; ///< Type of index of flag.
      
      using Bits_t = util::flags::Bits_t<Storage_t>; ///< Set of bits.
      
      /// Type identifying a single flag.
      using Flag_t = typename Bits_t::Flag_t;
      
      /// @{
      /// @name Exceptions
      
      /// Generic BitMask exception.
      using Exception = errors::Exception;
      
      /// Out-of-range flag index.
      using OutOfRangeError = errors::OutOfRange;
      
      /// Flag not defined.
      using FlagNotDefinedError = errors::FlagNotDefined;
      
      /// @}
      
      struct FromValuesTag {}; ///< Type for constructor tag from values.
      
      /// Constructor tag from values.
      static constexpr FromValuesTag fromValues {};
      

      /// Default constructor: no flag defined at all.
      explicit constexpr BitMask() = default;
      
      
      /**
       * @brief Constructor: defines and sets flags.
       * @param values a bit mask of the values to be defined and set
       * 
       * All bits in `values` will be defined and set.
       * Example:
       * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~{.cpp}
       * using MyMask_t = util::flags::BitMask<unsigned int>;
       * constexpr MyMask_t DefaultMask(MyMask_t::fromValues, 0x0300U);
       * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
       */
      constexpr BitMask(FromValuesTag, Storage_t defined);
      
      
      /**
       * @brief Constructor: defines and sets flags.
       * @param defined a bit mask of the flags to be defined
       * @param values a bit mask of the values to be set
       * 
       * If a bit value is requested to be set (in `values`), it will be also
       * defined, regardless whether its definition bit (in `defined`) is set.
       * Example:
       * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~{.cpp}
       * using MyMask_t = util::flags::BitMask<unsigned int>;
       * constexpr MyMask_t DefaultMask(MyMask_t::fromValues, 0x0300U, 0x0200U);
       * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
       */
      constexpr BitMask(FromValuesTag, Storage_t defined, Storage_t values);
      
      
      /**
       * @brief Constructor: defines and sets flags.
       * @param values a bit mask of the values to be set
       */
      constexpr BitMask(FromValuesTag, Bits_t values);
      
      
      /**
       * @brief Constructor: defines and sets flags.
       * @param defined a bit mask of the flags to be defined
       * @param values a bit mask of the values to be set
       * 
       * If a bit value is requested to be set (in `values`), it will be also
       * defined, regardless whether its definition bit (in `defined`) is set.
       */
      constexpr BitMask(FromValuesTag, Bits_t defined, Bits_t values);
      
      
      /**
       * @brief Constructor: merges all arguments in the argument list.
       * @tparam Others type of the remaining parameters to be merged
       * @param first first argument to be merged (here, a Flag_t)
       * @param others remaining arguments to be merged
       * @see create()
       * 
       * The effect is equivalent to call `create(first, others...)`.
       */
      template <typename... Others>
      constexpr BitMask(Flag_t first, Others... others)
        : BitMask(create(first, others...))
        {}
      
      /**
       * @brief Constructor: merges all arguments in the argument list.
       * @tparam Others type of the remaining parameters to be merged
       * @param first first argument to be merged (here, a Bits_t)
       * @param others remaining arguments to be merged
       * @see create()
       * 
       * The effect is equivalent to call `create(first, others...)`.
       */
      template <typename... Others>
      constexpr BitMask(Bits_t first, Others... others)
        : BitMask(create(first, others...))
        {}
      
      /**
       * @brief Constructor: merges all arguments in the argument list.
       * @tparam Others type of the remaining parameters to be merged
       * @param first first argument to be merged (here, a Mask_t)
       * @param others remaining arguments to be merged
       * @see create()
       * 
       * The effect is equivalent to call `create(first, others...)`.
       */
      template <typename... Others>
      constexpr BitMask(Mask_t first, Others... others)
        : BitMask(create(first, others...))
        {
          static_assert(sizeof...(Others) > 0, "This is no copy constructor!");
        }
      
      
      /// @{
      /// @name Access to flags
      
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
       */
      constexpr bool isDefined(Flag_t flag) const;
      
      /**
       * @brief Returns whether all specified bits are defined.
       * @param bits bits to check
       * @return whether all bits set in bits are defined
       * @see isDefined(Flag_t)
       * 
       * This method is equivalent to calling `isDefined(Flag_t)` on each single
       * flag defined in `bits`. The result is true only if all of them are
       * defined.
       */
      constexpr bool isDefined(Bits_t bits) const;
      
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
       * @brief Returns whether all specified bits are undefined.
       * @param bits bits to check
       * @return whether all bits set in bits are undefined
       * @see isDefined(Bits_t), isUndefined(Flag_t)
       * 
       * This is exactly the negation of `isDefined()`.
       */
      constexpr bool isUndefined(Bits_t bits) const;
      
      /**
       * @brief Returns if the specified flag is on ("set").
       * @param flag index of the flag to test
       * @return whether the specified flag is set
       * 
       * This method provides an unchecked access to a single flag value.
       * If the flag is `isUndefined()`, the behaviour of this method is also
       * undefined.
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
      
      /**
       * @brief Returns whether all the specified bits are set.
       * @param bits bits to check
       * @return whether all bits are set
       * @see isSet(Flag_t)
       * 
       * This method is equivalent to calling `isSet(Flag_t)` on each single
       * flag defined in `bits`. The result is true only if all of them are set,
       * that includes all of them being defined.
       */
      constexpr bool all(Bits_t bits) const;
      
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
      constexpr bool any(Bits_t bits) const;
      
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
      constexpr bool none(Bits_t bits) const;
      
      
      /**
       * @brief Returns whether any of the bits set in the mask are set.
       * @param mask the mask of bits
       * @return whether we have any of those bits set
       * @see noneSet(), match()
       * 
       * The bits that are undefined in mask are not used to compute the result.
       */
      constexpr bool anySet(Mask_t const& mask) const;
      
      /**
       * @brief Returns whether none of the bits set in the mask is set.
       * @param mask the mask of bits
       * @return whether we have any of those bits set
       * @see anySet(), match()
       * 
       * This is the logical negation of `anySet()`.
       */
      constexpr bool noneSet(Mask_t const& mask) const;
      
      /**
       * @brief Returns whether all bits defined in the mask are equal to ours.
       * @param mask the mask of bits
       * @return whether the mask matches ours
       * 
       * The bits that are undefined in mask are not used to compute the result.
       * If all the flags in mask that are set (`isSet()`) must be set, and all
       * the flags in mask that are unset (`isUnset()`) must be unset, this
       * method returns `true`.
       */
      constexpr bool match(Mask_t const& mask) const;
      
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
        { undefineImpl(first, others...); }
      
      /// Undefines all bits.
      void clear()
        { presence.clear(); values.clear(); }
      
      /// @}
      
      
      /// @{
      /// @name Number of flags
      
      /// Returns the number of flags the set has room for.
      static constexpr size_t capacity();
      
      /// @}
      
      /// Comparison: all flags must be the same
      /// @bug Also the value of undefined flags is currently checked
      constexpr bool operator== (Mask_t const& other) const
        { return (values == other.values) && (presence == other.presence); }
      
      /// Comparison: not all flags must be the same
      /// @bug Also the value of undefined flags is currently checked
      constexpr bool operator!= (Mask_t const& other) const
        { return (values != other.values) || (presence != other.presence); }
      
      
      /// Prints into the specified stream the least nBits significant bits.
      template <typename Stream>
      void dump(Stream&& out, unsigned int nBits) const;
      
      /// Prints into the specified stream all bits.
      template <typename Stream>
      void dump(Stream&& out) const
        { dump(std::forward<Stream>(out), capacity()); }
      
      
      /// @{
      /// @name Static mask manipulation
      
      /**
       * @brief Creates a new BitMask.
       * @tparam Args types of the arguments
       * @param args the data to create the mask from
       * @return a BitMask with the features specified in the arguments
       * 
       * If no argument is provided, the mask is returned with all the flags
       * undefined (like in default constructor).
       * The order of the arguments matters: the first arguments are processed
       * first and their effect can be overridden by the following arguments.
       * For the details, see `mergeIntoMask()`, which is used to incrementally
       * merge the argument to create the result.
       */
      template <typename... Args>
      static constexpr Mask_t create(Args... args);
      
      /**
       * @brief Returns a new mask with the content of the other `mask` merged.
       * @param baseMask the starting mask
       * @param mask the bits to be set
       * @return a new mask with all the content of base and the new mask
       * 
       * The content of the new mask overrides the base one: if a flag is
       * defined in mask, it is defined and copied into the result, otherwise
       * the state of the flag is copied from baseMask.
       * 
       * The truth table of the operation is described in the following table,
       * where '-' represents an undefined flag (`isUndefined()`), '0' an unset
       * flag (`isUnset()`), and '1' a set flag (`isSet()`).
       * 
       * |             mask  ||    -    |    0    |    1    |
       * | -------- | ------- | ------- | ------- | ------- |
       * | baseMask |         |         |         |         |
       * |    ^     |    -    |    -    |    0    |    1    |
       * |    ^     |    0    |    0    |    0    |    1    |
       * |    ^     |    1    |    1    |    0    |    1    |
       * 
       */
      static constexpr Mask_t mergeIntoMask(Mask_t baseMask, Mask_t mask);
      
      /**
       * @brief Returns a new mask with the specified bits defined and set.
       * @param baseMask the starting mask
       * @param bits the bits to be set
       * @return a new mask with all content from baseMask, plus the flag bit
       *         set
       * 
       * The truth table of this operation follows (see `mergeWithMask()` for
       * its legend).
       * 
       * |             bits  ||    0    |    1    |
       * | -------- | ------- | ------- | ------- |
       * | baseMask |         |         |         |
       * |    ^     |    -    |    -    |    1    |
       * |    ^     |    0    |    0    |    1    |
       * |    ^     |    1    |    1    |    1    |
       */
      static constexpr Mask_t mergeIntoMask(Mask_t baseMask, Bits_t bits);
      
      /**
       * @brief Returns a new mask with the specified flag defined and set.
       * @param baseMask the starting mask
       * @param flag the single flag to be set
       * @return a new mask with all content from baseMask, plus the flag bit
       *         set
       */
      static constexpr Mask_t mergeIntoMask(Mask_t baseMask, Flag_t flag);
      
      /**
       * @brief Returns a new mask combining bits set from two masks.
       * @param A one of the masks to be combined
       * @param B the other mask to be combined
       * @return a new mask with all set bits from of the two masks
       * 
       * The content of the new mask has a bit set if it was set (`isSet()`) in
       * either of the masks, otherwise unset if it was unset (`isUnset()`) in
       * either of the masks, or else undefined (`isUndefined()`), as it was
       * undefined in both masks.
       * 
       * The result differs from `mergeIntoMask()` only for bits which are
       * set (`isSet()`) in the first mask but unset (`isUnset()`) in the
       * second: `combineWithMask()` will leave the bit set, like in the first
       * mask, while `mergeIntoMask()` would have that bit always unset.
       * 
       * This is equivalent to a flagwise OR operation.
       * 
       * The truth table of the operation is described in the following table,
       * where '-' represents an undefined flag (`isUndefined()`), '0' an unset
       * flag (`isUnset()`), and '1' a set flag (`isSet()`).
       * 
       * |               B   ||    -    |    0    |    1    |
       * | -------- | ------- | ------- | ------- | ------- |
       * |    A     |         |         |         |         |
       * |    ^     |    -    |    -    |    0    |    1    |
       * |    ^     |    0    |    0    |    0    |    1    |
       * |    ^     |    1    |    1    |    1    |    1    |
       * 
       */
      static constexpr Mask_t combineWithMask(Mask_t A, Mask_t B);
      
      /**
       * @brief Returns a new mask with the specified flag defined and set.
       * @param baseMask the starting mask
       * @param bits the bits to be set
       * @return a new mask with all content from baseMask, plus the flag bit
       *         set
       * 
       * The truth table of this operation follows (see `mergeWithMask()` for
       * its legend).
       * 
       * |             bits  ||    0    |    1    |
       * | -------- | ------- | ------- | ------- |
       * | baseMask |         |         |         |
       * |    ^     |    -    |    -    |    1    |
       * |    ^     |    0    |    0    |    1    |
       * |    ^     |    1    |    1    |    1    |
       * 
       */
      static constexpr Mask_t combineWithMask(Mask_t baseMask, Bits_t bits);
      
      /**
       * @brief Returns a new mask with the specified flag defined and set.
       * @param baseMask the starting mask
       * @param flag the single flag to be set
       * @return a new mask with all content from baseMask, plus the bits set
       */
      static constexpr Mask_t combineWithMask(Mask_t baseMask, Flag_t flag);
      
      /**
       * @brief Returns a new mask with the bits set from both masks.
       * @param A one of the masks to be combined
       * @param B the other mask to be combined
       * @return a new mask with all bits set in both masks
       * 
       * The content of the new mask has a bit set if it was set (`isSet()`) in
       * both masks, otherwise unset if it was unset (`isUnset()`) in
       * either of the masks, or else undefined (`isUndefined()`), as it was
       * undefined in both masks.
       * 
       * This is equivalent to a flagwise AND operation.
       * 
       * The truth table of the operation is described in the following table,
       * where '-' represents an undefined flag (`isUndefined()`), '0' an unset
       * flag (`isUnset()`), and '1' a set flag (`isSet()`).
       * 
       * |               B   ||    -    |    0    |    1    |
       * | -------- | ------- | ------- | ------- | ------- |
       * |    A     |         |         |         |         |
       * |    ^     |    -    |    -    |    0    |    1    |
       * |    ^     |    0    |    0    |    0    |    0    |
       * |    ^     |    1    |    1    |    0    |    1    |
       * 
       */
      static constexpr Mask_t intersectWithMask(Mask_t A, Mask_t B);
      
      /**
       * @brief Returns a new mask with only the specified bits set.
       * @param baseMask the starting mask
       * @param bits the bits to be set
       * @return a new mask with all defined flags from baseMask, with only the
       *         specified bits set
       * 
       * All bits in the argument are also defined.
       * 
       * The truth table of this operation follows (see `mergeWithMask()` for
       * its legend).
       * 
       * |             bits  ||    0    |    1    |
       * | -------- | ------- | ------- | ------- |
       * | baseMask |         |         |         |
       * |    ^     |    -    |    -    |    1    |
       * |    ^     |    0    |    0    |    0    |
       * |    ^     |    1    |    0    |    1    |
       * 
       */
      static constexpr Mask_t intersectWithMask(Mask_t baseMask, Bits_t bits);
      
      /**
       * @brief Returns a new mask with the specified flag as only set flag.
       * @param baseMask the starting mask
       * @param flag the single flag to be set
       * @return a new mask with all flags from baseMask, with only flag set
       */
      static constexpr Mask_t intersectWithMask(Mask_t baseMask, Flag_t flag);
      
      /**
       * @brief Returns a new mask with the bits set from both masks.
       * @param A one of the masks to be combined
       * @param B the other mask to be combined
       * @return a new mask with all bits set in both masks
       * 
       * The content of the new mask has a bit set if it was set (`isSet()`) in
       * both masks, otherwise unset if it was unset (`isUnset()`) in
       * either of the masks, or else undefined (`isUndefined()`), as it was
       * undefined in both masks.
       * 
       * The truth table of this operation follows (see `mergeWithMask()` for
       * its legend).
       * 
       * |             mask  ||    -    |    0    |    1    |
       * | -------- | ------- | ------- | ------- | ------- |
       * | baseMask |         |         |         |         |
       * |    ^     |    -    |    -    |    0    |    0    |
       * |    ^     |    0    |    0    |    0    |    0    |
       * |    ^     |    1    |    1    |    1    |    0    |
       * 
       */
      static constexpr Mask_t unsetMask(Mask_t baseMask, Mask_t mask);
      
      /**
       * @brief Returns a new mask with only the specified bits set.
       * @param baseMask the starting mask
       * @param bits the bits to be set
       * @return a new mask with all defined flags from baseMask, with only the
       *         specified bits set
       * 
       * All bits in the argument are also defined.
       * 
       * The truth table of this operation follows (see `mergeWithMask()` for
       * its legend).
       * 
       * |             bits  ||    0    |    1    |
       * | -------- | ------- | ------- | ------- |
       * | baseMask |         |         |         |
       * |    ^     |    -    |    -    |    0    |
       * |    ^     |    0    |    0    |    0    |
       * |    ^     |    1    |    1    |    0    |
       * 
       */
      static constexpr Mask_t unsetMask(Mask_t baseMask, Bits_t bits);
      
      /**
       * @brief Returns a new mask with the specified flag as only set flag.
       * @param baseMask the starting mask
       * @param flag the single flag to be set
       * @return a new mask with all flags from baseMask, with only flag set
       */
      static constexpr Mask_t unsetMask(Mask_t baseMask, Flag_t flag);
      
      /**
       * @brief Returns the negation of mask.
       * @param mask the starting mask
       * @return a new mask with all defined bits flipped
       * 
       * The bits which were undefined, stay so. The others change their value.
       * 
       * The truth table of this operation follows (see `mergeWithMask()` for
       * its legend).
       * 
       * |  mask   | negate() |
       * | ------- | -------  |
       * |         |          |
       * |    -    |    -     |
       * |    0    |    1     |
       * |    1    |    0     |
       * 
       */
      static constexpr Mask_t negateMask(Mask_t mask);
      
      /**
       * @brief Returns a new mask with the specified bits unset.
       * @param bits bits to be unset
       * @return a new mask with the specified bits unset
       * 
       * Only the unset bits are defined.
       */
      static constexpr Mask_t negateMask(Bits_t bits);
      
      /**
       * @brief Returns a new mask with the specified flag unset.
       * @param flag the single flag to be set
       * @return a new mask with only the flag bit defined, and unset
       */
      static constexpr Mask_t negateMask(Flag_t flag);
      
      /// @}
      
        private:
      
      Bits_t values;   ///< Storage of value bits.
      Bits_t presence; ///< Storage of definition bits.
      
      // Storage details
      // We store values and definition information in two different bit
      // buckets; the same Flag_t pattern is good for both.
      
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
      
      /// Returns a bit set with all undefined bits unset.
      constexpr Bits_t definedOnly() const;
      
      /// Implementation detail of unset()
      template <typename... Flags>
      void unsetImpl(Flag_t flag, Flags... others);
      
      /// Implementation detail of unset()
      void unsetSingle(Flag_t flag);
      
      /// Returns whether any of the specified bits is set.
      static constexpr bool testBits(Storage_t data, Storage_t bits)
        { return data & bits; }
      
      /// Returns whether all the specified bits in the mask are set.
      static constexpr bool testBitmask(Storage_t data, Storage_t mask)
        { return (data & mask) == mask; }
      
      /// Returns whether all the specified bits in the mask are set.
      static constexpr bool testUnsetBitmask(Storage_t data, Storage_t mask)
        { return (data & ~mask) == data; }
      
      
    }; // BitMask<>
    
    
    /// Output of a bit mask into a stream.
    template <typename Stream, typename Storage>
    Stream& operator<< (Stream&& out, BitMask<Storage> const& mask)
      { mask.dump(std::forward<Stream>(out)); return out; }
    
    
    /// @{
    /**
     * @name Flag and mask management
     * 
     * The operations use the first operand as the starting point for the
     * result.
     * The binary operations in this group are:
     * 
     * * bit-wise OR (equivalent to `combineWithMask()`)
     * * bit-wise AND (equivalent to `intersectWithMask()`)
     * * addition: the flags of the right operand that are defined are copied
     *     into the result
     * * subtraction: the flags of the right operand that are set are unset
     *     in the result
     * 
     * The unary operations are:
     * * bitwise negation (equivalent to `negateMask()`)
     * * unary plus sign: no operation (bit it converts the operand to `Mask_t`)
     * * unary minus sign is not defined
     * 
     */
    
    
    /// Returns a mask which combines two of them.
    /// @see `BitMask<Storage>::combineWithMask()`
    template <typename Storage>
    constexpr BitMask<Storage> operator|
      (BitMask<Storage> left, BitMask<Storage> right);
    
    /// Returns a mask which merges two of them.
    /// @see `BitMask<Storage>::combineWithMask()`
    template <typename Storage>
    constexpr BitMask<Storage> operator|
      (BitMask<Storage> left, typename BitMask<Storage>::Bits_t right);
    
    /// Returns a mask which merges two of them.
    /// @see `BitMask<Storage>::combineWithMask()`
    template <typename Storage>
    constexpr BitMask<Storage> operator|
      (typename BitMask<Storage>::Bits_t left, BitMask<Storage> right);
    
    // operator|(Bits_t, Bits_t) is still a Bits_t
    
    
    /// Returns a mask which intersects two of them.
    /// @see `BitMask<Storage>::intersectWithMask()`
    template <typename Storage>
    constexpr BitMask<Storage> operator&
      (BitMask<Storage> left, BitMask<Storage> right);
    
    /// Returns a mask which intersects two of them.
    /// @see `BitMask<Storage>::intersectWithMask()`
    template <typename Storage>
    constexpr BitMask<Storage> operator&
      (BitMask<Storage> left, typename BitMask<Storage>::Bits_t right);
    
    /// Returns a mask which intersects two of them.
    /// @see `BitMask<Storage>::intersectWithMask()`
    template <typename Storage>
    constexpr BitMask<Storage> operator&
      (typename BitMask<Storage>::Bits_t left, BitMask<Storage> right);
    
    /// Returns a mask which intersects two of them.
    /// @see `BitMask<Storage>::intersectWithMask()`
    template <typename Storage>
    constexpr BitMask<Storage> operator&
      (Bits_t<Storage> left, Bits_t<Storage> right);
    
    
    /// Returns a mask which merges two of them.
    /// @see `BitMask<Storage>::mergeIntoMask()`
    template <typename Storage>
    constexpr BitMask<Storage> operator+
      (BitMask<Storage> baseMask, BitMask<Storage> mask);
    
    /// Returns a mask which merges two of them
    /// @see `BitMask<Storage>::mergeIntoMask()`
    template <typename Storage>
    constexpr BitMask<Storage> operator+
      (BitMask<Storage> baseMask, typename BitMask<Storage>::Bits_t bits);
    
    /// Returns a mask which merges two of them
    /// @see `BitMask<Storage>::mergeIntoMask()`
    template <typename Storage>
    constexpr BitMask<Storage> operator+
      (typename BitMask<Storage>::Bits_t baseBits, BitMask<Storage> mask);
    
    // operator+(Bits_t, Bits_t) is still a Bits_t
    
    
    /// Returns a mask set which defines and unsets the bits set in the mask.
    /// @see `BitMask<Storage>::unsetMask()`
    template <typename Storage>
    constexpr BitMask<Storage> operator-
      (BitMask<Storage> baseMask, BitMask<Storage> mask);
    
    /// Returns a mask set which defines and unsets the specified bits.
    /// @see `BitMask<Storage>::unsetMask()`
    template <typename Storage>
    constexpr BitMask<Storage> operator-
      (BitMask<Storage> baseMask, typename BitMask<Storage>::Bits_t bits);
    
    /// Returns a mask set which defines and unsets the bits set in the mask.
    /// @see `BitMask<Storage>::unsetMask()`
    template <typename Storage>
    constexpr BitMask<Storage> operator-
      (typename BitMask<Storage>::Bits_t baseBits, BitMask<Storage> mask);
    
    /// Returns a mask which defines and unsets the specified bits.
    /// @see `BitMask<Storage>::unsetMask()`
    template <typename Storage>
    constexpr BitMask<Storage> operator-
      (Bits_t<Storage> baseBits, Bits_t<Storage> bits);
    
    
    /// Returns a copy of the mask.
    template <typename Storage>
    constexpr BitMask<Storage> operator+ (BitMask<Storage> mask);
    
    /// Returns a mask with the specified bits set.
    template <typename Storage>
    constexpr BitMask<Storage> operator+ (Bits_t<Storage> bits);
    
    
    /// Returns a mask `M = -B` so that `A + M` is equivalent to `A - B`.
    template <typename Storage>
    constexpr BitMask<Storage> operator- (Bits_t<Storage> bits);
    
    /// Returns a mask `M = -B` so that `A + M` is equivalent to `A - B`.
    template <typename Storage>
    constexpr BitMask<Storage> operator- (Flag_t<Storage> flag);
    
    
    /// Returns a bit set which unsets the specified bits.
    template <typename Storage>
    constexpr BitMask<Storage> operator~
      (BitMask<Storage> mask);
    
    /*
    /// Returns a bit set which unsets the specified bits.
    template <typename Storage>
    constexpr BitMask<Storage> operator~ (Bits_t<Storage> bits);
    */
    
    
    /// Returns a bit mask which sets the specified flag.
    template <typename Storage>
    constexpr BitMask<Storage> Set(Flag_t<Storage> flag);
    
    /// Returns a bit mask which unsets the specified flag.
    template <typename Storage>
    constexpr BitMask<Storage> Unset(Flag_t<Storage> flag);
    
    
    /// @}
  
    /// Constructs a mask from bits
    template <typename Storage>
    constexpr BitMask<Storage> makeMask(Bits_t<Storage> bits);
    
    
  } // namespace flags
  
} // namespace util


//------------------------------------------------------------------------------
//---  template implementation
//---  

#include "lardataobj/Utilities/BitMask.tcc"

//------------------------------------------------------------------------------


#endif // LARDATAOBJ_UTILITIES_BITMASK_H
