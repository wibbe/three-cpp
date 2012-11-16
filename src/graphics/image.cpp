
#include "image.h"
#include "loader/StbImage.cpp"

namespace three {

  namespace image {

    Image * load(foundation::Allocator & allocator, const char * filename)
    {
      return 0;
    }

    uint32_t elementCount(Image & image)
    {
      case (image.format)
      {
        case RGBFormat:
          return 3;

        case RGBAFormat:
          return 4;

        case AlphaFormat:
        case DepthFormat:
        case LuminanceFormat:
          return 1;

        case LuminanceAlphaFormat:
          return 2;
      };
      return 0;
    }

    uint32_t typeSize(Image & image)
    {
      switch (image.type)
      {
        case ByteType:
        case UnsignedByteType:
          return 1;

        case ShortType:
        case UnsignedShortType:
          return 2;

        case IntType:
        case UnsignedIntType:
        case FloatTyp:
          return 4;
      }

      return 0;
    }

    uint32_t dataSize(Image & image)
    {
      return image.width * image.height * typeSize(image) * elementCount(image);
    }

  }

}