/**
 * @file   lardataobj/Utilities/LazyVector.h
 * @brief  Contiguous data container with lazy resizing on access.
 * @author Gianluca Petrillo (petrillo@slac.stanford.edu)
 * @date   July 18, 2018
 *
 * This is a header-only library.
 *
 */

#ifndef LARDATAOBJ_UTILITIES_LAZYVECTOR_H
#define LARDATAOBJ_UTILITIES_LAZYVECTOR_H

// C/C++ standard libraries
#include <algorithm>
#include <vector>
#include <string> // std::to_string()
#include <stdexcept> // std::out_of_range
#include <cassert>


namespace util {

  /**
   * @brief A contiguous data container expanded on write.
   * @tparam T type of contained data
   * @tparam A allocator for the data (default: STL vector's default allocator)
   *
   * This container class represents a number of data elements contiguously
   * allocated in memory.
   * It mimics the behaviour and interface of STL vector, but the actual data
   * allocation is lazy, that is it happens only when writing to an element is
   * requested. The internal allocation is always contiguous, including as little
   * data as it can accommodate elements from the first to the last index written
   * at any point.
   *
   * The interface is also a partial replica of STL vector, with the addition of
   * members specific to this class, whose names start with `data_`.
   * Among the relevant features missing from this object there is iterators.
   *
   * For some internal resizing operations, a default value is used to construct
   * the new values. This default value can be specified in some constructors,
   * Otherwise, a value equivalent to `0` (i.e. `value_type(0)`) is used for
   * arithmetic value types, and the default-constructed value is used for all
   * other types.
   *
   * Example of usage:
   * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~{.cpp}
   * // start with a lazy vector of nominal size 6 elements and no actual data:
   * util::LazyVector<double> v(6U);
   *
   * // add a value `-4.0` at the previous-to-last element `4`:
   * // the vector will look like: { ... [4] -4.0 ... }
   * // (nominal size: 6, 1 stored datum)
   * v[4] = -4.0;
   *
   * // add a value `-2.0` at the third element:
   * // the vector will look like: { ... [2] -2.0, [3] def, [4] -4.0 ... }
   * // (nominal size still 6, 3 stored data, the default value "def" is 0.0)
   * v[2] = -2.0;
   *
   * // we want to set element #6 to -6.0: we need to expand the vector first.
   * v.resize(7U); // barely enough for element #6
   * // the vector will look like: { ... [2] -2.0, [3] def, [4] -4.0 [5] def [6] -6.0 }
   * // (nominal size 7, 5 stored data, the default value "def" is 0.0)
   * v[6] = -6.0;
   *
   * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
   *
   * @note Special care needs to be used when accessing a non-const `LazyVector`,
   *       since every access will create storage for the specified element
   *       (like in STL map `operator[]`).
   *       For this reason, the special methods `const_at(size_type)` and
   *       `const_get(size_type)` are provided, which never create storage.
   *
   */
  template <typename T, typename A = typename std::vector<T>::allocator_type>
  class LazyVector {

    using Data_t = std::vector<T, A>; ///< Actual data storage type.

      public:

    // --- BEGIN STL vector types ----------------------------------------------
    /// @name STL vector types
    /// @{

    using allocator_type  = typename Data_t::allocator_type;
    using value_type      = typename Data_t::value_type;
    using size_type       = typename Data_t::size_type;
    using difference_type = typename Data_t::difference_type;
    using reference       = typename Data_t::reference;
    using const_reference = typename Data_t::const_reference;
    using pointer         = typename Data_t::pointer;
    using const_pointer   = typename Data_t::const_pointer;

    /// @}
    // --- END STL vector types ------------------------------------------------


    /// --- BEGIN Constructors -------------------------------------------------
    /// Default constructor: an empty vector.
    LazyVector() = default;

    /// Constructor: like default, but using the specified allocator.
    LazyVector(allocator_type const& a);


