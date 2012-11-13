
#include "loader/ImageUtils.h"
#include "base/Texture.h"
#include "loader/StbImage.cpp"

#include <cassert>
#include <stdio.h>

namespace three {

  static int formatToComponent(Format format)
  {
    switch (format)
    {
      case AlphaFormat:
      case LuminanceFormat:
        return 1;

      case LuminanceAlphaFormat:
        return 2;

      case RGBFormat:
        return 3;

      case RGBAFormat:
        return 4;
    };

    return 0;
  }

  ImageUtils::ImageUtils()
  {
    assert(0 && "Should never create an instance of this class");
  }

  Texture * ImageUtils::loadTexture(const char * path, Format format)
  {
    unsigned char * image = 0;
    int width, height, channels;

    FILE * file = fopen(path, "rb");
    if (!file)
        return 0;

    image = stbi_load_from_file(file, &width, &height, &channels, formatToComponent(format));
    fclose(file);

    if (!image)
    {
      fprintf(stderr, "Failed to load texture %s\n", path);
      return 0;
    }

    Texture * tex = new Texture();
    tex->images[0] = image;
    tex->imageDataType = UnsignedByteType;
    tex->width = width;
    tex->height = height;
    tex->format = format;
    tex->type = Texture2D;
    tex->needsUpdate = true;

    return tex;
  }

  Texture * ImageUtils::loadCubeTexture(const char * path[6], Format format)
  {
    void * images[6];
    int width, height, channels;

    for (int i = 0; i < 6; ++i)
    {
      images[i] = 0;
      
      FILE * file = fopen(path[i], "rb");
      if (!file)
          return 0;

      images[i] = stbi_load_from_file(file, &width, &height, &channels, formatToComponent(format));
      fclose(file);

      if (!images[i])
      {
        fprintf(stderr, "Failed to load texture %s\n", path[i]);
        return 0;
      }
    }

    Texture * tex = new Texture();
    for (int i = 0; i < 6; ++i)
      tex->images[i] = images[i];
    tex->imageDataType = UnsignedByteType;
    tex->width = width;
    tex->height = height;
    tex->format = format;
    tex->type = TextureCube;
    tex->needsUpdate = true;

    return tex;
  }

}

