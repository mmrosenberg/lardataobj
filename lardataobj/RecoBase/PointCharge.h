/**
 * @file   lardataobj/RecoBase/PointCharge.h
 * @brief  Information about charge reconstructed in the active volume.
 * @author Gianluca Petrillo (petrillo@fnal.gov)
 * @date   December 20, 2017
 *
 *
 */

#ifndef LARDATAOBJ_RECOBASE_CHARGE_H
#define LARDATAOBJ_RECOBASE_CHARGE_H

// C/C++ standard libraries
#include <limits>
#include <string>
#include <utility> // std::forward()
#include <iosfwd> // std::ostream


namespace recob {

  /**
   * @brief Charge reconstructed in the active volume.
   *
   * The reconstructed charge value is in arbitrary units in that it's hard to
   * precisely define the normalization, which ends up being
   * algorithm-dependent. As such, the charge from this object is expected to
   * require a specific calibration.
   *
   */
  class PointCharge {

      public:

    using Charge_t = float; ///< Type for the amount of reconstructed charge.

    /// Value used for default-constructed ("invalid") charge.
    static constexpr Charge_t InvalidCharge
      = std::numeric_limits<Charge_t>::lowest();


    //--- BEGIN Constructors ---------------------------------------------------
    /// @{
    /// @name Constructors

    /// Default constructor (for ROOT only).
    constexpr PointCharge(): fCharge(InvalidCharge) {}

    /**
     * @brief Constructor: sets all the data.
     * @param charge reconstructed charge (used unchanged)
     *
     */
    constexpr PointCharge(Charge_t charge): fCharge(charge) {}

    /// @}
    //--- END Constructors -----------------------------------------------------


    //--- BEGIN Accessors ------------------------------------------------------
    /// @{
    /// @name Access to the information

    /// Returns the stored value of the reconstructed charge.
    constexpr Charge_t charge() const { return fCharge; }

    /// @}
    //--- END Accessors --------------------------------------------------------


    //--- BEGIN Status ---------------------------------------------------------
    /// @{
    /// @name Object status and data validity

    /// Returns whether the reconstructed charge value is valid.
    constexpr bool hasCharge() const { return charge() != InvalidCharge; }

    /// @}
    //--- END Status -----------------------------------------------------------


    //--- BEGIN Printing operations --------------------------------------------
    /// @{
    /// @name Printing operations

    /// Default verbosity for dumping operations.
    static constexpr unsigned int DefaultVerbosity = 1U;

    /// Maximum available verbosity for dumping operations.
    static constexpr unsigned int MaxVerbosity = 1U;

    /**
     * @brief Dump the content of this object into an output stream.
     * @tparam Stream type of stream to write into
     * @param out output stream
     * @param verbosity the level of detail of dumped information, to be between
     *                  `0` and `MaxVerbosity` _(default: `DefaultVerbosity`)_
     * @param indent all lines except the first are prepended this string
     *               _(default: none)_
     * @param firstIndent the first output line is prepended this string
     *               _(default: same as `indent`)_
     *
     * The output starts on the current line, with `firstIndent` as indentation.
     * No end-of-line is inserted at the end of the output.
     */
    template <typename Stream>
    void dump(
      Stream&& out, unsigned int verbosity,
      std::string indent, std::string firstIndent
      ) const;

    // variants for the implementation of default values
    template <typename Stream>
    void dump
      (Stream&& out, unsigned int verbosity, std::string indent = "") const
      { dump(std::forward<Stream>(out), verbosity, indent, indent); }
    template <typename Stream>
    void dump(Stream&& out, std::string indent, std::string firstIndent) const
      { dump(std::forward<Stream>(out), DefaultVerbosity, indent, firstIndent); }
    template <typename Stream>
    void dump(Stream&& out, std::string indent = "") const
      { dump(std::forward<Stream>(out), indent, indent); }

    /// @}
    //--- END Printing operations ----------------------------------------------

      private:
    float fCharge; ///< Reconstructed charge.

  }; // class PointCharge


  /// Dumps the content of a `recob::PointCharge` object into an output stream.
  inline std::ostream& operator<<
    (std::ostream& out, recob::PointCharge const& charge)
    { charge.dump(out); return out; }


} // namespace recob



//------------------------------------------------------------------------------
//---  template implementation
//---
template <typename Stream>
void recob::PointCharge::dump(
  Stream&& out, unsigned int verbosity,
  std::string indent, std::string firstIndent
  ) const
{
  if (verbosity <= 0U) return;

  //----------------------------------------------------------------------------
  out << firstIndent
    << "charge: ";
  if (hasCharge()) out << charge();
  else             out << "none";

//  if (verbosity <= 1U) return;
  //----------------------------------------------------------------------------
  // if the following check fails,
  // consistency between `dump()` and `MaxVerbosity` needs to be restored
  static_assert(MaxVerbosity == 1U, "Please update the code!");

} // recob::PointCharge::dump()


//------------------------------------------------------------------------------

#endif // LARDATAOBJ_RECOBASE_CHARGE_H
