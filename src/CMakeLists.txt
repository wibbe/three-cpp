
SET(SOURCE
  Object.cpp
  Quaternion.cpp
  Matrix4.cpp

  ../third-party/glew/glew.c
  ../third-party/soil/SOIL.c
  ../third-party/soil/image_DXT.c
  ../third-party/soil/stb_image_aug.c
  ../third-party/soil/image_helper.c
  ../third-party/soil/stbi_DDS_aug_c.h
)

SET(LIBRARIES
    ${OPENGL_LIBRARIES}
    ${EXTRA_LIBRARIES}
)

ADD_LIBRARY(three ${SOURCE})
TARGET_LINK_LIBRARIES(three ${LIBRARIES})

# Installation
INSTALL(TARGETS three CONFIGURATIONS Release DESTINATION lib)

