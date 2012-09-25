
#include "Code.h"
#include "RenderMaterial.h"
#include "StringHash.h"
#include "Renderer.h"
#include <cassert>

namespace three {

  static const char * _text_envMapFragmentParams[] = {
    "#ifdef USE_ENVMAP\n",
    "  varying vec3 vReflect;\n",
    "  uniform float reflectivity;\n",
    "  uniform samplerCube envMap;\n",
    "  uniform float flipEnvMap;\n",
    "  uniform int combine;\n",
    "#endif\n",
  };

  static const char * _text_fogFragementParams[] = {
    "#ifdef USE_FOG\n",
    "  uniform vec3 fogColor;\n",
    "  #ifdef FOG_EXP2\n",
    "    uniform float fogDensity;\n",
    "  #else\n",
    "    uniform float fogNear;\n",
    "    uniform float fogFar;\n",
    "  #endif\n",
    "#endif\n",
  };

  static const char * _text_colorFragment[] = {
    "#ifdef USE_COLOR\n",
    "  gl_FragColor = gl_FragColor * vColor;\n",
    "#endif\n",
  };

  static const char * _text_lightMapVertexParams[] = {
    "#ifdef USE_LIGHTMAP\n",
    "  varying vec2 vUv2;\n",
    "#endif\n",
  };

  static const char * _text_envMapVertex[] = {
    "#ifdef USE_ENVMAP\n",
    "  vec4 mPosition = objectMatrix * vec4(position, 1.0);\n",
    "  vec3 nWorld = mat3(objectMatrix[0].xyz, objectMatrix[1].xyz, objectMatrix[2].xyz) * normal;\n",
    "  if (useRefract) {\n",
    "    vReflect = refract(normalize(mPosition.xyz - cameraPosition), normalize(nWorld.xyz), refractionRatio);\n",
    "    } else {\n",
    "      vReflect = reflect(normalize(mPosition.xyz - cameraPosition), normalize(nWorld.xyz));\n",
    "    }\n",
    "  #endif\n",
  };

  static const char * _text_fogFragment[] = {
    "#ifdef USE_FOG\n",
    "  float depth = gl_FragCoord.z / gl_FragCoord.w;\n",
    "  #ifdef FOG_EXP2\n",
    "    const float LOG2 = 1.442695;\n",
    "    float fogFactor = exp2( - fogDensity * fogDensity * depth * depth * LOG2 );\n",
    "    fogFactor = 1.0 - clamp( fogFactor, 0.0, 1.0 );\n",
    "  #else\n",
    "    float fogFactor = smoothstep( fogNear, fogFar, depth );\n",
    "  #endif\n",
    "  gl_FragColor = mix( gl_FragColor, vec4( fogColor, gl_FragColor.w ), fogFactor );\n",
    "#endif\n",
  };

  static const char * _text_linearToGammaFragment[] = {
    "#ifdef USE_GAMMA\n",
    "  gl_FragColor.xyz = sqrt(gl_FragColor.xyz);\n",
    "#endif\n",
  };

  static const char * _text_lightMapFragmentParams[] = {
    "#ifdef USE_LIGHTMAP\n",
    "  varying vec2 vUv2;\n",
    "  uniform sampler2D lightMap;\n",
    "#endif\n",
  };

  static const char * _text_envMapFragment[] = {
    "#ifdef USE_ENVMAP\n",
    "  #ifdef DOUBLE_SIDED\n",
    "    float flipNormal = (-1.0 + 2.0 * float(gl_FrontFacing));\n",
    "    vec4 cubeColor = textureCube(envMap, flipNormal * vec3(flipEnvMap * vReflect.x, vReflect.yz));\n",
    "  #else\n",
    "    vec4 cubeColor = textureCube(envMap, vec3(flipEnvMap * vReflect.x, vReflect.yz));\n",
    "  #endif\n",
    "  #ifdef USE_GAMMA\n",
    "    cubeColor.xyz *= cubeColor.xyz;\n",
    "  #endif\n",
    "  if (combine == 1) {\n",
    "    gl_FragColor.xyz = mix(gl_FragColor.xyz, cubeColor.xyz, reflectivity);\n",
    "    } else {\n",
    "      gl_FragColor.xyz = gl_FragColor.xyz * cubeColor.xyz;\n",
    "    }\n",
    "  #endif\n",
  };

