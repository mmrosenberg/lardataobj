///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// \version 
//
// \brief Data products to hold MVA output values and metadata, see MVAReader/MVAWriter wrappers for convenient usage.
//
// \author robert.sulej@cern.ch
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef ANAB_MVAOUTPUT_H
#define ANAB_MVAOUTPUT_H

#ifndef __GCCXML__
#include "cetlib/exception.h"
#include <iosfwd>
#include <iostream>
#include <iomanip>
#endif

#include <array>
#include <vector>

namespace anab {

/// MVA resuts for a single object, or just a feature vector of size N. Values are saved as 32bit fp's,
/// this is usually enough for classification purposes and the precision one can expect from MVA algorithms.
template <size_t N>
class MVAOutput {
public:

    MVAOutput() { }

    // MUST UPDATE WHEN CLASS IS CHANGED!
    static short Class_Version() { return 2; }

private:
    float fOutputs[N]; ///< Vector of MVA output values

#ifndef __GCCXML__
public:

    MVAOutput(float init) { set(init); }
    MVAOutput(std::array<float, N> const & values) { set(values); }
    MVAOutput(std::array<double, N> const & values) { set(values); }
    MVAOutput(std::vector<float> const & values) { set(values); }
    MVAOutput(std::vector<double> const & values) { set(values); }

    /// If you really have to use C arrays:
    MVAOutput(float const * values) { for (size_t i = 0; i < N; ++i) { fOutputs[i] = values[i]; } }
    MVAOutput(double const * values) { for (size_t i = 0; i < N; ++i) { fOutputs[i] = values[i]; } }

    /// Assignment operators, from the same types as constructors.
    MVAOutput& operator =(float init) { set(init); return *this; }
    MVAOutput& operator =(std::array<float, N> const & values) { set(values); return *this; }
    MVAOutput& operator =(std::array<double, N> const & values) { set(values); return *this; }
    MVAOutput& operator =(std::vector<float> const & values) { set(values); return *this; }
    MVAOutput& operator =(std::vector<double> const & values) { set(values); return *this; }

    friend std::ostream& operator<< (std::ostream &o, MVAOutput const& a)
    {
        o << "MVAOutput values:";
        for (size_t i = 0; i < N; ++i) { o << " " << a.fOutputs[i]; }
        o << std::endl; 
        return o;
    }

    size_t size() const { return N; }

    float at(size_t index) const
    {
        if (index < N) { return fOutputs[index]; }
        else { throw cet::exception("MVAOutput") << "Index out of range: " << index; }
    }

    float operator[] (size_t index) const { return fOutputs[index]; }

private:

    void set(float init) { for (size_t i = 0; i < N; ++i) { fOutputs[i] = init; } }
    void set(std::array<float, N> const & values) { for (size_t i = 0; i < N; ++i) { fOutputs[i] = values[i]; } }
    void set(std::array<double, N> const & values) { for (size_t i = 0; i < N; ++i) { fOutputs[i] = values[i]; } }
    void set(std::vector<float> const & values)
    {
        if (values.size() == N) { for (size_t i = 0; i < N; ++i) { fOutputs[i] = values[i]; } }
        else { throw cet::exception("MVAOutput") << "Expected length: " << N << ", provided: " << values.size(); }
    }
    void set(std::vector<double> const & values)
    {
        if (values.size() == N) { for (size_t i = 0; i < N; ++i) { fOutputs[i] = values[i]; } }
        else { throw cet::exception("MVAOutput") << "Expected length: " << N << ", provided: " << values.size(); }
    }

#endif

}; // class MVAOutput

/// MVA results metadata. The idea is to link entire collection of objects to the collection
/// of MVA results, and add metadata like meaning of columns in MVA results or recommended thresholds
/// for various applications of the MVA values.
template <size_t N>
class MVADescription {
public:

    MVADescription() { }

    // MUST UPDATE WHEN CLASS IS CHANGED!
    static short Class_Version() { return 2; }

private:
    std::string fDataTag;        ///< Tag of the reco data products (art::InputTag format)
    std::string fOutputInstance; ///< Instance name of MVA output values
    std::string fOutputNames[N]; ///< MVA output names/meaning

#ifndef __GCCXML__
public:

    MVADescription(std::string const & dataTag, std::string const & outputInstance,
        std::vector< std::string > const & outputNames = std::vector< std::string >(N, "")) :
        fDataTag(dataTag),
        fOutputInstance(outputInstance)
    {
        if (outputNames.size() <= N) { for (size_t i = 0; i < outputNames.size(); ++i) { fOutputNames[i] = outputNames[i]; } }
        else { throw cet::exception("MVAOutput") << "Expected max length of outputNames: " << N << ", provided: " << outputNames.size(); }
    }

    friend std::ostream& operator<< (std::ostream &o, MVADescription const& a)
    {
        o << "MVADescription: prepared for " << a.fDataTag << ", instance name " << a.fOutputInstance << ", " << N << " outputs:" << std::endl;
        for (size_t i = 0; i < N; ++i) { o << " " << a.fOutputNames[i] << std::endl; }
        return o;
    }

    const std::string & outputInstance() const { return fOutputInstance; }

    size_t size() const { return N; }

    const std::string & dataTag() const { return fDataTag; }

    const std::string & outputName(size_t index) const
    {
        if (index < N) { return fOutputNames[index]; }
        else { throw cet::exception("MVADescription") << "Index out of range: " << index; }
    }

#endif
}; // class MVADescription

} // namespace anab

#endif //ANAB_MVAOUTPUT

