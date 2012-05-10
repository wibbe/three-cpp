
SET(SOURCE
  Object.cpp
  Scene.cpp
  Camera.cpp
  PerspectiveCamera.cpp
  Renderer.cpp
  Geometry.cpp
  Mesh.cpp
  GLObject.cpp

  Vector3.cpp
  Matrix4.cpp
  Quaternion.cpp
)

SET(LIBRARIES
    ${OPENGL_LIBRARIES}
    ${EXTRA_LIBRARIES}
    SOIL
    GLEW
    glfw
)

ADD_LIBRARY(three ${SOURCE})
TARGET_LINK_LIBRARIES(three ${LIBRARIES})

# Installation
INSTALL(TARGETS three CONFIGURATIONS Release DESTINATION lib)

