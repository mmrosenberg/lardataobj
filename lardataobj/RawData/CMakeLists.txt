
include_directories( $ENV{NUSIMDATA_INC} )

cet_make( LIBRARIES larcoreobj_SummaryData
		    canvas_Persistency_Provenance
		    canvas_Utilities
		    MF_MessageLogger
		    MF_Utilities
		    cetlib
                    ${Boost_SYSTEM_LIBRARY}
		    ${ROOT_BASIC_LIB_LIST}
                    ${ROOT_EG}
                    NO_DICTIONARY )

art_dictionary( DICTIONARY_LIBRARIES lardataobj_RawData )

install_headers()
install_fhicl()
install_source()

#add_subdirectory(utils)

# uboone_datatypes requires some proprietary software
# we cannot build it everywhere
# that also means that we must also exclucde LArRawInputDriverUBooNE from the utils library build
# this code will be moved into a uboone package
#add_subdirectory(uboone_datatypes)
