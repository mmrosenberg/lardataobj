
cet_make( LIBRARIES nusimdata_SimulationBase
		    ${ROOT_BASIC_LIB_LIST}
                    ${ROOT_EG}
                    NO_DICTIONARY )

art_dictionary( DICTIONARY_LIBRARIES lardataobj_MCBase )

install_headers()
install_fhicl()
install_source()