    /**
     * @brief Constructor: a lazy vector with a specified maximum size.
     * @param n the initial maximum size of the vector
     *
     * @note This constructor is essentially different from the one of STL
     *       vector with the same signature.
     *
     * The vector is set to a nominal size of `n`, with _no stored data_.
     *
     * The default value of vector elements is the default-constructed `T`
     * value, as returned by `defaultValueType()`.
     */
    LazyVector(size_type n);

    /**
     * @brief Constructor: a lazy vector with a specified maximum size.
     * @param n the initial maximum size of the vector
     * @param defValue value to be used when resizing
     *
     * @note This constructor is essentially different from the one of STL
     *       vector with the same signature.
     *
     * The vector is set to a nominal size of `n`, with _no stored data_.
     * A default value `defValue` is registered, so that it can be used when
     * actual storage is needed for elements whose value is not explicitly
     * specified by the user:
     * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~{.cpp}
     * util::LazyVector<int> v(3, 5);
     * v[0] =  0;
     * v[2] = -2;
     *
     * std::cout << "Default element [1]: " << v.at(1) << std::endl;
     * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
     * will print something like: "Default element [1]: 5".
     */
    LazyVector(size_type n, value_type const& defValue);


    /// --- END Constructors -------------------------------------------------



    // --- BEGIN Container information -----------------------------------------
    /// @name Container information
    /// @{

    /// Returns the size of the vector.
    size_type size() const noexcept { return fNominalSize; }

    /// Returns whether the vector is empty.
    bool empty() const noexcept { return fNominalSize == 0U; }

    /// Returns the size of data actually stored.
    size_type data_size() const noexcept { return fData.size(); }

    /// Returns whether the specified position is within the vector.
    bool has_index(size_type pos) const noexcept { return pos < size(); }

    /// Returns whether no data is actually stored.
    bool data_empty() const noexcept { return fData.empty(); }

    /// Returns the default value.
    /// @see `LazyVector(size_type, value_type const&)`
    value_type const& data_defvalue() const { return fDefValue; }

    /// Index of the first data element in the storage
    /// (undefined if `data_empty()`).
    size_type data_begin_index() const { return fFirstIndex; }

    /// Index after the last data element in the storage
    /// (undefined if `data_empty()`).
    size_type data_end_index() const
      { return data_begin_index() + data_size(); }

    /// Returns the internal storage index for the specified position.
    bool data_has_index(size_type pos) const
      { return (pos >= data_begin_index()) && (pos < data_end_index()); }

    /**
     * @brief Returns a constant pointer to the specified element.
     * @param pos position of the element
     * @return pointer to storage for specified element, `nullptr` if not stored
     *
     * If `pos` represents an element that has storage, the pointer to that
     * element is returned. If instead `pos` represents a valid element with no
     * storage (default value), `nullptr` is returned.
     * If `pos` does not represent a valid element, the result is undefined.
     */
    const_pointer data_address(size_type pos) const;

    /// @}
    // --- END Container information -------------------------------------------



    // --- BEGIN Access to data elements ---------------------------------------
    /**
     * @name Access to data elements
     *
     * There are important differences between the access methods of
     * `LazyVector` and the ones of STL `vector`.
     *
     * The constant versions of the access methods differ from the corresponding
     * STL `vector` ones in that they return a _copy_ of the element rather than
     * a reference to it.
     * This happens because the element could not have current storage, and we
     * can't create it because the method is constant.
     *
     * Also, the non-constant versions of the access methods always ensure
     * storage for the accessed element. If such behaviour needs to be avoided
     * (e.g. for performance reasons), use the methods explicitly named
     * constant: `const_at()` and `const_get()`, explicitly constant versions of
     * `at()` and `operator[]` respectively. To preserve the naming scheme, a
     * method `get()` is also provided which is equivalent to the non-constant
     * version of `operator[]`.
     */
    /// @{

    //@{
    /**
     * @brief Returns a reference to the specified element, throws an exception
     *        if not present.
     * @param pos position to be accessed
     * @return a copy of the value of requested element
     * @throw std::out_of_range if the container is too small to contain `pos`
     *
     * Returns a copy of the specified element.
     * If the requested element is beyond the size of the container, an
     * exception `std::out_of_range` is thrown.
     *
     * @note This method differs from the corresponding STL vector in that it
     *       returns a _copy_ of the element rather than a reference to it.
     *       This happens because the element could not have current storage,
     *       and we can't create it because the method is constant.
     */
    value_type at(size_type pos) const;
    value_type const_at(size_type pos) const { return at(pos); }
    //@}

