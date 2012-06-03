
#include "ImageUtils.h"
#include "Texture.h"

#include <cassert>
#include <SOIL.h>

namespace three {

  static int formatToSoil(Format format)
  {
    switch (format)
    {
      case AlphaFormat:
      case LuminanceFormat:
        return SOIL_LOAD_L;

      case LuminanceAlphaFormat:
        return SOIL_LOAD_LA;

      case RGBFormat:
        return SOIL_LOAD_RGB;

      case RGBAFormat:
        return SOIL_LOAD_RGBA;
    };

    return 0;
  }

  ImageUtils::ImageUtils()
  {
    assert(0 && "Should never create an instance of this class");
  }

  Texture * ImageUtils::loadTexture(std::string const & path, Format format)
  {
    unsigned char * image = 0;
    int width, height, channels;

    image = SOIL_load_image(path.c_str(), &width, &height, &channels, formatToSoil(format));
    if (!image)
    {
      fprintf(stderr, "Failed to load texture %s: %s\n", path.c_str(), SOIL_last_result());
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

