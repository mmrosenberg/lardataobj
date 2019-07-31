cet_make(NO_DICTIONARY
         LIBRARIES
           nusimdata_SimulationBase
           ${ROOT_CORE}
           ${ROOT_PHYSICS}
         )

art_dictionary(DICTIONARY_LIBRARIES lardataobj_MCBase)

install_headers()
install_source()
