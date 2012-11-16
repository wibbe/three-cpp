
#include "RenderTarget.h"
#include "Texture.h"

namespace three {

  RenderTarget::RenderTarget(int width_, int height_,
                             Format colorFormat,
                             Type colorType,
                             bool depthBuffer)
    : width(width_),
      height(height_),
      colorTexture(0),
      depthTexture(0),
      __renderTarget(0)
  {
    colorTexture = new Texture();
    colorTexture->width = width;
    colorTexture->height = height;
    colorTexture->type = Texture2D;
    colorTexture->format = colorFormat;
    colorTexture->imageDataType = colorType;
    colorTexture->generateMipmaps = false;

    if (depthBuffer)
    {
      depthTexture = new Texture();
      depthTexture->width = width;
      depthTexture->height = height;
      depthTexture->type = Texture2D;
      depthTexture->format = DepthFormat;
      depthTexture->imageDataType = FloatType;
      depthTexture->generateMipmaps = false;
    }
  }

}