
#pragma once

namespace three {

  class Face
  {
    public:
      Face()
      { }

      Face(unsigned short _a, unsigned short _b, unsigned short _c)
        : a(_a),
          b(_b),
          c(_c)
      { }

    public:
      unsigned short a, b, c;
  };

}

