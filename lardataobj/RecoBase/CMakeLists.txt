cet_make(LIBRARIES lardataobj_RawData
                   canvas
                   MF_MessageLogger
                   cetlib_except
                   ROOT::Physics
                   ROOT::Matrix
                   ROOT::GenVector
                   NO_DICTIONARY)

art_dictionary(DICTIONARY_LIBRARIES lardataobj_RecoBase)

add_subdirectory(AssnsDicts)
add_subdirectory(TrackingDicts)

install_headers(EXTRAS TrajectoryPointFlags.tcc TrackTrajectory.tcc Trajectory.tcc)
install_source(EXTRAS RecoBase.dox)
