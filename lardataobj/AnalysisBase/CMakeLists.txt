cet_make(LIBRARIES canvas cetlib_except ${ROOT_CORE} NO_DICTIONARY)

art_dictionary(DICTIONARY_LIBRARIES lardataobj_AnalysisBase nusimdata_SimulationBase)

install_headers()
install_source()
