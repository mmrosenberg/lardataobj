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

#include <stdexcept>

//------------------------------------------------------------------------------
//--- FlagSet<>
//---

//------------------------------------------------------------------------------
template <unsigned int NFlags, typename Storage>
constexpr bool util::flags::FlagSet<NFlags, Storage>::isFlag
  (FlagIndex_t flagIndex) const
  { return flagIndex < This_t::size(); }

//------------------------------------------------------------------------------
template <unsigned int NFlags, typename Storage>
constexpr bool util::flags::FlagSet<NFlags, Storage>::isFlag(Flag_t flag) const
  { return isFlag(flag.index()); }

//------------------------------------------------------------------------------
template <unsigned int NFlags, typename Storage>
bool util::flags::FlagSet<NFlags, Storage>::test(FlagIndex_t index) const {
  if (!isFlag(index)) {
    throw OutOfRangeError
      ("Invalid flag index was tested: #" + std::to_string(index));
  }
  return This_t::testImpl(index);
} // util::flags::FlagSet<>::test()

//------------------------------------------------------------------------------
template <unsigned int NFlags, typename Storage>
bool util::flags::FlagSet<NFlags, Storage>::test(Flag_t flag) const {
  if (!isFlag(flag)) {
    throw OutOfRangeError
      ("Invalid flag was tested: #" + util::flags::to_string(flag));
  }
  return testImpl(flag);
} // util::flags::FlagSet<>::test()


//------------------------------------------------------------------------------
template <unsigned int NFlags, typename Storage>
bool util::flags::FlagSet<NFlags, Storage>::testImpl(Flag_t flag) const {
  if (!This_t::isDefined(flag)) {
    throw FlagNotDefinedError
      ("Undefined flag was tested: #" + util::flags::to_string(flag));
  }
  return This_t::get(flag);
} // util::flags::FlagSet<>::testImpl()

//------------------------------------------------------------------------------


#endif // LARDATAOBJ_UTILITIES_FLAGSET_TCC
