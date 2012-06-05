#pragma once

namespace three {

  static const char * basicVertex =
    "uniform mat4 objectMatrix;\n"
    "uniform mat4 modelViewMatrix;\n"
    "uniform mat4 projectionMatrix;\n"
    "uniform mat4 viewMatrix;\n"
    "uniform mat4 normalMatrix;\n"
    "uniform vec3 cameraPosition;\n"
    "attribute vec3 position;\n"
    "attribute vec3 normal;\n"
    "attribute vec2 uv;\n"
    "attribute vec2 uv2;\n"
    "#ifdef USE_COLOR\n"
    "  attribute vec3 color;\n"
    "#endif\n"
    "#ifdef USE_MAP\n"
    "  varying vec2 vUV;\n"
    "  uniform vec4 offsetRepeat;\n"
    "#endif\n"
    "#ifdef USE_COLOR\n"
    "  varying vec3 vColor;\n"
    "#endif\n"
    "void main()\n"
    "{\n"
    "  vec4 mvPosition = modelViewMatrix * vec4(position, 1.0);\n"
    "  #ifdef USE_MAP\n"
    "    vUv = uv * offsetRepeat.zw + offsetRepeat.xy;\n"
    "  #endif\n"
    "  #ifdef USE_COLOR\n"
    "    #ifdef GAMMA_INPUT\n"
    "      vColor = color * color;\n"
    "    #else\n"
    "      vColor = color;\n"
    "    #endif\n"
    "  #endif\n"
    "  gl_Position = projectionMatrix * mvPosition;\n"
    "}\n"
    "";

  static const char * basicFragment =
    "uniform mat4 viewMatrix;\n"
    "uniform vec3 cameraPosition;\n"
    "uniform vec3 diffuse\n"
    "uniform float opacity\n"
    "#ifdef USE_MAP\n"
    "  varying vUv;\n"
    "  uniform sampler2D map;\n"
    "#endif\n"
    "#ifdef USE_COLOR\n"
    "  varying vec3 vColor;\n"
    "#endif\n"
    "void main()\n"
    "{\n"
    "  gl_FragColor = vec4(diffuse, opacity);\n"
    "  #ifdef USE_COLOR\n"
    "    gl_FragColor = gl_FragColor * vec4( vColor, opacity );\n"
    "  #endif\n"
    "  #ifdef USE_MAP\n"
    "    #ifdef GAMMA_INPUT\n"
    "      vec4 texelColor = texture2D(map, vUv);\n"
    "      texelColor.xyz *= texelColor.xyz;\n"
    "      gl_FragColor = gl_FragColor * texelColor;\n"
    "    #else\n"
    "      gl_FragColor = gl_FragColor * texture2D( map, vUv );\n"
    "    #endif\n"
    "  #endif\n"
    "}\n"
    "";


}