  static const char * _text_mapVertexParams[] = {
    "#ifdef USE_MAP\n",
    "  varying vec2 vUv0;\n",
    "  uniform vec4 offsetRepeat;\n",
    "#endif\n",
  };

  static const char * _text_alphaTestFragment[] = {
    "#ifdef ALPHATEST\n",
    "  if ( gl_FragColor.a < ALPHATEST ) discard;\n",
    "#endif\n",
  };

  static const char * _text_lightMapVertex[] = {
    "#ifdef USE_LIGHTMAP\n",
    "  vUv2 = uv2;\n",
    "#endif\n",
  };

  static const char * _text_prefixVertex[] = {
    "uniform mat4 objectMatrix;\n",
    "uniform mat4 modelViewMatrix;\n",
    "uniform mat4 projectionMatrix;\n",
    "uniform mat4 viewMatrix;\n",
    "uniform mat4 normalMatrix;\n",
    "uniform vec3 cameraPosition;\n",
    "attribute vec3 position;\n",
    "attribute vec3 normal;\n",
    "attribute vec2 uv0;\n",
    "attribute vec2 uv1;\n",
    "attribute vec3 tangent;\n",
    "#ifdef USE_COLOR\n",
    "  attribute vec4 color;\n",
    "#endif\n",
  };

  static const char * _text_colorFragmentParams[] = {
    "#ifdef USE_COLOR\n",
    "  varying vec4 vColor;\n",
    "#endif\n",
  };

  static const char * _text_mapFragmentParams[] = {
    "#ifdef USE_MAP\n",
    "  varying vec2 vUv0;\n",
    "  uniform sampler2D map;\n",
    "#endif\n",
  };

  static const char * _text_lightMapFragment[] = {
    "#ifdef USE_LIGHTMAP\n",
    "  gl_FragColor = gl_FragColor * texture2D(lightMap, vUv2);\n",
    "#endif\n",
  };

  static const char * _text_prefixFragment[] = {
    "uniform mat4 viewMatrix;\n",
    "uniform vec3 cameraPosition;\n",
  };

  static const char * _text_colorVertexParams[] = {
    "#ifdef USE_COLOR\n",
    "  varying vec4 vColor;\n",
    "#endif\n",
  };

  static const char * _text_mapVertex[] = {
    "#ifdef USE_MAP\n",
    "  vUv0 = uv0 * offsetRepeat.zw + offsetRepeat.xy;\n",
    "#endif\n",
  };

  static const char * _text_envMapVertexParams[] = {
    "#ifdef USE_ENVMAP\n",
    "  varying vec3 vReflect;\n",
    "  uniform float refractionRatio;\n",
    "  uniform bool useRefract;\n",
    "#endif\n",
  };

  static const char * _text_defaultVertex[] = {
    "gl_Position = projectionMatrix * mvPosition;\n",
  };

  static const char * _text_colorVertex[] = {
    "#ifdef USE_COLOR\n",
    "  #ifdef USE_GAMMA\n",
    "    vColor = color * color;\n",
    "  #else\n",
    "    vColor = color;\n",
    "  #endif\n",
    "#endif\n",
  };

  static const char * _text_mapFragment[] = {
    "#ifdef USE_MAP\n",
    "  #ifdef USE_GAMMA\n",
    "    vec4 texelColor = texture2D(map, vUv0);\n",
    "    texelColor.xyz *= texelColor.xyz;\n",
    "    gl_FragColor = gl_FragColor * texelColor;\n",
    "  #else\n",
    "    gl_FragColor = gl_FragColor * texture2D(map, vUv0);\n",
    "  #endif\n",
    "#endif\n",
  };

