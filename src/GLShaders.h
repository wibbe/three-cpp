#pragma once

namespace three {

  static const char * mapParsVertex = 
    "#ifdef USE_MAP\n"
    "  varying vec2 vUv;\n"
    "  uniform vec4 offsetRepeat;\n"
    "#endif\n"
    "";

  static const char * mapParsFragment = 
    "#ifdef USE_MAP\n"
    "  varying vec2 vUv;\n"
    "  uniform sampler2D map;\n"
    "#endif\n"
    "";

  static const char * mapVertex = 
    "#ifdef USE_MAP\n"
    "  vUv = uv * offsetRepeat.zw + offsetRepeat.xy;\n"
    "#endif\n"
    "";

  static const char * mapFragment = 
    "#ifdef USE_MAP\n"
    "  #ifdef GAMMA_INPUT\n"
    "    vec4 texelColor = texture2D(map, vUv);\n"
    "    texelColor.xyz *= texelColor.xyz;\n"
    "    gl_FragColor = gl_FragColor * texelColor;\n"
    "  #else\n"
    "    gl_FragColor = gl_FragColor * texture2D(map, vUv);\n"
    "  #endif\n"
    "#endif\n"
    "";

  static const char * basicVertexShader = 
    "#ifdef USE_MAP\n"
    "  varying vec2 vUv;\n"
    "  uniform vec4 offsetRepeat;\n"
    "#endif\n"
    "void main()\n"
    "{\n"
    "  vec4 mvPosition = modelViewMatrix * vec4(position, 1.0);\n"
    "  #ifdef USE_MAP\n"
    "    vUv = uv * offsetRepeat.zw + offsetRepeat.xy;\n"
    "  #endif\n"
    "}\n"
    "";

  static const char * basicFragmentShader = 
    "uniform vec3 diffuse;\n"
    "uniform float opacity;\n"
    "#ifdef USE_MAP\n"
    "  varying vec2 vUv;\n"
    "  uniform sampler2D map;\n"
    "#endif\n"
    "void main()\n"
    "{\n"
    "  gl_FragColor = vec4(diffuse, opacity);\n"
    "  #ifdef USE_MAP\n"
    "    #ifdef GAMMA_INPUT\n"
    "      vec4 texelColor = texture2D(map, vUv);\n"
    "      texelColor.xyz *= texelColor.xyz;\n"
    "      gl_FragColor = gl_FragColor * texelColor;\n"
    "    #else\n"
    "      gl_FragColor = gl_FragColor * texture2D(map, vUv);\n"
    "    #endif\n"
    "  #endif\n"
    "}\n"
    "";

}

