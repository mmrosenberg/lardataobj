cet_test(Wire_test USE_BOOST_UNIT
  LIBRARIES lardataobj_RecoBase
  )

cet_test(Hit_test USE_BOOST_UNIT
  LIBRARIES lardataobj_RecoBase
  )

cet_test(Cluster_test USE_BOOST_UNIT
  LIBRARIES lardataobj_RecoBase
  )

cet_test(PointCharge_test USE_BOOST_UNIT
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
    ROOT::GenVector
    ROOT::Physics
    ROOT::Matrix
  )

cet_test(TrackTrajectory_test USE_BOOST_UNIT
  LIBRARIES
    lardataobj_RecoBase
    ROOT::GenVector
    ROOT::Physics
    ROOT::Matrix
  )

install_headers()
install_source()
