
#include "FontLoader.h"
#include "base/Texture.h"
#include "base/Font.h"
#include "base/File.h"

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
      unsigned char * image = new unsigned char[textureWidth * textureHeight];

      stbtt_bakedchar charDef[96];
      stbtt_BakeFontBitmap(data, 0, 32.0, image, textureWidth, textureHeight, 32, 96, charDef);
      delete[] data;

      // Create the texture
      Texture * texture = new Texture();
      texture->images[0] = image;
      texture->width = textureWidth;
      texture->height = textureHeight;
      texture->format = AlphaFormat;
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

        glyph.size = Vector2(quad.x1 - quad.x0, quad.y1 - quad.y0);
        glyph.uvTopLeft = Vector2(quad.s0, quad.t0);
        glyph.uvBottomRight = Vector2(quad.s1, quad.t1);
        glyph.advance = x;
      }
    }

    return font;
  }

}