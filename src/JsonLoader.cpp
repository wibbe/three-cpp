
#include "JSONLoader.h"

#include "Object.h"
#include "Mesh.h"
#include "PlaneGeometry.h"
#include "CubeGeometry.h"
#include "DefaultMaterials.h"

#include <parson.h>

namespace three {

  static void parseChildArray(JSON_Array * array, Object * root);

  static Vector3 parseVector3(JSON_Value * value)
  {
    if (json_value_get_type(value) != JSONArray)
      return Vector3();

    JSON_Array * array = json_value_get_array(value);
    if (json_array_get_count(array) < 3)
      return Vector3();

    return Vector3(json_array_get_number(array, 0), json_array_get_number(array, 1), json_array_get_number(array, 2));
  }

  static Object * parseMesh(JSON_Object * value)
  {
    return 0;
  }

  static Object * parseAmbientLight(JSON_Object * value)
  {
    return 0;
  }

  static Object * parseObject(JSON_Object * jsonObject)
  {
    const char * type = json_object_get_string(jsonObject, "type");
    const char * name = json_object_get_string(jsonObject, "name");

    if (!type)
      return 0;

    Object * object = 0;

    if (strcmp(type, "Mesh") == 0)
      object = parseMesh(jsonObject);
    else if (strcmp(type, "AmbientLight") == 0)
      object = parseAmbientLight(jsonObject);
    else if (strcmp(type, "Object") == 0)
      object = new Object();
    else
    {
      fprintf(stderr, "Unknown object type '%s'\n", type);
      return 0;
    }

    if (object)
    {
      object->name = name ? std::string(name) : std::string("NoName");
      object->position = parseVector3(json_object_get_value(jsonObject, "position"));

      // Parse child objects
      if (JSON_Value * children = json_object_get_value(jsonObject, "children"))
        if (json_value_get_type(children) == JSONArray)
          parseChildArray(json_value_get_array(children), object);
    }


    return object;
  }

  static void parseChildArray(JSON_Array * array, Object * root)
  {
    for (size_t i = 0, len = json_array_get_count(array); i < len; ++i)
    {
      if (JSON_Object * member = json_array_get_object(array, i))
        if (Object * object = parseObject(member))
          root->add(object);
    }
  }

  Object * JSONLoader::loadScene(const char * filename)
  {
    JSON_Value * root = json_parse_file(filename);
    if (!root)
      return 0;

    if (json_value_get_type(root) != JSONArray)
    {
      json_value_free(root);
      return 0;
    }

    Object * rootObject = new Object();

    parseChildArray(json_value_get_array(root), rootObject);

    json_value_free(root);
    return rootObject;
  }

}