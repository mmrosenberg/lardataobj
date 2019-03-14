cet_make( LIBRARIES nusimdata_SimulationBase
                    canvas
                    MF_MessageLogger
                   cetlib
                   cetlib_except
                   ${ROOT_CORE}
                   ${ROOT_PHYSICS}
                   NO_DICTIONARY)

art_dictionary( DICTIONARY_LIBRARIES lardataobj_Simulation )

install_headers()
install_fhicl()
install_source()
