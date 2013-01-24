
#include "FontLoader.h"
#include "base/Texture.h"
#include "base/Font.h"
#include "base/File.h"

#include <memory.h>
#define STBTT_malloc(x,u)  malloc(x)
#define STBTT_free(x,u)    free(x)

#define STB_TRUETYPE_IMPLEMENTATION
#include "StbTrueType.cpp"

#include <stdio.h>

namespace three {

  Font * FontLoader::loadFont(const char * path, float fontSize, int textureWidth, int textureHeight)
  {
    Font * font = 0;

    unsigned char * data;
    int length;
    if (fileContent(path, &data, length))
    {
      font = new Font();
      font->size = fontSize;
      unsigned char * alpha = new unsigned char[textureWidth * textureHeight];

      stbtt_bakedchar charDef[96];
      stbtt_BakeFontBitmap(data, 0, fontSize, alpha, textureWidth, textureHeight, 32, 96, charDef);
      delete[] data;

      // Construct an luminance-alpha texture
      unsigned char * image = new unsigned char[textureWidth * textureHeight * 2];
      for (int i = 0, len = textureWidth * textureHeight; i < len; ++i)
      {
        image[i * 2 + 0] = 255;
        image[i * 2 + 1] = alpha[i];
      }

      // Create the texture
      Texture * texture = new Texture();
      texture->images[0] = image;
      texture->width = textureWidth;
      texture->height = textureHeight;
      texture->format = LuminanceAlphaFormat;
      texture->imageDataType = UnsignedByteType;
      texture->type = Texture2D;
      font->texture = texture;

      // Copy glyph info
      for (int i = 0; i < 96; ++i)
      {
        float x = 0.0f, y = 0.0f;
        Font::Glyph & glyph = font->glyphs[i];
        stbtt_aligned_quad quad;

        stbtt_GetBakedQuad(charDef, textureWidth, textureHeight, i, &x, &y, &quad, 1);

        float offsetX = quad.x0 < 0.0f ? -quad.x0 : 0.0f;
        glyph.topLeft = Vector2(quad.x0 + offsetX, quad.y0);
        glyph.bottomRight = Vector2(quad.x1 + offsetX, quad.y1);
        glyph.uvTopLeft = Vector2(quad.s0, quad.t0);
        glyph.uvBottomRight = Vector2(quad.s1, quad.t1);
        glyph.advance = x;
      }
    }

    return font;
  }

}