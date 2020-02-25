cet_make(NO_DICTIONARY
         LIBRARIES
           nusimdata_SimulationBase
           ROOT::Core
           ROOT::Physics
         )

art_dictionary(DICTIONARY_LIBRARIES lardataobj_MCBase)

install_headers()
install_source()
