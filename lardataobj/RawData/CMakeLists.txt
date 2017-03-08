
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
