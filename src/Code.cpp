
#include "Code.h"
#include <string.h>
#include <sstream>
#include <iostream>

namespace {
  static three::Code * _codeHead = 0;
  static three::Code * _codeTail = 0;
}

namespace three {

  static Code * findCode(const char * name)
  {
    Code * it = _codeHead;
    for (; it; it = it->next)
    {
      if (strcmp(it->name, name) == 0)
        return it;
    }

    return 0;
  }

  static void generateCode(Code * code, std::stringstream & ss)
  {
    for (int i = 0; i < code->rowCount; ++i)
    {
      const char * row = code->code[i];
      char * snipet = strchr(row, '@');

      if (snipet)
      {
        Code * codeSnipet = findCode(snipet);
        if (codeSnipet)
        {
          //ss << "#line 0 \"" << snipet << "\"" << std::endl;
          generateCode(codeSnipet, ss);
          //ss << "#line " << i << " \"" << code->name << "\"" << std::endl;
        }
        else
          ss << "#error Could not find code snipet " << snipet << std::endl;
      }
      else
        ss << row;
    }
  }

  Code::Code(const char * name_, const char ** code_, int rowCount_)
    : name(name_),
      code(code_),
      rowCount(rowCount_),
      next(0)
  {
    if (_codeHead)
    {
      _codeTail->next = this;
      _codeTail = this;
    }
    else
    {
      _codeHead = this;
      _codeTail = this;
    }
  }

  std::string Code::generate(const char * name, std::vector<std::string> const& defines)
  { 
    Code * code = findCode(name);
    if (!code)
      return "#error \"Could not find code segment '" + std::string(name) + "'\"";

    std::stringstream ss;

    // Generate defines
    for (std::vector<std::string>::const_iterator it = defines.begin(), end = defines.end(); it != end; ++it)
      ss << "#define " << *it << std::endl;

    //ss << "#line 0 \"" << name << "\"" << std::endl;
    generateCode(code, ss);

    return ss.str();
  }
}