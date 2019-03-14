cet_make(LIBRARIES nusimdata_SimulationBase
                   ${ROOT_CORE}
                   ${ROOT_PHYSICS}
                   NO_DICTIONARY)

art_dictionary( DICTIONARY_LIBRARIES lardataobj_MCBase )

install_headers()
install_fhicl()
install_source()
