/**
 * @file   DataIOmanip.h
 * @brief  Provides I/O manipulators for streaming data
 * @author Gianluca Petrillo (petrillo@fnal.gov)
 * @date   January 4th, 2016
 * 
 * 
 * This is a template library with no implementation file and no additional
 * linking required.
 */

#ifndef LARDATAOBJ_UTILITIES_DATAIOMANIP_H
#define LARDATAOBJ_UTILITIES_DATAIOMANIP_H

// C++ standard library
#include <utility> // std::forward()


namespace util {
  
  namespace manip {
    
    
    namespace details {
      
      /**
       * @brief Utility class to store and print a 3D vector
       * @tparam Vect type of the vector to be printed (see the requirements)
       * 
       * The printing operation is stored internally just for convenience.
       * 
       * The `Vect` type is required to provide member functions `X()`, `Y()` and
       * `Z()`, whose return value can be directly streamed.
       */
      template <typename Vect>
      class Vector3DStruct {
        Vect const& v; ///< Vector to be printed
        
          public:
        /// Constructor: print the specified vector
        Vector3DStruct(Vect const& vector): v(vector) {}
        
        /// The printing operator
        template <typename Stream>
        Stream& operator() (Stream&& out) const
          {
            out << "( " << v.X() << " ; " << v.Y() << " ; " << v.Z() << " )";
            return out;
          } // operator()
        
      }; // class vector3D
      
      /// Operator to print the manipulator
      template <typename Stream, typename Vect>
      Stream& operator<< (Stream&& out, Vector3DStruct<Vect> const& vmanip)
        { return vmanip(std::forward<Stream>(out)); }
      
    } // namespace details
    
    
    /**
     * @brief Produces a manipulator to print a vector
     * @tparam Vect type of vector to be printed
     * @param v vector to be printed
     * 
     * The typical usage is:
     * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~{.cpp}
     * TVector3 Tv(2., 3., 4.);
     * recob::Track::Point_t p(5., 6., 7.);
     * 
     * std::cout << "Tv = " << util::manip::vector3D(Tv)
     *   << "; point: " << util::manip::vector3D(p)
     *   << std::endl;
     * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
     * 
     */
    template <typename Vect>
    details::Vector3DStruct<Vect> vector3D(Vect const& v) { return { v }; }
    
    
  } // namespace manip
  
  
} // namespace util


#endif // LARDATAOBJ_UTILITIES_DATAIOMANIP_H
