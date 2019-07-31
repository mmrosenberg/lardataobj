cet_make(NO_DICTIONARY
         LIBRARIES
           nusimdata_SimulationBase
           canvas
           MF_MessageLogger
           ${ROOT_CORE}
           ${ROOT_PHYSICS}
         )

art_dictionary(DICTIONARY_LIBRARIES lardataobj_Simulation)

install_headers()
install_source()