    /**
     * @brief Returns a reference to the specified element, throws an exception
     *        if not present.
     * @param pos position to be accessed
     * @return the reference to the requested element
     * @throw std::out_of_range if the container is too small to contain `pos`
     * @see `data_defvalue()`
     *
     * Returns a reference to the specified element. If the element is not
     * stored yet, it's created with the default value, and returned.
     * If the requested element is beyond the size of the container, an
     * exception `std::out_of_range` is thrown.
     */
    reference at(size_type pos);

    //@{
    /**
     * @brief Returns a copy of the specified element.
     * @param pos position to be accessed
     * @return a copy of the specified element
     * @see `at(size_type) const`
     *
     * Returns a copy of the specified element.
     * If the requested element is beyond the size of the container, the result
     * is undefined.
     *
     * @note See `at(value_type) const` for an explanation of why a value is
     *       returned rather than a reference.
     */
    value_type operator[](size_type pos) const;
    value_type const_get(size_type pos) const { return this->operator[](pos); }
    //@}

    //@{
    /**
     * @brief Returns a reference to the specified element.
     * @param pos position to be accessed
     * @return a reference to the specified element
     * @see `data_defvalue()`
     *
     * Returns a reference to the specified element.
     * If that element is not stored, it is allocated first; all missing
     * elements, including the required one, are initialised by copying into
     * them the default value stored at construction.
     * Like for STL vector, this method does not expand the vector: if `pos` is
     * beyond the vector size, the result is undefined.
     */
    reference operator[](size_type pos);
    reference get(size_type pos) { return this->operator[](pos); }
    //@}

    /// @}
    // --- END Access to data elements -----------------------------------------



    // --- BEGIN Setting data elements -----------------------------------------
    /// @name Setting data elements
    /// @{


    /// @}
    // --- END Setting data elements -------------------------------------------



    // --- BEGIN Container operations -----------------------------------------
    /// @name Container operations
    /// @{

    /**
     * @brief Changes the nominal size of the container.
     * @param newSize new container size
     *
     * The nominal size of the vector is set to newSize. Even when `newSize`
     * is larger than the current nominal size, no additional data is stored.
     * If the new nominal size is smaller, actual data may be released (i.e.
     * erased) so that no stored data is beyond the new nominal size.
     *
     * @note It is not possible to specify a filling value: the "default" value
     *       specified on construction is always used.
     */
    void resize(size_type newSize);

    /**
     * @brief Allocates enough memory in storage to store `n` elements.
     * @param n number of elements to have storage for
     * @see `data_prepare()`
     *
     * Storage allocation is resized to be able to host at least `n` elements
     * (it is not reduced).
     * Note that the use of `reserve()` for `LazyVector` is more subtle than for
     * a STL vector. The common use of `reserve()` is to avoid reallocations
     * when extending the vector. In this case, after a call to `reserve(n)` the
     * reallocation is avoided only as long as only the elements from
     * `data_begin_index()` to `data_begin_index() + n` (excluded) are written.
     *
     * Note that `data_prepare()` has a similar purpose and might be more
     * effective.
     */
    void reserve(size_type n) { storage().reserve(n); }

    /// Removes all stored data and sets the nominal size to 0.
    void clear();

    /// Reduces memory usage to the amount needed by the elements with storage.
    void shrink_to_fit() { storage().shrink_to_fit(); }

    /**
     * @brief Prepares the vector to store elements in the specified range.
     * @param startIndex index of the first element to be stored
     * @param endIndex index after the last element to be stored
     * @see `data_prepare(size_type)`
     *
     * This method sets the lower index to `startIndex`, and allocates enough
     * storage to store the whole requested range.
     * The elements are not initialized or constructed, but
     * Following access to elements in the specified range will not cause
     * memory reallocation, and that holds until an access outside that range
     * happens, after which all bets are off.
     *
     * **Old data is lost.**
     *
     * @note The nominal size of the vector is not changed, therefore the
     *       specified range might be not honored. If used in combination with
     *       `resize()`, `resize()` should be called first.
     */
    void data_prepare(size_type startIndex, size_type endIndex);

