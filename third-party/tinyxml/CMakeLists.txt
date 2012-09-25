
SET(SOURCE
  tinystr.cpp
  tinyxml.cpp
  tinyxmlerror.cpp
  tinyxmlparser.cpp
)

SET(LIBRARIES
)

ADD_LIBRARY(tinyxml ${SOURCE})
TARGET_LINK_LIBRARIES(tinyxml ${LIBRARIES})

# Installation
INSTALL(TARGETS tinyxml CONFIGURATIONS Release DESTINATION lib)

