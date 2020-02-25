cet_make(NO_DICTIONARY
         LIBRARIES
           larcoreobj_SummaryData
           canvas
           MF_MessageLogger
           cetlib_except
           ROOT::Core)

art_dictionary(DICTIONARY_LIBRARIES lardataobj_RawData)

install_headers()
install_source()
