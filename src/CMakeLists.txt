
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
  AmbientLight.cpp
  Code.cpp
  Material.cpp
  MeshBasicMaterial.cpp
  DefaultGLShaders.cpp
  Vector3.cpp
  Matrix4.cpp
  Quaternion.cpp

  opengl/GLObject.cpp
  opengl/GLGeometry.cpp
  opengl/GLRenderer.cpp
  opengl/GLMaterial.cpp
)

SET(LIBRARIES
    ${OPENGL_LIBRARIES}
    ${EXTRA_LIBRARIES}
    GLEW
    glfw
)

ADD_CUSTOM_COMMAND(OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/src/DefaultGLShaders.cpp ${CMAKE_CURRENT_SOURCE_DIR}/include/DefaultGLShaders.h
                   DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/materials/DefaultGLShaders.rb
                   COMMAND ruby ${CMAKE_CURRENT_SOURCE_DIR}/scripts/material-generator.rb -p ${CMAKE_CURRENT_SOURCE_DIR}/materials/DefaultGLShaders.rb)

ADD_LIBRARY(three ${SOURCE})
TARGET_LINK_LIBRARIES(three ${LIBRARIES})

# Installation
INSTALL(TARGETS three CONFIGURATIONS Release DESTINATION lib)

