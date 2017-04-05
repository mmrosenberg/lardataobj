
cet_make( LIBRARIES nusimdata_SimulationBase
                    canvas
                    MF_MessageLogger
                    MF_Utilities
                    cetlib cetlib_except
                    ${ROOT_BASIC_LIB_LIST}
          NO_DICTIONARY )

art_dictionary( DICTIONARY_LIBRARIES lardataobj_Simulation )

install_headers()
install_fhicl()
install_source()
