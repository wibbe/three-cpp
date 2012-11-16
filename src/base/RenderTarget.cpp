
#include "RenderTarget.h"
#include "Texture.h"

namespace three {

  RenderTarget::RenderTarget(int width, int height,
                             Format colorFormat,
                             Type colorType,
                             bool depthBuffer)
    : colorTexture(0),
      depthTexture(0),
      __renderBuffer(0)
  {
    colorTexture = new Texture();
    colorTexture->width = width;
    colorTexture->height = height;
    colorTexture->type = TextureTarget;
    colorTexture->format = colorFormat;
    colorTexture->imageDataType = colorType;

    if (depthBuffer)
    {
      depthTexture = new Texture();
      depthTexture->width = width;
      depthTexture->height = height;
      depthTexture->type = TextureTarget;
      depthTexture->format = DepthFormat;
      depthTexture->imageDataType = FloatType;
    }
  }

}