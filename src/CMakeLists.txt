
SET(SOURCE
  Object.cpp
  Scene.cpp
  Camera.cpp
  PerspectiveCamera.cpp
  Renderer.cpp
  Mesh.cpp
  Texture.cpp
  Window.cpp
  ImageUtils.cpp
  AmbientLight.cpp
  Code.cpp
  Material.cpp
  MurmurHash.cpp
  Color.cpp

  Geometry.cpp
  CubeGeometry.cpp
  PlaneGeometry.cpp

  Vector2.cpp
  Vector3.cpp
  Matrix4.cpp
  Quaternion.cpp

  DefaultMaterials.cpp
  DefaultGLShaders.cpp

  CTMLoader.cpp
  JsonLoader.cpp

  opengl/GLObject.cpp
  opengl/GLGeometry.cpp
  opengl/GLRenderer.cpp
  opengl/GLMaterial.cpp
  opengl/GLTexture.cpp
)

SET(LIBRARIES
    ${OPENGL_LIBRARIES}
    ${EXTRA_LIBRARIES}
    GLEW
    glfw
    openctm
)

ADD_LIBRARY(three ${SOURCE} ${GENERATED_SOURCE})
TARGET_LINK_LIBRARIES(three ${LIBRARIES})

# Installation
INSTALL(TARGETS three CONFIGURATIONS Release DESTINATION lib)

