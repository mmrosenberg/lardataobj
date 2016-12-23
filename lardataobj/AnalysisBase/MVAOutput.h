//////////////////////////////////////////////////////////////////////////////
// \version 
//
// \brief Definition of data products to hold MVA output values and metadata
//
// \author robert.sulej@cern.ch
//
//////////////////////////////////////////////////////////////////////////////
#ifndef ANAB_MVAOUTPUT_H
#define ANAB_MVAOUTPUT_H

#include "cetlib/exception.h"

// Needed by the MVA wrapper class
#include "art/Framework/Principal/Event.h"
#include "art/Framework/Principal/Handle.h"
#include "canvas/Utilities/InputTag.h"

#include <vector>
#include <iosfwd>
#include <iostream>
#include <iomanip>

namespace anab {

template <size_t N>
class MVAOutput {
public:
    
    MVAOutput() { }

    // MUST UPDATE WHEN CLASS IS CHANGED!
    static short Class_Version() { return 1; }

private:
    double fOutputs[N]; ///< Vector of MVA output values

#ifndef __GCCXML__
public:

    MVAOutput(double init) { for (size_t i = 0; i < N; ++i) { fOutputs[i] = init; } }

    template <size_t M>
    friend std::ostream& operator << (std::ostream &o, MVAOutput<M> const& a);

    size_t size() const { return N; }

    double at(size_t index) const
    {
        if (index < N) { return fOutputs[index]; }
        else { throw cet::exception() << "Index out of range: " << index; }
    }

    double operator[] (size_t index) const { return fOutputs[index]; }

#endif

}; // class MVAOutput

template <size_t N>
class MVADescription {
public:
    
    MVADescription() : fNOutpurs(0) { }

    // MUST UPDATE WHEN CLASS IS CHANGED!
    static short Class_Version() { return 1; }

private:
    art::InputTag fDataTag;           ///< Tag of the reco data products
    std::string   fMVAOutputInstance; ///< Instance name of MVA output values
    std::string   fOutputNames[N];    ///< MVA output names/meaning

#ifndef __GCCXML__
public:

    template <size_t M>
    friend std::ostream& operator << (std::ostream &o, MVADescription<M> const& a);

    const std::string & MVAOutputInstance() const { return fMVAOutputInstance; }

    size_t size() const { return N; }

    const art::InputTag & dataTag() const { return fDataTag; }

    const std::string & outputName(size_t index) const
    {
        if (index < N) { return fOutputNames[index]; }
        else { throw cet::exception() << "Index out of range: " << index; }
    }

#endif
};

/// Wrapper for navigation through reconstructed objects of type T and associated
/// N-outputs MVA results with their metadata (this class is not a data product).
template <class T, size_t N>
class MVAReader {
public:

    MVAReader(const art::Event & evt, const art::InputTag & tag);

private:

    const MVADescription<N> & fDescription;
    const std::vector< art::Ptr<T> > & fDataProd;
};

} // namespace anab

#ifndef __GCCXML__
//----------------------------------------------------------------------------
// ostream operator for MVAOutput.
//
template <size_t M>
std::ostream& operator<< (std::ostream & o, anab::MVAOutput<M> const& a)
{
    o << "MVAOutput values:";
    for (size_t i = 0; i < M; ++i) { o << " " << a.fOutputs[i]; }
    o << std::endl; 
    return o;
}

//----------------------------------------------------------------------------
// ostream operator for MVADescription.
//
template <size_t M>
std::ostream& operator<< (std::ostream & o, anab::MVADescription<M> const& a)
{
    o << "MVADescription: prepared for " << fDataTag << ", instance name " << a.fMVAOutputInstance << ", " << M << " outputs:" << std::endl;
    for (size_t i = 0; i < M; ++i) { o << " " << a.fOutputNames[i] << std::endl; }
    return o;
}

//----------------------------------------------------------------------------
// MVA functions.
//
template <class T, size_t N>
anab::MVAReader::MVAReader(const art::Event & evt, const art::InputTag & tag)
{
    auto descriptionHandle = evt.getValidHandle< std::vector< anab::MVADescription<N> > >(tag);
    if (descriptionHandle->empty())
    {
        throw cet::exception() << "MVA description not found.";
    }
    fDescription = *(descriptionHandle->front());

    fDataProd = *(evt.getValidHandle< std::vector<T> >(fDescription.dataTag()));
}

#endif

#endif //ANAB_MVAOUTPUT

