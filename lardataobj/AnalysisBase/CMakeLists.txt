
cet_make( LIBRARIES canvas
		    MF_MessageLogger
		    MF_Utilities
		    cetlib cetlib_except
		    ${ROOT_BASIC_LIB_LIST}
                    NO_DICTIONARY )

art_dictionary( DICTIONARY_LIBRARIES lardataobj_AnalysisBase )

install_headers()
install_fhicl()
install_source()
