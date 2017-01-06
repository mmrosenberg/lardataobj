
cet_make( LIBRARIES lardataobj_RawData
		    canvas_Persistency_Common
		    MF_MessageLogger
		    MF_Utilities
		    cetlib
		    ${ROOT_MATHCORE}
		    ${ROOT_GENVECTOR}
		    ${ROOT_PHYSICS}
		    ${ROOT_MATRIX}
		    NO_DICTIONARY )

art_dictionary( DICTIONARY_LIBRARIES lardataobj_RecoBase )


install_headers()
install_fhicl()
install_source()
