
SET(SOURCE
  openctm.c
  stream.c
  compressRAW.c
  compressMG1.c
  compressMG2.c

  liblzma/Alloc.c
  liblzma/LzFind.c
  liblzma/LzmaDec.c
  liblzma/LzmaEnc.c
  liblzma/LzmaLib.c
)

SET(LIBRARIES)

ADD_LIBRARY(openctm ${SOURCE})
TARGET_LINK_LIBRARIES(openctm ${LIBRARIES})

# Installation
INSTALL(TARGETS openctm CONFIGURATIONS Release DESTINATION lib)

