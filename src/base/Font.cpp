
#include "Font.h"
#include "MathUtils.h"
#include "Geometry.h"

namespace three {

  Font::Font()
    : texture(0)
  {
  }

  Vector2 Font::textSize(const char * str)
  {
    if (!texture)
      return Vector2(0, 0);

    Vector2 size(0, 0);

    for (; *str; ++str)
    {
      int character = *str - 32;

      if (character >= 0 && character < 96)
      {
        Glyph & glyph = glyphs[character];

        size.x += glyph.advance;
        size.y = max(size.y, glyph.size.y);
      }
    }

    return size;
  }

  void Font::buildTextGeometry(const char * str, Geometry * geom, Color const& textColor)
  {
    if (!texture)
      return;

    for (; *str; ++str)
    {
    }
  }

}