    /**
     * @brief Prepares the vector to store `n` elements from `0`.
     * @param n number of elements to prepare storage for
     * @see `data_prepare(size_type, size_type)`
     *
     * This method reserves storage for `n` elements starting with the element
     * #0.
     *
     * **Old data is lost.**
     *
     * See `data_prepare(size_type, size_type)` for more information.
     */
    void data_prepare(size_type n) { data_prepare(0U, n); }

    /**
     * @brief Allocates the specified range and stores default values for it.
     * @param startIndex index of the first element to be initialized
     * @param endIndex index after the last element to be initialized
     *
     * Each element in the range from `startIndex` to `endIndex` is stored and
     * the default value is assigned to it.
     *
     * **Old data is lost.**
     *
     * @note The nominal size of the vector is not changed, therefore the
     *       specified range might be not honored. If used in combination with
     *       `resize()`, `resize()` should be called first.
     */
    void data_init(size_type startIndex, size_type endIndex);

    /**
     * @brief Allocates and initializes `n` elements starting from index 0.
     * @param n number of elements to be initialized
     *
     * Each element in the range from `0` to `n` is stored and
     * the default value is assigned to it.
     * This is semantically similar to `std::vector::resize(n, data_defvalue()`,
     * except that this method does not change the nominal size of the vector.
     *
     * **Old data is lost.**
     *
     * @note The nominal size of the vector is not changed, therefore the
     *       specified range might be not honored. If used in combination with
     *       `resize()`, `resize()` should be called first.
     */
    void data_init(size_type n) { data_init(0U, n); }



    /// @}
    // --- END Container operations --------------------------------------------

      private:
    Data_t fData; ///< Actual data storage.

    size_type fNominalSize = 0U; ///< Alleged data size.
    size_type fFirstIndex = fData.max_size();  ///< First element currently stored.
    value_type fDefValue = defaultValueType(); ///< Default value.

    /// Default-initialised value of type `value_type` used as default fallback.
    static value_type const fDefaultDefaultValue;

    //@{
    /// Returns the data storage.
    Data_t& storage() { return fData; }
    Data_t const& storage() const { return fData; }
    //@}

    /// Returns the internal storage index for the specified position.
    size_type index_of(size_type pos) const { return pos - fFirstIndex; }

    /// Expands the storage to include the specified position.
    void expand(size_type pos);

    /// Makes the first data allocation.
    void init(size_type pos, size_type n = 1U);

    /// Expands the storage to include the specified position behind it.
    void expand_front(size_type pos);

    /// Expands the storage to include the specified position ahead of it.
    void expand_back(size_type pos);

    /// Makes sure the nominal size is large enough to include all stored data.
    void fix_size();

    /// Erases all stored data from the container; nominal size is not changed.
    void data_clear();

    /// Throws `std::out_of_range` if `pos` is not contained in the vector.
    void check_range(size_type pos) const;


    /// Returns the class default value (used when user does not specify any).
    static value_type const& defaultValueType() { return fDefaultDefaultValue; }


  }; // LazyVector<>

} // namespace util


//------------------------------------------------------------------------------
//---  template implementation
//------------------------------------------------------------------------------
template <typename T, typename A /* = std::vector<T>::allocator_type */>
typename util::LazyVector<T,A>::value_type const
util::LazyVector<T,A>::fDefaultDefaultValue{};


//------------------------------------------------------------------------------
template <typename T, typename A /* = std::vector<T>::allocator_type */>
util::LazyVector<T,A>::LazyVector(allocator_type const& a)
  : fData(a)
  {}


//------------------------------------------------------------------------------
template <typename T, typename A /* = std::vector<T>::allocator_type */>
util::LazyVector<T,A>::LazyVector(size_type n)
  : LazyVector(n, defaultValueType())
  {}


