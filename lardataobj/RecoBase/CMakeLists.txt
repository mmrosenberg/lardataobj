
cet_make( LIBRARIES lardataobj_RawData
		    canvas_Persistency_Common
		    canvas_Persistency_Provenance
		    canvas_Utilities
		    MF_MessageLogger
		    MF_Utilities
		    cetlib
		    ${ROOT_BASIC_LIB_LIST}
                    NO_DICTIONARY )

art_dictionary( DICTIONARY_LIBRARIES lardataobj_RecoBase )


install_headers()
install_fhicl()
install_source()
