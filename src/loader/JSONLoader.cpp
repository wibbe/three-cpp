
#include "loader/JSONLoader.h"

#include "base/Object.h"
#include "base/Mesh.h"
#include "base/PlaneGeometry.h"
#include "base/CubeGeometry.h"
#include "material/DefaultMaterials.h"
#include "loader/CTMLoader.h"
#include "loader/ImageUtils.h"

#include <parson.h>
#include <string.h>
#include <stdio.h>
#include <map>

namespace three {

  static void parseChildArray(JSON_Array * array, Object * root, std::map<std::string, Material *> const& materials);

  static Vector3 parseVector3(JSON_Value * value, Vector3 const& defaultValue = Vector3())
  {
    if (json_value_get_type(value) != JSONArray)
      return defaultValue;

    JSON_Array * array = json_value_get_array(value);
    if (json_array_get_count(array) < 3)
      return defaultValue;

    return Vector3(json_array_get_number(array, 0), json_array_get_number(array, 1), json_array_get_number(array, 2));
  }

  static Color parseColor(JSON_Value * value)
  {
    if (json_value_get_type(value) == JSONArray)
    {
      JSON_Array * array = json_value_get_array(value);
      if (json_array_get_count(array) < 3)
        return Color();

      return Color(json_array_get_number(array, 0),
                   json_array_get_number(array, 1),
                   json_array_get_number(array, 2),
                   json_array_get_count(array) == 4 ? json_array_get_number(array, 3) : 1.0f);
    }
    else if (json_value_get_type(value) == JSONString)
      return Color(json_value_get_string(value));

    return Color();
  }

  static Object * parseMesh(JSON_Object * value, std::map<std::string, Material *> const& materials)
  {
    const char * filename = json_object_get_string(value, "filename");
    if (!filename)
      return 0;

    Mesh * mesh = 0;

    if (strstr(filename, ".ctm") != NULL)
      mesh = CTMLoader::loadMesh(filename);

    if (mesh)
    {
      if (const char * material = json_object_get_string(value, "material"))
      {
        std::map<std::string, Material *>::const_iterator materialResult = materials.find(material);
        if (materialResult != materials.end())
          mesh->material = materialResult->second;
      }
    }

    return 0;
  }

  static Object * parseAmbientLight(JSON_Object * value)
  {
    return 0;
  }

  static Object * parseObject(JSON_Object * jsonObject, std::map<std::string, Material *> const& materials)
  {
    const char * type = json_object_get_string(jsonObject, "type");
    const char * name = json_object_get_string(jsonObject, "name");

    if (!type)
      return 0;

    Object * object = 0;

    if (strcmp(type, "Mesh") == 0)
      object = parseMesh(jsonObject, materials);
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
      object->rotation = parseVector3(json_object_get_value(jsonObject, "rotation"));
      object->scale = parseVector3(json_object_get_value(jsonObject, "scale"), Vector3(1, 1, 1));

      // Parse child objects
      if (JSON_Value * children = json_object_get_value(jsonObject, "children"))
        if (json_value_get_type(children) == JSONArray)
          parseChildArray(json_value_get_array(children), object, materials);
    }


    return object;
  }

  static void parseChildArray(JSON_Array * array, Object * root, std::map<std::string, Material *> const& materials)
  {
    for (size_t i = 0, len = json_array_get_count(array); i < len; ++i)
    {
      if (JSON_Object * member = json_array_get_object(array, i))
        if (Object * object = parseObject(member, materials))
          root->add(object);
    }
  }

  static Material * parseMaterial(JSON_Object * value)
  {
    const char * type = json_object_get_string(value, "type");
    Material * mat = 0;

    // Read the specific material properties
    if (strcmp(type, "MeshBasicMaterial") == 0)
    {
      MeshBasicMaterial * material = new MeshBasicMaterial();
      mat = material;

      // Parse texture map
      const char * map = json_object_get_string(value, "map");
      if (map)
      {
        if (Texture * texture = ImageUtils::loadTexture(map))
        {
          material->map = texture;
          material->useTextureMap = true;
        }
      }

      material->diffuse = parseColor(json_object_get_value(value, "diffuse"));
      material->reflectivity = json_object_get_number(value, "reflectivity");
    }

    // Read generic material properties
    if (mat)
    {
      if (const char * name = json_object_get_string(value, "name"))
        mat->name = name;

      const char * blending = json_object_get_string(value, "blending");
      if (strcmp(blending, "Additive") == 0)
        mat->blending = AdditiveBlending;
      else if (strcmp(blending, "Substractive") == 0)
        mat->blending = SubstractiveBlending;
      else if (strcmp(blending, "Multiply") == 0)
        mat->blending = MultiplyBlending;
      else if (strcmp(blending, "Normal") == 0)
        mat->blending = NormalBlending;
      else
        mat->blending = NoBlending;

      mat->depthWrite = json_object_get_boolean(value, "depthWrite") == 1;
      mat->depthTest = json_object_get_boolean(value, "depthTest") == 1;
      mat->opacity = json_object_get_number(value, "opacity");
      mat->transparent = json_object_get_boolean(value, "transparent") == 1;

      mat->needsUpdate = true;
    }

    return mat;
  }

  static void parseMaterials(JSON_Array * materials, std::map<std::string, Material *> & materialMap)
  {
    for (size_t i = 0, len = json_array_get_count(materials); i < len; ++i)
    {
      if (JSON_Object * member = json_array_get_object(materials, i))
        if (Material * material = parseMaterial(member))
          if (!material->name.empty() && materialMap.find(material->name) == materialMap.end())
            materialMap.insert(std::make_pair(material->name, material));
    }
  }

  Object * JSONLoader::loadScene(const char * filename)
  {
    JSON_Value * file = json_parse_file(filename);
    JSON_Object * root = json_value_get_object(file);
    if (!root)
      return 0;

    JSON_Value * materials = json_object_get_value(root, "materials");
    JSON_Value * scene = json_object_get_value(root, "scene");

    std::map<std::string, Material *> materialMap;
    parseMaterials(json_value_get_array(materials), materialMap);

    Object * rootObject = new Object();
    parseChildArray(json_value_get_array(scene), rootObject, materialMap);

    json_value_free(file);
    return rootObject;
  }

}