//------------------------------------------------------------------------------
template <typename T, typename A /* = std::vector<T>::allocator_type */>
util::LazyVector<T,A>::LazyVector(size_type n, value_type const& defValue)
  : fNominalSize(n)
  , fDefValue(defValue)
  {}


//------------------------------------------------------------------------------
template <typename T, typename A /* = std::vector<T>::allocator_type */>
typename util::LazyVector<T,A>::reference util::LazyVector<T,A>::at
  (size_type pos)
{
  /*
   * Behaviour summary:
   * * if `pos` is out of vector range, throw an exception
   * * if element at `pos` has no storage, create storage for it
   * * return a reference to the element at `pos`
   */
  check_range(pos); // verify that `pos` is valid, throw otherwise
  expand(pos);
  return storage()[index_of(pos)]; // we already know it's valid
}


//------------------------------------------------------------------------------
template <typename T, typename A /* = std::vector<T>::allocator_type */>
typename util::LazyVector<T,A>::value_type util::LazyVector<T,A>::at
  (size_type pos) const
{
  /*
   * Behaviour summary:
   * * if `pos` is out of vector range, throw an exception
   * * if element at `pos` has no storage, return a copy of the default value
   * * otherwise, return a copy of the element at `pos`
   */
  check_range(pos); // verify that `pos` is valid, throw otherwise
  return data_has_index(pos)? storage()[index_of(pos)]: data_defvalue();
} // util::LazyVector<T,A>::at() const


//------------------------------------------------------------------------------
template <typename T, typename A /* = std::vector<T>::allocator_type */>
typename util::LazyVector<T,A>::reference util::LazyVector<T,A>::operator[]
  (size_type pos)
{
  /*
   * Behaviour summary:
   * * if `pos` is out of vector range, behaviour is undefined
   * * if element at `pos` has no storage, create storage for it
   * * return a reference to the element at `pos`
   */
  // to have the common case where the requested position has storage be handled
  // the fastest, we "enforce" the order by nesting (optimiser has last word)
  if (!data_has_index(pos)) {
    if (has_index(pos)) expand(pos);
  }
  return storage()[index_of(pos)];
} // util::LazyVector<T,A>::operator[]()


//------------------------------------------------------------------------------
template <typename T, typename A /* = std::vector<T>::allocator_type */>
typename util::LazyVector<T,A>::value_type util::LazyVector<T,A>::operator[]
  (size_type pos) const
{
  /*
   * Behaviour summary:
   * * if `pos` is out of vector range, behaviour is undefined
   * * if element at `pos` has no storage, return a copy of the default value
   * * otherwise, return a copy of the element at `pos`
   */
  // this implementation will return a default value if `pos` is out of range;
  // this is not a requirement, and may change at any time.
  if (pos < data_begin_index()) return data_defvalue();
  auto const index = index_of(pos);
  return (index < data_size())? storage()[index]: data_defvalue();
} // util::LazyVector<T,A>::operator[] () const


//------------------------------------------------------------------------------
template <typename T, typename A /* = std::vector<T>::allocator_type */>
typename util::LazyVector<T,A>::const_pointer
util::LazyVector<T,A>::data_address(size_type pos) const
{
  /*
   * Behaviour summary:
   * * if `pos` is out of vector range, behaviour is undefined
   * * if element at `pos` has no storage, return nullptr
   * * otherwise, return the pointer to the specified element
   */
  // this implementation will return nullptr if `pos` is out of range;
  // this is not a requirement, and may change at any time.
  if (pos < data_begin_index()) return nullptr;
  auto const index = index_of(pos);
  return (index < data_size())? storage().data() + index: nullptr;
} // util::LazyVector<T,A>::data_address()


