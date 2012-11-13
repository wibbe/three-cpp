
SET(SOURCE
  mesh.cpp
  meshio.cpp
  ctm.cpp
  stl.cpp
  3ds.cpp
  dae.cpp
  obj.cpp
  lwo.cpp
  off.cpp
  wrl.cpp
  ctmconv.cpp
  common.cpp
  systimer.cpp
  convoptions.cpp
)

SET(LIBRARIES openctm tinyxml)

ADD_EXECUTABLE(convert-ctm ${SOURCE})
TARGET_LINK_LIBRARIES(convert-ctm ${LIBRARIES})

# Installation
INSTALL(TARGETS convert-ctm CONFIGURATIONS Release DESTINATION bin)

