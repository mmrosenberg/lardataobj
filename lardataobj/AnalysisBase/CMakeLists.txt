
cet_make( LIBRARIES canvas_Persistency_Provenance
		    canvas_Utilities
		    MF_MessageLogger
		    MF_Utilities
		    cetlib
		    ${ROOT_BASIC_LIB_LIST}
                    NO_DICTIONARY )

art_dictionary( DICTIONARY_LIBRARIES lardataobj_AnalysisBase  nusimdata_SimulationBase)

install_headers()
install_fhicl()
install_source()