//------------------------------------------------------------------------------
template <typename T, typename A /* = std::vector<T>::allocator_type */>
void util::LazyVector<T,A>::resize(size_type newSize) {
  /*
   * Behaviour summary:
   * * when extending, do not change storage
   * * when shrinking, cut the excess storage
   */
  fNominalSize = newSize;
  // delete any excess data
  if (data_end_index() > newSize) {
    if (fNominalSize <= data_begin_index()) data_clear(); // no data is left
    else {
      storage().erase
        (storage().begin() + index_of(fNominalSize), storage().end());
    }
  }
} // util::LazyVector<T,A>::resize()


//------------------------------------------------------------------------------
template <typename T, typename A /* = std::vector<T>::allocator_type */>
void util::LazyVector<T,A>::clear() {
  data_clear();
  fNominalSize = 0U;
} // util::LazyVector<T,A>::clear()


//------------------------------------------------------------------------------
template <typename T, typename A /* = std::vector<T>::allocator_type */>
void util::LazyVector<T,A>::data_prepare
  (size_type startIndex, size_type endIndex)
{
  // we do not go beyond the declared size of the vector:
  size_type const e = std::min(endIndex, size());
  if (startIndex >= e) return;

  data_clear(); // remove the old data
  storage().reserve(e - startIndex);
  fFirstIndex = startIndex;

} // util::LazyVector<T,A>::data_prepare(size_type, size_type)


//------------------------------------------------------------------------------
template <typename T, typename A /* = std::vector<T>::allocator_type */>
void util::LazyVector<T,A>::data_init
  (size_type startIndex, size_type endIndex)
{
  // we do not go beyond the declared size of the vector:
  size_type const e = std::min(endIndex, size());
  if (startIndex >= e) return;

  data_clear(); // remove the old data
  storage().resize(e - startIndex, data_defvalue());
  fFirstIndex = startIndex;

} // util::LazyVector<T,A>::data_init(size_type, size_type)


//------------------------------------------------------------------------------
template <typename T, typename A /* = std::vector<T>::allocator_type */>
void util::LazyVector<T,A>::expand(size_type pos) {
  // this is just a dispatcher
  if      (data_empty())              init(pos);
  else if (pos <  data_begin_index()) expand_front(pos);
  else if (pos >= data_end_index())   expand_back(pos);
}


//------------------------------------------------------------------------------
template <typename T, typename A /* = std::vector<T>::allocator_type */>
void util::LazyVector<T,A>::init(size_type pos, size_type n /* = 1 */) {
  assert(data_empty());
  storage().assign(n, data_defvalue());
  fFirstIndex = pos;
  fix_size();
}


//------------------------------------------------------------------------------
template <typename T, typename A /* = std::vector<T>::allocator_type */>
void util::LazyVector<T,A>::expand_front(size_type pos) {
  assert(pos < data_begin_index());
  storage().insert
    (storage().begin(), data_begin_index() - pos, data_defvalue());
  fFirstIndex = pos;
}


//------------------------------------------------------------------------------
template <typename T, typename A /* = std::vector<T>::allocator_type */>
void util::LazyVector<T,A>::expand_back(size_type pos) {
  assert(pos >= data_end_index());
  storage().resize(pos + 1U - data_begin_index(), data_defvalue());
  fix_size();
}


//------------------------------------------------------------------------------
template <typename T, typename A /* = std::vector<T>::allocator_type */>
void util::LazyVector<T,A>::fix_size(){
  auto const min_size = data_end_index();
  if (fNominalSize < min_size) fNominalSize = min_size;
}


//------------------------------------------------------------------------------
template <typename T, typename A /* = std::vector<T>::allocator_type */>
void util::LazyVector<T,A>::data_clear() {
  storage().clear();
  fFirstIndex = storage().max_size();
} // util::LazyVector<T,A>::data_clear()


//------------------------------------------------------------------------------
template <typename T, typename A /* = std::vector<T>::allocator_type */>
void util::LazyVector<T,A>::check_range(size_type pos) const {
  if (has_index(pos)) return;
  throw std::out_of_range(
    "Index " + std::to_string(pos) + " is out of LazyVector range (size: "
    + std::to_string(size()) + ")"
    );
} // util::LazyVector<T,A>::check_range()


//------------------------------------------------------------------------------

#endif // LARDATAOBJ_UTILITIES_LAZYVECTOR_H
