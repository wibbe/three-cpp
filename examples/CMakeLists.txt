
SET(LIBRARIES
  ${OPENGL_LIBRARIES}
  ${EXTRA_LIBRARIES}
  SOIL
  GLEW
  glfw
  three
)

# Simple cube demo
ADD_EXECUTABLE(cube cube/Cube.cpp)
TARGET_LINK_LIBRARIES(cube ${LIBRARIES})
