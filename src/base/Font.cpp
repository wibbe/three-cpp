
#include "Font.h"
#include "MathUtils.h"
#include "Geometry.h"
#include "util/GeometryHelper.h"

namespace three {

  Font::Font()
    : texture(0)
  {
  }

  float Font::textLength(const char * str)
  {
    if (!texture)
      return 0.0f;

    float length = 0.0f;
    for (; *str; ++str)
    {
      int character = *str - 32;

      if (character >= 0 && character < 96)
      {
        Glyph & glyph = glyphs[character];

        length += glyph.advance;
      }
    }

    return length;
  }

  void Font::buildTextGeometry(const char * str, Vector2 const& offset, Geometry * geom, Color const& textColor)
  {
    if (!texture)
      return;

    Vector2 pos = offset;

    for (; *str; ++str)
    {
      int character = *str - 32;

      if (character >= 0 && character < 96)
      {
        Glyph & glyph = glyphs[character];
        addQuad(geom, glyph.topLeft + pos, glyph.bottomRight + pos, glyph.uvTopLeft, glyph.uvBottomRight, textColor);
        pos.x += glyph.advance;
      }
    }
  }

}