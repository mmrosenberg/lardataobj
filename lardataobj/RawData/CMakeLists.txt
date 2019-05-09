cet_make( LIBRARIES larcoreobj_SummaryData
		    canvas
		    MF_MessageLogger
                   cetlib_except
                   ${ROOT_CORE}
                    NO_DICTIONARY )

art_dictionary( DICTIONARY_LIBRARIES lardataobj_RawData )

install_headers()
install_source()
