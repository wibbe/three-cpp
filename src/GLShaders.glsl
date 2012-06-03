
// Header, used by all vertex/fragment shaders

@prefixVertex
  uniform mat4 objectMatrix;
  uniform mat4 modelViewMatrix;
  uniform mat4 projectionMatrix;
  uniform mat4 viewMatrix;
  uniform mat4 normalMatrix;
  uniform vec3 cameraPosition;
  
  attribute vec3 position;
  attribute vec3 normal;
  attribute vec2 uv;
  attribute vec2 uv2;
  
  #ifdef USE_COLOR
    attribute vec3 color;
  #endif

@prefixFragment
  uniform mat4 viewMatrix;
  uniform vec3 cameraPosition;


@defaultVertex
  gl_Position = projectionMatrix * mvPosition;

// Vertex Colors

@colorParsVertex
  #ifdef USE_COLOR
    varying vec3 vColor;
  #endif

@colorParsFragment
  #ifdef USE_COLOR
    varying vec3 vColor;
  #endif

@colorVertex
  #ifdef USE_COLOR
    #ifdef GAMMA_INPUT
      vColor = color * color;
    #else
      vColor = color;
    #endif
  #endif

@colorFragment
  #ifdef USE_COLOR
    gl_FragColor = gl_FragColor * vec4(vColor, opacity);
  #endif

// Color mapped (Triangles)

@mapParsVertex
  #ifdef USE_MAP
    varying vec2 vUv;
    uniform vec4 offsetRepeat;
  #endif

@mapParsFragment
  #ifdef USE_MAP
    varying vec2 vUv;
    uniform sampler2D map;
  #endif

@mapVertex
  #ifdef USE_MAP
    vUv = uv * offsetRepeat.zw + offsetRepeat.xy;
  #endif

@mapFragment
  #ifdef USE_MAP
    #ifdef GAMMA_INPUT
      vec4 texelColor = texture2D(map, vUv);
      texelColor.xyz *= texelColor.xyz;
      gl_FragColor = gl_FragColor * texelColor;
    #else
      gl_FragColor = gl_FragColor * texture2D(map, vUv);
    #endif
  #endif


// Basic shader

@basicVertexShader
  =prefixVertex
  =mapParsVertex
  =colorParsVertex
  
  void main()
  {
    vec4 mvPosition = modelViewMatrix * vec4(position, 1.0);
    =mapVertex
    =colorVertex
    =defaultVertex
  }

@basicFragmentShader
  =prefixFragment
  uniform vec3 diffuse;
  uniform float opacity;
  
  =mapParsFragment
  =colorParsFragment

  void main()
  {
    gl_FragColor = vec4(diffuse, opacity);
  
    =mapFragment
    =colorFragment
  }
