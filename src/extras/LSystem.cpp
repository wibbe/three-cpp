
#include "LSystem.h"
#include "base/Geometry.h"

#include <stdio.h>
#include <memory.h>
#include "third-party/rapidxml/rapidxml.hpp"

namespace three {

  LSystem::LSystem()
  {
  }

  Geometry * LSystem::generate(std::string const& text)
  {
    char * str = new char[text.size() + 1];
    memcpy(str, text.c_str(), text.size() + 1);

    using namespace rapidxml;

    xml_document<> doc;
    doc.parse<0>(str);

    xml_node<> * rules = doc.first_node("rules");
    if (!rules)
    {
      fprintf(stderr, "No rules defined");
      delete[] str;
      return 0;
    }

    delete[] str;
    return 0;
  }

}