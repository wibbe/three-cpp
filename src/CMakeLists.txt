
SET(SOURCE
  Object.cpp
  Scene.cpp
  Camera.cpp
  PerspectiveCamera.cpp
  Renderer.cpp
  Geometry.cpp
  CubeGeometry.cpp
  Mesh.cpp
  Texture.cpp
	Window.cpp
  ImageUtils.cpp

  GLObject.cpp
  GLGeometry.cpp
	GLRenderer.cpp

  Material.cpp
  MeshBasicMaterial.cpp

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

