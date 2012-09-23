# Copyright (c) 2012 Daniel Wiberg
# 
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
# 
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
# 
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
# THE SOFTWARE.

define 'DefaultGLShaders', ['three'] do
  snipet :prefixVertex, [
    "uniform mat4 objectMatrix;",
    "uniform mat4 modelViewMatrix;",
    "uniform mat4 projectionMatrix;",
    "uniform mat4 viewMatrix;",
    "uniform mat4 normalMatrix;",
    "uniform vec3 cameraPosition;",

    "attribute vec3 position;",
    "attribute vec3 normal;",
    "attribute vec2 uv0;",
    "attribute vec2 uv1;",

    "#ifdef USE_COLOR",
      "attribute vec3 color;",
    "#endif",
  ]

  snipet :prefixFragment, [
    "uniform mat4 viewMatrix;",
    "uniform vec3 cameraPosition;"
  ]

  snipet :defaultVertex, [
    "gl_Position = projectionMatrix * mvPosition;"
  ]

  snipet :fogFragementParams, [
    "#ifdef USE_FOG",
      "uniform vec3 fogColor;",
      "#ifdef FOG_EXP2",
        "uniform float fogDensity;",
      "#else",
        "uniform float fogNear;",
        "uniform float fogFar;",
      "#endif",
    "#endif"
  ]

  snipet :fogFragment, [
    "#ifdef USE_FOG",
      "float depth = gl_FragCoord.z / gl_FragCoord.w;",
      "#ifdef FOG_EXP2",
        "const float LOG2 = 1.442695;",
        "float fogFactor = exp2( - fogDensity * fogDensity * depth * depth * LOG2 );",
        "fogFactor = 1.0 - clamp( fogFactor, 0.0, 1.0 );",
      "#else",
        "float fogFactor = smoothstep( fogNear, fogFar, depth );",
      "#endif",
      "gl_FragColor = mix( gl_FragColor, vec4( fogColor, gl_FragColor.w ), fogFactor );",
    "#endif"
  ]

  snipet :mapVertexParams, [
    "#ifdef USE_MAP",
      "varying vec2 vUv0;",
      "varying vec2 vUv1;",
      "uniform vec4 offsetRepeat;",
    "#endif"
  ]

  snipet :mapFragmentParams, [
    "#ifdef USE_MAP",
      "varying vUv0;",
      "varying vUv1;",
      "uniform sampler2D map;",
    "#endif"
  ]

  snipet :mapVertex, [
    "#ifdef USE_MAP",
      "vUv0 = uv0 * offsetRepeat.zw + offsetRepeat.xy;",
      "vUv1 = uv1;",
    "#endif"
  ]

  snipet :mapFragment, [
    "#ifdef USE_MAP",
      "#ifdef GAMMA_INPUT",
        "vec4 texelColor = texture2D(map, vUv0);",
        "texelColor.xyz *= texelColor.xyz;",
        "gl_FragColor = gl_FragColor * texelColor;",
      "#else",
        "gl_FragColor = gl_FragColor * texture2D(map, vUv0);",
      "#endif",
    "#endif"
  ]

  snipet :colorFragmentParams, [
    "#ifdef USE_COLOR",
      "varying vec3 vColor;",
    "#endif"
  ]

  snipet :colorVertexParams, [
    "#ifdef USE_COLOR",
      "varying vec3 vColor;",
    "#endif"
  ]

  snipet :colorVertex, [
    "#ifdef USE_COLOR",
      "#ifdef GAMMA_INPUT",
        "vColor = color * color;",
      "#else",
        "vColor = color;",
      "#endif",
    "#endif"
  ]

  snipet :colorFragment, [
    "#ifdef USE_COLOR",
      "gl_FragColor = gl_FragColor * vec4( vColor, opacity );",
    "#endif"
  ]

  # Define the shaders
  shader :basicVertex, [
    :prefixVertex,
    :mapVertexParams,
    :colorVertexParams,

    "void main()",
    "{",
      "vec4 mvPosition = modelViewMatrix * vec4(position, 1.0);",
      :mapVertex,
      :colorVertex,
      :defaultVertex,
    "}"
  ]

  shader :basicFragment, [
    :prefixFragment,
    "uniform vec3 diffuse",
    "uniform float opacity",
    :mapFragmentParams,
    :colorFragmentParams,

    "void main()",
    "{",
      "gl_FragColor = vec4(diffuse, opacity);",
      :colorFragment,
      :mapFragment,
    "}"
  ]
end