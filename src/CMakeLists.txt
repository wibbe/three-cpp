
SET(SOURCE
  base/Object.cpp
  base/Scene.cpp
  base/Camera.cpp
  base/PerspectiveCamera.cpp
  base/Renderer.cpp
  base/Mesh.cpp
  base/Texture.cpp
  base/Window.cpp
  base/AmbientLight.cpp
  base/Code.cpp
  base/Material.cpp
  base/MurmurHash.cpp
  base/Color.cpp
  base/Geometry.cpp
  base/CubeGeometry.cpp
  base/PlaneGeometry.cpp
  base/Vector2.cpp
  base/Vector3.cpp
  base/Matrix4.cpp
  base/Quaternion.cpp
  base/RenderTarget.cpp

  material/DefaultMaterials.cpp
  material/DefaultGLShaders.cpp

  loader/ImageUtils.cpp
  loader/CTMLoader.cpp
  loader/JSONLoader.cpp

  opengl/GLObject.cpp
  opengl/GLGeometry.cpp
  opengl/GLRenderer.cpp
  opengl/GLMaterial.cpp
  opengl/GLTexture.cpp
  opengl/GLRenderTarget.cpp
)

SET(LIBRARIES
    ${OPENGL_LIBRARIES}
    ${EXTRA_LIBRARIES}
    openctm
    parson
    GLEW
    glfw
)

ADD_LIBRARY(three ${SOURCE})
TARGET_LINK_LIBRARIES(three ${LIBRARIES})

# Installation
INSTALL(TARGETS three CONFIGURATIONS Release DESTINATION lib)

