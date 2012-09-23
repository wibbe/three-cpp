
#include "Code.h"
#include "RenderMaterial.h"
#include "StringHash.h"
#include "Renderer.h"
#include <cassert>

namespace three {

  static const char * _text_mapVertex[] = {
    "#ifdef USE_MAP\n",
    "  vUv0 = uv0 * offsetRepeat.zw + offsetRepeat.xy;\n",
    "  vUv1 = uv1;\n",
    "#endif\n",
  };

  static const char * _text_defaultVertex[] = {
    "gl_Position = projectionMatrix * mvPosition;\n",
  };

  static const char * _text_mapFragment[] = {
    "#ifdef USE_MAP\n",
    "  #ifdef GAMMA_INPUT\n",
    "    vec4 texelColor = texture2D(map, vUv0);\n",
    "    texelColor.xyz *= texelColor.xyz;\n",
    "    gl_FragColor = gl_FragColor * texelColor;\n",
    "  #else\n",
    "    gl_FragColor = gl_FragColor * texture2D(map, vUv0);\n",
    "  #endif\n",
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

  static const char * _text_colorFragmentParams[] = {
    "#ifdef USE_COLOR\n",
    "  varying vec4 vColor;\n",
    "#endif\n",
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

  static const char * _text_colorVertexParams[] = {
    "#ifdef USE_COLOR\n",
    "  varying vec4 vColor;\n",
    "#endif\n",
  };

  static const char * _text_mapVertexParams[] = {
    "#ifdef USE_MAP\n",
    "  varying vec2 vUv0;\n",
    "  varying vec2 vUv1;\n",
    "  uniform vec4 offsetRepeat;\n",
    "#endif\n",
  };

  static const char * _text_colorVertex[] = {
    "#ifdef USE_COLOR\n",
    "  #ifdef GAMMA_INPUT\n",
    "    vColor = color * color;\n",
    "  #else\n",
    "    vColor = color;\n",
    "  #endif\n",
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

  static const char * _text_mapFragmentParams[] = {
    "#ifdef USE_MAP\n",
    "  varying vec2 vUv0;\n",
    "  varying vec2 vUv1;\n",
    "  uniform sampler2D map;\n",
    "#endif\n",
  };

  static const char * _text_colorFragment[] = {
    "#ifdef USE_COLOR\n",
    "  gl_FragColor = gl_FragColor * vColor;\n",
    "#endif\n",
  };

  static const char * _text_prefixFragment[] = {
    "uniform mat4 viewMatrix;\n",
    "uniform vec3 cameraPosition;\n",
  };

  three::Code _code_mapVertex("@mapVertex", _text_mapVertex, 4);
  three::Code _code_defaultVertex("@defaultVertex", _text_defaultVertex, 1);
  three::Code _code_mapFragment("@mapFragment", _text_mapFragment, 9);
  three::Code _code_fogFragementParams("@fogFragementParams", _text_fogFragementParams, 9);
  three::Code _code_colorFragmentParams("@colorFragmentParams", _text_colorFragmentParams, 3);
  three::Code _code_fogFragment("@fogFragment", _text_fogFragment, 11);
  three::Code _code_colorVertexParams("@colorVertexParams", _text_colorVertexParams, 3);
  three::Code _code_mapVertexParams("@mapVertexParams", _text_mapVertexParams, 5);
  three::Code _code_colorVertex("@colorVertex", _text_colorVertex, 7);
  three::Code _code_prefixVertex("@prefixVertex", _text_prefixVertex, 14);
  three::Code _code_mapFragmentParams("@mapFragmentParams", _text_mapFragmentParams, 5);
  three::Code _code_colorFragment("@colorFragment", _text_colorFragment, 3);
  three::Code _code_prefixFragment("@prefixFragment", _text_prefixFragment, 2);

  static const char * _text_basicVertexShader[] = {
    "@prefixVertex",
    "void main()\n",
    "{\n",
    "  vec4 mvPosition = modelViewMatrix * vec4(position, 1.0);\n",
    "@defaultVertex",
    "}\n",
  };

  static const char * _text_basicFragmentShader[] = {
    "uniform vec4 color;\n",
    "void main()\n",
    "{\n",
    "  gl_FragColor = color;\n",
    "}\n",
  };

  static const char * _text_defaultVertexShader[] = {
    "@prefixVertex",
    "@mapVertexParams",
    "@colorVertexParams",
    "void main()\n",
    "{\n",
    "  vec4 mvPosition = modelViewMatrix * vec4(position, 1.0);\n",
    "@mapVertex",
    "@colorVertex",
    "@defaultVertex",
    "}\n",
  };

  static const char * _text_defaultFragmentShader[] = {
    "@prefixFragment",
    "uniform vec4 diffuse;\n",
    "uniform float opacity;\n",
    "@mapFragmentParams",
    "@colorFragmentParams",
    "void main()\n",
    "{\n",
    "  gl_FragColor = vec4(diffuse.xyz, opacity);\n",
    "@colorFragment",
    "@mapFragment",
    "}\n",
  };

  three::Code _code_basicVertexShader("basicVertexShader", _text_basicVertexShader, 6);
  three::Code _code_basicFragmentShader("basicFragmentShader", _text_basicFragmentShader, 5);
  three::Code _code_defaultVertexShader("defaultVertexShader", _text_defaultVertexShader, 10);
  three::Code _code_defaultFragmentShader("defaultFragmentShader", _text_defaultFragmentShader, 11);

  void initializeDefaultGLShaders()
  {
    // Does nothing, just here so we have something to link agains
  }
}
