
#include "ImageUtils.h"
#include "Texture.h"
#include "StbImage.cpp"

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

    FILE *file = fopen(path, "rb");
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
    tex->image = image;
    tex->width = width;
    tex->height = height;
    tex->format = format;
    tex->needsUpdate = true;

    return tex;
  }

}

