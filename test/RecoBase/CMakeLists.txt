cet_test(Wire_test USE_BOOST_UNIT
  LIBRARIES lardataobj_RecoBase
  )

cet_test(Hit_test USE_BOOST_UNIT
  LIBRARIES lardataobj_RecoBase
  )

cet_test(Cluster_test USE_BOOST_UNIT
  LIBRARIES lardataobj_RecoBase
  )

cet_test(Edge_test USE_BOOST_UNIT
  LIBRARIES lardataobj_RecoBase
  )

cet_test(TrajectoryPointFlags_test USE_BOOST_UNIT
  LIBRARIES lardataobj_RecoBase
  )

cet_test(Trajectory_test USE_BOOST_UNIT
  LIBRARIES
    lardataobj_RecoBase
    ${ROOT_GENVECTOR}
    ${ROOT_PHYSICS}
    ${ROOT_MATRIX}
  )

cet_test(TrackTrajectory_test USE_BOOST_UNIT
  LIBRARIES
    lardataobj_RecoBase
    ${ROOT_GENVECTOR}
    ${ROOT_PHYSICS}
    ${ROOT_MATRIX}
  )

install_headers()
install_source()
