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
    "attribute vec3 tangent;",

    "#ifdef USE_COLOR",
      "attribute vec4 color;",
    "#endif",
  ]

  snipet :prefixFragment, [
    "uniform mat4 viewMatrix;",
    "uniform vec3 cameraPosition;"
  ]

  snipet :defaultVertex, [
    "gl_Position = projectionMatrix * mvPosition;"
  ]

  # -- Fog --
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

  # -- Texture --
  snipet :mapVertexParams, [
    "#ifdef USE_MAP",
      "varying vec2 vUv0;",
      "uniform vec4 offsetRepeat;",
    "#endif"
  ]

  snipet :mapFragmentParams, [
    "#ifdef USE_MAP",
      "varying vec2 vUv0;",
      "uniform sampler2D map;",
    "#endif"
  ]

  snipet :mapVertex, [
    "#ifdef USE_MAP",
      "vUv0 = uv0 * offsetRepeat.zw + offsetRepeat.xy;",
    "#endif"
  ]

  snipet :mapFragment, [
    "#ifdef USE_MAP",
      "#ifdef USE_GAMMA",
        "vec4 texelColor = texture2D(map, vUv0);",
        "texelColor.xyz *= texelColor.xyz;",
        "gl_FragColor = gl_FragColor * texelColor;",
      "#else",
        "gl_FragColor = gl_FragColor * texture2D(map, vUv0);",
      "#endif",
    "#endif"
  ]

  # -- Lightmap --
  snipet :lightMapVertexParams, [
    "#ifdef USE_LIGHTMAP",
      "varying vec2 vUv2;",
    "#endif"
  ]

  snipet :lightMapFragmentParams, [
    "#ifdef USE_LIGHTMAP",
      "varying vec2 vUv2;",
      "uniform sampler2D lightMap;",
    "#endif"
  ]

  snipet :lightMapVertex, [
    "#ifdef USE_LIGHTMAP",
      "vUv2 = uv2;",
    "#endif"
  ]

  snipet :lightMapFragment, [
    "#ifdef USE_LIGHTMAP",
      "gl_FragColor = gl_FragColor * texture2D(lightMap, vUv2);",
    "#endif"
  ]

  # -- Environment Map --
  snipet :envMapVertexParams, [
    "#ifdef USE_ENVMAP",
      "varying vec3 vReflect;",

      "uniform float refractionRatio;",
      "uniform bool useRefract;",
    "#endif"
  ]

  snipet :envMapFragmentParams, [
    "#ifdef USE_ENVMAP",
      "varying vec3 vReflect;",

      "uniform float reflectivity;",
      "uniform samplerCube envMap;",
      "uniform float flipEnvMap;",
      "uniform int combine;",
    "#endif"
  ]

  snipet :envMapVertex, [
    "#ifdef USE_ENVMAP",
      "vec4 mPosition = objectMatrix * vec4(position, 1.0);",
      "vec3 nWorld = mat3(objectMatrix[0].xyz, objectMatrix[1].xyz, objectMatrix[2].xyz) * normal;",
      "if (useRefract) {",
        "vReflect = refract(normalize(mPosition.xyz - cameraPosition), normalize(nWorld.xyz), refractionRatio);",
      "} else {",
        "vReflect = reflect(normalize(mPosition.xyz - cameraPosition), normalize(nWorld.xyz));",
      "}",
    "#endif"
  ]

  snipet :envMapFragment, [
    "#ifdef USE_ENVMAP",
      "#ifdef DOUBLE_SIDED",
        "float flipNormal = (-1.0 + 2.0 * float(gl_FrontFacing));",
        "vec4 cubeColor = textureCube(envMap, flipNormal * vec3(flipEnvMap * vReflect.x, vReflect.yz));",
      "#else",
        "vec4 cubeColor = textureCube(envMap, vec3(flipEnvMap * vReflect.x, vReflect.yz));",
      "#endif",

      "#ifdef USE_GAMMA",
        "cubeColor.xyz *= cubeColor.xyz;",
      "#endif",
      "if (combine == 1) {",
        "gl_FragColor.xyz = mix(gl_FragColor.xyz, cubeColor.xyz, reflectivity);",
      "} else {",
        "gl_FragColor.xyz = gl_FragColor.xyz * cubeColor.xyz;",
      "}",
    "#endif"
  ]

  # -- Vertex Color --
  snipet :colorFragmentParams, [
    "#ifdef USE_COLOR",
      "varying vec4 vColor;",
    "#endif"
  ]

  snipet :colorVertexParams, [
    "#ifdef USE_COLOR",
      "varying vec4 vColor;",
    "#endif"
  ]

  snipet :colorVertex, [
    "#ifdef USE_COLOR",
      "#ifdef USE_GAMMA",
        "vColor = color * color;",
      "#else",
        "vColor = color;",
      "#endif",
    "#endif"
  ]

  snipet :colorFragment, [
    "#ifdef USE_COLOR",
      "gl_FragColor = gl_FragColor * vColor;",
    "#endif"
  ]

  # -- Gamma --
  snipet :linearToGammaFragment, [
    "#ifdef USE_GAMMA",
      "gl_FragColor.xyz = sqrt(gl_FragColor.xyz);",
    "#endif"
  ]

  # -- Alpha Test --
  snipet :alphaTestFragment, [
    "#ifdef ALPHATEST",
      "if ( gl_FragColor.a < ALPHATEST ) discard;",
    "#endif"
  ]

  # -- Basic shader --
  shader :basicVertexShader, [
    :prefixVertex,
    :mapVertexParams,
    :lightMapVertexParams,
    :envMapVertexParams,
    :colorVertexParams,

    "void main()",
    "{",
      "vec4 mvPosition = modelViewMatrix * vec4(position, 1.0);",
      :mapVertex,
      :lightMapVertex,
      :envMapVertex,
      :colorVertex,
      :defaultVertex,
    "}"
  ]

  shader :basicFragmentShader, [
    :prefixFragment,
    "uniform vec4 diffuse;",
    "uniform float opacity;",
    :mapFragmentParams,
    :lightMapFragmentParams,
    :envMapFragmentParams,
    :colorFragmentParams,

    "void main()",
    "{",
      "gl_FragColor = vec4(diffuse.xyz, opacity);",
      :mapFragment,
      :alphaTestFragment,
      :lightMapFragment,
      :colorFragment,
      :envMapFragment,
      :linearToGammaFragment,
    "}"
  ]
end