#! /bin/sh

OS=$(uname)
BUILD_TEMP=$(pwd)/build

if [ "$OS" = "Darwin" ] ; then
  mkdir -p $BUILD_TEMP/include
  mkdir -p $BUILD_TEMP/lib
  mkdir -p $BUILD_TEMP/soil

  # Build GLEW
  (cd third-party/glew && GLEW_DEST=$BUILD_TEMP SYSTEM=Darwin make install)
  (cd third-party/glew && GLEW_DEST=$BUILD_TEMP SYSTEM=Darwin make distclean)
  
  # Build SOIL
  (cd third-party/soil/projects/makefile && make OBJDIR=$BUILD_TEMP/soil)
  (cd third-party/soil/projects/makefile && make install LOCAL=$BUILD_TEMP OBJDIR=$BUILD_TEMP/soil)
  (cd third-party/soil/projects/makefile && make clean OBJDIR=$BUILD_TEMP/soil)

  # Build GLFW
  (cd third-party/glfw && make cocoa)
  (cd third-party/glfw && PREFIX=$BUILD_TEMP make cocoa-install)
  (cd third-party/glfw && make cocoa-clean)

  # Some cleanup
  rm $BUILD_TEMP/lib/libGLEW.1.7*
  rm $BUILD_TEMP/lib/libGLEW.dylib

fi

