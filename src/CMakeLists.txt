
SET(SOURCE
  Object.cpp
  Quaternion.cpp
  Matrix4.cpp
)

SET(LIBRARIES
    ${OPENGL_LIBRARIES}
    ${EXTRA_LIBRARIES}
    SOIL
    glfw
)

ADD_LIBRARY(three ${SOURCE})
TARGET_LINK_LIBRARIES(three ${LIBRARIES})

# Installation
INSTALL(TARGETS three CONFIGURATIONS Release DESTINATION lib)

