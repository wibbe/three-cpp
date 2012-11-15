
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
  base/Geometry.cpp
  base/CubeGeometry.cpp
  base/PlaneGeometry.cpp
  base/Buffer.cpp

  math/Color.cpp
  math/Vector2.cpp
  math/Vector3.cpp
  math/Matrix4.cpp
  math/Quaternion.cpp

  material/DefaultMaterials.cpp
  material/DefaultGLShaders.cpp

  graphics/Scene.cpp
  graphics/Context.cpp

  foundation/memory.cpp
  foundation/murmur_hash.cpp
  foundation/string_stream.cpp
  foundation/scheduler.cpp
  foundation/freelist.cpp

  loader/ImageUtils.cpp
  loader/CTMLoader.cpp
  loader/JSONLoader.cpp

  opengl/GLObject.cpp
  opengl/GLGeometry.cpp
  opengl/GLRenderer.cpp
  opengl/GLMaterial.cpp
  opengl/GLTexture.cpp
  opengl/GLBuffer.cpp
  opengl/GLContext.cpp
)

SET(LIBRARIES
    ${OPENGL_LIBRARIES}
    ${EXTRA_LIBRARIES}
    openctm
    parson
    GLEW
    glfw
    pthread
)

ADD_LIBRARY(three ${SOURCE})
TARGET_LINK_LIBRARIES(three ${LIBRARIES})

# Installation
INSTALL(TARGETS three CONFIGURATIONS Release DESTINATION lib)

