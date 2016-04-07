
# for classes_def.xml!!
include_directories( ${CMAKE_CURRENT_SOURCE_DIR} )

art_make( NO_PLUGINS
          LIB_LIBRARIES nusimdata_SimulationBase
			${ART_FRAMEWORK_CORE}
                        ${ART_FRAMEWORK_PRINCIPAL}
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
