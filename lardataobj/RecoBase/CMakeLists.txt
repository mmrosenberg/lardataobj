
# for classes_def.xml!!
include_directories( ${CMAKE_CURRENT_SOURCE_DIR} )

art_make( 
          LIB_LIBRARIES lardataobj_RawData
			${ART_FRAMEWORK_PRINCIPAL}
			${ART_FRAMEWORK_SERVICES_REGISTRY}
			${CANVAS_PERSISTENCY_COMMON}
			${CANVAS_PERSISTENCY_PROVENANCE}
			${CANVAS_UTILITIES}
			${MF_MESSAGELOGGER}
			${MF_UTILITIES}
			${CETLIB}
			${ROOT_BASIC_LIB_LIST}
        )


install_headers()
install_fhicl()
install_source()
