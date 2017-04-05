
cet_make( LIBRARIES lardataobj_RawData
		    canvas
		    MF_MessageLogger
		    MF_Utilities
		    cetlib cetlib_except
		    ${ROOT_MATHCORE}
		    ${ROOT_GENVECTOR}
		    ${ROOT_PHYSICS}
		    ${ROOT_MATRIX}
		    NO_DICTIONARY )

art_dictionary( DICTIONARY_LIBRARIES lardataobj_RecoBase )


install_headers(EXTRAS TrajectoryPointFlags.tcc TrackTrajectory.tcc Trajectory.tcc)
install_fhicl()
install_source()