  three::Code _code_envMapFragmentParams("@envMapFragmentParams", _text_envMapFragmentParams, 7);
  three::Code _code_fogFragementParams("@fogFragementParams", _text_fogFragementParams, 9);
  three::Code _code_colorFragment("@colorFragment", _text_colorFragment, 3);
  three::Code _code_lightMapVertexParams("@lightMapVertexParams", _text_lightMapVertexParams, 3);
  three::Code _code_envMapVertex("@envMapVertex", _text_envMapVertex, 9);
  three::Code _code_fogFragment("@fogFragment", _text_fogFragment, 11);
  three::Code _code_linearToGammaFragment("@linearToGammaFragment", _text_linearToGammaFragment, 3);
  three::Code _code_lightMapFragmentParams("@lightMapFragmentParams", _text_lightMapFragmentParams, 4);
  three::Code _code_envMapFragment("@envMapFragment", _text_envMapFragment, 16);
  three::Code _code_mapVertexParams("@mapVertexParams", _text_mapVertexParams, 4);
  three::Code _code_alphaTestFragment("@alphaTestFragment", _text_alphaTestFragment, 3);
  three::Code _code_lightMapVertex("@lightMapVertex", _text_lightMapVertex, 3);
  three::Code _code_prefixVertex("@prefixVertex", _text_prefixVertex, 14);
  three::Code _code_colorFragmentParams("@colorFragmentParams", _text_colorFragmentParams, 3);
  three::Code _code_mapFragmentParams("@mapFragmentParams", _text_mapFragmentParams, 4);
  three::Code _code_lightMapFragment("@lightMapFragment", _text_lightMapFragment, 3);
  three::Code _code_prefixFragment("@prefixFragment", _text_prefixFragment, 2);
  three::Code _code_colorVertexParams("@colorVertexParams", _text_colorVertexParams, 3);
  three::Code _code_mapVertex("@mapVertex", _text_mapVertex, 3);
  three::Code _code_envMapVertexParams("@envMapVertexParams", _text_envMapVertexParams, 5);
  three::Code _code_defaultVertex("@defaultVertex", _text_defaultVertex, 1);
  three::Code _code_colorVertex("@colorVertex", _text_colorVertex, 7);
  three::Code _code_mapFragment("@mapFragment", _text_mapFragment, 9);

  static const char * _text_basicVertexShader[] = {
    "@prefixVertex",
    "@mapVertexParams",
    "@lightMapVertexParams",
    "@envMapVertexParams",
    "@colorVertexParams",
    "void main()\n",
    "{\n",
    "  vec4 mvPosition = modelViewMatrix * vec4(position, 1.0);\n",
    "@mapVertex",
    "@lightMapVertex",
    "@envMapVertex",
    "@colorVertex",
    "@defaultVertex",
    "}\n",
  };

  static const char * _text_basicFragmentShader[] = {
    "@prefixFragment",
    "uniform vec4 diffuse;\n",
    "uniform float opacity;\n",
    "@mapFragmentParams",
    "@lightMapFragmentParams",
    "@envMapFragmentParams",
    "@colorFragmentParams",
    "void main()\n",
    "{\n",
    "  gl_FragColor = vec4(diffuse.xyz, opacity);\n",
    "@mapFragment",
    "@alphaTestFragment",
    "@lightMapFragment",
    "@colorFragment",
    "@envMapFragment",
    "@linearToGammaFragment",
    "}\n",
  };

  three::Code _code_basicVertexShader("basicVertexShader", _text_basicVertexShader, 14);
  three::Code _code_basicFragmentShader("basicFragmentShader", _text_basicFragmentShader, 17);

  void initializeDefaultGLShaders()
  {
    // Does nothing, just here so we have something to link agains
  }
}
